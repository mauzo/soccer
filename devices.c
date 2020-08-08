/*
 * devices.c: define the devices included in the system.
 */

#include <sys/dev.h>
#include <sys/usart.h>

static usart_cdev   tty0_cdev;

device Devices[NDEV] = {
    { cdev: (void *)&tty0_cdev, },
};
