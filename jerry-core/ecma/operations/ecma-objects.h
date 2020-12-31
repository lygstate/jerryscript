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

#ifndef ECMA_OBJECTS_H
#define ECMA_OBJECTS_H

#include "ecma-builtins.h"
#include "ecma-conversion.h"
#include "ecma-proxy-object.h"
#include "ecma-globals.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmaobjectsinternalops ECMA objects' operations
 * @{
 */

ecma_property_t ecma_op_object_get_own_property (ecma_object_t *object_p, ecma_string_t *property_name_p,
                                                 ecma_property_ref_t *property_ref_p, uint32_t options);

/**
 * Checks whether an object (excluding prototypes) has a named property
 *
 * @return true - if property is found
 *         false - otherwise
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_op_ordinary_object_has_own_property (ecma_object_t *object_p, /**< the object */
                                          ecma_string_t *property_name_p) /**< property name */
{
  JERRY_ASSERT (!ECMA_OBJECT_IS_PROXY (object_p));

  ecma_property_t property = ecma_op_object_get_own_property (object_p,
                                                              property_name_p,
                                                              NULL,
                                                              ECMA_PROPERTY_GET_NO_OPTIONS);

  return property != ECMA_PROPERTY_TYPE_NOT_FOUND && property != ECMA_PROPERTY_TYPE_NOT_FOUND_AND_STOP;
} /* ecma_op_ordinary_object_has_own_property */

/**
 * Ordinary object [[GetPrototypeOf]] operation
 *
 * See also:
 *          ECMAScript v6, 9.1.1
 *
 * @return the value of the [[Prototype]] internal slot of the given object.
 */
inline jmem_cpointer_t JERRY_ATTR_ALWAYS_INLINE
ecma_op_ordinary_object_get_prototype_of (ecma_object_t *obj_p) /**< object */
{
  JERRY_ASSERT (!ecma_is_lexical_environment (obj_p));
  JERRY_ASSERT (!ECMA_OBJECT_IS_PROXY (obj_p));

  return obj_p->u2.prototype_cp;
} /* ecma_op_ordinary_object_get_prototype_of */

/**
 * Generic [[HasProperty]] operation
 *
 * See also:
 *          ECMAScript v6, 9.1.7.1
 *
 * @return ECMA_VALUE_ERROR - if the operation fails
 *         ECMA_VALUE_{TRUE_FALSE} - whether the property is found
 */
inline ecma_value_t JERRY_ATTR_ALWAYS_INLINE
ecma_op_object_has_property (ecma_object_t *object_p, /**< the object */
                             ecma_string_t *property_name_p) /**< property name */
{
  while (true)
  {
#if ENABLED (JERRY_BUILTIN_PROXY)
    if (ECMA_OBJECT_IS_PROXY (object_p))
    {
      return ecma_proxy_object_has (object_p, property_name_p);
    }
#endif /* ENABLED (JERRY_BUILTIN_PROXY) */

    /* 2 - 3. */
    if (ecma_op_ordinary_object_has_own_property (object_p, property_name_p))
    {
      return ECMA_VALUE_TRUE;
    }

    jmem_cpointer_t proto_cp = ecma_op_ordinary_object_get_prototype_of (object_p);

    /* 7. */
    if (proto_cp == JMEM_CP_NULL)
    {
      return ECMA_VALUE_FALSE;
    }

    object_p = ECMA_GET_NON_NULL_POINTER (ecma_object_t, proto_cp);
  }
} /* ecma_op_object_has_property */

ecma_value_t ecma_op_object_find_own (ecma_value_t base_value, ecma_object_t *object_p, ecma_string_t *property_name_p);
ecma_value_t ecma_op_object_find (ecma_object_t *object_p, ecma_string_t *property_name_p);
ecma_value_t ecma_op_object_find_by_index (ecma_object_t *object_p, ecma_length_t index);
ecma_value_t ecma_op_object_get (ecma_object_t *object_p, ecma_string_t *property_name_p);
ecma_value_t ecma_op_object_get_with_receiver (ecma_object_t *object_p, ecma_string_t *property_name_p,
                                               ecma_value_t receiver);
ecma_value_t ecma_op_object_get_length (ecma_object_t *object_p, ecma_length_t *length_p);
ecma_value_t ecma_op_object_get_by_index (ecma_object_t *object_p, ecma_length_t index);
ecma_value_t ecma_op_object_get_by_magic_id (ecma_object_t *object_p, lit_magic_string_id_t property_id);
#if ENABLED (JERRY_ESNEXT)
ecma_string_t *ecma_op_get_global_symbol (lit_magic_string_id_t property_id);
ecma_value_t ecma_op_object_get_by_symbol_id (ecma_object_t *object_p, lit_magic_string_id_t property_id);
ecma_value_t ecma_op_get_method_by_symbol_id (ecma_value_t value, lit_magic_string_id_t symbol_id);
ecma_value_t ecma_op_get_method_by_magic_id (ecma_value_t value, lit_magic_string_id_t magic_id);
#endif /* ENABLED (JERRY_ESNEXT) */
ecma_value_t ecma_op_object_put_with_receiver (ecma_object_t *object_p, ecma_string_t *property_name_p,
                                               ecma_value_t value, ecma_value_t receiver, bool is_throw);
ecma_value_t ecma_op_object_put (ecma_object_t *object_p, ecma_string_t *property_name_p, ecma_value_t value,
                                 bool is_throw);
ecma_value_t ecma_op_object_put_with_receiver (ecma_object_t *object_p, ecma_string_t *property_name_p,
                                               ecma_value_t value, ecma_value_t receiver, bool is_throw);
ecma_value_t ecma_op_object_put_by_index (ecma_object_t *object_p, ecma_length_t index,
                                          ecma_value_t value, bool is_throw);
ecma_value_t ecma_op_object_delete (ecma_object_t *obj_p, ecma_string_t *property_name_p, bool is_throw);
ecma_value_t ecma_op_object_delete_by_index (ecma_object_t *obj_p, ecma_length_t index, bool is_throw);
ecma_value_t ecma_op_object_default_value (ecma_object_t *obj_p, ecma_preferred_type_hint_t hint);
ecma_value_t ecma_op_object_define_own_property (ecma_object_t *obj_p, ecma_string_t *property_name_p,
                                                 const ecma_property_descriptor_t *property_desc_p);
ecma_value_t ecma_op_object_get_own_property_descriptor (ecma_object_t *object_p, ecma_string_t *property_name_p,
                                                         ecma_property_descriptor_t *prop_desc_p);
ecma_value_t ecma_op_object_has_instance (ecma_object_t *obj_p, ecma_value_t value);
ecma_value_t ecma_op_object_is_prototype_of (ecma_object_t *base_p, ecma_object_t *target_p);
ecma_collection_t * ecma_op_object_get_enumerable_property_names (ecma_object_t *obj_p,
                                                                  ecma_enumerable_property_names_options_t option);
ecma_collection_t *ecma_op_object_own_property_keys (ecma_object_t *obj_p);
ecma_collection_t *ecma_op_object_enumerate (ecma_object_t *obj_p);

lit_magic_string_id_t ecma_object_get_class_name (ecma_object_t *obj_p);

/**
 * Get value of an object if the class matches
 *
 * @return value of the object if the class matches
 *         ECMA_VALUE_NOT_FOUND otherwise
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_object_class_is (ecma_object_t *object_p, /**< object */
                      uint32_t class_id) /**< class id */
{
  if (ecma_get_object_type (object_p) == ECMA_OBJECT_TYPE_CLASS)
  {
    ecma_extended_object_t *ext_object_p = (ecma_extended_object_t *) object_p;

    if (ext_object_p->u.class_prop.class_id == class_id)
    {
      return true;
    }
  }

  return false;
} /* ecma_object_class_is */

/**
 * Checks if the given argument has [[RegExpMatcher]] internal slot
 *
 * @return true - if the given argument is a regexp
 *         false - otherwise
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_object_is_regexp_object (ecma_value_t arg) /**< argument */
{
  return (ecma_is_value_object (arg)
          && ecma_object_class_is (ecma_get_object_from_value (arg), LIT_MAGIC_STRING_REGEXP_UL));
} /* ecma_object_is_regexp_object */

ecma_value_t ecma_op_invoke (ecma_value_t object, ecma_string_t *property_name_p, ecma_value_t *args_p,
                             uint32_t args_len);

#if ENABLED (JERRY_ESNEXT)
ecma_value_t ecma_op_is_concat_spreadable (ecma_value_t arg);
ecma_value_t ecma_op_is_regexp (ecma_value_t arg);
ecma_value_t ecma_op_species_constructor (ecma_object_t *this_value, ecma_builtin_id_t default_constructor_id);

/**
 * 7.3.18 Abstract operation Invoke when property name is a magic string
 *
 * @return ecma_value result of the invoked function or raised error
 *         note: returned value must be freed with ecma_free_value
 */
inline ecma_value_t JERRY_ATTR_ALWAYS_INLINE
ecma_op_invoke_by_symbol_id (ecma_value_t object, /**< Object value */
                             lit_magic_string_id_t symbol_id, /**< Symbol ID */
                             ecma_value_t *args_p, /**< Argument list */
                             uint32_t args_len) /**< Argument list length */
{
  ecma_string_t *symbol_p = ecma_op_get_global_symbol (symbol_id);
  ecma_value_t ret_value = ecma_op_invoke (object, symbol_p, args_p, args_len);
  ecma_deref_ecma_string (symbol_p);

  return ret_value;
} /* ecma_op_invoke_by_symbol_id */

#endif /* ENABLED (JERRY_ESNEXT) */

/**
 * 7.3.18 Abstract operation Invoke when property name is a magic string
 *
 * @return ecma_value result of the invoked function or raised error
 *         note: returned value must be freed with ecma_free_value
 */
inline ecma_value_t JERRY_ATTR_ALWAYS_INLINE
ecma_op_invoke_by_magic_id (ecma_value_t object, /**< Object value */
                            lit_magic_string_id_t magic_string_id, /**< Magic string ID */
                            ecma_value_t *args_p, /**< Argument list */
                            uint32_t args_len) /**< Argument list length */
{
  return ecma_op_invoke (object, ecma_get_magic_string (magic_string_id), args_p, args_len);
} /* ecma_op_invoke_by_magic_id */

ecma_value_t ecma_op_ordinary_object_set_prototype_of (ecma_object_t *base_p, ecma_value_t proto);
bool JERRY_ATTR_PURE ecma_op_ordinary_object_is_extensible (ecma_object_t *object_p);
void ecma_op_ordinary_object_prevent_extensions (ecma_object_t *object_p);

#if ENABLED (JERRY_BUILTIN_PROXY)
ecma_value_t ecma_op_get_own_property_descriptor (ecma_value_t target,
                                                  ecma_string_t *property_name_p,
                                                  ecma_property_descriptor_t *prop_desc_p);
#endif /* ENABLED (JERRY_BUILTIN_PROXY) */

/**
 * @}
 * @}
 */

#endif /* !ECMA_OBJECTS_H */
