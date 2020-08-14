/*
 * gpiotest
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/buf.h>
#include <sys/dev.h>
#include <sys/gpio.h>
#include <sys/tty.h>

#include <arduino/uno.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#define _S(_s) (const byte *)(_s), sizeof(_s)-1

#define NPINS 4
const byte pin[NPINS] = {
    GPIO_PIN_13,
    GPIO_PIN_10,
    GPIO_PIN_3,
    GPIO_PIN_A1,
};

static char buf[64];

void
xprintf (const char *f, ...)
{
    va_list     ap;

    /* We must poll before formatting or we overwrite the buffer */
    poll(DEV_tty0, DEV_WRITING);

    va_start(ap, f);
    vsnprintf(buf, sizeof(buf), f, ap);
    va_end(ap);

    write(DEV_tty0, (byte *)buf, strlen(buf), 0);
}

void
print (const char *s)
{
    write(DEV_tty0, (const byte *)s, strlen(s), F_WAIT);
}

int
main (void)
{
    int                 i;
    struct gpio_pin     gppin = { 0 };
    struct gpio_req     gpreq = { 0 };

    open(DEV_tty0, DEV_WRITING);
    ioctl(DEV_tty0, TIOCSETBAUD, 9600);
    ioctl(DEV_tty0, TIOCSETMODE, CS8);
    sei();
    _delay_ms(1000);
    print("Starting...\r\n");

    gppin.gp_flags  = GPIO_PIN_OUTPUT;
    gpreq.gp_value  = GPIO_PIN_LOW;
    for (i = 0; i < NPINS; i++) {
        xprintf("Setting pin %i to output low.\r\n", pin[i]);

        gppin.gp_pin    = pin[i];
        ioctl(DEV_gpio0, GPIOSETCONFIG, (iocp_t)&gppin);

        gpreq.gp_pin    = pin[i];
        ioctl(DEV_gpio0, GPIOSET, (iocp_t)&gpreq);
    }

    while (1) {
        for (i = 0; i < NPINS; i++) {
            gpreq.gp_pin    = pin[i];

            _delay_ms(1000);
            xprintf("Setting pin %i high.\r\n", pin[i]);
            gpreq.gp_value   = GPIO_PIN_HIGH;
            ioctl(DEV_gpio0, GPIOSET, (iocp_t)&gpreq);

            _delay_ms(500);
            xprintf("Setting pin %i low.\r\n", pin[i]);
            gpreq.gp_value  = GPIO_PIN_LOW;
            ioctl(DEV_gpio0, GPIOSET, (iocp_t)&gpreq);
        }
    }

    write(DEV_tty0, _S("Finished.\r\n"), F_WAIT|F_SYNC);
}
