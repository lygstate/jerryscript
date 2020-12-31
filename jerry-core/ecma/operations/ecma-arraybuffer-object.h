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

#ifndef ECMA_ARRAYBUFFER_OBJECT_H
#define ECMA_ARRAYBUFFER_OBJECT_H

#include "ecma-globals.h"
#include "ecma-objects.h"

#if ENABLED (JERRY_BUILTIN_TYPEDARRAY)

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmaarraybufferobject ECMA ArrayBuffer object related routines
 * @{
 */

ecma_value_t
ecma_op_create_arraybuffer_object (const ecma_value_t *, uint32_t);

/**
 * Helper functions for arraybuffer.
 */
ecma_object_t *
ecma_arraybuffer_new_object (uint32_t lengh);
ecma_object_t *
ecma_arraybuffer_new_object_external (uint32_t length,
                                      void *buffer_p,
                                      ecma_object_native_free_callback_t free_cb);

/**
 * Helper function: check if the target ArrayBuffer is detached
 *
 * @return true - if value is an detached ArrayBuffer object
 *         false - otherwise
 */
inline bool JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_arraybuffer_is_detached (ecma_object_t *object_p) /**< pointer to the ArrayBuffer object */
{
  JERRY_ASSERT (ecma_object_class_is (object_p, LIT_MAGIC_STRING_ARRAY_BUFFER_UL));

  return (((ecma_extended_object_t *) object_p)->u.class_prop.extra_info & ECMA_ARRAYBUFFER_DETACHED) != 0;
} /* ecma_arraybuffer_is_detached */

/**
 * Helper function: return the pointer to the data buffer inside the arraybuffer object
 *
 * @return pointer to the data buffer
 */
inline lit_utf8_byte_t * JERRY_ATTR_PURE JERRY_ATTR_ALWAYS_INLINE
ecma_arraybuffer_get_buffer (ecma_object_t *object_p) /**< pointer to the ArrayBuffer object */
{
  JERRY_ASSERT (ecma_object_class_is (object_p, LIT_MAGIC_STRING_ARRAY_BUFFER_UL));

  ecma_extended_object_t *ext_object_p = (ecma_extended_object_t *) object_p;

  if (ECMA_ARRAYBUFFER_HAS_EXTERNAL_MEMORY (ext_object_p))
  {
    ecma_arraybuffer_external_info *array_p = (ecma_arraybuffer_external_info *) ext_object_p;
    JERRY_ASSERT (!ecma_arraybuffer_is_detached (object_p) || array_p->buffer_p == NULL);
    return (lit_utf8_byte_t *) array_p->buffer_p;
  }
  else if (ext_object_p->u.class_prop.extra_info & ECMA_ARRAYBUFFER_DETACHED)
  {
    return NULL;
  }

  return (lit_utf8_byte_t *) (ext_object_p + 1);
} /* ecma_arraybuffer_get_buffer */

uint32_t JERRY_ATTR_PURE
ecma_arraybuffer_get_length (ecma_object_t *obj_p);

/**
 * ArrayBuffer object detaching operation
 *
 * See also: ES2015 24.1.1.3
 *
 * @return true - if detach op succeeded
 *         false - otherwise
 */
inline bool JERRY_ATTR_ALWAYS_INLINE
ecma_arraybuffer_detach (ecma_object_t *object_p) /**< pointer to the ArrayBuffer object */
{
  JERRY_ASSERT (ecma_object_class_is (object_p, LIT_MAGIC_STRING_ARRAY_BUFFER_UL));

  if (ecma_arraybuffer_is_detached (object_p))
  {
    return false;
  }

  ecma_extended_object_t *ext_object_p = (ecma_extended_object_t *) object_p;
  ext_object_p->u.class_prop.extra_info |= ECMA_ARRAYBUFFER_DETACHED;

  if (ECMA_ARRAYBUFFER_HAS_EXTERNAL_MEMORY (ext_object_p))
  {
    ecma_arraybuffer_external_info *array_p = (ecma_arraybuffer_external_info *) ext_object_p;

    if (array_p->free_cb != NULL)
    {
      array_p->free_cb (array_p->buffer_p);
      array_p->free_cb = NULL;
    }

    ext_object_p->u.class_prop.u.length = 0;
    array_p->buffer_p = NULL;
  }

  return true;
} /* ecma_arraybuffer_detach */

bool
ecma_is_arraybuffer (ecma_value_t val);

/**
 * @}
 * @}
 */

#endif /* ENABLED (JERRY_BUILTIN_TYPEDARRAY) */
#endif /* !ECMA_ARRAYBUFFER_OBJECT_H */
