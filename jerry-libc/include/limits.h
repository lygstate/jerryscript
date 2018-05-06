/* Sizes of integer types <limits.h>

This file is part of the Public Domain C Library (PDCLib).
Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _LIMITS_H
#define _LIMITS_H

/* MSVC 2010 defines this to 5, which is enough for UTF-8 but might rule out
stateful encodings (like ISO/IEC 2022). GCC 5.3 defines this to 16, which
is meant to ensure future compatibility. For the same reason, we go along
with GCC's definition.
http://lists.gnu.org/archive/html/bug-gnulib/2015-05/msg00001.html
*/
#define MB_LEN_MAX 16

#define LLONG_MIN  (-0x7fffffffffffffffLL - 1LL)
#define LLONG_MAX  0x7fffffffffffffffLL
#define ULLONG_MAX 0xffffffffffffffffULL

#define CHAR_BIT   8
#define CHAR_MAX   0x7f
#define CHAR_MIN   (-0x7f - 1)
#define SCHAR_MAX  0x7f
#define SCHAR_MIN  (-0x7f - 1)
#define UCHAR_MAX  0xff
#define SHRT_MAX   0x7fff
#define SHRT_MIN   (-0x7fff - 1)
#define INT_MAX    0x7fffffff
#define INT_MIN    (-0x7fffffff - 1)
#define LONG_MAX   0x7fffffff
#define LONG_MIN   (-0x7fffffff - 1)
#define USHRT_MAX  0xffff
#define UINT_MAX   0xffffffffU
#define ULONG_MAX  0xffffffffUL

#endif
