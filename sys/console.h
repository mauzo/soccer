/* 
 * sys/console.h
 * Functions for using the console (if defined).
 */

#ifndef _sys_console_h
#define _sys_console_h

#include <sys/cdefs.h>
#include <sys/types.h>

#define panic(_m)   _panic(_F(_m), sizeof(_m))

__BEGIN_DECLS

void    cons_write  (const char *msg, size_t sz, byte flags);
void    _panic      (_FLASH char *msg, size_t sz) _DEAD;

__END_DECLS

#endif
