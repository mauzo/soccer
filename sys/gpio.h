/*
 * sys/gpio.h: definitions for using GPIO pins.
 */

#ifndef __sys_gpio_h
#define __sys_gpio_h

#include <sys/types.h>
#include <sys/config.h>
#include <sys/console.h>

#include <avr/io.h>

#include <xprintf.h>

typedef byte    gpio_pin_t;
typedef byte    gpio_flag_t;
typedef byte    gpio_value_t;

struct gpio_softc {
    byte        gp_iop;
    gpio_pin_t  gp_maxpin;
};

/* for gpio_req->gp_value */
#define GPIO_PIN_LOW        0x0
#define GPIO_PIN_HIGH       0x1

/* for gpio_pin->gp_flags */
#define GPIO_PIN_INPUT          0x01
#define GPIO_PIN_OUTPUT         0x02
#define GPIO_PIN_PULLUP         0x04
#define GPIO_PIN_PRESET_HIGH    0x04    /* the same as PULLUP */
#define GPIO_PIN_PRESET_LOW     0x08

extern devsw_t  gpio_devsw;

gpio_pin_t  gpio_pin_max    (dev_t d);
errno_t gpio_pin_set_flag   (dev_t d, gpio_pin_t n, gpio_flag_t f);
errno_t gpio_pin_set        (dev_t d, gpio_pin_t n, gpio_value_t v);

_HANDLE_FIXED_SOFTC(gpio);

_MACRO byte 
gpio_iop (dev_t d, gpio_pin_t pin)
{
    device_t        *dev;
    gpio_softc_t    *sc;

    if (d > NDEV)
        panic("Device out of range");

    dev     = devnum2dev(d);
    sc      = gpio_softc(dev);

    if (pin > sc->gp_maxpin)
        panic("GPIO pin number too large");

    return sc->gp_iop + (pin/8)*3;
}

#define GPIO_PIN(_d, _p)    _SFR_MEM8(gpio_iop(_d, _p))
#define GPIO_DDR(_d, _p)    _SFR_MEM8(gpio_iop(_d, _p) + 1)
#define GPIO_PORT(_d, _p)   _SFR_MEM8(gpio_iop(_d, _p) + 2)
#define GPIO_BIT(_d, _p)    _ENSURE_USED(_d, 1 << ((_p) % 8))

_MACRO errno_t
gpio_pin_input (dev_t d, gpio_pin_t n)
{
    xprintf("Setting pin %d to input.\n", n);
    GPIO_DDR(d, n)      &= ~GPIO_BIT(d, n);
    GPIO_PORT(d, n)     &= ~GPIO_BIT(d, n);
    return 0;
}

/* Pullup implies input */
_MACRO errno_t
gpio_pin_pullup (dev_t d, gpio_pin_t n)
{
    xprintf("Setting pin %d to pullup.\n", n);
    GPIO_DDR(d, n)      &= ~GPIO_BIT(d, n);
    GPIO_PORT(d, n)     |= GPIO_BIT(d, n);
    return 0;
}

_MACRO errno_t
gpio_pin_output (dev_t d, gpio_pin_t n)
{
    xprintf("Setting pin %d to output.\n", n);
    GPIO_DDR(d, n)      |= GPIO_BIT(d, n);
    return 0;
}

_MACRO errno_t
gpio_pin_get (dev_t d, gpio_pin_t n)
{
    byte    v;

    v = GPIO_PIN(d, n) & GPIO_BIT(d, n);
    return v ? GPIO_PIN_HIGH : GPIO_PIN_LOW;
}

_MACRO errno_t
gpio_pin_low (dev_t d, gpio_pin_t n)
{
    GPIO_PORT(d, n)     &= ~GPIO_BIT(d, n);
    return 0;
}

_MACRO errno_t
gpio_pin_high (dev_t d, gpio_pin_t n)
{
    GPIO_PORT(d, n)     |= GPIO_BIT(d, n);
    return 0;
}

_MACRO errno_t
gpio_pin_toggle (dev_t d, gpio_pin_t n)
{
    GPIO_PIN(d, n)      |= GPIO_BIT(d, n);
    return 0;
}

#endif
