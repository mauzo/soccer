/*
 * devices.c: define the devices included in the system.
 */

#include <sys/dev.h>
#include <sys/gpio.h>
#include <sys/twi.h>
#include <sys/twi-isr.h>
#include <sys/usart.h>
#include <sys/usart-isr.h>

#include <avr/interrupt.h>

static struct usart_cdev    tty0_cdev = { };
static struct twi_cdev      iic0_cdev = { };

device_t Devices[NDEV] = {
    {   /* tty0 */
        .d_devsw    = &usart_devsw,
        .d_cdev     = (cdev_t *)&tty0_cdev,
    },
    {   /* gpio0 */
        .d_devsw    = &gpio_devsw,
    },
    {   /* iic0 */
        .d_devsw    = &twi_devsw,
        .d_cdev     = (cdev_t *)&iic0_cdev,
    },
};

ISR(USART_RX_vect)
{
    usart_isr_rxc(devnum2dev(DEV_tty0));
}

ISR(USART_UDRE_vect)
{
    usart_isr_udre(devnum2dev(DEV_tty0));
}

ISR(TWI_vect)
{
    twi_isr(devnum2dev(DEV_iic0));
}
