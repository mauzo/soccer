/* 
 * sys/console.h
 * Functions for using the console (if defined).
 */

#ifndef _sys_console_h
#define _sys_console_h

#include <sys/cdefs.h>
#include <sys/types.h>

#define panic(_m)   _panic(_F(_m), sizeof(_m))
#define print(_s)   _print(_F(_s), sizeof(_s))

__BEGIN_DECLS

void    cons_write  (const char *msg, size_t sz, byte flags);
void    _panic      (_FLASH char *msg, size_t sz) _DEAD;
void    _print      (_FLASH char *s, size_t sz);

__END_DECLS

#endif
