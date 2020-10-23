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

static errno_t usart_ioctl     (device_t *c, ioc_t r, iocp_t p);
static errno_t usart_open      (device_t *c, byte mode);
static errno_t usart_write     (device_t *c);

devsw_t usart_devsw = {
    sw_open:    usart_open,
    sw_ioctl:   usart_ioctl,
    sw_write:   usart_write,
};

static errno_t
usart_open (device_t *d, byte mode)
{
    if (mode & DEV_WRITING)
        USART_CSRB(d)   |= USART_ENABLE_TX;

    return 0;
}

/* XXX no error checking */
static errno_t
usart_ioctl (device_t *d, ioc_t r, iocp_t p)
{
    int         i;
    byte        b;
    uint16_t    ubrr;    

    switch (r) {
    case TIOCSETBAUD:
        i               = p.iop_int;
        ubrr            = F_CPU/16/i - 1;
        USART_BRR(d)    = ubrr;
        break;

    case TIOCSETMODE:
        i       = p.iop_int;
        b       = 0;

        switch (i & CSIZE) {
        case CS5:                       break;
        case CS6:   b |= (1<<UCSZ00);   break;
        case CS7:   b |= (2<<UCSZ00);   break;
        case CS8:   b |= (3<<UCSZ00);   break;
        }
        if (i & CSTOPB) b |= (1<<USBS0);
        if (i & PARENB) {
            b |= (1<<UPM01);
            if (i & PARODD)
                b |= (1<<UPM00);
        }

        USART_CSRC(d)   = b;
        break;
    }

    return 0;
}

static errno_t
usart_write (device_t *d)
{
    USART_CSRB(d)   |= USART_ENABLE_DRE;
    return 0;
}

/* This ISR must either set UDR or disable itself. Otherwise it will
 * re-trigger immediately.
 */
void
usart_isr_udre (device_t *dev)
{
    cdev_rw_t   *cd     = (cdev_rw_t *)dev->d_cdev;
    iovec_t     *iov    = &cd->cd_writing;

    if (iov->iov_len) {
        if (cd->cd_flags & DEV_WR_FLASH)
            USART_DR(dev)   = *(_FLASH byte *)iov->iov_base++;
        else
            USART_DR(dev)   = *(const byte *)iov->iov_base++;
        iov->iov_len--;
    }
    else {
        USART_CSRB(dev) &= ~USART_ENABLE_DRE;
        cd->cd_flags    &= ~DEV_WRITING;
    }
}

