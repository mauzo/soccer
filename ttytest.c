/*
 * ttytest: test the tty interface.
 */

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/dev.h>
#include <sys/tty.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#define _S(_s) (const byte *)(_s), sizeof(_s)-1


static iovec_t msgs[] = {
    str2iovf("Hello world!"),
    str2iovf("foobar"),
    str2iovf("Goodbye..."),
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

    write(DEV_tty0, (byte *)"Starting\r\n", 10, F_WAIT);

    while (1) {
        for (i = 0; i < lengthof(msgs); i++) {
            write(DEV_tty0, msgs[i].iov_base, msgs[i].iov_len, F_WAIT|F_FLASH);
            write(DEV_tty0, (byte *)"\r\n", 2, F_WAIT);
            _delay_ms(500);
        }
        _delay_ms(1000);
    }
}

