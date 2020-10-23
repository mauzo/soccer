/*
 * gpiotest
 */

#include <sys/types.h>
#include <sys/console.h>
#include <sys/dev.h>
#include <sys/gpio.h>
#include <sys/task.h>

#include <arduino/uno.h>

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

static bool
check_switch ()
{
    struct gpio_req     gpreq   = { .gp_pin = PIN_SWITCH };

    ioctl(DEV_gpio0, GPIOGET, &gpreq);

    return (gpreq.gp_value == GPIO_PIN_LOW);
}

static void
toggle_light ()
{
    struct gpio_req     gpreq   = { .gp_pin = PIN_LIGHT };

    ioctl(DEV_gpio0, GPIOTOGGLE, &gpreq);
}

enum {
    ST_START    = 0,
    ST_WAIT_PRESS,
    ST_WAIT_RELEASE,
};


wchan_t
gpiotest_run (byte next)
{
    switch (next) {
    case ST_START:
        setup_gpio();
        return yield(ST_WAIT_PRESS);

    case ST_WAIT_PRESS:
        if (!check_switch())
            return yield(ST_WAIT_PRESS);
        
        print("Button pressed!\n");
        toggle_light();
        return yield(ST_WAIT_RELEASE);

    case ST_WAIT_RELEASE:
        if (check_switch())
            return yield(ST_WAIT_RELEASE);
        else
            return yield(ST_WAIT_PRESS);
    }

    panic("Nothing to do!");
}
