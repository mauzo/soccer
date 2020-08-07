/*
 * usart.c
 * Interface to the USART interface on AVR ATmega chips.
 */

#include <sys/types.h>
#include <sys/atomic.h>
#include <sys/usart.h>

#include <avr/interrupt.h>
#include <util/delay.h>

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

static void
tx_one (buffer *buf)
{
    UDR0    = *buf->bf_ptr++;
    buf->bf_len--;
}

void
usart_write (byte *ptr, size_t len)
{
    buffer  *buf    = &usart0.us_wr_buf;

    CRIT_START {
        buf->bf_ptr     = ptr;
        buf->bf_len     = len;
        buf->bf_xptr    = ptr;
        buf->bf_xlen    = len;

        usart0.us_flags |= DEV_WRITING;

        if (UCSR0A & UDRE0)
            tx_one(buf);
    } CRIT_END;
}

ISR(USART_UDRE_vect)
{
    buffer *buf = &usart0.us_wr_buf;

    if (buf->bf_len) {
        tx_one(buf);
    }
    else {
        //buf->bf_ptr     = buf->bf_xptr;
        //buf->bf_len     = buf->bf_xlen;

        usart0.us_flags &= ~DEV_WRITING;
    }

    _delay_ms(200);
}

