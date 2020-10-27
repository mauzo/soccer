/*
 * ttytest: test the tty interface.
 */

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/dev.h>
#include <sys/task.h>
#include <sys/tty.h>
#include <sys/usart.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#include <lib/console.h>
#include <lib/xprintf.h>

enum {
    ST_SETUP    = 0,
    ST_PRINT,
    ST_WAIT,
};

static _FLASH iovec_t msgs[] = {
    str2iovf("Hello world!"),
    str2iovf("foobar"),
    str2iovf("Goodbye..."),
};

static uint32_t timer_cnt   = 0;

static wchan_t
setup (void)
{
    open(DEV_tty0, DEV_READING|DEV_WRITING);
    usart_setbaud(DEV_tty0, TTY_SPEED);
    usart_setmode(DEV_tty0, CS8);
    _delay_ms(2000);
    sei();

    print("Starting\n");

    return yield(ST_PRINT);
}

static wchan_t
print_msgs (void)
{
    byte    i;
    byte    buf[5];

    for (i = 0; i < lengthof(msgs); i++) {
        write(DEV_tty0, msgs[i].iov_base, msgs[i].iov_len, F_WAIT|F_FLASH);
        write(DEV_tty0, (byte *)"\n", 1, F_WAIT);
    }

    read(DEV_tty0, buf, 4, F_WAIT|F_SYNC);
    buf[4] = '\0';
    xprintf("Read [%s]\n", buf);

    timer_cnt = SECS_PER_CYCLE * 2;
    return yield(ST_WAIT);
}

static wchan_t
wait_until_msgs (void)
{
    timer_cnt--;

    if (timer_cnt)
        return yield(ST_WAIT);
    else
        return yield(ST_PRINT);
}
    
wchan_t
ttytest_run (byte next)
{
    switch (next) {
    case ST_SETUP:
        return setup();
    case ST_PRINT:
        return print_msgs();
    case ST_WAIT:
        return wait_until_msgs();
    }

    panic("ttytest: nothing to do!");
}
