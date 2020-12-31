/* Copyright JS Foundation and other contributors, http://js.foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ecma-alloc.h"
#include "ecma-exceptions.h"
#include "ecma-gc.h"
#include "ecma-globals.h"
#include "ecma-helpers.h"
#include "jrt.h"
#include "jrt-bit-fields.h"
#include "vm-defines.h"

#include "ecma-function-object.h"

JERRY_STATIC_ASSERT (ECMA_TYPE___MAX <= ECMA_VALUE_TYPE_MASK,
                     ecma_types_must_be_less_than_mask);

JERRY_STATIC_ASSERT ((ECMA_VALUE_TYPE_MASK + 1) == (1 << ECMA_VALUE_SHIFT),
                     ecma_value_part_must_start_after_flags);

JERRY_STATIC_ASSERT (ECMA_VALUE_SHIFT <= JMEM_ALIGNMENT_LOG,
                     ecma_value_shift_must_be_less_than_or_equal_than_mem_alignment_log);

JERRY_STATIC_ASSERT (sizeof (jmem_cpointer_t) <= sizeof (ecma_value_t),
                     size_of_jmem_cpointer_t_must_be_less_or_equal_to_the_size_of_ecma_value_t);

JERRY_STATIC_ASSERT (sizeof (jmem_cpointer_t) <= sizeof (jmem_cpointer_tag_t),
                     size_of_jmem_cpointer_t_must_be_less_or_equal_to_the_size_of_jmem_cpointer_tag_t);

#ifdef ECMA_VALUE_CAN_STORE_UINTPTR_VALUE_DIRECTLY

/* cppcheck-suppress zerodiv */
JERRY_STATIC_ASSERT (sizeof (uintptr_t) <= sizeof (ecma_value_t),
                     uintptr_t_must_fit_in_ecma_value_t);

#else /* !ECMA_VALUE_CAN_STORE_UINTPTR_VALUE_DIRECTLY */

JERRY_STATIC_ASSERT (sizeof (uintptr_t) > sizeof (ecma_value_t),
                     uintptr_t_must_not_fit_in_ecma_value_t);

#endif /* ECMA_VALUE_CAN_STORE_UINTPTR_VALUE_DIRECTLY */

JERRY_STATIC_ASSERT ((ECMA_VALUE_FALSE | (1 << ECMA_DIRECT_SHIFT)) == ECMA_VALUE_TRUE
                     && ECMA_VALUE_FALSE != ECMA_VALUE_TRUE,
                     only_the_lowest_bit_must_be_different_for_simple_value_true_and_false);

#if ENABLED (JERRY_BUILTIN_BIGINT)

JERRY_STATIC_ASSERT (ECMA_NULL_POINTER == (ECMA_BIGINT_ZERO & ~(ecma_value_t) ECMA_VALUE_TYPE_MASK),
                     ecma_bigint_zero_must_be_encoded_as_null_pointer);

#endif /* ENABLED (JERRY_BUILTIN_BIGINT) */

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmahelpers Helpers for operations with ECMA data types
 * @{
 */

/**
 * Debug assertion that specified value's type is one of ECMA-defined
 * script-visible types, i.e.: undefined, null, boolean, number, string, object.
 */
void
ecma_check_value_type_is_spec_defined (ecma_value_t value) /**< ecma value */
{
  JERRY_ASSERT (ecma_is_value_undefined (value)
                || ecma_is_value_null (value)
                || ecma_is_value_boolean (value)
                || ecma_is_value_number (value)
                || ecma_is_value_string (value)
                || ecma_is_value_bigint (value)
                || ecma_is_value_symbol (value)
                || ecma_is_value_object (value));
} /* ecma_check_value_type_is_spec_defined */

/**
 * Checks if the given argument is an array or not.
 *
 * @return ECMA_VALUE_ERROR- if the operation fails
 *         ECMA_VALUE_{TRUE/FALSE} - depends on whether 'arg' is an array object
 */
ecma_value_t
ecma_is_value_array (ecma_value_t arg) /**< argument */
{
  if (!ecma_is_value_object (arg))
  {
    return ECMA_VALUE_FALSE;
  }

  ecma_object_t *arg_obj_p = ecma_get_object_from_value (arg);

  if (ecma_get_object_type (arg_obj_p) == ECMA_OBJECT_TYPE_ARRAY)
  {
    return ECMA_VALUE_TRUE;
  }

#if ENABLED (JERRY_BUILTIN_PROXY)
  if (ECMA_OBJECT_IS_PROXY (arg_obj_p))
  {
    ecma_proxy_object_t *proxy_obj_p = (ecma_proxy_object_t *) arg_obj_p;

    if (proxy_obj_p->handler == ECMA_VALUE_NULL)
    {
      return ecma_raise_type_error (ECMA_ERR_MSG ("Cannot perform 'IsArray' on the given proxy "
                                                  "because handler is null"));
    }

    return ecma_is_value_array (proxy_obj_p->target);
  }
#endif /* ENABLED (JERRY_BUILTIN_PROXY) */

  return ECMA_VALUE_FALSE;
} /* ecma_is_value_array */

/**
 * Allocate and initialize a new float number without checks.
 *
 * @return ecma-value
 */
static ecma_value_t
ecma_create_float_number (ecma_number_t ecma_number) /**< value of the float number */
{
  ecma_number_t *ecma_num_p = ecma_alloc_number ();

  *ecma_num_p = ecma_number;

  return ecma_pointer_to_ecma_value (ecma_num_p) | ECMA_TYPE_FLOAT;
} /* ecma_create_float_number */

/**
 * Encode float number without checks.
 *
 * @return ecma-value
 */
ecma_value_t
ecma_make_float_value (ecma_number_t *ecma_num_p) /**< pointer to the float number */
{
  return ecma_pointer_to_ecma_value (ecma_num_p) | ECMA_TYPE_FLOAT;
} /* ecma_make_float_value */

/**
 * Create a new NaN value.
 *
 * @return ecma-value
 */
extern inline ecma_value_t JERRY_ATTR_ALWAYS_INLINE
ecma_make_nan_value (void)
{
  return ecma_create_float_number (ecma_number_make_nan ());
} /* ecma_make_nan_value */

/**
 * Checks whether the passed number is +0.0
 *
 * @return true, if it is +0.0, false otherwise
 */
static inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_number_equal_to_positive_zero (ecma_number_t ecma_number) /**< number */
{
  ecma_number_accessor_t u;
  u.as_ecma_number_t = ecma_number;
#if !ENABLED (JERRY_NUMBER_TYPE_FLOAT64)
  return u.as_uint32_t == 0;
#else /* ENABLED (JERRY_NUMBER_TYPE_FLOAT64) */
  return u.as_uint64_t == 0;
#endif /* !ENABLED (JERRY_NUMBER_TYPE_FLOAT64) */
} /* ecma_is_number_equal_to_positive_zero */

/**
 * Encode a property length number into an ecma-value
 *
 * @return ecma-value
 */
ecma_value_t
ecma_make_length_value (ecma_length_t number) /**< number to be encoded */
{
  if (number <= ECMA_INTEGER_NUMBER_MAX)
  {
    return ecma_make_integer_value ((ecma_integer_value_t) number);
  }

  return ecma_create_float_number ((ecma_number_t) number);
} /* ecma_make_length_value */

/**
 * Encode a number into an ecma-value
 *
 * @return ecma-value
 */
ecma_value_t
ecma_make_number_value (ecma_number_t ecma_number) /**< number to be encoded */
{
  ecma_integer_value_t integer_value = (ecma_integer_value_t) ecma_number;

  if ((ecma_number_t) integer_value == ecma_number
      && ((integer_value == 0) ? ecma_is_number_equal_to_positive_zero (ecma_number)
                               : ECMA_IS_INTEGER_NUMBER (integer_value)))
  {
    return ecma_make_integer_value (integer_value);
  }

  return ecma_create_float_number (ecma_number);
} /* ecma_make_number_value */

/**
 * Encode an int32 number into an ecma-value
 *
 * @return ecma-value
 */
ecma_value_t
ecma_make_int32_value (int32_t int32_number) /**< int32 number to be encoded */
{
  if (ECMA_IS_INTEGER_NUMBER (int32_number))
  {
    return ecma_make_integer_value ((ecma_integer_value_t) int32_number);
  }

  return ecma_create_float_number ((ecma_number_t) int32_number);
} /* ecma_make_int32_value */

/**
 * Encode an unsigned int32 number into an ecma-value
 *
 * @return ecma-value
 */
ecma_value_t
ecma_make_uint32_value (uint32_t uint32_number) /**< uint32 number to be encoded */
{
  if (uint32_number <= ECMA_INTEGER_NUMBER_MAX)
  {
    return ecma_make_integer_value ((ecma_integer_value_t) uint32_number);
  }

  return ecma_create_float_number ((ecma_number_t) uint32_number);
} /* ecma_make_uint32_value */

/**
 * Get floating point value from an ecma value
 *
 * @return floating point value
 */
ecma_number_t JERRY_ATTR_PURE
ecma_get_number_from_value (ecma_value_t value) /**< ecma value */
{
  if (ecma_is_value_integer_number (value))
  {
    return (ecma_number_t) ecma_get_integer_from_value (value);
  }

  return ecma_get_float_from_value (value);
} /* ecma_get_number_from_value */

/**
 * Copy ecma value.
 *
 * @return copy of the given value
 */
ecma_value_t
ecma_copy_value (ecma_value_t value)  /**< value description */
{
  switch (ecma_get_value_type_field (value))
  {
    case ECMA_TYPE_FLOAT:
    {
      ecma_number_t *num_p = (ecma_number_t *) ecma_get_pointer_from_ecma_value (value);

      return ecma_create_float_number (*num_p);
    }
    case ECMA_TYPE_STRING:
    {
      ecma_ref_ecma_string (ecma_get_string_from_value (value));
      return value;
    }
#if ENABLED (JERRY_ESNEXT)
    case ECMA_TYPE_SYMBOL:
    {
      ecma_ref_ecma_string (ecma_get_symbol_from_value (value));
      return value;
    }
#endif /* ENABLED (JERRY_ESNEXT) */
#if ENABLED (JERRY_BUILTIN_BIGINT)
    case ECMA_TYPE_BIGINT:
    {
      if (value != ECMA_BIGINT_ZERO)
      {
        ecma_ref_extended_primitive (ecma_get_extended_primitive_from_value (value));
      }
      return value;
    }
#endif /* ENABLED (JERRY_BUILTIN_BIGINT) */
    case ECMA_TYPE_OBJECT:
    {
      ecma_ref_object (ecma_get_object_from_value (value));
      return value;
    }
    default:
    {
      JERRY_ASSERT (ecma_get_value_type_field (value) == ECMA_TYPE_DIRECT
                    || ecma_get_value_type_field (value) == ECMA_TYPE_DIRECT_STRING);

      return value;
    }
  }
} /* ecma_copy_value */

/**
 * Assign a new value to an ecma-value
 *
 * Note:
 *      value previously stored in the property is freed
 */
void
ecma_value_assign_value (ecma_value_t *value_p, /**< [in, out] ecma value */
                         ecma_value_t ecma_value) /**< value to assign */
{
  JERRY_STATIC_ASSERT (ECMA_TYPE_DIRECT == 0,
                       ecma_type_direct_must_be_zero_for_the_next_check);

  if (*value_p == ecma_value)
  {
    return;
  }

  if (ecma_get_value_type_field (ecma_value || *value_p) == ECMA_TYPE_DIRECT)
  {
    *value_p = ecma_value;
  }
  else if (ecma_is_value_float_number (ecma_value)
           && ecma_is_value_float_number (*value_p))
  {
    const ecma_number_t *num_src_p = (ecma_number_t *) ecma_get_pointer_from_ecma_value (ecma_value);
    ecma_number_t *num_dst_p = (ecma_number_t *) ecma_get_pointer_from_ecma_value (*value_p);

    *num_dst_p = *num_src_p;
  }
  else
  {
    ecma_free_value_if_not_object (*value_p);
    *value_p = ecma_copy_value_if_not_object (ecma_value);
  }
} /* ecma_value_assign_value */

/**
 * Update the value of a float number to a new value
 *
 * Note:
 *   The original value is destroyed.
 *
 * @return updated ecma value
 */
ecma_value_t
ecma_update_float_number (ecma_value_t float_value, /**< original float value */
                          ecma_number_t new_number) /**< updated number value */
{
  JERRY_ASSERT (ecma_is_value_float_number (float_value));

  ecma_integer_value_t integer_number = (ecma_integer_value_t) new_number;
  ecma_number_t *number_p = (ecma_number_t *) ecma_get_pointer_from_ecma_value (float_value);

  if ((ecma_number_t) integer_number == new_number
      && ((integer_number == 0) ? ecma_is_number_equal_to_positive_zero (new_number)
                                : ECMA_IS_INTEGER_NUMBER (integer_number)))
  {
    ecma_dealloc_number (number_p);
    return ecma_make_integer_value (integer_number);
  }

  *number_p = new_number;
  return float_value;
} /* ecma_update_float_number */

/**
 * Assign a float number to an ecma-value
 *
 * Note:
 *      value previously stored in the property is freed
 */
static void
ecma_value_assign_float_number (ecma_value_t *value_p, /**< [in, out] ecma value */
                                ecma_number_t ecma_number) /**< number to assign */
{
  if (ecma_is_value_float_number (*value_p))
  {
    ecma_number_t *num_dst_p = (ecma_number_t *) ecma_get_pointer_from_ecma_value (*value_p);

    *num_dst_p = ecma_number;
    return;
  }

  if (ecma_get_value_type_field (*value_p) != ECMA_TYPE_DIRECT
      && ecma_get_value_type_field (*value_p) != ECMA_TYPE_OBJECT)
  {
    ecma_free_value (*value_p);
  }

  *value_p = ecma_create_float_number (ecma_number);
} /* ecma_value_assign_float_number */

/**
 * Assign a number to an ecma-value
 *
 * Note:
 *      value previously stored in the property is freed
 */
void
ecma_value_assign_number (ecma_value_t *value_p, /**< [in, out] ecma value */
                          ecma_number_t ecma_number) /**< number to assign */
{
  ecma_integer_value_t integer_value = (ecma_integer_value_t) ecma_number;

  if ((ecma_number_t) integer_value == ecma_number
      && ((integer_value == 0) ? ecma_is_number_equal_to_positive_zero (ecma_number)
                               : ECMA_IS_INTEGER_NUMBER (integer_value)))
  {
    if (ecma_get_value_type_field (*value_p) != ECMA_TYPE_DIRECT
        && ecma_get_value_type_field (*value_p) != ECMA_TYPE_OBJECT)
    {
      ecma_free_value (*value_p);
    }
    *value_p = ecma_make_integer_value (integer_value);
    return;
  }

  ecma_value_assign_float_number (value_p, ecma_number);
} /* ecma_value_assign_number */

/**
 * Free the ecma value
 */
void
ecma_free_value (ecma_value_t value) /**< value description */
{
  switch (ecma_get_value_type_field (value))
  {
    case ECMA_TYPE_FLOAT:
    {
      ecma_number_t *number_p = (ecma_number_t *) ecma_get_pointer_from_ecma_value (value);
      ecma_dealloc_number (number_p);
      break;
    }

    case ECMA_TYPE_STRING:
    {
      ecma_string_t *string_p = ecma_get_string_from_value (value);
      ecma_deref_ecma_string (string_p);
      break;
    }
#if ENABLED (JERRY_ESNEXT)
    case ECMA_TYPE_SYMBOL:
    {
      ecma_deref_ecma_string (ecma_get_symbol_from_value (value));
      break;
    }
#endif /* ENABLED (JERRY_ESNEXT) */
    case ECMA_TYPE_OBJECT:
    {
      ecma_deref_object (ecma_get_object_from_value (value));
      break;
    }

#if ENABLED (JERRY_BUILTIN_BIGINT)
    case ECMA_TYPE_BIGINT:
    {
      if (value != ECMA_BIGINT_ZERO)
      {
        ecma_deref_bigint (ecma_get_extended_primitive_from_value (value));
      }
      break;
    }
#endif /* ENABLED (JERRY_BUILTIN_BIGINT) */

    default:
    {
      JERRY_ASSERT (ecma_get_value_type_field (value) == ECMA_TYPE_DIRECT
                    || ecma_get_value_type_field (value) == ECMA_TYPE_DIRECT_STRING);

      /* no memory is allocated */
      break;
    }
  }
} /* ecma_free_value */

/**
 * Free the ecma value
 *
 * Note:
 *   this function is similar to ecma_free_value, but it is
 *   faster for direct values since no function call is performed.
 *   It also increases the binary size so it is recommended for
 *   critical code paths only.
 */
inline void JERRY_ATTR_ALWAYS_INLINE
ecma_fast_free_value (ecma_value_t value) /**< value description */
{
  if (ecma_get_value_type_field (value) != ECMA_TYPE_DIRECT)
  {
    ecma_free_value (value);
  }
} /* ecma_fast_free_value */

/**
 * Free the ecma value if not an object
 */
void
ecma_free_value_if_not_object (ecma_value_t value) /**< value description */
{
  if (ecma_get_value_type_field (value) != ECMA_TYPE_OBJECT)
  {
    ecma_free_value (value);
  }
} /* ecma_free_value_if_not_object */

/**
 * Free an ecma-value object
 */
inline void JERRY_ATTR_ALWAYS_INLINE
ecma_free_object (ecma_value_t value) /**< value description */
{
  ecma_deref_object (ecma_get_object_from_value (value));
} /* ecma_free_object */

/**
 * Free an ecma-value number
 */
inline void JERRY_ATTR_ALWAYS_INLINE
ecma_free_number (ecma_value_t value) /**< value description */
{
  JERRY_ASSERT (ecma_is_value_number (value));

  if (ecma_is_value_float_number (value))
  {
    ecma_number_t *number_p = (ecma_number_t *) ecma_get_pointer_from_ecma_value (value);
    ecma_dealloc_number (number_p);
  }
} /* ecma_free_number */

/**
 * Get the literal id associated with the given ecma_value type.
 * This operation is equivalent to the JavaScript 'typeof' operator.
 *
 * @returns one of the following value:
 *          - LIT_MAGIC_STRING_UNDEFINED
 *          - LIT_MAGIC_STRING_OBJECT
 *          - LIT_MAGIC_STRING_BOOLEAN
 *          - LIT_MAGIC_STRING_NUMBER
 *          - LIT_MAGIC_STRING_STRING
 *          - LIT_MAGIC_STRING_FUNCTION
 */
lit_magic_string_id_t
ecma_get_typeof_lit_id (ecma_value_t value) /**< input ecma value */
{
  lit_magic_string_id_t ret_value = LIT_MAGIC_STRING__EMPTY;

  if (ecma_is_value_undefined (value))
  {
    ret_value = LIT_MAGIC_STRING_UNDEFINED;
  }
  else if (ecma_is_value_null (value))
  {
    ret_value = LIT_MAGIC_STRING_OBJECT;
  }
  else if (ecma_is_value_boolean (value))
  {
    ret_value = LIT_MAGIC_STRING_BOOLEAN;
  }
  else if (ecma_is_value_number (value))
  {
    ret_value = LIT_MAGIC_STRING_NUMBER;
  }
  else if (ecma_is_value_string (value))
  {
    ret_value = LIT_MAGIC_STRING_STRING;
  }
#if ENABLED (JERRY_ESNEXT)
  else if (ecma_is_value_symbol (value))
  {
    ret_value = LIT_MAGIC_STRING_SYMBOL;
  }
#endif /* ENABLED (JERRY_ESNEXT) */
#if ENABLED (JERRY_BUILTIN_BIGINT)
  else if (ecma_is_value_bigint (value))
  {
    ret_value = LIT_MAGIC_STRING_BIGINT;
  }
#endif /* ENABLED (JERRY_BUILTIN_BIGINT) */
  else
  {
    JERRY_ASSERT (ecma_is_value_object (value));

    ret_value = ecma_op_is_callable (value) ? LIT_MAGIC_STRING_FUNCTION : LIT_MAGIC_STRING_OBJECT;
  }

  JERRY_ASSERT (ret_value != LIT_MAGIC_STRING__EMPTY);

  return ret_value;
} /* ecma_get_typeof_lit_id */

/**
 * @}
 * @}
 */
