/*
 * devices.c: define the devices included in the system.
 */

#include <sys/dev.h>
#include <sys/gpio.h>
#include <sys/usart.h>

#include <avr/interrupt.h>

static struct usart_cdev    tty0_cdev = { };

device_t Devices[NDEV] = {
    {   /* tty0 */
        .d_devsw    = &usart_devsw,
        .d_cdev     = (cdev_t *)&tty0_cdev,
    },
    {   /* gpio0 */
        .d_devsw    = &gpio_devsw,
    },
};

ISR(USART_UDRE_vect)
{
    usart_isr_udre(devnum2dev(DEV_tty0));
}

