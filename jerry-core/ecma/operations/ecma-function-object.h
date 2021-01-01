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

#ifndef ECMA_FUNCTION_OBJECT_H
#define ECMA_FUNCTION_OBJECT_H

#include "ecma-globals.h"
#include "ecma-builtins.h"
#include "ecma-helpers.h"
#include "ecma-exceptions.h"
#include "ecma-builtin-handlers.h"
#include "vm.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmafunctionobject ECMA Function object related routines
 * @{
 */

#if ENABLED (JERRY_ESNEXT)
ecma_value_t ecma_op_function_form_name (ecma_string_t *prop_name_p, char *prefix_p, lit_utf8_size_t prefix_size);
#endif /* ENABLED (JERRY_ESNEXT) */

bool ecma_op_is_callable (ecma_value_t value);

/**
 * IsCallable operation.
 *
 * See also: ECMA-262 v5, 9.11
 *
 * @return true - if the given object is callable;
 *         false - otherwise
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_op_object_is_callable (ecma_object_t *obj_p) /**< ecma object */
{
  JERRY_ASSERT (!ecma_is_lexical_environment (obj_p));

  const ecma_object_type_t type = ecma_get_object_type (obj_p);

#if ENABLED (JERRY_BUILTIN_PROXY)
  if (ECMA_OBJECT_TYPE_IS_PROXY (type))
  {
    return ECMA_GET_FIRST_BIT_FROM_POINTER_TAG (obj_p->u1.property_list_cp) != 0;
  }
#endif /* ENABLED (JERRY_BUILTIN_PROXY) */

  return type >= ECMA_OBJECT_TYPE_FUNCTION;
} /* ecma_op_object_is_callable */

bool ecma_is_constructor (ecma_value_t value);
bool ecma_object_is_constructor (ecma_object_t *obj_p);

/**
 * Special constant indicating that the value is a valid constructor
 *
 * Use after the ecma_*_check_constructor calls.
 */
#define ECMA_IS_VALID_CONSTRUCTOR ((char *) 0x1)

char *ecma_object_check_constructor (ecma_object_t *obj_p);

/**
 * Implement IsConstructor abstract operation.
 *
 * @return ECMA_IS_VALID_CONSTRUCTOR - if the input value is a constructor.
 *         any other value - if the input value is not a valid constructor, the pointer contains the error message.
 */
inline char *JERRY_ATTR_ALWAYS_INLINE
ecma_check_constructor (ecma_value_t value) /**< ecma object */
{
  if (!ecma_is_value_object (value))
  {
    return ECMA_ERR_MSG ("Invalid type for constructor call.");
  }

  return ecma_object_check_constructor (ecma_get_object_from_value (value));
} /* ecma_check_constructor */

ecma_object_t *
ecma_op_create_simple_function_object (ecma_object_t *scope_p, const ecma_compiled_code_t *bytecode_data_p);

ecma_object_t *
ecma_op_create_external_function_object (ecma_native_handler_t handler_cb);

/**
 * Get compiled code of a function object.
 *
 * @return compiled code
 */
inline const ecma_compiled_code_t * JERRY_ATTR_ALWAYS_INLINE
ecma_op_function_get_compiled_code (ecma_extended_object_t *function_p) /**< function pointer */
{
#if ENABLED (JERRY_SNAPSHOT_EXEC)
  if (JERRY_LIKELY (function_p->u.function.bytecode_cp != ECMA_NULL_POINTER))
  {
    return ECMA_GET_INTERNAL_VALUE_POINTER (const ecma_compiled_code_t,
                                            function_p->u.function.bytecode_cp);
  }

  return ((ecma_static_function_t *) function_p)->bytecode_p;
#else /* !ENABLED (JERRY_SNAPSHOT_EXEC) */
  return ECMA_GET_INTERNAL_VALUE_POINTER (const ecma_compiled_code_t,
                                          function_p->u.function.bytecode_cp);
#endif /* ENABLED (JERRY_SNAPSHOT_EXEC) */
} /* ecma_op_function_get_compiled_code */

#if ENABLED (JERRY_BUILTIN_REALMS)

/**
 * Get realm from a byte code.
 *
 * Note:
 *   Does not increase the reference counter.
 *
 * @return pointer to realm (global) object
 */
inline ecma_global_object_t * JERRY_ATTR_ALWAYS_INLINE
ecma_op_function_get_realm (const ecma_compiled_code_t *bytecode_header_p) /**< byte code header */
{
  ecma_value_t realm_value;

  if (bytecode_header_p->status_flags & CBC_CODE_FLAGS_UINT16_ARGUMENTS)
  {
    cbc_uint16_arguments_t *args_p = (cbc_uint16_arguments_t *) bytecode_header_p;
    realm_value = args_p->realm_value;
  }
  else
  {
    cbc_uint8_arguments_t *args_p = (cbc_uint8_arguments_t *) bytecode_header_p;
    realm_value = args_p->realm_value;
  }

#if ENABLED (JERRY_SNAPSHOT_EXEC)
  if (JERRY_LIKELY (realm_value != JMEM_CP_NULL))
  {
    return ECMA_GET_INTERNAL_VALUE_POINTER (ecma_global_object_t, realm_value);
  }

  return (ecma_global_object_t *) ecma_builtin_get_global ();
#else /* !ENABLED (JERRY_SNAPSHOT_EXEC) */
  return ECMA_GET_INTERNAL_VALUE_POINTER (ecma_global_object_t, realm_value);
#endif /* ENABLED (JERRY_SNAPSHOT_EXEC) */
} /* ecma_op_function_get_realm */

ecma_global_object_t *
ecma_op_function_get_function_realm (ecma_object_t *func_obj_p);
#endif /* ENABLED (JERRY_BUILTIN_REALMS) */

ecma_value_t
ecma_op_create_dynamic_function (const ecma_value_t *arguments_list_p,
                                 uint32_t arguments_list_len,
                                 ecma_parse_opts_t opts);

#if ENABLED (JERRY_ESNEXT)
ecma_value_t
ecma_op_function_get_super_constructor (ecma_object_t *func_obj_p);

ecma_object_t *
ecma_op_create_any_function_object (ecma_object_t *scope_p, const ecma_compiled_code_t *bytecode_data_p);

ecma_object_t *
ecma_op_create_arrow_function_object (ecma_object_t *scope_p, const ecma_compiled_code_t *bytecode_data_p,
                                      ecma_value_t this_binding);

ecma_object_t *
ecma_op_create_native_handler (ecma_native_handler_id_t id, size_t object_size);

#endif /* ENABLED (JERRY_ESNEXT) */

ecma_object_t *
ecma_op_get_prototype_from_constructor (ecma_object_t *ctor_obj_p, ecma_builtin_id_t default_proto_id);

ecma_value_t
ecma_op_function_has_instance (ecma_object_t *func_obj_p, ecma_value_t value);

ecma_value_t
ecma_op_function_call (ecma_object_t *func_obj_p, ecma_value_t this_arg_value,
                       const ecma_value_t *arguments_list_p, uint32_t arguments_list_len);

ecma_value_t
ecma_op_function_construct (ecma_object_t *func_obj_p, ecma_object_t *new_target_p,
                            const ecma_value_t *arguments_list_p, uint32_t arguments_list_len);

ecma_property_t *
ecma_op_function_try_to_lazy_instantiate_property (ecma_object_t *object_p, ecma_string_t *property_name_p);

ecma_property_t *
ecma_op_external_function_try_to_lazy_instantiate_property (ecma_object_t *object_p, ecma_string_t *property_name_p);

ecma_property_t *
ecma_op_bound_function_try_to_lazy_instantiate_property (ecma_object_t *object_p, ecma_string_t *property_name_p);

void
ecma_op_function_list_lazy_property_names (ecma_object_t *object_p,
                                           ecma_collection_t *prop_names_p,
                                           ecma_property_counter_t *prop_counter_p);

void
ecma_op_external_function_list_lazy_property_names (ecma_object_t *object_p,
                                                    ecma_collection_t *prop_names_p,
                                                    ecma_property_counter_t *prop_counter_p);

void
ecma_op_bound_function_list_lazy_property_names (ecma_object_t *object_p,
                                                 ecma_collection_t *prop_names_p,
                                                 ecma_property_counter_t *prop_counter_p);

/**
 * @}
 * @}
 */

#endif /* !ECMA_FUNCTION_OBJECT_H */
