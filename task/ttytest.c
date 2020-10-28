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
    ST_READ,
};

#define BUFLEN 4

static uint32_t timer_cnt   = 0;
static byte     buf[2][BUFLEN + 1];
static byte     which = 0;

static wchan_t
st_setup (void)
{
    open(DEV_tty0, DEV_READING|DEV_WRITING);
    usart_setbaud(DEV_tty0, TTY_SPEED);
    usart_setmode(DEV_tty0, CS8);
    _delay_ms(2000);
    sei();

    print("Starting\n");
    xprintf("buf[0]: %x, buf[1]: %x\n", buf[0], buf[1]);

    read(DEV_tty0, buf[0], BUFLEN, 0);
    read(DEV_tty0, buf[1], BUFLEN, 0);
    return yield(ST_PRINT);
}

static wchan_t
st_print (void)
{
    xprintf("Reading into buf %c.\n", which);
    print("Type something:\n");

    return yield(ST_READ);
}

static void
show_tty0_cdev (_FLASH char *m)
{
    device_t    *dev    = devnum2dev(DEV_tty0);
    cdev_rw_t   *cd     = (cdev_rw_t *)dev->d_cdev;
    xprintf("%S: flags %x, reading %x, rd_next %x\n",
        m, cd->cd_flags, 
        cd->cd_reading.iov_base,
        cd->cd_read_next.iov_base);
}

static wchan_t
st_read (void)
{
    /* wait for the first read to finish */
    show_tty0_cdev(_F("Before poll"));
    poll(DEV_tty0, DEV_RD_NEXT);
    show_tty0_cdev(_F("After poll"));

    /* process the data read */
    xprintf("Read [%s]\n", buf[which]);

    /* set up the next read into the buf we just finished with */
    read(DEV_tty0, buf[which], BUFLEN, 0);

    /* use the other buf next time */
    which = !which;

    timer_cnt = SECS_PER_CYCLE * 10;
    return yield(ST_WAIT);
}

static wchan_t
st_wait (void)
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
        return st_setup();
    case ST_PRINT:
        return st_print();
    case ST_WAIT:
        return st_wait();
    case ST_READ:
        return st_read();
    }

    panic("ttytest: nothing to do!");
}
