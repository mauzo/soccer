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

static void usart_ioctl     (device_t *c, ioc_t r, uintptr_t p);
static void usart_open      (device_t *c, byte mode);
static void usart_write     (device_t *c, const byte *ptr, size_t len);

devsw_t usart_devsw = {
    sw_open:    usart_open,
    sw_ioctl:   usart_ioctl,
    sw_write:   usart_write,
};

static void
usart_open (device_t *d, byte mode)
{
    _device_cdev(usart, cd, d);

    if (mode & DEV_WRITING)
        USART_CSRB(d)   |= USART_ENABLE_TX;

    cdev_set_flag(&cd->us_cdev, DEV_OPEN);
}

/* XXX no error checking */
static void
usart_ioctl (device_t *d, ioc_t r, uintptr_t p)
{
    byte        b;
    uint16_t    ubrr;    

    switch (r) {
    case TIOCSETBAUD:
        ubrr            = F_CPU/16/p - 1;
        USART_BRR(d)    = ubrr;
        break;

    case TIOCSETMODE:
        b       = 0;
        switch (p & CSIZE) {
        case CS5:                       break;
        case CS6:   b |= (1<<UCSZ00);   break;
        case CS7:   b |= (2<<UCSZ00);   break;
        case CS8:   b |= (3<<UCSZ00);   break;
        }
        if (p & CSTOPB) b |= (1<<USBS0);
        if (p & PARENB) {
            b |= (1<<UPM01);
            if (p & PARODD)
                b |= (1<<UPM00);
        }
        USART_CSRC(d)   = b;
        break;
    }
}

static void
usart_write (device_t *d, const byte *ptr, size_t len)
{
    _device_cdev(usart, cd, d);
    c_buffer    *buf  = &cd->us_wr_buf;

    CRIT_START {
        buf->bf_ptr     = ptr;
        buf->bf_len     = len;

        USART_CSRB(d)   |= USART_ENABLE_DRE;
        cdev_set_flag(&cd->us_cdev, DEV_WRITING);
    } CRIT_END;
}

/* This ISR must either set UDR or disable itself. Otherwise it will
 * re-trigger immediately.
 */
void
usart_isr_udre (device_t *dev)
{
    _device_cdev(usart, cd, dev);
    c_buffer    *buf    = &cd->us_wr_buf;

    if (buf->bf_len) {
        USART_DR(dev)   = *buf->bf_ptr++;
        buf->bf_len--;
    }
    else {
        USART_CSRB(dev) &= ~USART_ENABLE_DRE;
        cdev_clr_flag(&cd->us_cdev, DEV_WRITING);
    }
}

