#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

typedef uint8_t byte;
typedef uint8_t bool;

byte    buf[]   = "Hello world!\r\n";
byte    *ptr    = buf;

#define     USART_TX    (1<<TXEN0)
#define     USART_UDRE  (1<<UDRIE0)

void
usart_init (uint16_t baud, uint8_t mode)
{
    uint16_t    ubrr    = F_CPU/16/baud - 1;

    // Set baud rate
    UBRR0H  = (byte)(ubrr >> 8);
    UBRR0L  = (byte)ubrr;

    // Set frame: 8 bit, 1 stop
    UCSR0C  = (3<<UCSZ00);

    // Set the mode (tx/tx/isr/etc)
    UCSR0B  = mode;
}

void
usart_busy_tx (uint8_t data)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1<<UDRE0))) ;

    // Put a character to send
    UDR0    = data;
}

ISR(USART_UDRE_vect)
{
    UDR0    = *ptr++;

    if (ptr == buf + sizeof(buf) - 1)
        ptr = buf;

    _delay_ms(500);
}

static void
busy_tx_sreg (void)
{
    byte    sreg, i;

    sreg    = SREG;
    for (i = 0; i < 8; i++)
        usart_busy_tx((sreg & 1<<(7 - i)) ? '1' : '0');
    usart_busy_tx('\r');
    usart_busy_tx('\n');
}

int
main (void)
{
    usart_init(9600, USART_TX|USART_UDRE);
    sei();

    while (1) {
    }
}

