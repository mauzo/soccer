/*
 * devops.c: implement the generic syscalls.
 * These call through the devsw into the device-specific functions.
 */

#include <sys/atomic.h>
#include <sys/dev.h>
#include <sys/sleep.h>
#include <sys/uio.h>

void
ioctl (dev_t d, ioc_t r, iocp_t p)
{
    device_t    *dev    = devnum2dev(d);
    devsw_t     *dsw    = dev->d_devsw;

    if (!dsw->sw_ioctl)
        return;

    dsw->sw_ioctl(dev, r, p);
}

void
open (dev_t d, byte mode)
{
    device_t    *dev    = devnum2dev(d);
    devsw_t     *dsw    = dev->d_devsw;

    if (!dsw->sw_open)
        return;

    CRIT_START {
        dsw->sw_open(dev, mode);
        dev->d_cdev->cd_flags |= DEV_OPEN;
    } CRIT_END;
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

static void
setup_read (device_t *dev, byte *b, size_t l, byte f _UNUSED)
{
    cdev_rw_t   *cd     = (cdev_rw_t *)dev->d_cdev;
    iovec_t     *iov    = &cd->cd_reading;

    iov->iov_len    = l;
    iov->iov_base   = b;

    cd->cd_flags    |= DEV_READING;
}

void
read (dev_t d, byte *b, size_t l, byte f)
{
    device_t    *dev    = devnum2dev(d);
    devsw_t     *dsw    = dev->d_devsw;

    if (!dsw->sw_read)
        return;

    /* XXX F_WAIT / F_SYNC */

    CRIT_START {
        setup_read(dev, b, l, f);
        dsw->sw_read(dev);
    } CRIT_END;
}

static void
setup_write (device_t *d, const byte *b, size_t l, byte f)
{
    cdev_rw_t   *cd     = (cdev_rw_t *)d->d_cdev;
    iovec_t     *iov    = &cd->cd_writing;

    iov->iov_base   = (void *)b;
    iov->iov_len    = l;

    if (f & F_FLASH) 
        cd->cd_flags    |= DEV_WR_FLASH;
    else
        cd->cd_flags    &= ~DEV_WR_FLASH;

    cd->cd_flags    |= DEV_WRITING;
}

void
write (dev_t d, const byte *b, size_t l, byte f)
{
    device_t    *dev    = devnum2dev(d);
    devsw_t     *dsw    = dev->d_devsw;

    if (!dsw->sw_write)
        return;

    if (f & F_WAIT)
        poll(d, DEV_WRITING);

    CRIT_START {
        setup_write(dev, b, l, f);
        dsw->sw_write(dev);
    } CRIT_END;

    /* XXX someone else might get in and start another write */
    if (f & F_SYNC)
        poll(d, DEV_WRITING);
}
