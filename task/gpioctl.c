/*
 * gpioctl: control the gpio pins from the tty
 */

#include <sys/types.h>
#include <sys/dev.h>
#include <sys/gpio.h>
#include <sys/sleep.h>
#include <sys/task.h>
#include <sys/tty.h>
#include <sys/usart.h>

#include <lib/console.h>
#include <lib/debug.h>
#include <lib/xprintf.h>

enum {
    ST_SETUP    = 0,
    ST_READ,
    ST_ACTION,
};

static byte     buf[10];
static byte     *ptr;

static bool     check_nul       (byte b);
static void     do_op           (byte op, byte pin, byte val);
static void     do_pin_mode     (byte pin, byte val);
static void     do_pin_out      (byte pin, byte val);
static void     do_pin_read     (byte pin);
static byte    *parse_number    (byte *str, byte *num);
static void     process_cmd     (byte *cmd);

static bool
check_nul (byte b)
{
    if (!b) {
        print("Command too short!\n");
        return 0;
    }

    return 1;
}

static void
do_op (byte op, byte pin, byte val)
{
    xprintf("Running op [%c] pin [%u] val [%c]\n",
        op, pin, val);

    switch (op) {
    case 'm':   do_pin_mode(pin, val);          break;
    case 's':   do_pin_out(pin, val);           break;
    case 'r':   do_pin_read(pin);               break;
    default:    xprintf("Bad op [%c]", op);     break;
    }
}

static void
do_pin_mode (byte pin, byte val)
{
    if (!check_nul(val))
        return;

    switch (val) {
    case 'i':
        gpio_pin_input(DEV_gpio0, pin);
        break;
    case 'p':
        gpio_pin_pullup(DEV_gpio0, pin);
        break;
    case 'o':
        gpio_pin_output(DEV_gpio0, pin);
        break;
    default:
        xprintf("Bad pin mode [%c]!\n", val);
        break;
    }
}

static void
do_pin_out (byte pin, byte val)
{
    if (!check_nul(val))
        return;

    switch (val) {
    case 'h':
        gpio_pin_high(DEV_gpio0, pin);
        break;
    case 'l':
        gpio_pin_low(DEV_gpio0, pin);
        break;
    case 't':
        gpio_pin_toggle(DEV_gpio0, pin);
        break;
    default:
        xprintf("Bad pin value [%c]!\n", val);
        break;
    }
}

static void
do_pin_read (byte pin)
{
    byte v;

    v   = gpio_pin_get(DEV_gpio0, pin);
    xprintf("Pin [%u] is %S\n", pin, (v ? _F("HIGH") : _F("LOW")));
}

static byte *
parse_number (byte *str, byte *num)
{
    byte    n   = 0;

    while (1) {
        if (*str < '0' || *str > '9')
            break;

        n   = n * 10 + (*str - '0');
        str++;
    }

    *num    = n;
    return str;
}

static void
process_cmd (byte *cmd)
{
    byte    op;
    byte    pin;
    byte    val;

    op  = *cmd++;
    if (!check_nul(op))
        return;

    if (!(cmd = parse_number(cmd, &pin)))
        return;

    val = *cmd;
    do_op(op, pin, val);
}

static task_st_t
st_setup (void)
{
    open(DEV_tty0, O_READ|O_WRITE);
    usart_setbaud(DEV_tty0, TTY_SPEED);
    usart_setmode(DEV_tty0, CS8);
    delay(2000000);
    sei();

    print("Ready.\n");
    ptr     = buf;

    return ST_READ;
}

static task_st_t
st_read (void)
{
    read_queue(DEV_tty0, ptr, 1, F_WAIT);
    read_poll(DEV_tty0, ptr, 1, F_WAIT);

    if (!(*ptr == '\n' || *ptr == '\r')) {
        ptr++;
        return ST_READ;
    }

    *ptr    = '\0';
    xprintf("Read command [%s]\n", buf);
    process_cmd(buf);

    ptr     = buf;

    return ST_READ;
}

task_st_t
gpioctl_run (task_st_t next)
{
    switch (next) {
    case ST_SETUP:  return st_setup();
    case ST_READ:   return st_read();
    default:
        print("Bad state!");
        task_stop();
        return ST_SETUP;
    }
}
