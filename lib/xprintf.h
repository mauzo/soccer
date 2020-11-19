#ifndef _lib_xprintf_h
#define _lib_xprintf_h

#include <sys/cdefs.h>
#include <sys/types.h>

#define panic_if(_e, _m)    _panic_if((_e), _F(_m))
#define xprintf(_f, ...)    _xprintf(_F(_f), __VA_ARGS__)

__BEGIN_DECLS

void    _panic_if   (errno_t err, _FLASH char *msg);
void    _xprintf    (_FLASH char *fmt, ...);

__END_DECLS

#endif
