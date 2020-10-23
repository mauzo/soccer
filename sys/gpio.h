/*
 * sys/gpio.h: definitions for using GPIO pins.
 */

#ifndef __sys_gpio_h
#define __sys_gpio_h

#include <sys/types.h>
#include <sys/config.h>
#include <sys/console.h>

#include <avr/io.h>

struct gpio_pin {
    byte    gp_pin;
    byte    gp_flags;
};

struct gpio_req {
    byte    gp_pin;
    byte    gp_value;
};

struct gpio_softc {
    byte    gp_iop;
    byte    gp_maxpin;
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

/* ioctls */
#define GPIOMAXPIN          0x01
#define GPIOGETCONFIG       0x02
#define GPIOSETCONFIG       0x03
#define GPIOGET             0x04
#define GPIOSET             0x05
#define GPIOTOGGLE          0x06

extern devsw_t  gpio_devsw;

_HANDLE_FIXED_SOFTC(gpio);

_MACRO byte 
gpio_iop (device_t *d, byte pin)
{
    gpio_softc_t    *sc  = gpio_softc(d);

    if (pin > sc->gp_maxpin)
        panic("GPIO pin number too large");

    return sc->gp_iop + (pin/8)*3;
}

#define GPIO_PIN(_d, _p)    _SFR_MEM8(gpio_iop(_d, _p))
#define GPIO_DDR(_d, _p)    _SFR_MEM8(gpio_iop(_d, _p) + 1)
#define GPIO_PORT(_d, _p)   _SFR_MEM8(gpio_iop(_d, _p) + 2)
#define GPIO_BIT(_d, _p)    _ENSURE_USED(_d, 1 << ((_p) % 8))

#endif
