/*
 * ttytest: test the tty interface.
 */

#include <sys/types.h>
#include <sys/dev.h>
#include <sys/tty.h>
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

    usart_open(DEV_tty0, DEV_WRITING);
    usart_ioctl(DEV_tty0, TIOCSETBAUD, 9600);
    usart_ioctl(DEV_tty0, TIOCSETMODE, CS8);
    _delay_ms(2000);
    sei();

    while (1) {
        for (i = 0; i < lengthof(msgs); i++) {
            poll(DEV_tty0, DEV_WRITING);
            usart_write(DEV_tty0, msgs[i].bf_ptr, msgs[i].bf_len);
            poll(DEV_tty0, DEV_WRITING);
            usart_write(DEV_tty0, _S("\r\n"));
            _delay_ms(500);
        }
        _delay_ms(1000);
    }
}

