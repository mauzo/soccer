/*
 * panic.c: panic!
 *
 */

#include <stdlib.h>

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/config.h>
#include <sys/dev.h>
#include <sys/gpio.h>
#include <sys/sleep.h>
#include <sys/tty.h>
#include <sys/usart.h>

#include <arduino/uno.h>

#include <lib/console.h>

#define CONS_TTY_DEV    DEV_tty0
#define CONS_GPIO_DEV   DEV_gpio0
#define CONS_GPIO_PIN   GPIO_LED_BUILTIN

static _FLASH char panic_header[] = "\r\n\007PANIC!\r\n";
static _FLASH char panic_footer[] = "\r\nSystem halted.\r\n";

void
cons_flash (void)
{
    gpio_value_t    v;

    v   = gpio_pin_get(CONS_GPIO_DEV, CONS_GPIO_PIN);

    gpio_pin_high(CONS_GPIO_DEV, CONS_GPIO_PIN);
    delay(60000);
    gpio_pin_low(CONS_GPIO_DEV, CONS_GPIO_PIN);
    delay(60000);
    gpio_pin_set(CONS_GPIO_DEV, CONS_GPIO_PIN, v);
}

void
cons_setup (byte flags)
{
    byte    mode    = O_WRITE;

    if (flags & CONS_READ)
        mode    |= O_READ;

    open(CONS_TTY_DEV, mode);
    usart_setbaud(CONS_TTY_DEV, TTY_SPEED);
    usart_setmode(CONS_TTY_DEV, CS8);
    delay(2000000);
    sei();

    if (flags & CONS_FLASH)
        gpio_pin_output(CONS_GPIO_DEV, CONS_GPIO_PIN);

    print("Ready.\n");
}

void
cons_write (const char *msg, size_t sz, byte flags)
{
    const byte  *buf    = (const byte *)msg;

    write_queue(CONS_TTY_DEV, buf, sz, flags|F_CONSWRITE|F_WAIT);
    write_poll(CONS_TTY_DEV, buf, sz, flags|F_CONSWRITE|F_WAIT);
}

void
_print (_FLASH char *s, size_t sz)
{
    cons_write(s, sz, F_FLASH);
}

void
_panic (_FLASH char *msg, size_t sz)
{
    cons_write(panic_header, sizeof(panic_header), F_FLASH);
    cons_write(msg, sz, F_FLASH);
    cons_write(panic_footer, sizeof(panic_footer), F_FLASH);

    delay(5000000);
    exit(255);
}
