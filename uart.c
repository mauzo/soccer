#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#define sbi(p, b) ((p) |= (1 << (b)))
#define cbi(p, b) ((p) &= ~(1 << (b)))

typedef uint8_t byte;
typedef uint8_t bool;

byte    buf[]   = "Hello world!\r\n";
byte    *ptr    = buf;

/* registers:
 *  UBRR0H/L, UCSR0A/B/C, UDR0, RXEN0, TXEN0, RXCIE0, UDRIE0, U2X0
 */

#if 0
#if F_CPU == 16000000UL
#  define USE_U2X_57600 0
#else
#  define USE_U2X_57600 1
#endif

void
uart_init (uint16_t baud)
{
    bool        u2x;
    uint16_t    ubrr;

    u2x     = 1;
    ubrr    = (F_CPU / 4 / baud - 1) / 2;

    if (ubrr > 4095 || (baud == 57600 && !USE_U2X_57600)) {
        u2x     = 0;
        ubrr    = (F_CPU / 8 / baud - 1) / 2;
    }
        
    if (u2x)
        UCSR0A = 1 << U2X0;
    else
        UCSR0A = 0;

    UBRR0H     = ubrr >> 8;
    UBRR0L     = ubrr;

    sbi(UCSR0B, RXEN0);
    sbi(UCSR0B, TXEN0);
    sbi(UCSR0B, RXCIE0);
    cbi(UCSR0B, UDRIE0);
}

ISR(USART_UDRE_vect)
{
    UDR0    = *ptr++;

    if (ptr - buf == sizeof(buf))
        ptr = buf;
}
#endif

int
main (void)
{
    uint16_t    baud    = 9600;
    uint16_t    ubrr    = F_CPU/16/baud - 1;

    // Set baud rate
    UBRR0H  = (byte)(ubrr >> 8);
    UBRR0L  = (byte)ubrr;

    // Set frame: 8 bit, 1 stop
    UCSR0C  = (3<<UCSZ00);

    // Enable tx
    UCSR0B  = (1<<TXEN0);

    while (1) {
        // Wait for empty transmit buffer
        while (!(UCSR0A & (1<<UDRE0))) ;

        // Put a character to send
        UDR0    = 'X';
    }
}

