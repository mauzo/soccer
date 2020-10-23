/*
 * sys/panic.h: panic!
 *
 */

#ifndef __sys_panic_h
#define __sys_panic_h

#include <sys/cdefs.h>

#define panic(_m)   _panic(_F(_m), sizeof(_m))

__BEGIN_DECLS

void _panic (_FLASH char *msg, size_t sz) _DEAD;

__END_DECLS

#endif
