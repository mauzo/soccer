/*
 * devops.c: implement the generic syscalls.
 * These call through the devsw into the device-specific functions.
 */

#include <sys/atomic.h>
#include <sys/dev.h>
#include <sys/errno.h>
#include <sys/sleep.h>
#include <sys/task.h>
#include <sys/uio.h>

static errno_t  get_cdev    (dev_t d, cdev_rw_t **cd, device_t **devp);
static bool     in_vec      (const byte *ptr, iovec_t *iov);
static errno_t  setup_read  (cdev_rw_t *cd, byte *b, size_t l, byte f);
static errno_t  setup_write (cdev_rw_t *cd, const byte *b, size_t l, byte f);

static errno_t
get_cdev (dev_t d, cdev_rw_t **cd, device_t **devp)
{
    device_t    *dev;

    if (d > NDEV)
        return ENXIO;

    dev     = devnum2dev(d);
    *cd     = (cdev_rw_t *)dev->d_cdev;

    if (!*cd)
        return ENODEV;

    if (devp)
        *devp   = dev;

    return 0;
}

static bool
in_vec (const byte *ptr, iovec_t *iov)
{
    if ((void *)ptr >= iov->iov_base && 
        (void *)ptr < (iov->iov_base + iov->iov_len)
    )
        return 1;
    return 0;
}

errno_t
open (dev_t d, byte mode)
{
    device_t    *dev;
    cdev_rw_t   *cd;
    errno_t     err     = 0;

    if ((err = get_cdev(d, &cd, &dev)))
        return err;
    if (!dev->d_devsw->sw_open)
        return ENODEV;
    if (mode & O_READ && cd->cd_rd_tid)
        return EBUSY;
    if (mode & O_WRITE && cd->cd_wr_tid)
        return EBUSY;

    CRIT_START {
        err = dev->d_devsw->sw_open(dev, mode);
        if (!err) {
            if (mode & O_READ)
                cd->cd_rd_tid   = Currtask;
            if (mode & O_WRITE)
                cd->cd_wr_tid   = Currtask;
        }
    } CRIT_END;

    return err;
}

void
poll (dev_t d, byte mode)
{
    cdev_rw_t   *cd;
    errno_t     err;

    if ((err = get_cdev(d, &cd, NULL)))
        return;

    switch (mode) {
    case O_WRITE:
        while (cd->cd_wr_flags & DEV_WRITING)
            _NOP();
        break;

    case O_READ:
        while (cd->cd_rd_flags & DEV_RD_NEXT)
            _NOP();
        break;
    }
}

static errno_t /*CRIT*/
setup_read (cdev_rw_t *cd, byte *b, size_t l, byte f _UNUSED)
{
    iovec_t     *iov;

/* can't take a pointer to a bitfield :( */
#define fl cd->cd_rd_flags

    if (fl & DEV_READING) {
        if (fl & DEV_RD_NEXT)
            return EAGAIN;

        iov = &cd->cd_rd_next;
        fl  |= DEV_RD_NEXT;
    }
    else {
        iov = &cd->cd_reading;
        fl  |= DEV_READING;
    }

#undef fl

    iov->iov_len    = l;
    iov->iov_base   = b;

    return 0;
}

errno_t
read_queue (dev_t d, byte *b, size_t l, byte f)
{
    device_t    *dev;
    cdev_rw_t   *cd;
    errno_t     err     = 0;

    if ((err = get_cdev(d, &cd, &dev)))
        return err;
    if (!dev->d_devsw->sw_read)
        return ENODEV;
    if (cd->cd_rd_tid != Currtask)
        return EBADF;

    if (f & F_WAIT)
        poll(d, O_READ);

    CRIT_START {
        err = setup_read(cd, b, l, f);
        if (!err) err = dev->d_devsw->sw_read(dev);
    } CRIT_END;

    return err;
}

errno_t
read_poll (dev_t d, byte *ptr, byte flg _UNUSED)
{
    errno_t     err     = 0;
    cdev_rw_t   *cd;
    byte        *cur;

    if ((err = get_cdev(d, &cd, NULL)))
        return err;
    if (cd->cd_rd_tid != Currtask)
        return EBADF;

    CRIT_START {
        if (in_vec(ptr, &cd->cd_reading) || in_vec(ptr, &cd->cd_rd_next))
            err = EAGAIN;
        else
            cur = cd->cd_reading.iov_base;
    } CRIT_END;

    if (err) return err;

    /* If ptr is within the original extent of cd_reading, this returns
     * the number of bytes read since we read ptr. Caller needs to
     * verify the return value is within the orignal length of the buf. */
    return cur > ptr ? cur - ptr : 0;
}

errno_t
read_setlen (dev_t d, byte *b, size_t l)
{
    cdev_rw_t   *cd;
    errno_t     err     = 0;
    byte        *cur;

    if ((err = get_cdev(d, &cd, NULL)))
        return err;

    CRIT_START {
        cur     = cd->cd_reading.iov_base;

        if (b == cd->cd_rd_next.iov_base)
            cd->cd_rd_next.iov_len  = l;
        else if (cur >= b && cur < b + l)
            cd->cd_reading.iov_len  = (b + l) - cur;
        else
            err = EINVAL;
    } CRIT_END;

    return err;
}

static errno_t /*CRIT*/
setup_write (cdev_rw_t *cd, const byte *b, size_t l, byte f)
{
    iovec_t     *iov    = &cd->cd_writing;

    iov->iov_base   = (void *)b;
    iov->iov_len    = l;

    if (f & F_FLASH) 
        cd->cd_wr_flags |= DEV_WR_FLASH;
    else
        cd->cd_wr_flags &= ~DEV_WR_FLASH;

    cd->cd_wr_flags |= DEV_WRITING;
    return 0;
}

errno_t
write_queue (dev_t d, const byte *b, size_t l, byte f)
{
    device_t    *dev;
    cdev_rw_t   *cd;
    errno_t     err;

    if ((err = get_cdev(d, &cd, &dev)))
        return err;
    if (!dev->d_devsw->sw_write)
        return ENODEV;
    if (!(cd->cd_wr_tid == Currtask || f & F_CONSWRITE))
        return EBADF;

    if (f & F_WAIT)
        poll(d, O_WRITE);

    CRIT_START {
        err = setup_write(cd, b, l, f);
        if (!err) err = dev->d_devsw->sw_write(dev);
    } CRIT_END;

    return err;
}

errno_t
write_poll (dev_t d, const byte *ptr, byte flg)
{
    errno_t     err     = 0;
    cdev_rw_t   *cd;
    byte        *cur;

    if ((err = get_cdev(d, &cd, NULL)))
        return err;
    if (!(cd->cd_wr_tid == Currtask || flg & F_CONSWRITE))
        return EBADF;

    CRIT_START {
        if (in_vec(ptr, &cd->cd_writing))
            err = EAGAIN;
        else
            cur = cd->cd_writing.iov_base;
    } CRIT_END;

    if (err) return err;

    /* If ptr is within the original extent of cd_reading, this returns
     * the number of bytes read since we read ptr. Caller needs to
     * verify the return value is within the orignal length of the buf. */
    return cur > ptr ? cur - ptr : 0;
}

