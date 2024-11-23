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

#ifndef ECMA_HELPERS_NUMBER_H
#define ECMA_HELPERS_NUMBER_H

#include <math.h>

#include "ecma-globals.h"

#include "config.h"

/**
 * Binary representation of an ecma-number
 */
typedef uint64_t ecma_binary_num_t;

bool ecma_number_sign (ecma_binary_num_t binary);
uint32_t ecma_number_biased_exp (ecma_binary_num_t binary);
uint64_t ecma_number_fraction (ecma_binary_num_t binary);
ecma_number_t ecma_number_create (bool sign, uint32_t biased_exp, uint64_t fraction);
ecma_number_t ecma_number_get_prev (ecma_number_t num);
ecma_number_t ecma_number_get_next (ecma_number_t num);
ecma_number_t ecma_number_trunc (ecma_number_t num);
ecma_number_t ecma_number_remainder (ecma_number_t left_num, ecma_number_t right_num);
ecma_number_t ecma_number_pow (ecma_number_t x, ecma_number_t y);
bool ecma_number_is_nan (ecma_number_t num);
bool ecma_number_is_negative (ecma_number_t num);
bool ecma_number_is_zero (ecma_number_t num);
bool ecma_number_is_infinity (ecma_number_t num);

/**
 * Maximum number of significant decimal digits that an ecma-number can store
 */
#define ECMA_NUMBER_MAX_DIGITS (19)

/**
 * Width of sign field
 *
 * See also:
 *          IEEE-754 2008, 3.6, Table 3.5
 */
#define ECMA_NUMBER_SIGN_WIDTH (1)

/**
 * Width of biased exponent field
 *
 * See also:
 *          IEEE-754 2008, 3.6, Table 3.5
 */
#define ECMA_NUMBER_BIASED_EXP_WIDTH (11)

/**
 * Exponent bias
 */
#define ECMA_NUMBER_EXPONENT_BIAS (1023)

/**
 * Width of fraction field
 *
 * See also:
 *          IEEE-754 2008, 3.6, Table 3.5
 */
#define ECMA_NUMBER_FRACTION_WIDTH (52)

/**
 * Sign bit in ecma-numbers
 */
#define ECMA_NUMBER_SIGN_BIT 0x8000000000000000ull

/**
 * Binary representation of an IEEE-754 QNaN value.
 */
#define ECMA_NUMBER_BINARY_QNAN 0x7ff8000000000000ull

/**
 * Binary representation of an IEEE-754 Infinity value.
 */
#define ECMA_NUMBER_BINARY_INF 0x7ff0000000000000ull

/**
 * Binary representation of an IEEE-754 zero value.
 */
#define ECMA_NUMBER_BINARY_ZERO 0x0ull

/**
 * Binary representation of an IEEE-754 negative zero value.
 */
#define ECMA_NUMBER_BINARY_ZERO_NEGATIVE (ECMA_NUMBER_BINARY_ZERO | ECMA_NUMBER_SIGN_BIT)

/**
 * Number.MIN_VALUE (i.e., the smallest positive value of ecma-number)
 *
 * See also: ECMA_262 v5, 15.7.3.3
 */
#define ECMA_NUMBER_MIN_VALUE ((ecma_number_t) 5e-324)

/**
 * Number.MAX_VALUE (i.e., the maximum value of ecma-number)
 *
 * See also: ECMA_262 v5, 15.7.3.2
 */
#define ECMA_NUMBER_MAX_VALUE ((ecma_number_t) 1.7976931348623157e+308)

/**
 * Number.EPSILON
 *
 * See also: ECMA_262 v6, 20.1.2.1
 */
#define ECMA_NUMBER_EPSILON ((ecma_number_t) 2.2204460492503130808472633361816e-16)

/**
 * Number.MAX_SAFE_INTEGER
 *
 * See also: ECMA_262 v6, 20.1.2.6
 */
#define ECMA_NUMBER_MAX_SAFE_INTEGER (0x1FFFFFFFFFFFFFLL)

/**
 * Number.MIN_SAFE_INTEGER
 *
 * See also: ECMA_262 v6, 20.1.2.8
 */
#define ECMA_NUMBER_MIN_SAFE_INTEGER (-0x1FFFFFFFFFFFFFLL)

/**
 * Number.MAX_VALUE exponent part
 */
#define NUMBER_MAX_DECIMAL_EXPONENT 308

/**
 * Number.MIN_VALUE exponent part
 */
#define NUMBER_MIN_DECIMAL_EXPONENT -324

/**
 * Euler number
 */
#define ECMA_NUMBER_E ((ecma_number_t) 2.7182818284590452354)

/**
 * Natural logarithm of 10
 */
#define ECMA_NUMBER_LN10 ((ecma_number_t) 2.302585092994046)

/**
 * Natural logarithm of 2
 */
#define ECMA_NUMBER_LN2 ((ecma_number_t) 0.6931471805599453)

/**
 * Logarithm base 2 of the Euler number
 */
#define ECMA_NUMBER_LOG2E ((ecma_number_t) 1.4426950408889634)

/**
 * Logarithm base 10 of the Euler number
 */
#define ECMA_NUMBER_LOG10E ((ecma_number_t) 0.4342944819032518)

/**
 * Pi number
 */
#define ECMA_NUMBER_PI ((ecma_number_t) 3.1415926535897932)

/**
 * Square root of 0.5
 */
#define ECMA_NUMBER_SQRT_1_2 ((ecma_number_t) 0.7071067811865476)

/**
 * Square root of 2
 */
#define ECMA_NUMBER_SQRT2 ((ecma_number_t) 1.4142135623730951)

/**
 * Convert an ecma-number to it's binary representation.
 *
 * @return binary representation
 */
extern inline uint64_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_to_binary (ecma_number_t number) /**< ecma number */
{
  return number.as_binary;
} /* ecma_number_to_binary */

/**
 * Convert a binary representation to the corresponding ecma-number.
 *
 * @return ecma-number
 */
extern inline ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_from_binary (uint64_t binary) /**< binary representation */
{
  ecma_number_t f;
  f.as_binary = binary;

  return f;
} /* ecma_number_from_binary */

/**
 * Check if ecma-number is positive
 *
 * @return true - if sign bit of ecma-number is set
 *         false - otherwise
 */
extern inline bool JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_is_positive (ecma_number_t num) /**< ecma-number */
{
  JERRY_ASSERT (!ecma_number_is_nan (num));

  return (ecma_number_to_binary (num) & ECMA_NUMBER_SIGN_BIT) == 0;
} /* ecma_number_is_positive */

double JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_cast_double (ecma_number_t number)
{
  return number.as_number;
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_from_double (double dbl)
{
  ecma_number_t number;
  number.as_number = dbl;
  return number;
}

int32_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_cast_int32 (ecma_number_t num)
{
  return (int32_t) num.as_number;
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_from_int32 (int32_t val)
{
  ecma_number_t number;
  number.as_number = (double) val;
  return number;
}

uint32_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_cast_uint32 (ecma_number_t num)
{
  return (uint32_t) num.as_number;
}

uint32_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_clamp_uint32 (ecma_number_t num)
{
  if (num.as_number >= UINT32_MAX)
  {
    return UINT32_MAX;
  }
  if (num.as_number <= 0)
  {
    return 0;
  }
  return ecma_number_cast_uint32 (num);
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_from_uint32 (uint32_t val)
{
  ecma_number_t number;
  number.as_number = (double) val;
  return number;
}

int64_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_cast_int64 (ecma_number_t num)
{
  return (int64_t) num.as_number;
}

int64_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_clamp_int64 (ecma_number_t num)
{
  if (num.as_number >= INT64_MAX)
  {
    return INT64_MAX;
  }
  if (num.as_number <= INT64_MIN)
  {
    return INT64_MIN;
  }
  return ecma_number_cast_int64 (num);
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_from_int64 (int64_t val)
{
  ecma_number_t number;
  number.as_number = (double) val;
  return number;
}

uint64_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_cast_uint64 (ecma_number_t num)
{
  return (uint64_t) num.as_number;
}

uint64_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_clamp_uint64 (ecma_number_t num)
{
  if (num.as_number >= UINT64_MAX)
  {
    return UINT64_MAX;
  }
  if (num.as_number <= 0)
  {
    return 0;
  }
  return ecma_number_cast_uint64 (num);
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_from_uint64 (uint64_t val)
{
  ecma_number_t number;
  number.as_number = (double) val;
  return number;
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_abs (ecma_number_t number)
{
  number.as_binary &= ~ECMA_NUMBER_SIGN_BIT;
  return number;
}

bool JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_greater_than (ecma_number_t a, ecma_number_t b)
{
  return a.as_number > b.as_number;
}

bool JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_greater_equal (ecma_number_t a, ecma_number_t b)
{
  return a.as_number >= b.as_number;
}

bool JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_less_than (ecma_number_t a, ecma_number_t b)
{
  return a.as_number < b.as_number;
}

bool JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_less_equal (ecma_number_t a, ecma_number_t b)
{
  return a.as_number <= b.as_number;
}

bool JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_equal_to (ecma_number_t a, ecma_number_t b)
{
  return a.as_number == b.as_number;
}

bool JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_equal_binary (ecma_number_t a, ecma_number_t b)
{
  return a.as_binary == b.as_binary;
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_div_trunc (ecma_number_t left_num, ecma_number_t right_num)
{
  double divded = ecma_number_cast_double (left_num) / ecma_number_cast_double (right_num);
  const ecma_number_t q = ecma_number_trunc (ecma_number_from_double (divded));
  return q;
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_add (ecma_number_t a, ecma_number_t b)
{
  double sub = ecma_number_cast_double (a) + ecma_number_cast_double (b);
  return ecma_number_from_double (sub);
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_add_i32 (ecma_number_t a, int32_t b)
{
  double sub = ecma_number_cast_double (a) + b;
  return ecma_number_from_double (sub);
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_sub (ecma_number_t a, ecma_number_t b)
{
  double sub = ecma_number_cast_double (a) - ecma_number_cast_double (b);
  return ecma_number_from_double (sub);
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_sub_i32 (ecma_number_t a, int32_t b)
{
  double sub = ecma_number_cast_double (a) - b;
  return ecma_number_from_double (sub);
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_mul (ecma_number_t a, ecma_number_t b)
{
  double mul = ecma_number_cast_double (a) - ecma_number_cast_double (b);
  return ecma_number_from_double (mul);
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_sqrt (ecma_number_t a)
{
  return ecma_number_from_double (sqrt(a.as_number));
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_mul_i32 (ecma_number_t a, int32_t v)
{
  double mul = ecma_number_cast_double (a) - v;
  return ecma_number_from_double (mul);
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_negative (ecma_number_t num)
{
  return ecma_number_from_binary (ecma_number_to_binary (num) ^ ECMA_NUMBER_SIGN_BIT);
}

ecma_number_t JERRY_ATTR_ALWAYS_INLINE JERRY_ATTR_CONST
ecma_number_step (ecma_number_t num, int32_t increment)
{
  num.as_number += increment;
  return num;
#if 0
  if (increment == 1)
  {
    return ecma_number_get_next (num);
  }
  else
  {
    return ecma_number_get_prev (num);
  }
#endif
}
/**
 * Exponent bits
 */
#define ECMA_NUMBER_EXPONENT_BITS 0x7FF0000000000000ull

/**
 * Fraction bits
 */
#define ECMA_NUMBER_FRACTION_BITS ((1ULL << ECMA_NUMBER_FRACTION_WIDTH) - 1)

/**
 * truncate function
 * See also:
 *          ECMA-262 v15, 5.2.5 https://262.ecma-international.org/#eqn-truncate
 * @param number
 * @return ecma_number_t
 */
ecma_number_t
ecma_number_truncate (ecma_number_t number)
{
  double value = ecma_number_is_negative (number) ? -floor (-number.as_number) : floor (number.as_number);
  return ecma_number_from_double (value);
}

/**
 * ToIntegerOrInfinity body.
 *
 * See also:
 *          ECMA-262 v15, 7.1.5
 */
ecma_number_t
ecma_number_to_integer_or_infinity (ecma_number_t number)
{
  /* 2 */
  if (ecma_number_is_nan (number) || ecma_number_is_zero (number))
  {
    return ECMA_NUMBER_ZERO;
  }
  /* 3, 4 */
  if (ecma_number_is_infinity (number))
  {
    return number;
  }
  /* 5 */
  return ecma_number_truncate (number);
}

/**
 * @brief Convert ecma_number_t to safe integer when possible
 *
 * @param number
 * @return int64_t INT64_MIN means not a safe integer and not convert
 */
int64_t
ecma_number_to_safe_integer (ecma_number_t number)
{
  if ((number.as_binary & ECMA_NUMBER_EXPONENT_BITS) == 0)
  {
    int64_t val = number.as_binary & ECMA_NUMBER_FRACTION_BITS;
    return number.as_binary & ECMA_NUMBER_SIGN_BIT ? -val : val;
  }
  return INT64_MIN;
}

#endif /* !ECMA_HELPERS_NUMBER_H */
