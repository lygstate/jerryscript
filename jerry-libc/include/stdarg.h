/* Variable arguments <stdarg.h>

This file is part of the Public Domain C Library (PDCLib).
Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _STDARG_H
#define _STDARG_H

#ifdef __cplusplus
extern "C" {
#endif

#define _PDCLIB_va_round( type ) ( (sizeof(type) + sizeof(void *) - 1) & ~(sizeof(void *) - 1) )

typedef char * va_list;
#define va_arg( ap, type ) ( (ap) += (_PDCLIB_va_round(type)), ( *(type*) ( (ap) - (_PDCLIB_va_round(type)) ) ) )
#define va_copy( dest, src ) ( (dest) = (src), (void)0 )
#define va_end( ap ) ( (ap) = (void *)0, (void)0 )
#define va_start( ap, parmN ) ( (ap) = (char *) &parmN + ( _PDCLIB_va_round(parmN) ), (void)0 )

#ifdef __cplusplus
}
#endif

#endif
