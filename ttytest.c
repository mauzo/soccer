/*
 * ttytest: test the tty interface.
 */

#include <sys/types.h>
#include <sys/usart.h>

#include <avr/interrupt.h>
#include <util/delay.h>

static byte     buf[]   = "Hello world!\r\n";

int
main (void)
{
    usart_init(9600);
    sei();

    while (1) {
        //_delay_ms(1000);
        usart_write(buf, sizeof(buf) - 1);
        while (usart_poll(DEV_WRITING)) ;
    }
}

