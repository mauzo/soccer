/*
 * usart.c
 * Interface to the USART interface on AVR ATmega chips.
 */

#include <sys/types.h>
#include <sys/atomic.h>
#include <sys/dev.h>
#include <sys/tty.h>
#include <sys/usart.h>

#include <avr/interrupt.h>

static errno_t usart_open   (device_t *c, byte mode);
static errno_t usart_read   (device_t *d);
static errno_t usart_write  (device_t *c);

devsw_t usart_devsw = {
    .sw_open    = usart_open,
    .sw_read    = usart_read,
    .sw_write   = usart_write,
};

static errno_t
usart_open (device_t *d, byte mode)
{
    if (mode & DEV_WRITING)
        USART_CSRB(d)   |= USART_ENABLE_TX;
    if (mode & DEV_READING)
        USART_CSRB(d)   |= USART_ENABLE_RX;

    return 0;
}

/* XXX no error checking */
errno_t
usart_setbaud (dev_t d, uint16_t spd)
{
    device_t    *dev    = devnum2dev(d);
    uint16_t    ubrr    = F_CPU/16/spd - 1;

    USART_BRR(dev)    = ubrr;
    return 0;
}

errno_t
usart_setmode (dev_t d, byte mode)
{
    device_t    *dev    = devnum2dev(d);
    byte        b       = 0;

    switch (mode & CSIZE) {
    case CS5:                       break;
    case CS6:   b |= (1<<UCSZ00);   break;
    case CS7:   b |= (2<<UCSZ00);   break;
    case CS8:   b |= (3<<UCSZ00);   break;
    }
    if (mode & CSTOPB) b |= (1<<USBS0);
    if (mode & PARENB) {
        b |= (1<<UPM01);
        if (mode & PARODD)
            b |= (1<<UPM00);
    }

    USART_CSRC(dev) = b;
    return 0;
}

static errno_t
usart_read (device_t *d)
{
    USART_CSRB(d)   |= USART_IRQ_RXC;
    return 0;
}

static errno_t
usart_write (device_t *d)
{
    USART_CSRB(d)   |= USART_IRQ_UDRE;
    return 0;
}

