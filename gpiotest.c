/*
 * gpiotest
 */

#include <xprintf.h>

#include <sys/types.h>
#include <sys/dev.h>
#include <sys/gpio.h>
#include <sys/tty.h>

#include <arduino/uno.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#define PIN_LIGHT   GPIO_LED_BUILTIN
#define PIN_SWITCH  GPIO_PIN_2

static void
setup_gpio (void)
{
    struct gpio_pin     gppin = { 0 };

    gppin.gp_pin    = PIN_LIGHT;
    gppin.gp_flags  = GPIO_PIN_OUTPUT|GPIO_PIN_PRESET_LOW;
    ioctl(DEV_gpio0, GPIOSETCONFIG, &gppin);

    gppin.gp_pin    = PIN_SWITCH;
    gppin.gp_flags  = GPIO_PIN_INPUT|GPIO_PIN_PULLUP;
    ioctl(DEV_gpio0, GPIOSETCONFIG, &gppin);
}

static void
setup_tty (void)
{
    open(DEV_tty0, DEV_WRITING);
    ioctl(DEV_tty0, TIOCSETBAUD, 9600);
    ioctl(DEV_tty0, TIOCSETMODE, CS8);

    sei();
    _delay_ms(1000);
    print("Starting...\n");
}

static void
setup (void)
{
    setup_tty();
    setup_gpio();
}

int
main (void)
{
    struct gpio_req     gpreq = { 0 };

    setup();

    while (1) {
        gpreq.gp_pin    = PIN_SWITCH;
        gpreq.gp_value  = GPIO_PIN_HIGH;

        while (gpreq.gp_value == GPIO_PIN_HIGH)
            ioctl(DEV_gpio0, GPIOGET, &gpreq);

        print("Button pressed!\n");

        gpreq.gp_pin    = PIN_LIGHT;
        ioctl(DEV_gpio0, GPIOTOGGLE, &gpreq);

        gpreq.gp_pin    = PIN_SWITCH;
        while (gpreq.gp_value == GPIO_PIN_LOW)
            ioctl(DEV_gpio0, GPIOGET, &gpreq);
    }

    write(DEV_tty0, _S("Finished.\n"), F_WAIT|F_SYNC);
}
