/*
 * devices.c: define the devices included in the system.
 */

#include <sys/dev.h>
#include <sys/sleep.h>
#include <sys/usart.h>

#include <avr/interrupt.h>

static usart_cdev   tty0_cdev;

device Devices[NDEV] = {
    { d_cdev: (void *)&tty0_cdev, },
};

bool
poll (byte d, byte mode)
{
    struct cdev *cdev   = dev2cdev(d);
    byte        *flg    = &cdev->cd_flags;

    if (!(*flg & mode))
        return 1;

    sleep_while(*flg & mode);

    return 1;
}

