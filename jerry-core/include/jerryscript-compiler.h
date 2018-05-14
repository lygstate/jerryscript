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

#ifndef JERRYSCRIPT_COMPILER_H
#define JERRYSCRIPT_COMPILER_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** \addtogroup jerry-compiler Jerry compiler compatibility components
 * @{
 */

#ifdef __GNUC__

/*
 * Compiler-specific macros relevant for GCC.
 */
#define JERRY_ATTR_ALIGNED(ALIGNMENT) __attribute__((aligned(ALIGNMENT)))
#define JERRY_ATTR_ALWAYS_INLINE inline __attribute__((always_inline))
#define JERRY_ATTR_CONST __attribute__((const))
#define JERRY_ATTR_DEPRECATED __attribute__((deprecated))
#define JERRY_ATTR_FORMAT(...) __attribute__((format(__VA_ARGS__)))
#define JERRY_ATTR_HOT __attribute__((hot))
#define JERRY_ATTR_NOINLINE __attribute__((noinline))
#define JERRY_ATTR_NORETURN __attribute__((noreturn))
#define JERRY_ATTR_PURE __attribute__((pure))
#define JERRY_ATTR_SECTION(SECTION) __attribute__((section(SECTION)))
#define JERRY_ATTR_WARN_UNUSED_RESULT __attribute__((warn_unused_result))

#define JERRY_LIKELY(x) __builtin_expect(!!(x), 1)
#define JERRY_UNLIKELY(x) __builtin_expect(!!(x), 0)

#endif /* __GNUC__ */

#ifdef _MSC_VER
void * __cdecl _alloca (size_t _Size);

#define JERRY_ATTR_ALWAYS_INLINE __forceinline
#define JERRY_ATTR_DEPRECATED __declspec(deprecated)
#define JERRY_ATTR_NOINLINE __declspec(noinline)
#define JERRY_ATTR_NORETURN __declspec(noreturn)

#define JERRY_ALLOCA(type, name, size) type *name = (type *) (_alloca (sizeof (type) * size))
#endif /* _MSC_VER */

/*
 * Default empty definitions for all compiler-specific macros. Define any of
 * these in a guarded block above (e.g., as for GCC) to fine tune compilation
 * for your own compiler. */

/*
 * Attributes
 */
#ifndef JERRY_ATTR_ALIGNED
#define JERRY_ATTR_ALIGNED(ALIGNMENT)
#endif /* !JERRY_ATTR_ALIGNED */

#ifndef JERRY_ATTR_ALWAYS_INLINE
#define JERRY_ATTR_ALWAYS_INLINE
#endif /* !JERRY_ATTR_ALWAYS_INLINE */

#ifndef JERRY_ATTR_CONST
#define JERRY_ATTR_CONST
#endif /* !JERRY_ATTR_CONST */

#ifndef JERRY_ATTR_DEPRECATED
#define JERRY_ATTR_DEPRECATED
#endif /* !JERRY_ATTR_DEPRECATED */

#ifndef JERRY_ATTR_FORMAT
#define JERRY_ATTR_FORMAT(...)
#endif /* !JERRY_ATTR_FORMAT */

#ifndef JERRY_ATTR_HOT
#define JERRY_ATTR_HOT
#endif /* !JERRY_ATTR_HOT */

#ifndef JERRY_ATTR_NOINLINE
#define JERRY_ATTR_NOINLINE
#endif /* !JERRY_ATTR_NOINLINE */

#ifndef JERRY_ATTR_NORETURN
#define JERRY_ATTR_NORETURN
#endif /* !JERRY_ATTR_NORETURN */

#ifndef JERRY_ATTR_PURE
#define JERRY_ATTR_PURE
#endif /* !JERRY_ATTR_PURE */

#ifndef JERRY_ATTR_SECTION
#define JERRY_ATTR_SECTION(SECTION)
#endif /* !JERRY_ATTR_SECTION */

#ifndef JERRY_ATTR_WARN_UNUSED_RESULT
#define JERRY_ATTR_WARN_UNUSED_RESULT
#endif /* !JERRY_ATTR_WARN_UNUSED_RESULT */

/*
 * Condition likeliness, unlikeliness
 */
#ifndef JERRY_LIKELY
#define JERRY_LIKELY(x) (x)
#endif /* !JERRY_LIKELY */

#ifndef JERRY_UNLIKELY
#define JERRY_UNLIKELY(x) (x)
#endif /* !JERRY_UNLIKELY */

#ifndef JERRY_ALLOCA
#define JERRY_ALLOCA(type, name, size) type name[size]
#endif /* JERRY_ALLOCA */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* !JERRYSCRIPT_COMPILER_H */
