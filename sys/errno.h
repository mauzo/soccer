#ifndef _sys_errno_h
#define _sys_errno_h

typedef int errno_t;

/* All errors are negative */
#define EBADF       (-1)    /* device not open */
#define ENXIO       (-2)    /* device does not exist */
#define ENODEV      (-3)    /* operation not supported by device */
#define EINVAL      (-4)    /* invalid parameters */
#define ESRCH       (-5)    /* process not found */

#endif
