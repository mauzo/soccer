/*
 * sys/types.h for SoCcer.
 * This need to override sys/types.h from avr-libc.
 */

#ifndef __SYS_TYPES_H
#define __SYS_TYPES_H

// for now...
#include <stddef.h>
#include <stdint.h>

#include <sys/cdefs.h>

typedef long    off_t;

#ifdef __cplusplus
# define bool __C_bool_type
#endif

typedef uint8_t byte;
typedef uint8_t bool;

#endif
