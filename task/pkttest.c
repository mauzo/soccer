/*
 * pkttest: test the packet read interface.
 */

#include <string.h>

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/dev.h>
#include <sys/sleep.h>
#include <sys/task.h>
#include <sys/tty.h>
#include <sys/usart.h>

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

static byte     buf[2][BUFLEN + 1];
static byte     pktlen;
static byte     which   = 0;

static void
queue_buf (byte b)
{
    xprintf("Queuing buf [%u]\n", b);

    memset(buf[b], 0, BUFLEN+1);
    read_queue(DEV_tty0, buf[b], BUFLEN, F_WAIT);

    debug_show_cdev_rw(DEV_tty0, _F("After queue"));
}

static task_st_t
st_setup (void)
{
    cons_setup(CONS_READ);
    return ST_RD_QUEUE;
}

static task_st_t
st_rd_queue (void)
{
    queue_buf(which);
    return ST_RD_LEN;
}

static task_st_t
st_rd_len (void)
{
    byte    *b  = buf[which];

    if (read_poll(DEV_tty0, b, 1, F_POLL) < 0)
        return ST_RD_LEN;

    pktlen  = b[0] - 'a' + 1;
    xprintf("[%u] Polled for len, got [%c] => [%u]\n", which, b[0], pktlen);

    xprintf("Before adjust b=[%x], pktlen=[%u]\n", (unsigned int)b, pktlen);
    debug_show_cdev_rw(DEV_tty0, _F("Before adjust"));
    if (read_adjust(DEV_tty0, b, pktlen) < 0)
        print("OVERFLOW!\n");
    debug_show_cdev_rw(DEV_tty0, _F("After adjust"));

    queue_buf(!which);

    return ST_RD_PKT;
}

static task_st_t
st_rd_pkt (void)
{
    byte    *b  = buf[which];

    if (read_poll(DEV_tty0, b, pktlen, F_POLL) < 0)
        return ST_RD_PKT;

    debug_show_cdev_rw(DEV_tty0, _F("After pkt poll"));
    xprintf("[%u] Read packet len [%u]: [%s]\n", which, pktlen, b);

    which   = !which;

    return ST_RD_LEN;
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
