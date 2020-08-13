/*
 * gpio.c: driver for the GPIO pins on the AVR.
 *
 */

#include <sys/types.h>
#include <sys/dev.h>
#include <sys/gpio.h>

static void gpio_ioctl (device_t *d, ioc_t r, uintptr_t p);

devsw_t gpio_devsw = {
    sw_ioctl:   gpio_ioctl,
};

static void
gpio_ioctl (device_t *d, ioc_t r, uintptr_t p)
{
    byte            n, bit;
    struct gpio_pin *pin;
    struct gpio_req *req;

    switch (r) {
    case GPIOMAXPIN:
        *(byte *)p  = gpio_max_pin(d);
        break;

    case GPIOSETCONFIG:
        pin = (struct gpio_pin *)p;
        n   = pin->gp_pin;
        bit = GPIO_BIT(d, n);

        if (pin->gp_flags & GPIO_PIN_PULLUP)
            GPIO_PORT(d, n) |= bit;

        if (pin->gp_flags & GPIO_PIN_INPUT)
            GPIO_DDR(d, n)  &= ~bit;
        if (pin->gp_flags & GPIO_PIN_OUTPUT)
            GPIO_DDR(d, n)  |= bit;

        break;

    case GPIOSET:
        req = (struct gpio_req *)p;
        n   = req->gp_pin;
        bit = GPIO_BIT(d, n);

        if (req->gp_value)
            GPIO_PORT(d, n) |= bit;
        else
            GPIO_PORT(d, n) &= ~bit;

        break;
    }
}
