/*
 * sys/types.h for SoCcer.
 * This need to override sys/types.h from avr-libc.
 */

#ifndef _sys_types_h
#define _sys_types_h

// for now...
#include <stddef.h>
#include <stdint.h>

#include <sys/cdefs.h>

#ifdef __cplusplus
# define bool __C_bool_type
#endif

typedef uint8_t byte;
typedef uint8_t bool;

typedef byte        dev_t;      /* device number */
typedef long        off_t;      /* LFS */

#endif
