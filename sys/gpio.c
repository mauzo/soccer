/*
 * gpio.c: driver for the GPIO pins on the AVR.
 *
 */

#include <sys/types.h>
#include <sys/dev.h>
#include <sys/gpio.h>

#include <lib/xprintf.h>

devsw_t gpio_devsw = { 0 };

static errno_t
get_softc (dev_t d, gpio_softc_t **sc)
{
    device_t    *dev;

    if (d > NDEV)
        return ENXIO;
    
    dev     = devnum2dev(d);
    *sc     = gpio_softc(dev);
    return 0;
}

gpio_pin_t
gpio_pin_max (dev_t d)
{
    gpio_softc_t    *sc;
    errno_t         err;

    if ((err = get_softc(d, &sc))) return err;
    return sc->gp_maxpin;
}

errno_t
gpio_pin_set_flag (dev_t d, gpio_pin_t n, gpio_flag_t flags)
{
    if (flags & GPIO_PIN_INPUT) {
        if (flags & GPIO_PIN_PULLUP)
            gpio_pin_pullup(d, n);
        else
            gpio_pin_input(d, n);
    }
    else if (flags & GPIO_PIN_OUTPUT) {
        if (flags & GPIO_PIN_PRESET_LOW)
            gpio_pin_low(d, n);
        else if (flags & GPIO_PIN_PRESET_HIGH)
            gpio_pin_high(d, n);

        gpio_pin_output(d, n);
    }

    return 0;
}

errno_t
gpio_pin_set (dev_t d, gpio_pin_t n, gpio_value_t v)
{
    switch (v) {
    case GPIO_PIN_HIGH:
        gpio_pin_high(d, n);
        return 0;
    case GPIO_PIN_LOW:
        gpio_pin_low(d, n);
        return 0;
    default:
        return EINVAL;
    }
}

