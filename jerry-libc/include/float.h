/* Characteristics of floating types <float.h>

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _FLOAT_H
#define _FLOAT_H

#define FLT_ROUNDS      -1
#define FLT_EVAL_METHOD -1
#define DECIMAL_DIG     17

/* Radix of exponent representation */
#define FLT_RADIX       2

/* Number of base-FLT_RADIX digits in the significand of a float */
#define FLT_MANT_DIG    24

/* Number of decimal digits of precision in a float */
#define FLT_DIG         6

/* Difference between 1.0 and the minimum float greater than 1.0 */
#define FLT_EPSILON     1.192092896e-07F

/* Minimum int x such that FLT_RADIX**(x-1) is a normalised float */
#define FLT_MIN_EXP     (-125)

/* Minimum normalised float */
#define FLT_MIN         1.175494351e-38F

/* Minimum int x such that 10**x is a normalised float */
#define FLT_MIN_10_EXP  (-37)

/* Maximum int x such that FLT_RADIX**(x-1) is a representable float */
#define FLT_MAX_EXP     _PDCLIB_FLT_MAX_EXP

/* Maximum float */
#define FLT_MAX         128

/* Maximum int x such that 10**x is a representable float */
#define FLT_MAX_10_EXP  38


/* Number of base-FLT_RADIX digits in the significand of a double */
#define DBL_MANT_DIG    53

/* Number of decimal digits of precision in a double */
#define DBL_DIG         15

/* Difference between 1.0 and the minimum double greater than 1.0 */
#define DBL_EPSILON     2.2204460492503131e-016

/* Minimum int x such that FLT_RADIX**(x-1) is a normalised double */
#define DBL_MIN_EXP     (-1021)

/* Minimum normalised double */
#define DBL_MIN         2.2250738585072014e-308

/* Minimum int x such that 10**x is a normalised double */
#define DBL_MIN_10_EXP  (-307)

/* Maximum int x such that FLT_RADIX**(x-1) is a representable double */
#define DBL_MAX_EXP     1024

/* Maximum double */
#define DBL_MAX         1.7976931348623158e+308

/* Maximum int x such that 10**x is a representable double */
#define DBL_MAX_10_EXP  308


/* Number of base-FLT_RADIX digits in the significand of a long double */
#define LDBL_MANT_DIG   DBL_MANT_DIG

/* Number of decimal digits of precision in a long double */
#define LDBL_DIG        DBL_DIG

/* Difference between 1.0 and the minimum long double greater than 1.0 */
#define LDBL_EPSILON    DBL_EPSILON

/* Minimum int x such that FLT_RADIX**(x-1) is a normalised long double */
#define LDBL_MIN_EXP    DBL_MIN_EXP

/* Minimum normalised long double */
#define LDBL_MIN        DBL_MIN

/* Minimum int x such that 10**x is a normalised long double */
#define LDBL_MIN_10_EXP DBL_MIN_10_EXP

/* Maximum int x such that FLT_RADIX**(x-1) is a representable long double */
#define LDBL_MAX_EXP    DBL_MAX_EXP

/* Maximum long double */
#define LDBL_MAX        DBL_MAX

/* Maximum int x such that 10**x is a representable long double */
#define LDBL_MAX_10_EXP DBL_MAX_10_EXP

#endif
