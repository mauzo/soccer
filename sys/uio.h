/*
 * sys/iovec.h: buffer structures.
 */

#ifndef __sys_uio_h
#define __sys_uio_h

struct iovec {
    union {
        byte        *iov_base;
        _FLASH byte *iov_flash;
    };
    size_t          iov_len;
};

typedef struct iovec iovec_t;

#define _str_len(_s) (sizeof(_s) - 1)

#define str2iov(_s) ((struct iovec){ \
    .iov_base   = (void *)(const char [_str_len(_s)]){ _s }, \
    .iov_len    = _str_len(_s), \
})

#define str2iovf(_s) ((struct iovec){ \
    .iov_flash = (_FLASH char [_str_len(_s)]){ _s }, \
    .iov_len    = _str_len(_s), \
})

#endif
