/*
 * sys/uio.h: buffer structures.
 */

#ifndef _sys_uio_h
#define _sys_uio_h

struct iovec {
    void        *iov_base;
    size_t      iov_len;
};

typedef struct iovec iovec_t;

#define _sizeof_1(_s) (sizeof(_s) - 1)

#define str2iov(_s) ((struct iovec){ \
    .iov_base   = (void *)(const char [_sizeof_1(_s)]){ _s }, \
    .iov_len    = _sizeof_1(_s), \
})

/* Casting through uintptr_t is ridiculous, but it is the only way I can
 * find to get GCC to stop moaning that the initialiser is not constant.
 */

#define str2iovf(_s) ((struct iovec){ \
    .iov_base   = (void *)(uintptr_t)(_FLASH char [_sizeof_1(_s)]){ _s }, \
    .iov_len    = _sizeof_1(_s), \
})

#endif
