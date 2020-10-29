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

static task_st_t
st_setup (void)
{
    open(DEV_tty0, O_READ|O_WRITE);
    usart_setbaud(DEV_tty0, TTY_SPEED);
    usart_setmode(DEV_tty0, CS8);
    _delay_ms(2000);
    sei();

    print("Starting\n");
    print("And again...\n");
    xprintf("buf[0]: %x, buf[1]: %x\n", buf[0], buf[1]);

    read_setbuf(DEV_tty0, buf[0], BUFLEN, 0);
    read_setbuf(DEV_tty0, buf[1], BUFLEN, 0);
    return ST_PRINT;
}

static task_st_t
st_print (void)
{
    xprintf("Reading into buf %c.\n", which);
    print("Type something:\n");

    return ST_READ;
}

static void
show_tty0_cdev (_FLASH char *m)
{
    device_t    *dev    = devnum2dev(DEV_tty0);
    cdev_rw_t   *cd     = (cdev_rw_t *)dev->d_cdev;
    xprintf("%S: flags %x, reading %x, rd_next %x\n",
        m, cd->cd_rd_flags, 
        cd->cd_reading.iov_base,
        cd->cd_rd_next.iov_base);
}

static task_st_t
st_read (void)
{
    /* wait for the first read to finish */
    show_tty0_cdev(_F("Before poll"));
    poll(DEV_tty0, O_READ);
    show_tty0_cdev(_F("After poll"));

    /* process the data read */
    xprintf("Read [%s]\n", buf[which]);

    /* set up the next read into the buf we just finished with */
    read_setbuf(DEV_tty0, buf[which], BUFLEN, 0);

    /* use the other buf next time */
    which = !which;

    timer_cnt = SECS_PER_CYCLE * 10;
    return ST_WAIT;
}

static task_st_t
st_wait (void)
{
    timer_cnt--;

    if (timer_cnt)
        return ST_WAIT;
    else
        return ST_PRINT;
}
    
task_st_t
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
