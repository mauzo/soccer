/*
 * twi.c
 * Interface to the TWI (I2C) interface on AVR ATmega chips.
 */

#include <sys/types.h>
#include <sys/atomic.h>
#include <sys/dev.h>
#include <sys/errno.h>
#include <sys/twi.h>

#include <avr/interrupt.h>

#include <lib/console.h>

static errno_t twi_open   (device_t *c, byte mode);
static errno_t twi_read   (device_t *d);
static errno_t twi_write  (device_t *c);

devsw_t twi_devsw = {
    .sw_open    = twi_open,
    .sw_read    = twi_read,
    .sw_write   = twi_write,
};

static errno_t
twi_open (device_t *d, byte mode)
{
    TWI_CR(d)   = TWI_CR_ENABLE;

    return 0;
}

static errno_t
twi_read (device_t *d)
{
    return EINVAL;
}

static errno_t
twi_write (device_t *d)
{
    TWI_CR(d)   |= TWI_CR_START;

    return 0;
}

