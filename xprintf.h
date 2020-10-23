#ifndef __xprintf_h
#define __xprintf_h

#include <sys/cdefs.h>
#include <sys/types.h>

#define xprintf(_f, ...)    _xprintf(_F(_f), __VA_ARGS__)

__BEGIN_DECLS

void    _xprintf    (_FLASH char *fmt, ...);

__END_DECLS

#endif
