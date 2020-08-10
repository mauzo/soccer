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

static void usart_ioctl     (cdev_t *c, ioc_t r, uintptr_t p);
static void usart_open      (cdev_t *c, byte mode);
static void usart_write     (cdev_t *c, const byte *ptr, size_t len);

devsw_t usart_devsw = {
    sw_open:    usart_open,
    sw_ioctl:   usart_ioctl,
    sw_write:   usart_write,
};

static void
usart_open (cdev_t *c, byte mode)
{
    _cdev_downcast(usart, cd, c);

    if (mode & DEV_WRITING)
        USART_CSRB(cd) |= USART_ENABLE_TX;

    cdev_set_flag(c, DEV_OPEN);
}

/* XXX no error checking */
static void
usart_ioctl (cdev_t *c, ioc_t r, uintptr_t p)
{
    _cdev_downcast(usart, cd, c);
    byte        b;
    uint16_t    ubrr;    

    switch (r) {
    case TIOCSETBAUD:
        ubrr            = F_CPU/16/p - 1;
        USART_BRR(cd)   = ubrr;
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
        USART_CSRC(cd)  = b;
        break;
    }
}

static void
usart_write (cdev_t *c, const byte *ptr, size_t len)
{
    _cdev_downcast(usart, cd, c);
    c_buffer    *buf  = &cd->us_wr_buf;

    CRIT_START {
        buf->bf_ptr     = ptr;
        buf->bf_len     = len;

        USART_CSRB(cd)  |= USART_ENABLE_DRE;
        cdev_set_flag(c, DEV_WRITING);
    } CRIT_END;
}

/* This ISR must either set UDR or disable itself. Otherwise it will
 * re-trigger immediately.
 */
void
usart_isr_udre (device_t *dev)
{
    cdev_t      *c      = dev->d_cdev;
    _cdev_downcast(usart, cd, c);
    c_buffer    *buf    = &cd->us_wr_buf;

    if (buf->bf_len) {
        USART_DR(cd)    = *buf->bf_ptr++;
        buf->bf_len--;
    }
    else {
        USART_CSRB(cd)  &= ~USART_ENABLE_DRE;
        cdev_clr_flag(c, DEV_WRITING);
    }
}

