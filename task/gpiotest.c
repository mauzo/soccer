/*
 * gpiotest
 */

#include <sys/types.h>
#include <sys/dev.h>
#include <sys/gpio.h>
#include <sys/task.h>

#include <lib/console.h>
#include <arduino/uno.h>

#define PIN_LIGHT   GPIO_LED_BUILTIN
#define PIN_SWITCH  GPIO_PIN_2

static void
setup_gpio (void)
{
    gpio_pin_output(DEV_gpio0, PIN_LIGHT);
    gpio_pin_pullup(DEV_gpio0, PIN_SWITCH);
}

static bool
check_switch ()
{
    return gpio_pin_get(DEV_gpio0, PIN_SWITCH) == GPIO_PIN_LOW;
}

static void
toggle_light ()
{
    gpio_pin_toggle(DEV_gpio0, PIN_LIGHT);
}

enum {
    ST_START    = 0,
    ST_WAIT_PRESS,
    ST_WAIT_RELEASE,
};


task_st_t
gpiotest_run (byte next)
{
    switch (next) {
    case ST_START:
        setup_gpio();
        return ST_WAIT_PRESS;

    case ST_WAIT_PRESS:
        if (!check_switch())
            return ST_WAIT_PRESS;
        
        print("Button pressed!\n");
        toggle_light();
        return ST_WAIT_RELEASE;

    case ST_WAIT_RELEASE:
        if (check_switch())
            return ST_WAIT_RELEASE;
        else
            return ST_WAIT_PRESS;
    }

    panic("Nothing to do!");
}
