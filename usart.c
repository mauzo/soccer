/*
 * usart.c
 * Interface to the USART interface on AVR ATmega chips.
 */

#include <sys/types.h>
#include <sys/atomic.h>
#include <sys/usart.h>

#include <avr/interrupt.h>

usart_dev   usart0 = { 0 };

void
usart_init (uint16_t baud)
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

bool
usart_poll (byte mode)
{
    return !!(usart0.us_flags & mode);
}

void
usart_write (byte *ptr, size_t len)
{
    buffer  *buf    = &usart0.us_wr_buf;

    CRIT_START {
        buf->bf_ptr     = ptr;
        buf->bf_len     = len;

        usart0.us_flags |= DEV_WRITING;
        UCSR0B          |= USART_ENABLE_DRE;
    } CRIT_END;
}

/* This ISR must either set UDR or disable itself. Otherwise it will
 * re-trigger immediately.
 */
ISR(USART_UDRE_vect)
{
    buffer *buf = &usart0.us_wr_buf;

    if (buf->bf_len) {
        UDR0    = *buf->bf_ptr++;
        buf->bf_len--;
    }
    else {
        UCSR0B          &= ~USART_ENABLE_DRE;
        usart0.us_flags &= ~DEV_WRITING;
    }
}

