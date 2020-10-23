#ifndef __xprintf_h
#define __xprintf_h

#include <sys/cdefs.h>
#include <sys/types.h>

#define xprintf(_f, ...)    _xprintf(_F(_f), __VA_ARGS__)
#define print(_s)           _print(_F(_s), sizeof(_s))

__BEGIN_DECLS

void    _xprintf    (_FLASH char *fmt, ...);
void    _print      (_FLASH char *s, size_t sz);

__END_DECLS

#endif
