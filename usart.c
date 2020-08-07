/*
 * usart.c
 * Interface to the USART interface on AVR ATmega chips.
 */

#include <sys/types.h>
#include <sys/usart.h>

#include <avr/interrupt.h>
#include <util/delay.h>

typedef struct {
    byte    *buf;
    byte    *ptr;
    size_t  len;
} usart_dev;

usart_dev   usart0 = { buf: NULL, ptr: NULL, len: 0 };

void
usart_init (uint16_t baud, uint8_t mode)
{
    uint16_t    ubrr    = F_CPU/16/baud - 1;

    // Set baud rate
    //UBRR0H  = (byte)(ubrr >> 8);
    //UBRR0L  = (byte)ubrr;
    UBRR0   = ubrr;

    // Set frame: 8 bit, 1 stop
    UCSR0C  = (3<<UCSZ00);

    // Set the mode (tx/tx/isr/etc)
    UCSR0B  = mode;
}

void
usart_busy_tx (byte data)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1<<UDRE0))) ;

    // Put a character to send
    UDR0    = data;
}

void
usart_set_buf(byte *buf, size_t len)
{
    usart0.buf  = buf;
    usart0.ptr  = buf;
    usart0.len  = len - 1;
}

ISR(USART_UDRE_vect)
{
    UDR0    = *usart0.ptr++;

    if (usart0.ptr == usart0.buf + usart0.len)
        usart0.ptr = usart0.buf;

    _delay_ms(500);
}

