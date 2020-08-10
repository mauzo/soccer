/*
 * devops.c: implement the generic syscalls.
 * These call through the devsw into the device-specific functions.
 */

#include <sys/dev.h>
#include <sys/sleep.h>

void
ioctl (dev_t d, ioc_t r, uintptr_t p)
{
    device_t    *dev    = devnum2dev(d);
    devsw_t     *dsw    = dev->d_devsw;
    cdev_t      *c      = dev->d_cdev;

    if (!dsw->sw_ioctl)
        return;

    dsw->sw_ioctl(c, r, p);
}

void
open (dev_t d, byte mode)
{
    device_t    *dev    = devnum2dev(d);
    devsw_t     *dsw    = dev->d_devsw;
    cdev_t      *c      = dev->d_cdev;

    if (!dsw->sw_open)
        return;

    dsw->sw_open(c, mode);
}

bool
poll (dev_t d, byte mode)
{
    struct cdev *cdev   = dev2cdev(d);
    byte        *flg    = &cdev->cd_flags;

    if (!(*flg & mode))
        return 1;

    sleep_while(*flg & mode);

    return 1;
}

void
read (dev_t d, byte *b, size_t l, byte f _UNUSED)
{
    device_t    *dev    = devnum2dev(d);
    devsw_t     *dsw    = dev->d_devsw;
    cdev_t      *c      = dev->d_cdev;

    if (!dsw->sw_read)
        return;

    dsw->sw_read(c, b, l);
}

void
write (dev_t d, const byte *b, size_t l, byte f _UNUSED)
{
    device_t    *dev    = devnum2dev(d);
    devsw_t     *dsw    = dev->d_devsw;
    cdev_t      *c      = dev->d_cdev;

    if (!dsw->sw_write)
        return;

    dsw->sw_write(c, b, l);
}
