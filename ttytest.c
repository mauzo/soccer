/*
 * ttytest: test the tty interface.
 */

#include <sys/types.h>
#include <sys/usart.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#define MKBUF(s) { bf_ptr: (byte*)(s), bf_len: sizeof(s)-1 }

#define lengthof(x) (sizeof(x)/sizeof(*(x)))

static buffer msgs[] = {
    MKBUF("Hello world!"),
    MKBUF("foobar"),
    MKBUF("Goodbye..."),
};

int
main (void)
{
    byte i;

    _delay_ms(2000);
    usart_init(9600);
    sei();

    while (1) {
        for (i = 0; i < lengthof(msgs); i++) {
            usart_write(msgs[i].bf_ptr, msgs[i].bf_len);
            while (usart_poll(DEV_WRITING)) ;
            usart_write("\r\n", 2);
            while (usart_poll(DEV_WRITING)) ;
            _delay_ms(500);
        }
        _delay_ms(1000);
    }
}

