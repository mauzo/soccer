/*
 * devices.c: define the devices included in the system.
 */

#include <sys/dev.h>
#include <sys/usart.h>

#include <avr/interrupt.h>

static struct usart_cdev    tty0_cdev = { us_iop: 0xC0 };

device_t Devices[NDEV] = {
    { 
        d_cdev:     (cdev_t *)&tty0_cdev,
        d_devsw:    &usart_devsw,
    },
};

ISR(USART_UDRE_vect)
{
    usart_isr_udre(devnum2dev(DEV_tty0));
}

