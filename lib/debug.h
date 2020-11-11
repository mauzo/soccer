/*
 * debug.h: functions for debugging
 */

#ifndef _lib_debug_h
#define _lib_debug_h

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/dev.h>

void    debug_show_cdev_rw  (dev_t d, _FLASH char *m);

#endif
