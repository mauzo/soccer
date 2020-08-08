/*
 * usart.c
 * Interface to the USART interface on AVR ATmega chips.
 */

#include <sys/types.h>
#include <sys/atomic.h>
#include <sys/dev.h>
#include <sys/sleep.h>
#include <sys/usart.h>

#include <avr/interrupt.h>

void
usart_init (byte d _UNUSED, uint16_t baud)
{
    uint16_t    ubrr    = F_CPU/16/baud - 1;

    CRIT_START {
        // Set baud rate
        UBRR0   = ubrr;

        // Set frame: 8 bit, 1 stop
        UCSR0C  = (3<<UCSZ00);

        UCSR0B  |= USART_ENABLE_TX;
    } CRIT_END;
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

