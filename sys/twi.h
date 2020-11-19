/*
 * sys/twi.h
 * Interface to the TWI (I2C) interface on AVR ATmega chips.
 *
 */

#ifndef _sys_twi_h
#define _sys_twi_h

#include <sys/types.h>
#include <sys/config.h>
#include <sys/dev.h>
#include <sys/uio.h>

#include <avr/io.h>

#define TWI_SR_MASK         0xF8
#define TWI_SR_START        0x08
#define TWI_SR_RSTART       0x10
#define TWI_SR_ADDR_ACK     0x18
#define TWI_SR_ADDR_NACK    0x20
#define TWI_SR_DATA_ACK     0x28
#define TWI_SR_DATA_NACK    0x30
#define TWI_SR_ARB_LOST     0x38

#define TWI_CR_ENABLE       (1<<TWIE|1<<TWEN)
#define TWI_CR_OK           (1<<TWINT|TWI_CR_ENABLE)
#define TWI_CR_SENDACK      (1<<TWEA)
#define TWI_CR_START        (1<<TWSTA)
#define TWI_CR_STOP         (1<<TWSTO)

#define TWI_ADDR(_a, _rw)   ((_a) << 1 | (_rw))
#define TWI_WRITE           0
#define TWI_READ            1

struct twi_cdev {
    struct cdev_rw  tw_buf;
    byte            tw_addr;
};

struct twi_softc {
    byte        tw_iop;
};

typedef struct twi_cdev twi_cdev_t;

extern devsw_t twi_devsw;

_HANDLE_FIXED_SOFTC(twi);

_MACRO byte twi_iop (device_t *d) { return twi_softc(d)->tw_iop; }

struct twi_sr_reg {
    byte    tw_ps   : 2;
    byte    tw_sr   : 6;
};

#define TWI_BR(_d)      _SFR_MEM8(twi_iop(_d))
#define TWI_SRPS(_d)    _SFR_MEM8(twi_iop(_d) + 1)
#define TWI_AR(_d)      _SFR_MEM8(twi_iop(_d) + 2)
#define TWI_DR(_d)      _SFR_MEM8(twi_iop(_d) + 3)
#define TWI_CR(_d)      _SFR_MEM8(twi_iop(_d) + 4)
#define TWI_AMR(_d)     _SFR_MEM8(twi_iop(_d) + 5)

#define TWI_SR(_d)      (TWI_SRPS(_d) & TWI_SR_MASK)

__BEGIN_DECLS

_MACRO errno_t
twi_setaddr (dev_t d, byte addr)
{
    device_t    *dev    = devnum2dev(d);
    twi_cdev_t  *cd     = (twi_cdev_t *)dev->d_cdev;

    cd->tw_addr = addr << 1;
    return 0;
}

__END_DECLS

#endif
