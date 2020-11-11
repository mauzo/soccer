/*
 * debug.c: functions for debugging
 */

#include <lib/debug.h>
#include <lib/xprintf.h>

void
debug_show_cdev_rw (dev_t d, _FLASH char *m)
{
    device_t    *dev    = devnum2dev(d);
    cdev_rw_t   *cd     = (cdev_rw_t *)dev->d_cdev;
    xprintf("%S: flags %x, reading %x+%u, rd_next %x+%u\n",
        m, cd->cd_rd_flags, 
        cd->cd_reading.iov_base, cd->cd_reading.iov_len,
        cd->cd_rd_next.iov_base, cd->cd_rd_next.iov_len);
}

