/*
 * ttytest: test the tty interface.
 */

#include <sys/types.h>
#include <sys/usart.h>

#include <avr/interrupt.h>

static byte     buf[]   = "Hello world!\r\n";

int
main (void)
{
    usart_write(buf, sizeof(buf) - 1);
    usart_init(9600);
    sei();

    while (1) {
    }
}

