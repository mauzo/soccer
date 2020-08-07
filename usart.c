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

        // Set the mode (tx/tx/isr/etc)
        UCSR0B  = USART_ENABLE_TX;
    } CRIT_END;
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
    } CRIT_END;
}

ISR(USART_UDRE_vect)
{
    buffer *buf = &usart0.us_wr_buf;

    UDR0    = *buf->bf_ptr++;

    if (--buf->bf_len == 0) {
        buf->bf_ptr = buf->bf_xptr;
        buf->bf_len = buf->bf_xlen;
    }

    _delay_ms(500);
}

