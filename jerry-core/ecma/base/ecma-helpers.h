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

#ifndef ECMA_HELPERS_H
#define ECMA_HELPERS_H

#include "ecma-globals.h"
#include "ecma-builtins.h"
#include "ecma-gc.h"
#include "ecma-alloc.h"
#include "jmem.h"
#include "lit-strings.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmahelpers Helpers for operations with ECMA data types
 * @{
 */

/**
 * Get value of pointer from specified non-null compressed pointer.
 */
#define ECMA_GET_NON_NULL_POINTER(type, field) JMEM_CP_GET_NON_NULL_POINTER (type, field)

/**
 * Extract value of pointer from specified pointer-tag value
 */
#define ECMA_GET_NON_NULL_POINTER_FROM_POINTER_TAG(type, field) \
  JMEM_CP_GET_NON_NULL_POINTER_FROM_POINTER_TAG (type, field)

/**
 * Get value of pointer from specified compressed pointer.
 */
#define ECMA_GET_POINTER(type, field) JMEM_CP_GET_POINTER (type, field)

/**
 * Set value of non-null compressed pointer so that it will correspond
 * to specified non_compressed_pointer.
 */
#define ECMA_SET_NON_NULL_POINTER(field, non_compressed_pointer) JMEM_CP_SET_NON_NULL_POINTER (field, \
                                                                                               non_compressed_pointer)

/**
 * Set value of pointer-tag value so that it will correspond
 * to specified non_compressed_pointer along with tag
 */
#define ECMA_SET_NON_NULL_POINTER_TAG(field, non_compressed_pointer, tag) \
  JMEM_CP_SET_NON_NULL_POINTER_TAG (field, non_compressed_pointer, tag)

/**
 * Set value of compressed pointer so that it will correspond
 * to specified non_compressed_pointer.
 */
#define ECMA_SET_POINTER(field, non_compressed_pointer) JMEM_CP_SET_POINTER (field, non_compressed_pointer)

/**
 * Get value of each tag bit from specified pointer-tag value
 */
#define ECMA_GET_FIRST_BIT_FROM_POINTER_TAG(field) \
  JMEM_CP_GET_FIRST_BIT_FROM_POINTER_TAG (field) /**< get first tag bit from jmem_cpointer_tag_t **/
#define ECMA_GET_SECOND_BIT_FROM_POINTER_TAG(field) \
  JMEM_CP_GET_SECOND_BIT_FROM_POINTER_TAG (field) /**< get second tag bit from jmem_cpointer_tag_t **/
#define ECMA_GET_THIRD_BIT_FROM_POINTER_TAG(field) \
  JMEM_CP_GET_THIRD_BIT_FROM_POINTER_TAG (field) /**< get third tag bit from jmem_cpointer_tag_t **/

/**
 * Set value of each tag bit to specified pointer-tag value
 */
#define ECMA_SET_FIRST_BIT_TO_POINTER_TAG(field) \
  JMEM_CP_SET_FIRST_BIT_TO_POINTER_TAG (field) /**< set first tag bit to jmem_cpointer_tag_t **/
#define ECMA_SET_SECOND_BIT_TO_POINTER_TAG(field) \
  JMEM_CP_SET_SECOND_BIT_TO_POINTER_TAG (field) /**< set second tag bit to jmem_cpointer_tag_t **/
#define ECMA_SET_THIRD_BIT_TO_POINTER_TAG(field) \
  JMEM_CP_SET_THIRD_BIT_TO_POINTER_TAG (field) /**< set third tag bit to jmem_cpointer_tag_t **/

/**
 * Status flags for ecma_string_get_chars function
 */
typedef enum
{
  ECMA_STRING_FLAG_EMPTY = 0,                /**< No options are provided. */
  ECMA_STRING_FLAG_IS_ASCII = (1 << 0),      /**< The string contains only ASCII characters. */
  ECMA_STRING_FLAG_REHASH_NEEDED = (1 << 1), /**< The hash of the string must be recalculated.
                                              *   For more details see ecma_append_chars_to_string */
  ECMA_STRING_FLAG_IS_UINT32 = (1 << 2),     /**< The string repesents an UINT32 number */
  ECMA_STRING_FLAG_MUST_BE_FREED = (1 << 3), /**< The returned buffer must be freed */
} ecma_string_flag_t;

/**
 * Underscore is ignored when this option is passed.
 */
 #define ECMA_CONVERSION_ALLOW_UNDERSCORE 0x1

/**
 * Convert ecma-string's contents to a cesu-8 string and put it into a buffer.
 */
#define ECMA_STRING_TO_UTF8_STRING(ecma_str_ptr, /**< ecma string pointer */ \
                                   utf8_ptr, /**< [out] output buffer pointer */ \
                                   utf8_str_size) /**< [out] output buffer size */ \
  lit_utf8_size_t utf8_str_size; \
  uint8_t utf8_ptr ## flags = ECMA_STRING_FLAG_EMPTY; \
  const lit_utf8_byte_t *utf8_ptr = ecma_string_get_chars (ecma_str_ptr, \
                                                           &utf8_str_size, \
                                                           NULL, \
                                                           NULL, \
                                                           &utf8_ptr ## flags);

/**
 * Free the cesu-8 string buffer allocated by 'ECMA_STRING_TO_UTF8_STRING'
 */
#define ECMA_FINALIZE_UTF8_STRING(utf8_ptr, /**< pointer to character buffer */ \
                                  utf8_str_size) /**< buffer size */ \
  if (utf8_ptr ## flags & ECMA_STRING_FLAG_MUST_BE_FREED) \
  { \
    JERRY_ASSERT (utf8_ptr != NULL); \
    jmem_heap_free_block ((void *) utf8_ptr, utf8_str_size); \
  }

#ifdef ECMA_VALUE_CAN_STORE_UINTPTR_VALUE_DIRECTLY

/**
 * Set an internal property value from pointer.
 */
#define ECMA_SET_INTERNAL_VALUE_POINTER(field, pointer) \
  ((field) = ((ecma_value_t) pointer))

/**
 * Set an internal property value from pointer. Pointer can be NULL.
 */
#define ECMA_SET_INTERNAL_VALUE_ANY_POINTER(field, pointer) \
  ((field) = ((ecma_value_t) pointer))

/**
 * Convert an internal property value to pointer.
 */
#define ECMA_GET_INTERNAL_VALUE_POINTER(type, field) \
  ((type *) field)

/**
 * Convert an internal property value to pointer. Result can be NULL.
 */
#define ECMA_GET_INTERNAL_VALUE_ANY_POINTER(type, field) \
  ((type *) field)

/**
 * Checks whether an internal property is NULL.
 */
#define ECMA_IS_INTERNAL_VALUE_NULL(field) \
  ((field) == ((ecma_value_t) NULL))

#else /* !ECMA_VALUE_CAN_STORE_UINTPTR_VALUE_DIRECTLY */

/**
 * Set an internal property value from pointer.
 */
#define ECMA_SET_INTERNAL_VALUE_POINTER(field, pointer) \
  ECMA_SET_NON_NULL_POINTER (field, pointer)

/**
 * Set an internal property value from pointer. Pointer can be NULL.
 */
#define ECMA_SET_INTERNAL_VALUE_ANY_POINTER(field, pointer) \
  ECMA_SET_POINTER (field, pointer)

/**
 * Convert an internal property value to pointer.
 */
#define ECMA_GET_INTERNAL_VALUE_POINTER(type, field) \
  ECMA_GET_NON_NULL_POINTER (type, field)

/**
 * Convert an internal property value to pointer. Result can be NULL.
 */
#define ECMA_GET_INTERNAL_VALUE_ANY_POINTER(type, field) \
  ECMA_GET_POINTER (type, field)

/**
 * Checks whether an internal property is NULL.
 */
#define ECMA_IS_INTERNAL_VALUE_NULL(field) \
  ((field) == ((ecma_value_t) JMEM_CP_NULL))

#endif /* ECMA_VALUE_CAN_STORE_UINTPTR_VALUE_DIRECTLY */

/**
 * Convert boolean to bitfield value.
 */
#define ECMA_BOOL_TO_BITFIELD(x) ((x) ? 1 : 0)

/**
 * Check whether the given type is ECMA_OBJECT_TYPE_PROXY
 *
 * @param type object type
 */
#define ECMA_OBJECT_TYPE_IS_PROXY(type) (JERRY_UNLIKELY ((type) == ECMA_OBJECT_TYPE_PROXY))

/**
 * Check whether the given object has [[ProxyHandler]] and [[ProxyTarger]] internal slots
 *
 * @param obj_p ecma-object
 */
#if ENABLED (JERRY_BUILTIN_PROXY)
#define ECMA_OBJECT_IS_PROXY(obj_p) (ECMA_OBJECT_TYPE_IS_PROXY (ecma_get_object_type ((obj_p))))
#else /* !ENABLED (JERRY_BUILTIN_PROXY) */
#define ECMA_OBJECT_IS_PROXY(obj_p) (false)
#endif /* ENABLED (JERRY_BUILTIN_PROXY) */

/* ecma-helpers-value.c */

/**
 * Get type field of ecma value
 *
 * @return type field
 */
inline ecma_type_t JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_get_value_type_field (ecma_value_t value) /**< ecma value */
{
  return value & ECMA_VALUE_TYPE_MASK;
} /* ecma_get_value_type_field */

/**
 * Convert a pointer into an ecma value.
 *
 * @return ecma value
 */
inline ecma_value_t JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_pointer_to_ecma_value (const void *ptr) /**< pointer */
{
#ifdef ECMA_VALUE_CAN_STORE_UINTPTR_VALUE_DIRECTLY

  JERRY_ASSERT (ptr != NULL);
  uintptr_t uint_ptr = (uintptr_t) ptr;
  JERRY_ASSERT ((uint_ptr & ECMA_VALUE_TYPE_MASK) == 0);
  return (ecma_value_t) uint_ptr;

#else /* !ECMA_VALUE_CAN_STORE_UINTPTR_VALUE_DIRECTLY */

  jmem_cpointer_t ptr_cp;
  ECMA_SET_NON_NULL_POINTER (ptr_cp, ptr);
  return ((ecma_value_t) ptr_cp) << ECMA_VALUE_SHIFT;

#endif /* ECMA_VALUE_CAN_STORE_UINTPTR_VALUE_DIRECTLY */
} /* ecma_pointer_to_ecma_value */

/**
 * Get a pointer from an ecma value
 *
 * @return pointer
 */
inline void * JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_get_pointer_from_ecma_value (ecma_value_t value) /**< value */
{
#ifdef ECMA_VALUE_CAN_STORE_UINTPTR_VALUE_DIRECTLY
  void *ptr = (void *) (uintptr_t) ((value) & ~ECMA_VALUE_TYPE_MASK);
  JERRY_ASSERT (ptr != NULL);
  return ptr;
#else /* !ECMA_VALUE_CAN_STORE_UINTPTR_VALUE_DIRECTLY */
  return ECMA_GET_NON_NULL_POINTER (void, value >> ECMA_VALUE_SHIFT);
#endif /* ECMA_VALUE_CAN_STORE_UINTPTR_VALUE_DIRECTLY */
} /* ecma_get_pointer_from_ecma_value */

/**
 * Check if the value is direct ecma-value.
 *
 * @return true - if the value is a direct value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_direct (ecma_value_t value) /**< ecma value */
{
  return (ecma_get_value_type_field (value) == ECMA_TYPE_DIRECT);
} /* ecma_is_value_direct */

/**
 * Check if the value is simple ecma-value.
 *
 * @return true - if the value is a simple value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_simple (ecma_value_t value) /**< ecma value */
{
  return (value & ECMA_DIRECT_TYPE_MASK) == ECMA_DIRECT_TYPE_SIMPLE_VALUE;
} /* ecma_is_value_simple */

/**
 * Check whether the value is a given simple value.
 *
 * @return true - if the value is equal to the given simple value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_equal_to_simple_value (ecma_value_t value, /**< ecma value */
                                     ecma_value_t simple_value) /**< simple value */
{
  return value == simple_value;
} /* ecma_is_value_equal_to_simple_value */

/**
 * Check if the value is empty.
 *
 * @return true - if the value contains implementation-defined empty simple value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_empty (ecma_value_t value) /**< ecma value */
{
  return ecma_is_value_equal_to_simple_value (value, ECMA_VALUE_EMPTY);
} /* ecma_is_value_empty */

/**
 * Check if the value is undefined.
 *
 * @return true - if the value contains ecma-undefined simple value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_undefined (ecma_value_t value) /**< ecma value */
{
  return ecma_is_value_equal_to_simple_value (value, ECMA_VALUE_UNDEFINED);
} /* ecma_is_value_undefined */

/**
 * Check if the value is null.
 *
 * @return true - if the value contains ecma-null simple value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_null (ecma_value_t value) /**< ecma value */
{
  return ecma_is_value_equal_to_simple_value (value, ECMA_VALUE_NULL);
} /* ecma_is_value_null */

/**
 * Check if the value is true.
 *
 * @return true - if the value contains ecma-true simple value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_true (ecma_value_t value) /**< ecma value */
{
  return ecma_is_value_equal_to_simple_value (value, ECMA_VALUE_TRUE);
} /* ecma_is_value_true */

/**
 * Check if the value is boolean.
 *
 * @return true - if the value contains ecma-true or ecma-false simple values,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_boolean (ecma_value_t value) /**< ecma value */
{
  return ecma_is_value_true (value | (1 << ECMA_DIRECT_SHIFT));
} /* ecma_is_value_boolean */

/**
 * Check if the value is false.
 *
 * @return true - if the value contains ecma-false simple value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_false (ecma_value_t value) /**< ecma value */
{
  return ecma_is_value_equal_to_simple_value (value, ECMA_VALUE_FALSE);
} /* ecma_is_value_false */

/**
 * Check if the value is not found.
 *
 * @return true - if the value contains ecma-not-found simple value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_found (ecma_value_t value) /**< ecma value */
{
  return value != ECMA_VALUE_NOT_FOUND;
} /* ecma_is_value_found */

/**
 * Check if the value is array hole.
 *
 * @return true - if the value contains ecma-array-hole simple value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_array_hole (ecma_value_t value) /**< ecma value */
{
  return ecma_is_value_equal_to_simple_value (value, ECMA_VALUE_ARRAY_HOLE);
} /* ecma_is_value_array_hole */

/**
 * Check if the value is integer ecma-number.
 *
 * @return true - if the value contains an integer ecma-number value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_integer_number (ecma_value_t value) /**< ecma value */
{
  return (value & ECMA_DIRECT_TYPE_MASK) == ECMA_DIRECT_TYPE_INTEGER_VALUE;
} /* ecma_is_value_integer_number */

/**
 * Check if both values are integer ecma-numbers.
 *
 * @return true - if both values contain integer ecma-number values,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_are_values_integer_numbers (ecma_value_t first_value, /**< first ecma value */
                                 ecma_value_t second_value) /**< second ecma value */
{
  JERRY_STATIC_ASSERT (ECMA_DIRECT_TYPE_INTEGER_VALUE == 0,
                       ecma_direct_type_integer_value_must_be_zero);

  return ((first_value | second_value) & ECMA_DIRECT_TYPE_MASK) == ECMA_DIRECT_TYPE_INTEGER_VALUE;
} /* ecma_are_values_integer_numbers */

/**
 * Check if the value is floating-point ecma-number.
 *
 * @return true - if the value contains a floating-point ecma-number value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_float_number (ecma_value_t value) /**< ecma value */
{
  return (ecma_get_value_type_field (value) == ECMA_TYPE_FLOAT);
} /* ecma_is_value_float_number */

/**
 * Check if the value is ecma-number.
 *
 * @return true - if the value contains ecma-number value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_number (ecma_value_t value) /**< ecma value */
{
  return (ecma_is_value_integer_number (value)
          || ecma_is_value_float_number (value));
} /* ecma_is_value_number */

JERRY_STATIC_ASSERT ((ECMA_TYPE_STRING | 0x4) == ECMA_TYPE_DIRECT_STRING,
                     ecma_type_string_and_direct_string_must_have_one_bit_difference);

/**
 * Check if the value is ecma-string.
 *
 * @return true - if the value contains ecma-string value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_string (ecma_value_t value) /**< ecma value */
{
  return ((value & (ECMA_VALUE_TYPE_MASK - 0x4)) == ECMA_TYPE_STRING);
} /* ecma_is_value_string */

/**
 * Check if the value is symbol.
 *
 * @return true - if the value contains symbol value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_symbol (ecma_value_t value) /**< ecma value */
{
#if ENABLED (JERRY_ESNEXT)
  return (ecma_get_value_type_field (value) == ECMA_TYPE_SYMBOL);
#else /* ENABLED (JERRY_ESNEXT) */
  JERRY_UNUSED (value);
  return false;
#endif /* ENABLED (JERRY_ESNEXT) */
} /* ecma_is_value_symbol */

/**
 * Check if the value is bigint.
 *
 * @return true - if the value contains bigint value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_bigint (ecma_value_t value) /**< ecma value */
{
  #if ENABLED (JERRY_BUILTIN_BIGINT)
  return (ecma_get_value_type_field (value) == ECMA_TYPE_BIGINT);
  #else /* !ENABLED (JERRY_BUILTIN_BIGINT) */
  JERRY_UNUSED (value);
  return false;
  #endif /* ENABLED (JERRY_BUILTIN_BIGINT) */
} /* ecma_is_value_bigint */

/**
 * Check if the value can be property name.
 *
 * @return true - if the value can be property name value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_prop_name (ecma_value_t value) /**< ecma value */
{
#if ENABLED (JERRY_ESNEXT)
  return ecma_is_value_string (value) || ecma_is_value_symbol (value);
#else /* !ENABLED (JERRY_ESNEXT) */
  return ecma_is_value_string (value);
#endif /* ENABLED (JERRY_ESNEXT) */
} /* ecma_is_value_prop_name */

/**
 * Check if the value is direct ecma-string.
 *
 * @return true - if the value contains direct ecma-string value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_direct_string (ecma_value_t value) /**< ecma value */
{
  return (ecma_get_value_type_field (value) == ECMA_TYPE_DIRECT_STRING);
} /* ecma_is_value_direct_string */

/**
 * Check if the value is non-direct ecma-string.
 *
 * @return true - if the value contains non-direct ecma-string value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_non_direct_string (ecma_value_t value) /**< ecma value */
{
  return (ecma_get_value_type_field (value) == ECMA_TYPE_STRING);
} /* ecma_is_value_non_direct_string */

/**
 * Check if the value is object.
 *
 * @return true - if the value contains object value,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_object (ecma_value_t value) /**< ecma value */
{
  return (ecma_get_value_type_field (value) == ECMA_TYPE_OBJECT);
} /* ecma_is_value_object */

/**
 * Check if the value is error reference.
 *
 * @return true - if the value contains an error reference,
 *         false - otherwise
 */
inline bool JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_is_value_error_reference (ecma_value_t value) /**< ecma value */
{
  return (ecma_get_value_type_field (value) == ECMA_TYPE_ERROR);
} /* ecma_is_value_error_reference */

ecma_value_t ecma_is_value_array (ecma_value_t arg);

void ecma_check_value_type_is_spec_defined (ecma_value_t value);

/**
 * Creates an ecma value from the given raw boolean.
 *
 * @return boolean ecma_value
 */
inline ecma_value_t JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_make_boolean_value (bool boolean_value) /**< raw bool value from which the ecma value will be created */
{
  return boolean_value ? ECMA_VALUE_TRUE : ECMA_VALUE_FALSE;
} /* ecma_make_boolean_value */

/**
 * Encode an integer number into an ecma-value without allocating memory
 *
 * Note:
 *   The value must fit into the range of allowed ecma integer values
 *
 * @return ecma-value
 */
inline ecma_value_t JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_make_integer_value (ecma_integer_value_t integer_value) /**< integer number to be encoded */
{
  JERRY_ASSERT (ECMA_IS_INTEGER_NUMBER (integer_value));

  return (((ecma_value_t) integer_value) << ECMA_DIRECT_SHIFT) | ECMA_DIRECT_TYPE_INTEGER_VALUE;
} /* ecma_make_integer_value */

ecma_value_t ecma_make_nan_value (void);
ecma_value_t ecma_make_float_value (ecma_number_t *ecma_num_p);
ecma_value_t ecma_make_length_value (ecma_length_t length);
ecma_value_t ecma_make_number_value (ecma_number_t ecma_number);
ecma_value_t ecma_make_int32_value (int32_t int32_number);
ecma_value_t ecma_make_uint32_value (uint32_t uint32_number);
#if ENABLED (JERRY_ESNEXT)
bool ecma_prop_name_is_symbol (ecma_string_t *string_p);
#endif /* ENABLED (JERRY_ESNEXT) */

/**
 * String value constructor
 *
 * @return ecma-value representation of the string argument
 */
inline ecma_value_t JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_make_string_value (const ecma_string_t *ecma_string_p) /**< string to reference in value */
{
  JERRY_ASSERT (ecma_string_p != NULL);
#if ENABLED (JERRY_ESNEXT)
  JERRY_ASSERT (!ecma_prop_name_is_symbol ((ecma_string_t *) ecma_string_p));
#endif /* ENABLED (JERRY_ESNEXT) */

  if ((((uintptr_t) ecma_string_p) & ECMA_VALUE_TYPE_MASK) != 0)
  {
    return (ecma_value_t) (uintptr_t) ecma_string_p;
  }

  return ecma_pointer_to_ecma_value (ecma_string_p) | ECMA_TYPE_STRING;
} /* ecma_make_string_value */

#if ENABLED (JERRY_ESNEXT)
/**
 * Symbol value constructor
 *
 * @return ecma-value representation of the string argument
 */
inline ecma_value_t JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_make_symbol_value (const ecma_string_t *ecma_symbol_p) /**< symbol to reference in value */
{
  JERRY_ASSERT (ecma_symbol_p != NULL);
  JERRY_ASSERT (ecma_prop_name_is_symbol ((ecma_string_t *) ecma_symbol_p));

  return ecma_pointer_to_ecma_value (ecma_symbol_p) | ECMA_TYPE_SYMBOL;
} /* ecma_make_symbol_value */
#endif /* ENABLED (JERRY_ESNEXT) */

/**
 * Property-name value constructor
 *
 * @return ecma-value representation of a property name argument
 */
inline ecma_value_t JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_make_prop_name_value (const ecma_string_t *ecma_prop_name_p) /**< property name to reference in value */
{
  JERRY_ASSERT (ecma_prop_name_p != NULL);

#if ENABLED (JERRY_ESNEXT)
  if (ecma_prop_name_is_symbol ((ecma_string_t *) ecma_prop_name_p))
  {
    return ecma_make_symbol_value (ecma_prop_name_p);
  }
#endif /* ENABLED (JERRY_ESNEXT) */

  return ecma_make_string_value (ecma_prop_name_p);
} /* ecma_make_prop_name_value */

/**
 * String value constructor
 *
 * @return ecma-value representation of the string argument
 */
inline ecma_value_t JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_make_magic_string_value (lit_magic_string_id_t id) /**< magic string id */
{
  return (ecma_value_t) ECMA_CREATE_DIRECT_STRING (ECMA_DIRECT_STRING_MAGIC, (uintptr_t) id);
} /* ecma_make_magic_string_value */

/**
 * Object value constructor
 *
 * @return ecma-value representation of the object argument
 */
inline ecma_value_t JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_make_object_value (const ecma_object_t *object_p) /**< object to reference in value */
{
  JERRY_ASSERT (object_p != NULL);

  return ecma_pointer_to_ecma_value (object_p) | ECMA_TYPE_OBJECT;
} /* ecma_make_object_value */

/**
 * Error reference constructor
 *
 * @return ecma-value representation of the Error reference
 */
inline ecma_value_t JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_make_extended_primitive_value (const ecma_extended_primitive_t *primitve_p, /**< extended primitve value */
                                    uint32_t type) /**< ecma type of extended primitve value */
{
  JERRY_ASSERT (primitve_p != NULL);
#if ENABLED (JERRY_BUILTIN_BIGINT)
  JERRY_ASSERT (primitve_p != ECMA_BIGINT_POINTER_TO_ZERO);
#endif /* ENABLED (JERRY_BUILTIN_BIGINT) */
  JERRY_ASSERT (type == ECMA_TYPE_BIGINT || type == ECMA_TYPE_ERROR);

  return ecma_pointer_to_ecma_value (primitve_p) | type;
} /* ecma_make_extended_primitive_value */

/**
 * Get integer value from an integer ecma value
 *
 * @return integer value
 */
inline ecma_integer_value_t JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_get_integer_from_value (ecma_value_t value) /**< ecma value */
{
  JERRY_ASSERT (ecma_is_value_integer_number (value));

  return ((ecma_integer_value_t) value) >> ECMA_DIRECT_SHIFT;
} /* ecma_get_integer_from_value */

/**
 * Get floating point value from an ecma value
 *
 * @return floating point value
 */
inline ecma_number_t JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_get_float_from_value (ecma_value_t value) /**< ecma value */
{
  JERRY_ASSERT (ecma_get_value_type_field (value) == ECMA_TYPE_FLOAT);

  return *(ecma_number_t *) ecma_get_pointer_from_ecma_value (value);
} /* ecma_get_float_from_value */

/**
 * Get floating point value pointer from an ecma value
 *
 * @return floating point value
 */
inline ecma_number_t * JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_get_pointer_from_float_value (ecma_value_t value) /**< ecma value */
{
  JERRY_ASSERT (ecma_get_value_type_field (value) == ECMA_TYPE_FLOAT);

  return (ecma_number_t *) ecma_get_pointer_from_ecma_value (value);
} /* ecma_get_pointer_from_float_value */

ecma_number_t JERRY_ATTR_PURE ecma_get_number_from_value (ecma_value_t value);

/**
 * Get pointer to ecma-string from ecma value
 *
 * @return the string pointer
 */
inline ecma_string_t *JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_get_string_from_value (ecma_value_t value) /**< ecma value */
{
  JERRY_ASSERT (ecma_is_value_string (value));

  if ((value & ECMA_VALUE_TYPE_MASK) == ECMA_TYPE_DIRECT_STRING)
  {
    return (ecma_string_t *) (uintptr_t) value;
  }

  return (ecma_string_t *) ecma_get_pointer_from_ecma_value (value);
} /* ecma_get_string_from_value */

#if ENABLED (JERRY_ESNEXT)
/**
 * Get pointer to ecma-string from ecma value
 *
 * @return the string pointer
 */
inline ecma_string_t *JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_get_symbol_from_value (ecma_value_t value) /**< ecma value */
{
  JERRY_ASSERT (ecma_is_value_symbol (value));

  return (ecma_string_t *) ecma_get_pointer_from_ecma_value (value);
} /* ecma_get_symbol_from_value */
#endif /* ENABLED (JERRY_ESNEXT) */

/**
 * Get pointer to a property name from ecma value
 *
 * @return the string pointer
 */
inline ecma_string_t *JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_get_prop_name_from_value (ecma_value_t value) /**< ecma value */
{
  JERRY_ASSERT (ecma_is_value_prop_name (value));

  if ((value & ECMA_VALUE_TYPE_MASK) == ECMA_TYPE_DIRECT_STRING)
  {
    return (ecma_string_t *) (uintptr_t) value;
  }

  return (ecma_string_t *) ecma_get_pointer_from_ecma_value (value);
} /* ecma_get_prop_name_from_value */

/**
 * Get pointer to ecma-object from ecma value
 *
 * @return the pointer
 */
inline ecma_object_t *JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_get_object_from_value (ecma_value_t value) /**< ecma value */
{
  JERRY_ASSERT (ecma_is_value_object (value));

  return (ecma_object_t *) ecma_get_pointer_from_ecma_value (value);
} /* ecma_get_object_from_value */

/**
 * Get pointer to error reference from ecma value
 *
 * @return the pointer
 */
inline ecma_extended_primitive_t *JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_get_extended_primitive_from_value (ecma_value_t value) /**< ecma value */
{
#if ENABLED (JERRY_BUILTIN_BIGINT)
  JERRY_ASSERT (value != ECMA_BIGINT_ZERO);
#endif /* ENABLED (JERRY_BUILTIN_BIGINT) */
  JERRY_ASSERT (ecma_get_value_type_field (value) == ECMA_TYPE_BIGINT
                || ecma_get_value_type_field (value) == ECMA_TYPE_ERROR);

  return (ecma_extended_primitive_t *) ecma_get_pointer_from_ecma_value (value);
} /* ecma_get_extended_primitive_from_value */

/**
 * Invert a boolean value
 *
 * @return ecma value
 */
inline ecma_value_t JERRY_ATTR_CONST JERRY_ATTR_ALWAYS_INLINE
ecma_invert_boolean_value (ecma_value_t value) /**< ecma value */
{
  JERRY_ASSERT (ecma_is_value_boolean (value));

  return (value ^ (1 << ECMA_DIRECT_SHIFT));
} /* ecma_invert_boolean_value */

ecma_value_t ecma_copy_value (ecma_value_t value);

/**
 * Copy ecma value.
 *
 * Note:
 *   this function is similar to ecma_copy_value, but it is
 *   faster for direct values since no function call is performed.
 *   It also increases the binary size so it is recommended for
 *   critical code paths only.
 *
 * @return copy of the given value
 */
inline ecma_value_t JERRY_ATTR_ALWAYS_INLINE
ecma_fast_copy_value (ecma_value_t value)  /**< value description */
{
  return (ecma_get_value_type_field (value) == ECMA_TYPE_DIRECT) ? value : ecma_copy_value (value);
} /* ecma_fast_copy_value */

/**
 * Copy the ecma value if not an object
 *
 * @return copy of the given value
 */
inline ecma_value_t JERRY_ATTR_ALWAYS_INLINE
ecma_copy_value_if_not_object (ecma_value_t value) /**< value description */
{
  if (!ecma_is_value_object (value))
  {
    return ecma_copy_value (value);
  }

  return value;
} /* ecma_copy_value_if_not_object */

/**
 * Increase reference counter of a value if it is an object.
 */
inline void JERRY_ATTR_ALWAYS_INLINE
ecma_ref_if_object (ecma_value_t value) /**< value description */
{
  if (ecma_is_value_object (value))
  {
    ecma_ref_object (ecma_get_object_from_value (value));
  }
} /* ecma_ref_if_object */

/**
 * Decrease reference counter of a value if it is an object.
 */
inline void JERRY_ATTR_ALWAYS_INLINE
ecma_deref_if_object (ecma_value_t value) /**< value description */
{
  if (ecma_is_value_object (value))
  {
    ecma_deref_object (ecma_get_object_from_value (value));
  }
} /* ecma_deref_if_object */

ecma_value_t ecma_update_float_number (ecma_value_t float_value, ecma_number_t new_number);
void ecma_value_assign_value (ecma_value_t *value_p, ecma_value_t ecma_value);
void ecma_value_assign_number (ecma_value_t *value_p, ecma_number_t ecma_number);
void ecma_free_value (ecma_value_t value);

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

void ecma_free_value_if_not_object (ecma_value_t value);

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

lit_magic_string_id_t ecma_get_typeof_lit_id (ecma_value_t value);

/* ecma-helpers-string.c */
#if ENABLED (JERRY_ESNEXT)
ecma_string_t *ecma_new_symbol_from_descriptor_string (ecma_value_t string_desc);
ecma_length_t ecma_op_advance_string_index (ecma_string_t *str_p, ecma_length_t index_num, bool is_unicode);
#endif /* ENABLED (JERRY_ESNEXT) */
#if ENABLED (JERRY_BUILTIN_MAP) || ENABLED (JERRY_BUILTIN_SET)
ecma_string_t *ecma_new_map_key_string (ecma_value_t value);
bool ecma_prop_name_is_map_key (ecma_string_t *string_p);
#endif /* ENABLED (JERRY_BUILTIN_MAP) || ENABLED (JERRY_BUILTIN_SET) */
ecma_string_t *ecma_new_ecma_string_from_utf8 (const lit_utf8_byte_t *string_p, lit_utf8_size_t string_size);
ecma_string_t *ecma_new_ecma_string_from_utf8_converted_to_cesu8 (const lit_utf8_byte_t *string_p,
                                                                  lit_utf8_size_t string_size);
ecma_string_t *ecma_new_ecma_external_string_from_cesu8 (const lit_utf8_byte_t *string_p, lit_utf8_size_t string_size,
                                                         ecma_object_native_free_callback_t free_cb);
ecma_string_t *ecma_new_ecma_string_from_code_unit (ecma_char_t code_unit);
#if ENABLED (JERRY_ESNEXT)
ecma_string_t *ecma_new_ecma_string_from_code_units (ecma_char_t first_code_unit, ecma_char_t second_code_unit);
#endif /* ENABLED (JERRY_ESNEXT) */
ecma_string_t *ecma_new_ecma_string_from_length (ecma_length_t index);
ecma_string_t *ecma_new_ecma_string_from_uint32 (uint32_t uint32_number);
ecma_string_t *ecma_new_non_direct_string_from_uint32 (uint32_t uint32_number);
ecma_string_t *ecma_get_ecma_string_from_uint32 (uint32_t uint32_number);
ecma_string_t *ecma_new_ecma_string_from_number (ecma_number_t num);
ecma_string_t *ecma_get_magic_string (lit_magic_string_id_t id);
ecma_string_t *ecma_append_chars_to_string (ecma_string_t *string1_p,
                                            const lit_utf8_byte_t *cesu8_string2_p,
                                            lit_utf8_size_t cesu8_string2_size,
                                            lit_utf8_size_t cesu8_string2_length);
ecma_string_t *ecma_concat_ecma_strings (ecma_string_t *string1_p, ecma_string_t *string2_p);
void ecma_ref_ecma_string (ecma_string_t *string_p);
void ecma_deref_ecma_string (ecma_string_t *string_p);
void ecma_destroy_ecma_string (ecma_string_t *string_p);
ecma_number_t ecma_string_to_number (const ecma_string_t *str_p);
uint32_t ecma_string_get_array_index (const ecma_string_t *str_p);

lit_utf8_size_t JERRY_ATTR_WARN_UNUSED_RESULT
ecma_string_copy_to_cesu8_buffer (const ecma_string_t *string_desc_p,
                                  lit_utf8_byte_t *buffer_p,
                                  lit_utf8_size_t buffer_size);
lit_utf8_size_t JERRY_ATTR_WARN_UNUSED_RESULT
ecma_string_copy_to_utf8_buffer (const ecma_string_t *string_desc_p,
                                 lit_utf8_byte_t *buffer_p,
                                 lit_utf8_size_t buffer_size);
lit_utf8_size_t
ecma_substring_copy_to_cesu8_buffer (const ecma_string_t *string_desc_p,
                                     lit_utf8_size_t start_pos,
                                     lit_utf8_size_t end_pos,
                                     lit_utf8_byte_t *buffer_p,
                                     lit_utf8_size_t buffer_size);
lit_utf8_size_t
ecma_substring_copy_to_utf8_buffer (const ecma_string_t *string_desc_p,
                                    lit_utf8_size_t start_pos,
                                    lit_utf8_size_t end_pos,
                                    lit_utf8_byte_t *buffer_p,
                                    lit_utf8_size_t buffer_size);
/**
 * Convert ecma-string's contents to a cesu-8 string and put it to the buffer.
 * It is the caller's responsibility to make sure that the string fits in the buffer.
 * Check if the size of the string is equal with the size of the buffer.
 */
inline void JERRY_ATTR_ALWAYS_INLINE
ecma_string_to_utf8_bytes (const ecma_string_t *string_desc_p, /**< ecma-string descriptor */
                           lit_utf8_byte_t *buffer_p, /**< destination buffer pointer
                                                       * (can be NULL if buffer_size == 0) */
                           lit_utf8_size_t buffer_size) /**< size of buffer */
{
  const lit_utf8_size_t size = ecma_string_copy_to_cesu8_buffer (string_desc_p, buffer_p, buffer_size);
  JERRY_ASSERT (size == buffer_size);
} /* ecma_string_to_utf8_bytes */

const lit_utf8_byte_t *ecma_string_get_chars (const ecma_string_t *string_p,
                                              lit_utf8_size_t *size_p,
                                              lit_utf8_size_t *length_p,
                                              lit_utf8_byte_t *uint32_buff_p,
                                              uint8_t *flags_p);

/**
 * Checks whether the string equals to the magic string id.
 *
 * @return true - if the string equals to the magic string id
 *         false - otherwise
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_compare_ecma_string_to_magic_id (const ecma_string_t *string_p, /**< property name */
                                      lit_magic_string_id_t id) /**< magic string id */
{
  return (string_p == ecma_get_magic_string (id));
} /* ecma_compare_ecma_string_to_magic_id */

/**
 * Checks whether ecma string is empty or not
 *
 * @return true - if the string is an empty string
 *         false - otherwise
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_string_is_empty (const ecma_string_t *string_p) /**< ecma-string */
{
  return ecma_compare_ecma_string_to_magic_id (string_p, LIT_MAGIC_STRING__EMPTY);
} /* ecma_string_is_empty */

/**
 * Checks whether the string equals to "length".
 *
 * @return true - if the string equals to "length"
 *         false - otherwise
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_string_is_length (const ecma_string_t *string_p) /**< property name */
{
  return ecma_compare_ecma_string_to_magic_id (string_p, LIT_MAGIC_STRING_LENGTH);
} /* ecma_string_is_length */

/**
 * Converts a property name into a string
 *
 * @return pointer to the converted ecma string
 */
static inline ecma_string_t * JERRY_ATTR_ALWAYS_INLINE
ecma_property_to_string (ecma_property_t property, /**< property name type */
                         jmem_cpointer_t prop_name_cp) /**< property name compressed pointer */
{
  uintptr_t property_string = ((uintptr_t) (property)) & (0x3 << ECMA_PROPERTY_NAME_TYPE_SHIFT);
  property_string = (property_string >> ECMA_STRING_TYPE_CONVERSION_SHIFT) | ECMA_TYPE_DIRECT_STRING;
  return (ecma_string_t *) (property_string | (((uintptr_t) prop_name_cp) << ECMA_DIRECT_STRING_SHIFT));
} /* ecma_property_to_string */

/**
 * Converts a string into a property name
 *
 * @return the compressed pointer part of the name
 */
inline jmem_cpointer_t JERRY_ATTR_ALWAYS_INLINE
ecma_string_to_property_name (ecma_string_t *prop_name_p, /**< property name */
                              ecma_property_t *name_type_p) /**< [out] property name type */
{
  if (ECMA_IS_DIRECT_STRING (prop_name_p))
  {
    *name_type_p = (ecma_property_t) ECMA_DIRECT_STRING_TYPE_TO_PROP_NAME_TYPE (prop_name_p);
    return (jmem_cpointer_t) ECMA_GET_DIRECT_STRING_VALUE (prop_name_p);
  }

  *name_type_p = ECMA_DIRECT_STRING_PTR << ECMA_PROPERTY_NAME_TYPE_SHIFT;

  ecma_ref_ecma_string (prop_name_p);

  jmem_cpointer_t prop_name_cp;
  ECMA_SET_NON_NULL_POINTER (prop_name_cp, prop_name_p);
  return prop_name_cp;
} /* ecma_string_to_property_name */

ecma_string_t *ecma_string_from_property_name (ecma_property_t property, jmem_cpointer_t prop_name_cp);

/**
 * Get hash code of property name
 *
 * @return hash code of property name
 */
inline lit_string_hash_t JERRY_ATTR_ALWAYS_INLINE
ecma_string_get_property_name_hash (ecma_property_t property, /**< property name type */
                                    jmem_cpointer_t prop_name_cp) /**< property name compressed pointer */
{
  if (ECMA_PROPERTY_GET_NAME_TYPE (property) == ECMA_DIRECT_STRING_PTR)
  {
    ecma_string_t *prop_name_p = ECMA_GET_NON_NULL_POINTER (ecma_string_t, prop_name_cp);
    return prop_name_p->u.hash;
  }

  return (lit_string_hash_t) prop_name_cp;
} /* ecma_string_get_property_name_hash */

uint32_t ecma_string_get_property_index (ecma_property_t property, jmem_cpointer_t prop_name_cp);
bool ecma_string_compare_to_property_name (ecma_property_t property, jmem_cpointer_t prop_name_cp,
                                           const ecma_string_t *string_p);
/**
 * Long path part of ecma-string to ecma-string comparison routine
 *
 * See also:
 *          ecma_compare_ecma_strings
 *
 * @return true - if strings are equal;
 *         false - otherwise
 */
bool
ecma_compare_ecma_strings_longpath (const ecma_string_t *string1_p, /**< ecma-string */
                                    const ecma_string_t *string2_p); /**< ecma-string */

/**
 * Compare two ecma-strings
 *
 * @return true - if strings are equal;
 *         false - otherwise
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_compare_ecma_strings (const ecma_string_t *string1_p, /**< ecma-string */
                           const ecma_string_t *string2_p) /**< ecma-string */
{
  JERRY_ASSERT (string1_p != NULL && string2_p != NULL);

  /* Fast paths first. */
  if (string1_p == string2_p)
  {
    return true;
  }

  /* Either string is direct, return with false. */
  if (ECMA_IS_DIRECT_STRING (((uintptr_t) string1_p) | ((uintptr_t) string2_p)))
  {
    return false;
  }

  /* Also compares uint32 values in descriptor. */
  if (string1_p->u.hash != string2_p->u.hash)
  {
    return false;
  }

  if (ECMA_STRING_GET_CONTAINER (string1_p) == ECMA_STRING_CONTAINER_UINT32_IN_DESC)
  {
    return ECMA_STRING_GET_CONTAINER (string2_p) == ECMA_STRING_CONTAINER_UINT32_IN_DESC;
  }

  return ecma_compare_ecma_strings_longpath (string1_p, string2_p);
} /* ecma_compare_ecma_strings */

/**
 * Compare two non-direct ecma-strings
 *
 * @return true - if strings are equal;
 *         false - otherwise
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_compare_ecma_non_direct_strings (const ecma_string_t *string1_p, /**< ecma-string */
                                      const ecma_string_t *string2_p) /**< ecma-string */
{
  JERRY_ASSERT (string1_p != NULL && string2_p != NULL);
  JERRY_ASSERT (!ECMA_IS_DIRECT_STRING (string1_p) && !ECMA_IS_DIRECT_STRING (string2_p));

  /* Fast paths first. */
  if (string1_p == string2_p)
  {
    return true;
  }

  if (string1_p->u.hash != string2_p->u.hash)
  {
    return false;
  }

  if (ECMA_STRING_GET_CONTAINER (string1_p) == ECMA_STRING_CONTAINER_UINT32_IN_DESC)
  {
    return ECMA_STRING_GET_CONTAINER (string2_p) == ECMA_STRING_CONTAINER_UINT32_IN_DESC;
  }

  return ecma_compare_ecma_strings_longpath (string1_p, string2_p);
} /* ecma_compare_ecma_non_direct_strings */

bool ecma_compare_ecma_strings_relational (const ecma_string_t *string1_p, const ecma_string_t *string2_p);
lit_utf8_size_t ecma_string_get_length (const ecma_string_t *string_p);
lit_utf8_size_t ecma_string_get_utf8_length (const ecma_string_t *string_p);
lit_utf8_size_t ecma_string_get_size (const ecma_string_t *string_p);
lit_utf8_size_t ecma_string_get_utf8_size (const ecma_string_t *string_p);
ecma_char_t ecma_string_get_char_at_pos (const ecma_string_t *string_p, lit_utf8_size_t index);

lit_magic_string_id_t ecma_get_string_magic (const ecma_string_t *string_p);

/**
 * Try to calculate hash of the ecma-string
 *
 * @return calculated hash
 */
inline lit_string_hash_t JERRY_ATTR_ALWAYS_INLINE
ecma_string_hash (const ecma_string_t *string_p) /**< ecma-string to calculate hash for */
{
  if (ECMA_IS_DIRECT_STRING (string_p))
  {
    return (lit_string_hash_t) ECMA_GET_DIRECT_STRING_VALUE (string_p);
  }

  return (lit_string_hash_t) string_p->u.hash;
} /* ecma_string_hash */

ecma_string_t *ecma_string_substr (const ecma_string_t *string_p, lit_utf8_size_t start_pos, lit_utf8_size_t end_pos);
const lit_utf8_byte_t *ecma_string_trim_front (const lit_utf8_byte_t *start_p, const lit_utf8_byte_t *end_p);
const lit_utf8_byte_t *ecma_string_trim_back (const lit_utf8_byte_t *start_p, const lit_utf8_byte_t *end_p);
void ecma_string_trim_helper (const lit_utf8_byte_t **utf8_str_p,
                              lit_utf8_size_t *utf8_str_size);
ecma_string_t *ecma_string_trim (const ecma_string_t *string_p);
#if ENABLED (JERRY_ESNEXT)
ecma_value_t ecma_string_pad (ecma_value_t original_string_p,
                              ecma_value_t max_length,
                              ecma_value_t fill_string,
                              bool pad_on_start);
#endif /* ENABLED (JERRY_ESNEXT) */

ecma_stringbuilder_t ecma_stringbuilder_create (void);
ecma_stringbuilder_t ecma_stringbuilder_create_from (ecma_string_t *string_p);
ecma_stringbuilder_t ecma_stringbuilder_create_raw (const lit_utf8_byte_t *data_p,
                                                    const lit_utf8_size_t data_size);
lit_utf8_size_t ecma_stringbuilder_get_size (ecma_stringbuilder_t *builder_p);
lit_utf8_byte_t *ecma_stringbuilder_get_data (ecma_stringbuilder_t *builder_p);
void ecma_stringbuilder_revert (ecma_stringbuilder_t *builder_p, const lit_utf8_size_t size);
void ecma_stringbuilder_append (ecma_stringbuilder_t *builder_p, const ecma_string_t *string_p);
void ecma_stringbuilder_append_magic (ecma_stringbuilder_t *builder_p, const lit_magic_string_id_t id);
void ecma_stringbuilder_append_raw (ecma_stringbuilder_t *builder_p,
                                    const lit_utf8_byte_t *data_p,
                                    const lit_utf8_size_t data_size);
void ecma_stringbuilder_append_codepoint (ecma_stringbuilder_t *builder_p, lit_code_point_t cp);
void ecma_stringbuilder_append_char (ecma_stringbuilder_t *builder_p, const ecma_char_t c);
void ecma_stringbuilder_append_byte (ecma_stringbuilder_t *builder_p, const lit_utf8_byte_t);
ecma_string_t *ecma_stringbuilder_finalize (ecma_stringbuilder_t *builder_p);
void ecma_stringbuilder_destroy (ecma_stringbuilder_t *builder_p);

/* ecma-helpers-number.c */
ecma_number_t ecma_number_pack (bool sign, uint32_t biased_exp, uint64_t fraction);
void ecma_number_unpack (ecma_number_t num, bool *sign_p, uint32_t *biased_exp_p, uint64_t *fraction_p);
ecma_number_t ecma_number_make_nan (void);
ecma_number_t ecma_number_make_infinity (bool sign);
bool ecma_number_is_nan (ecma_number_t num);
bool ecma_number_is_negative (ecma_number_t num);
bool ecma_number_is_zero (ecma_number_t num);
bool ecma_number_is_infinity (ecma_number_t num);
bool ecma_number_is_finite (ecma_number_t num);
ecma_number_t
ecma_number_make_from_sign_mantissa_and_exponent (bool sign, uint64_t mantissa, int32_t exponent);
ecma_number_t ecma_number_get_prev (ecma_number_t num);
ecma_number_t ecma_number_get_next (ecma_number_t num);
ecma_number_t ecma_number_trunc (ecma_number_t num);
ecma_number_t ecma_number_calc_remainder (ecma_number_t left_num, ecma_number_t right_num);
ecma_number_t ecma_number_pow (ecma_number_t x, ecma_number_t y);
ecma_value_t ecma_number_parse_int (const lit_utf8_byte_t *string_buff,
                                    lit_utf8_size_t string_buff_size,
                                    ecma_value_t radix);
ecma_value_t ecma_number_parse_float (const lit_utf8_byte_t *string_buff,
                                      lit_utf8_size_t string_buff_size);

/**
 * ECMA-integer number multiplication.
 *
 * @return number - result of multiplication.
 */
inline ecma_value_t JERRY_ATTR_ALWAYS_INLINE
ecma_integer_multiply (ecma_integer_value_t left_integer, /**< left operand */
                       ecma_integer_value_t right_integer) /**< right operand */
{
#if defined (__GNUC__) || defined (__clang__)
  /* Check if left_integer is power of 2 */
  if (JERRY_UNLIKELY ((left_integer & (left_integer - 1)) == 0))
  {
    /* Right shift right_integer with log2 (left_integer) */
    return ecma_make_integer_value (right_integer << (__builtin_ctz ((unsigned int) left_integer)));
  }
  else if (JERRY_UNLIKELY ((right_integer & (right_integer - 1)) == 0))
  {
    /* Right shift left_integer with log2 (right_integer) */
    return ecma_make_integer_value (left_integer << (__builtin_ctz ((unsigned int) right_integer)));
  }
#endif /* defined (__GNUC__) || defined (__clang__) */
  return ecma_make_integer_value (left_integer * right_integer);
} /* ecma_integer_multiply */

lit_utf8_size_t ecma_number_to_decimal (ecma_number_t num, lit_utf8_byte_t *out_digits_p, int32_t *out_decimal_exp_p);

/* ecma-helpers-collection.c */
ecma_collection_t *ecma_new_collection (void);
void ecma_collection_push_back (ecma_collection_t *collection_p, ecma_value_t value);
void ecma_collection_reserve (ecma_collection_t *collection_p, uint32_t count);
void ecma_collection_append (ecma_collection_t *collection_p, const ecma_value_t *buffer_p, uint32_t count);
void ecma_collection_destroy (ecma_collection_t *collection_p);
void ecma_collection_free (ecma_collection_t *collection_p);
void ecma_collection_free_if_not_object (ecma_collection_t *collection_p);
void ecma_collection_free_objects (ecma_collection_t *collection_p);
#if ENABLED (JERRY_ESNEXT)
void ecma_collection_free_template_literal (ecma_collection_t *collection_p);
#endif /* ENABLED (JERRY_ESNEXT) */
bool ecma_collection_check_duplicated_entries (ecma_collection_t *collection_p);
bool ecma_collection_has_string_value (ecma_collection_t *collection_p, ecma_string_t *string_p);

ecma_value_t *ecma_new_compact_collection (void);
ecma_value_t *ecma_compact_collection_push_back (ecma_value_t *compact_collection_p, ecma_value_t value);
ecma_value_t *ecma_compact_collection_shrink (ecma_value_t *compact_collection_p);
void ecma_compact_collection_free (ecma_value_t *compact_collection_p);

/* ecma-helpers.c */
ecma_object_t *ecma_create_object (ecma_object_t *prototype_object_p, size_t ext_object_size, ecma_object_type_t type);
ecma_object_t *ecma_create_decl_lex_env (ecma_object_t *outer_lexical_environment_p);
ecma_object_t *ecma_create_object_lex_env (ecma_object_t *outer_lexical_environment_p, ecma_object_t *binding_obj_p,
                                           ecma_lexical_environment_type_t type);

/**
 * Check if the object is lexical environment.
 *
 * @return true  - if object is a lexical environment
 *         false - otherwise
 */
inline bool JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_is_lexical_environment (const ecma_object_t *object_p) /**< object or lexical environment */
{
  JERRY_ASSERT (object_p != NULL);

  uint32_t full_type = object_p->type_flags_refs & (ECMA_OBJECT_FLAG_BUILT_IN_OR_LEXICAL_ENV | ECMA_OBJECT_TYPE_MASK);

  return full_type >= (ECMA_OBJECT_FLAG_BUILT_IN_OR_LEXICAL_ENV | ECMA_LEXICAL_ENVIRONMENT_TYPE_START);
} /* ecma_is_lexical_environment */

/**
 * Set value of [[Extensible]] object's internal property.
 */
inline void JERRY_ATTR_ALWAYS_INLINE
ecma_op_ordinary_object_set_extensible (ecma_object_t *object_p) /**< object */
{
  JERRY_ASSERT (object_p != NULL);
  JERRY_ASSERT (!ecma_is_lexical_environment (object_p));

  object_p->type_flags_refs = (uint16_t) (object_p->type_flags_refs | ECMA_OBJECT_FLAG_EXTENSIBLE);
} /* ecma_op_ordinary_object_set_extensible */

/**
 * Get object's internal implementation-defined type.
 *
 * @return type of the object (ecma_object_type_t)
 */
inline ecma_object_type_t JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_get_object_type (const ecma_object_t *object_p) /**< object */
{
  JERRY_ASSERT (object_p != NULL);
  JERRY_ASSERT (!ecma_is_lexical_environment (object_p));

  return (ecma_object_type_t) (object_p->type_flags_refs & ECMA_OBJECT_TYPE_MASK);
} /* ecma_get_object_type */

/**
 * Check if the object is a built-in object
 *
 * @return true  - if object is a built-in object
 *         false - otherwise
 */
inline bool JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_get_object_is_builtin (const ecma_object_t *object_p) /**< object */
{
  JERRY_ASSERT (object_p != NULL);
  JERRY_ASSERT (!ecma_is_lexical_environment (object_p));

  return (object_p->type_flags_refs & ECMA_OBJECT_FLAG_BUILT_IN_OR_LEXICAL_ENV) != 0;
} /* ecma_get_object_is_builtin */

/**
 * Set flag indicating whether the object is a built-in object
 */
inline void JERRY_ATTR_ALWAYS_INLINE
ecma_set_object_is_builtin (ecma_object_t *object_p) /**< object */
{
  JERRY_ASSERT (object_p != NULL);
  JERRY_ASSERT (!(object_p->type_flags_refs & ECMA_OBJECT_FLAG_BUILT_IN_OR_LEXICAL_ENV));
  JERRY_ASSERT ((object_p->type_flags_refs & ECMA_OBJECT_TYPE_MASK) < ECMA_LEXICAL_ENVIRONMENT_TYPE_START);

  object_p->type_flags_refs = (uint16_t) (object_p->type_flags_refs | ECMA_OBJECT_FLAG_BUILT_IN_OR_LEXICAL_ENV);
} /* ecma_set_object_is_builtin */

/**
 * Get the built-in ID of the object.
 * If the object is not builtin, return ECMA_BUILTIN_ID__COUNT
 *
 * @return the ID of the built-in
 */
inline uint8_t JERRY_ATTR_ALWAYS_INLINE
ecma_get_object_builtin_id (ecma_object_t *object_p) /**< object */
{
  if (!ecma_get_object_is_builtin (object_p))
  {
    return ECMA_BUILTIN_ID__COUNT;
  }

  ecma_built_in_props_t *built_in_props_p;
  ecma_object_type_t object_type = ecma_get_object_type (object_p);

  if (ECMA_BUILTIN_IS_EXTENDED_BUILT_IN (object_type))
  {
    built_in_props_p = &((ecma_extended_built_in_object_t *) object_p)->built_in;
  }
  else
  {
    built_in_props_p = &((ecma_extended_object_t *) object_p)->u.built_in;
  }

  return built_in_props_p->id;
} /* ecma_get_object_builtin_id */

/**
 * Get type of lexical environment.
 *
 * @return type of the lexical environment (ecma_lexical_environment_type_t)
 */
inline ecma_lexical_environment_type_t JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_get_lex_env_type (const ecma_object_t *object_p) /**< lexical environment */
{
  JERRY_ASSERT (object_p != NULL);
  JERRY_ASSERT (ecma_is_lexical_environment (object_p));

  return (ecma_lexical_environment_type_t) (object_p->type_flags_refs & ECMA_OBJECT_TYPE_MASK);
} /* ecma_get_lex_env_type */

/**
 * Get lexical environment's bound object.
 *
 * @return pointer to ecma object
 */
inline ecma_object_t *JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_get_lex_env_binding_object (const ecma_object_t *object_p) /**< object-bound lexical environment */
{
  JERRY_ASSERT (object_p != NULL);
  JERRY_ASSERT (ecma_is_lexical_environment (object_p));
#if ENABLED (JERRY_ESNEXT)
  JERRY_ASSERT (ecma_get_lex_env_type (object_p) == ECMA_LEXICAL_ENVIRONMENT_THIS_OBJECT_BOUND
                || ecma_get_lex_env_type (object_p) == ECMA_LEXICAL_ENVIRONMENT_HOME_OBJECT_BOUND);
#else /* !ENABLED (JERRY_ESNEXT) */
  JERRY_ASSERT (ecma_get_lex_env_type (object_p) == ECMA_LEXICAL_ENVIRONMENT_THIS_OBJECT_BOUND);
#endif /* ENABLED (JERRY_ESNEXT) */

  return ECMA_GET_NON_NULL_POINTER (ecma_object_t, object_p->u1.bound_object_cp);
} /* ecma_get_lex_env_binding_object */

ecma_object_t *ecma_clone_decl_lexical_environment (ecma_object_t *lex_env_p, bool copy_values);

ecma_property_value_t *
ecma_create_named_data_property (ecma_object_t *object_p, ecma_string_t *name_p, uint8_t prop_attributes,
                                 ecma_property_t **out_prop_p);
ecma_property_value_t *
ecma_create_named_accessor_property (ecma_object_t *object_p, ecma_string_t *name_p, ecma_object_t *get_p,
                                     ecma_object_t *set_p, uint8_t prop_attributes, ecma_property_t **out_prop_p);
ecma_property_t *
ecma_find_named_property (ecma_object_t *obj_p, ecma_string_t *name_p);
ecma_property_value_t *
ecma_get_named_data_property (ecma_object_t *obj_p, ecma_string_t *name_p);

void ecma_free_property (ecma_object_t *object_p, jmem_cpointer_t name_cp, ecma_property_t *property_p);

void ecma_delete_property (ecma_object_t *object_p, ecma_property_value_t *prop_value_p);

/**
 * Check whether the object contains a property
 */
void
ecma_assert_object_contains_the_property (const ecma_object_t *object_p, /**< ecma-object */
                                          const ecma_property_value_t *prop_value_p, /**< property value */
                                          ecma_property_types_t type); /**< expected property type */

/**
 * Assign value to named data property
 *
 * Note:
 *      value previously stored in the property is freed
 */
inline void JERRY_ATTR_ALWAYS_INLINE
ecma_named_data_property_assign_value (ecma_object_t *obj_p, /**< object */
                                       ecma_property_value_t *prop_value_p, /**< property value reference */
                                       ecma_value_t value) /**< value to assign */
{
  ecma_assert_object_contains_the_property (obj_p, prop_value_p, ECMA_PROPERTY_TYPE_NAMEDDATA);

  ecma_value_assign_value (&prop_value_p->value, value);
} /* ecma_named_data_property_assign_value */

ecma_getter_setter_pointers_t *
ecma_get_named_accessor_property (const ecma_property_value_t *prop_value_p);
void ecma_set_named_accessor_property_getter (ecma_object_t *object_p, ecma_property_value_t *prop_value_p,
                                              ecma_object_t *getter_p);
void ecma_set_named_accessor_property_setter (ecma_object_t *object_p, ecma_property_value_t *prop_value_p,
                                              ecma_object_t *setter_p);
/**
 * Get property's 'Writable' attribute value
 *
 * @return true - property is writable,
 *         false - otherwise
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_is_property_writable (ecma_property_t property) /**< property */
{
  JERRY_ASSERT (ECMA_PROPERTY_GET_TYPE (property) == ECMA_PROPERTY_TYPE_NAMEDDATA
                || ECMA_PROPERTY_GET_TYPE (property) == ECMA_PROPERTY_TYPE_VIRTUAL);

  return (property & ECMA_PROPERTY_FLAG_WRITABLE) != 0;
} /* ecma_is_property_writable */

void ecma_set_property_writable_attr (ecma_property_t *property_p, bool is_writable);

/**
 * Get property's 'Enumerable' attribute value
 *
 * @return true - property is enumerable,
 *         false - otherwise
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_is_property_enumerable (ecma_property_t property) /**< property */
{
  JERRY_ASSERT (ECMA_PROPERTY_GET_TYPE (property) == ECMA_PROPERTY_TYPE_NAMEDDATA
                || ECMA_PROPERTY_GET_TYPE (property) == ECMA_PROPERTY_TYPE_NAMEDACCESSOR
                || ECMA_PROPERTY_GET_TYPE (property) == ECMA_PROPERTY_TYPE_VIRTUAL);

  return (property & ECMA_PROPERTY_FLAG_ENUMERABLE) != 0;
} /* ecma_is_property_enumerable */

void ecma_set_property_enumerable_attr (ecma_property_t *property_p, bool is_enumerable);

/**
 * Get property's 'Configurable' attribute value
 *
 * @return true - property is configurable,
 *         false - otherwise
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_is_property_configurable (ecma_property_t property) /**< property */
{
  JERRY_ASSERT (ECMA_PROPERTY_GET_TYPE (property) == ECMA_PROPERTY_TYPE_NAMEDDATA
                || ECMA_PROPERTY_GET_TYPE (property) == ECMA_PROPERTY_TYPE_NAMEDACCESSOR
                || ECMA_PROPERTY_GET_TYPE (property) == ECMA_PROPERTY_TYPE_VIRTUAL);

  return (property & ECMA_PROPERTY_FLAG_CONFIGURABLE) != 0;
} /* ecma_is_property_configurable */

void ecma_set_property_configurable_attr (ecma_property_t *property_p, bool is_configurable);

#if ENABLED (JERRY_LCACHE)

/**
 * Check whether the property is registered in LCache
 *
 * @return true / false
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_is_property_lcached (ecma_property_t *property_p) /**< property */
{
  JERRY_ASSERT (ECMA_PROPERTY_GET_TYPE (*property_p) == ECMA_PROPERTY_TYPE_NAMEDDATA
                || ECMA_PROPERTY_GET_TYPE (*property_p) == ECMA_PROPERTY_TYPE_NAMEDACCESSOR
                || ECMA_PROPERTY_GET_TYPE (*property_p) == ECMA_PROPERTY_TYPE_INTERNAL);

  return (*property_p & ECMA_PROPERTY_FLAG_LCACHED) != 0;
} /* ecma_is_property_lcached */

/**
 * Set value of flag indicating whether the property is registered in LCache
 */
inline void JERRY_ATTR_ALWAYS_INLINE
ecma_set_property_lcached (ecma_property_t *property_p, /**< property */
                           bool is_lcached) /**< new value for lcached flag */
{
  JERRY_ASSERT (ECMA_PROPERTY_GET_TYPE (*property_p) == ECMA_PROPERTY_TYPE_NAMEDDATA
                || ECMA_PROPERTY_GET_TYPE (*property_p) == ECMA_PROPERTY_TYPE_NAMEDACCESSOR
                || ECMA_PROPERTY_GET_TYPE (*property_p) == ECMA_PROPERTY_TYPE_INTERNAL);

  if (is_lcached)
  {
    *property_p = (uint8_t) (*property_p | ECMA_PROPERTY_FLAG_LCACHED);
  }
  else
  {
    *property_p = (uint8_t) (*property_p & ~ECMA_PROPERTY_FLAG_LCACHED);
  }
} /* ecma_set_property_lcached */

#endif /* ENABLED (JERRY_LCACHE) */

ecma_property_descriptor_t ecma_make_empty_property_descriptor (void);
void ecma_free_property_descriptor (ecma_property_descriptor_t *prop_desc_p);

void ecma_ref_extended_primitive (ecma_extended_primitive_t *primitve_p);
void ecma_deref_error_reference (ecma_extended_primitive_t *error_ref_p);
#if ENABLED (JERRY_BUILTIN_BIGINT)
void ecma_deref_bigint (ecma_extended_primitive_t *bigint_p);
#endif /* ENABLED (JERRY_BUILTIN_BIGINT) */

ecma_value_t ecma_create_error_reference (ecma_value_t value, bool is_exception);
ecma_value_t ecma_create_error_reference_from_context (void);

/**
 * Create an error reference from a given object.
 *
 * Note:
 *   Reference of the value is taken.
 *
 * @return error reference value
 */
inline ecma_value_t JERRY_ATTR_ALWAYS_INLINE
ecma_create_error_object_reference (ecma_object_t *object_p) /**< referenced object */
{
  return ecma_create_error_reference (ecma_make_object_value (object_p), true);
} /* ecma_create_error_object_reference */

void ecma_raise_error_from_error_reference (ecma_value_t value);

void ecma_bytecode_ref (ecma_compiled_code_t *bytecode_p);
void ecma_bytecode_deref (ecma_compiled_code_t *bytecode_p);
ecma_value_t *ecma_compiled_code_resolve_arguments_start (const ecma_compiled_code_t *bytecode_header_p);
#if ENABLED (JERRY_ESNEXT)
ecma_value_t *ecma_compiled_code_resolve_function_name (const ecma_compiled_code_t *bytecode_header_p);
uint32_t ecma_compiled_code_resolve_extended_info (const ecma_compiled_code_t *bytecode_header_p);
ecma_collection_t *ecma_compiled_code_get_tagged_template_collection (const ecma_compiled_code_t *bytecode_header_p);
#endif /* ENABLED (JERRY_ESNEXT) */
ecma_value_t ecma_get_resource_name (const ecma_compiled_code_t *bytecode_p);
#if (JERRY_STACK_LIMIT != 0)
uintptr_t ecma_get_current_stack_usage (void);
#endif /* (JERRY_STACK_LIMIT != 0) */

/* ecma-helpers-external-pointers.c */
bool ecma_create_native_pointer_property (ecma_object_t *obj_p, void *native_p, void *info_p);
ecma_native_pointer_t *ecma_get_native_pointer_value (ecma_object_t *obj_p, void *info_p);
bool ecma_delete_native_pointer_property (ecma_object_t *obj_p, void *info_p);

/* ecma-helpers-conversion.c */
ecma_number_t ecma_utf8_string_to_number (const lit_utf8_byte_t *str_p, lit_utf8_size_t str_size,
                                          uint32_t option);
lit_utf8_size_t ecma_uint32_to_utf8_string (uint32_t value, lit_utf8_byte_t *out_buffer_p, lit_utf8_size_t buffer_size);
uint32_t ecma_number_to_uint32 (ecma_number_t num);
int32_t ecma_number_to_int32 (ecma_number_t num);
lit_utf8_size_t ecma_number_to_utf8_string (ecma_number_t num, lit_utf8_byte_t *buffer_p, lit_utf8_size_t buffer_size);

/* ecma-helpers-errol.c */
lit_utf8_size_t ecma_errol0_dtoa (double val, lit_utf8_byte_t *buffer_p, int32_t *exp_p);

/**
 * @}
 * @}
 */

#endif /* !ECMA_HELPERS_H */
