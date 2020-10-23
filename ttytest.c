/*
 * ttytest: test the tty interface.
 */

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/dev.h>
#include <sys/panic.h>
#include <sys/task.h>
#include <sys/tty.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#define _S(_s) (const byte *)(_s), sizeof(_s)-1

enum {
    ST_SETUP    = 0,
    ST_LOOP,
};

static iovec_t msgs[] = {
    str2iovf("Hello world!"),
    str2iovf("foobar"),
    str2iovf("Goodbye..."),
};

static void
setup (void)
{
    open(DEV_tty0, DEV_WRITING);
    ioctl(DEV_tty0, TIOCSETBAUD, 9600);
    ioctl(DEV_tty0, TIOCSETMODE, CS8);
    _delay_ms(2000);
    sei();

    write(DEV_tty0, (byte *)"Starting\n", 9, F_WAIT);
}

static void
loop (void)
{
    byte    i;

    for (i = 0; i < lengthof(msgs); i++) {
        write(DEV_tty0, msgs[i].iov_base, msgs[i].iov_len, F_WAIT|F_FLASH);
        write(DEV_tty0, (byte *)"\n", 1, F_WAIT);
        _delay_ms(500);
    }
    _delay_ms(1000);
}

wchan_t
ttytest_run (byte next)
{
    switch (next) {
    case ST_SETUP:
        setup();
        return (wchan_t){ .wc_next = ST_LOOP };
    case ST_LOOP:
        loop();
        return (wchan_t){ .wc_next = ST_LOOP };
    default:
        panic("ttytest: bad state");
    }
}
