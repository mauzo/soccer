#ifndef _sys_isr_util_h
#define _sys_isr_util_h

#include <sys/cdefs.h>
#include <sys/types.h>

_MACRO errno_t
isr_do_write (cdev_rw_t *cd, byte *reg)
{
    iovec_t     *iov    = &cd->cd_writing;

    if (iov->iov_len) {
        if (cd->cd_wr_flags & DEV_WR_FLASH)
            *reg    = *(_FLASH byte *)iov->iov_base++;
        else
            *reg    = *(const byte *)iov->iov_base++;
        iov->iov_len--;
    }

    return iov->iov_len != 0;
}

#endif
