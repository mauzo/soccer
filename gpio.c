/*
 * gpio.c: driver for the GPIO pins on the AVR.
 *
 */

#include <xprintf.h>

#include <sys/types.h>
#include <sys/dev.h>
#include <sys/gpio.h>

static errno_t gpio_ioctl (device_t *d, ioc_t r, iocp_t p);

devsw_t gpio_devsw = {
    sw_ioctl:   gpio_ioctl,
};

static void
gpio_set_config (device_t *d, byte n, byte flags)
{
    byte    bit = GPIO_BIT(d, n);

    if (flags & GPIO_PIN_INPUT) {
        xprintf("Setting pin %d to input.\r\n", n);
        GPIO_DDR(d, n)  &= ~bit;

        if (flags & GPIO_PIN_PULLUP) {
            xprintf("Setting pin %d to pullup.\r\n", n);
            GPIO_PORT(d, n) |= bit;
        }
        else {
            xprintf("Setting pin %d to no-pullup.\r\n", n);
            GPIO_PORT(d, n) &= ~bit;
        }
    }
    else if (flags & GPIO_PIN_OUTPUT) {
        if (flags & GPIO_PIN_PRESET_LOW) {
            xprintf("Setting pin %d to low.\r\n", n);
            GPIO_PORT(d, n) &= ~bit;
        }
        else if (flags & GPIO_PIN_PRESET_HIGH) {
            xprintf("Setting pin %d to high.\r\n", n);
            GPIO_PORT(d, n) |= bit;
        }

        xprintf("Setting pin %d to output.\r\n", n);
        GPIO_DDR(d, n)  |= bit;
    }
}

static errno_t
gpio_ioctl (device_t *d, ioc_t r, iocp_t p)
{
    gpio_softc_t    *sc     = gpio_softc(d);
    byte            n, bit;
    struct gpio_pin *pin;
    struct gpio_req *req;

    switch (r) {
    case GPIOMAXPIN:
        *(byte *)p.iop_ptr = sc->gp_maxpin;
        break;

    case GPIOSETCONFIG:
        pin = p.iop_ptr;
        gpio_set_config(d, pin->gp_pin, pin->gp_flags);
        break;

    case GPIOGET:
        req = p.iop_ptr;
        n   = req->gp_pin;
        bit = GPIO_BIT(d, n);

        req->gp_value   = (GPIO_PIN(d, n) & bit) ? 1 : 0;

        break;

    case GPIOSET:
        req = p.iop_ptr;
        n   = req->gp_pin;
        bit = GPIO_BIT(d, n);

        if (req->gp_value)
            GPIO_PORT(d, n) |= bit;
        else
            GPIO_PORT(d, n) &= ~bit;

        break;

    case GPIOTOGGLE:
        req = p.iop_ptr;
        n   = req->gp_pin;
        GPIO_PIN(d, n) |= GPIO_BIT(d, n);
        break;
    }

    return 0;
}
