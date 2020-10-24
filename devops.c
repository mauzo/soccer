/*
 * devops.c: implement the generic syscalls.
 * These call through the devsw into the device-specific functions.
 */

#include <sys/atomic.h>
#include <sys/dev.h>
#include <sys/errno.h>
#include <sys/sleep.h>
#include <sys/uio.h>

static errno_t  get_devsw   (dev_t d, device_t **dev, devsw_t **dsw);
static errno_t  setup_read  (device_t *dev, byte *b, size_t l, byte f);
static errno_t  setup_write (device_t *d, const byte *b, size_t l, byte f);

static errno_t
get_devsw (dev_t d, device_t **dev, devsw_t **dsw)
{
    if (d > NDEV)
        return ENXIO;

    *dev    = devnum2dev(d);
    *dsw    = (*dev)->d_devsw;

    return 0;
}

errno_t
open (dev_t d, byte mode)
{
    device_t    *dev;
    devsw_t     *dsw;
    errno_t     err     = 0;

    if ((err = get_devsw(d, &dev, &dsw)))
        return err;
    if (!dsw->sw_open)
        return ENODEV;

    CRIT_START {
        err = dsw->sw_open(dev, mode);
        if (!err) dev->d_cdev->cd_flags |= DEV_OPEN;
    } CRIT_END;

    return err;
}

bool
poll (dev_t d, byte mode)
{
    device_t    *dev    = devnum2dev(d);
    byte        *flg    = &dev->d_cdev->cd_flags;

    if (!(*flg & mode))
        return 1;

    sleep_while(*flg & mode);

    return 1;
}

static errno_t
setup_read (device_t *dev, byte *b, size_t l, byte f _UNUSED)
{
    cdev_rw_t   *cd     = (cdev_rw_t *)dev->d_cdev;
    iovec_t     *iov    = &cd->cd_reading;

    if (!cd->cd_flags & DEV_OPEN)
        return EBADF;

    iov->iov_len    = l;
    iov->iov_base   = b;

    cd->cd_flags    |= DEV_READING;
    return 0;
}

errno_t
read (dev_t d, byte *b, size_t l, byte f)
{
    device_t    *dev;
    devsw_t     *dsw;
    errno_t     err     = 0;

    if ((err = get_devsw(d, &dev, &dsw)))
        return err;
    if (!dsw->sw_read)
        return ENODEV;

    /* XXX F_WAIT / F_SYNC */

    CRIT_START {
        err = setup_read(dev, b, l, f);
        if (!err) err = dsw->sw_read(dev);
    } CRIT_END;

    return err;
}

static errno_t
setup_write (device_t *d, const byte *b, size_t l, byte f)
{
    cdev_rw_t   *cd     = (cdev_rw_t *)d->d_cdev;
    iovec_t     *iov    = &cd->cd_writing;

    if (!(cd->cd_flags & DEV_OPEN))
        return EBADF;

    iov->iov_base   = (void *)b;
    iov->iov_len    = l;

    if (f & F_FLASH) 
        cd->cd_flags    |= DEV_WR_FLASH;
    else
        cd->cd_flags    &= ~DEV_WR_FLASH;

    cd->cd_flags    |= DEV_WRITING;
    return 0;
}

errno_t
write (dev_t d, const byte *b, size_t l, byte f)
{
    device_t    *dev;
    devsw_t     *dsw;
    errno_t     err;

    if ((err = get_devsw(d, &dev, &dsw)))
        return err;
    if (!dsw->sw_write)
        return ENODEV;

    if (f & F_WAIT)
        poll(d, DEV_WRITING);

    CRIT_START {
        err = setup_write(dev, b, l, f);
        if (!err) err = dsw->sw_write(dev);
    } CRIT_END;

    /* XXX someone else might get in and start another write */
    if (f & F_SYNC)
        poll(d, DEV_WRITING);

    return err;
}
