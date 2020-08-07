/*
 * ttytest: test the tty interface.
 */

#include <sys/types.h>
#include <sys/usart.h>

#include <avr/interrupt.h>

static byte     buf[]   = "Hello world!\r\n";

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
    usart_set_buf(&buf[0], sizeof(buf));
    usart_init(9600, USART_TX|USART_UDRE);
    sei();

    while (1) {
    }
}

