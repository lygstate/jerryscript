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

#ifndef JRT_H
#define JRT_H

#if !defined (_XOPEN_SOURCE) || _XOPEN_SOURCE < 500
#undef _XOPEN_SOURCE
/* Required macro for sleep functions (nanosleep or usleep) */
#define _XOPEN_SOURCE 500
#endif

#include <stdio.h>
#include <string.h>

#include "jerryscript-port.h"
#include "jrt-types.h"

/*
 * Normally compilers store const(ant)s in ROM. Thus saving RAM.
 * But if your compiler does not support it then the directive below can force it.
 *
 * For the moment it is mainly meant for the following targets:
 *      - ESP8266
 */
#ifndef JERRY_CONST_DATA
# define JERRY_CONST_DATA
#endif /* JERRY_CONST_DATA */

/*
 * Constants
 */
#define JERRY_BITSINBYTE 8

/*
 * Make sure unused parameters, variables, or expressions trigger no compiler warning.
 */
#define JERRY_UNUSED(x) ((void) (x))

/*
 * Asserts
 *
 * Warning:
 *         Don't use JERRY_STATIC_ASSERT in headers, because
 *         __LINE__ may be the same for asserts in a header
 *         and in an implementation file.
 */
#define JERRY_STATIC_ASSERT_GLUE_(a, b, c) a ## b ## _ ## c
#define JERRY_STATIC_ASSERT_GLUE(a, b, c) JERRY_STATIC_ASSERT_GLUE_ (a, b, c)
#define JERRY_STATIC_ASSERT(x, msg) \
  enum { JERRY_STATIC_ASSERT_GLUE (static_assertion_failed_, __LINE__, msg) = 1 / (!!(x)) }

#ifndef JERRY_NDEBUG
void JERRY_ATTR_NORETURN
jerry_assert_fail (const char *assertion, const char *file, const char *function, const uint32_t line);
void JERRY_ATTR_NORETURN
jerry_unreachable (const char *file, const char *function, const uint32_t line);

#define JERRY_ASSERT(x) \
  do \
  { \
    if (JERRY_UNLIKELY (!(x))) \
    { \
      jerry_assert_fail (#x, __FILE__, __func__, __LINE__); \
    } \
  } while (0)

#define JERRY_UNREACHABLE() \
  do \
  { \
    jerry_unreachable (__FILE__, __func__, __LINE__); \
  } while (0)
#else /* JERRY_NDEBUG */
#define JERRY_ASSERT(x) \
  do \
  { \
    if (false) \
    { \
      JERRY_UNUSED (x); \
    } \
  } while (0)

#ifdef __GNUC__
#define JERRY_UNREACHABLE() __builtin_unreachable ()
#else /* !__GNUC__ */
static JERRY_ATTR_ALWAYS_INLINE void JERRY_ATTR_NORETURN
jerry_unreachable() { }

#define JERRY_UNREACHABLE() jerry_unreachable()
#endif /* __GNUC__ */
#endif /* !JERRY_NDEBUG */

/**
 * Exit on fatal error
 */
void JERRY_ATTR_NORETURN jerry_fatal (jerry_fatal_code_t code);

/*
 * Logging
 */
#define JERRY_ERROR_MSG(...) jerry_port_log (JERRY_LOG_LEVEL_ERROR, __VA_ARGS__)
#define JERRY_WARNING_MSG(...) jerry_port_log (JERRY_LOG_LEVEL_WARNING, __VA_ARGS__)
#define JERRY_DEBUG_MSG(...) jerry_port_log (JERRY_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define JERRY_TRACE_MSG(...) jerry_port_log (JERRY_LOG_LEVEL_TRACE, __VA_ARGS__)

/**
 * Size of struct member
 */
#define JERRY_SIZE_OF_STRUCT_MEMBER(struct_name, member_name) sizeof (((struct_name *) NULL)->member_name)

/**
 * Aligns @a value to @a alignment. @a must be the power of 2.
 *
 * Returns minimum positive value, that divides @a alignment and is more than or equal to @a value
 */
#define JERRY_ALIGNUP(value, alignment) (((value) + ((alignment) - 1)) & ~((alignment) - 1))

/*
 * min, max
 */
#define JERRY_MIN(v1, v2) (((v1) < (v2)) ? (v1) : (v2))
#define JERRY_MAX(v1, v2) (((v1) < (v2)) ? (v2) : (v1))

#endif /* !JRT_H */
