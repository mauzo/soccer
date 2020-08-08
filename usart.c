/*
 * usart.c
 * Interface to the USART interface on AVR ATmega chips.
 */

#include <sys/types.h>
#include <sys/atomic.h>
#include <sys/dev.h>
#include <sys/sleep.h>
#include <sys/tty.h>
#include <sys/usart.h>

#include <avr/interrupt.h>

void
usart_open (dev_t d _UNUSED, byte mode)
{
    usart_cdev  *cd     = dev2cdev(d);

    if (mode & DEV_WRITING)
        UCSR0B |= USART_ENABLE_TX;

    cdev_set_flag(&cd->us_cdev, DEV_OPEN);
}

/* XXX this always sets USART0 and ignores the dev_t */
/* XXX no error checking */
void
usart_ioctl (dev_t d _UNUSED, ioc_t r, uintptr_t p)
{
    byte        b;
    uint16_t    ubrr;    

    switch (r) {
    case TIOCSETBAUD:
        ubrr    = F_CPU/16/p - 1;
        UBRR0   = ubrr;
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
        UCSR0C  = b;
        break;
    }
}

void
usart_write (byte d, byte *ptr, size_t len)
{
    usart_cdev  *cdev   = dev2cdev(d);
    buffer      *buf    = &cdev->us_wr_buf;

    CRIT_START {
        buf->bf_ptr     = ptr;
        buf->bf_len     = len;

        UCSR0B          |= USART_ENABLE_DRE;
        cdev_set_flag(&cdev->us_cdev, DEV_WRITING);
    } CRIT_END;
}

/* This ISR must either set UDR or disable itself. Otherwise it will
 * re-trigger immediately.
 */
static void
usart_isr_udre (byte d)
{
    usart_cdev  *cdev   = dev2cdev(d);
    buffer      *buf    = &cdev->us_wr_buf;

    if (buf->bf_len) {
        UDR0    = *buf->bf_ptr++;
        buf->bf_len--;
    }
    else {
        UCSR0B          &= ~USART_ENABLE_DRE;
        cdev_clr_flag(&cdev->us_cdev, DEV_WRITING);
    }
}

/* Install the ISR statically for now, with hardcoded device number */
ISR(USART_UDRE_vect)
{
    usart_isr_udre(0);
}

