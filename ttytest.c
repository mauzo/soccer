/*
 * ttytest: test the tty interface.
 */

#include <sys/types.h>
#include <sys/buf.h>
#include <sys/dev.h>
#include <sys/tty.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#define _S(_s) (const byte *)(_s), sizeof(_s)-1


static c_buffer msgs[] = {
    MKBUF("Hello world!"),
    MKBUF("foobar"),
    MKBUF("Goodbye..."),
};

int
main (void)
{
    byte i;

    open(DEV_tty0, DEV_WRITING);
    ioctl(DEV_tty0, TIOCSETBAUD, 9600);
    ioctl(DEV_tty0, TIOCSETMODE, CS8);
    _delay_ms(2000);
    sei();

    while (1) {
        for (i = 0; i < lengthof(msgs); i++) {
            poll(DEV_tty0, DEV_WRITING);
            write(DEV_tty0, msgs[i].bf_ptr, msgs[i].bf_len, 0);
            poll(DEV_tty0, DEV_WRITING);
            write(DEV_tty0, _S("\r\n"), 0);
            _delay_ms(500);
        }
        _delay_ms(1000);
    }
}

