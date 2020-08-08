/*
 * ttytest: test the tty interface.
 */

#include <sys/types.h>
#include <sys/dev.h>
#include <sys/usart.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#define _S(_s) (const byte *)(_s), sizeof(_s)-1
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
    usart_init(DEV_tty0, 9600);
    sei();

    while (1) {
        for (i = 0; i < lengthof(msgs); i++) {
            usart_poll(DEV_tty0, DEV_WRITING);
            usart_write(DEV_tty0, msgs[i].bf_ptr, msgs[i].bf_len);
            usart_poll(DEV_tty0, DEV_WRITING);
            usart_write(DEV_tty0, _S("\r\n"));
            _delay_ms(500);
        }
        _delay_ms(1000);
    }
}

