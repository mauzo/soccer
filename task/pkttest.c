/*
 * pkttest: test the packet read interface.
 */

#include <string.h>

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/dev.h>
#include <sys/task.h>
#include <sys/tty.h>
#include <sys/usart.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#include <lib/console.h>
#include <lib/debug.h>
#include <lib/xprintf.h>

enum {
    ST_SETUP    = 0,
    ST_RD_QUEUE,
    ST_RD_LEN,
    ST_RD_PKT,
    ST_STOP,
};

#define BUFLEN 26

static byte     buf[BUFLEN + 1];
static byte     pktlen;

static task_st_t
st_setup (void)
{
    open(DEV_tty0, O_READ|O_WRITE);
    usart_setbaud(DEV_tty0, TTY_SPEED);
    usart_setmode(DEV_tty0, CS8);
    _delay_ms(2000);
    sei();

    print("Starting\n");
    return ST_RD_QUEUE;
}

static task_st_t
st_rd_queue (void)
{
    print("Queuing buf\n");
    memset(buf, 0, BUFLEN+1);
    read_queue(DEV_tty0, buf, BUFLEN, F_WAIT);

    return ST_RD_LEN;
}

static task_st_t
st_rd_len (void)
{
    if (read_poll(DEV_tty0, &buf[0], F_POLL) < 0)
        return ST_RD_LEN;

    pktlen  = buf[0] - 'a' + 1;
    xprintf("Polled for len, got [%c] => [%u]\n", buf[0], pktlen);

    debug_show_cdev_rw(DEV_tty0, _F("Before adjust"), NULL);
    if (read_adjust(DEV_tty0, buf, pktlen) < 0)
        print("OVERFLOW!\n");
    debug_show_cdev_rw(DEV_tty0, _F("After adjust"), NULL);

    return ST_RD_PKT;
}

static task_st_t
st_rd_pkt (void)
{
    byte    *end    = &buf[pktlen - 1];

    if (read_poll(DEV_tty0, end, F_POLL) < 0)
        return ST_RD_PKT;

    debug_show_cdev_rw(DEV_tty0, _F("After pkt poll"), end);
    xprintf("Read packet len [%u]: [%s]\n", pktlen, buf);

    return ST_STOP;
}

task_st_t
pkttest_run (task_st_t next)
{
    switch (next) {
    case ST_SETUP:      return st_setup();
    case ST_RD_QUEUE:   return st_rd_queue();
    case ST_RD_LEN:     return st_rd_len();
    case ST_RD_PKT:     return st_rd_pkt();
    case ST_STOP:       task_stop(); return ST_STOP;
    default:            xprintf("Bad state [%u]", next); return ST_STOP;
    }
}
