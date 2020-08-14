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

#ifdef __cplusplus
# define bool __C_bool_type
#endif

typedef uint8_t byte;
typedef uint8_t bool;

typedef byte        dev_t;      /* device number */
typedef byte        ioc_t;      /* ioctl request */
typedef long        off_t;      /* LFS */

typedef union _TRANSPARENT_UNION iocp {
    void    *iop_ptr;
    int     iop_int;
} iocp_t;

#endif
