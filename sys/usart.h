/*
 * usart.h
 * Interface to the USART on AVR ATmega chips.
 *
 */

#ifndef __sys_usart_h
#define __sys_usart_h

#include <sys/types.h>
#include <sys/buf.h>
#include <sys/dev.h>

#include <avr/io.h>

#define USART_ENABLE_TX     (1<<TXEN0)
#define USART_ENABLE_DRE    (1<<UDRIE0)

struct usart_cdev {
    struct cdev us_cdev;
    c_buffer    us_wr_buf;
};

extern devsw_t usart_devsw;

#ifdef USART_FIXED_IOP
_MACRO byte usart_iop (device_t *d _UNUSED) { return USART_FIXED_IOP; }
#else
_MACRO byte usart_iop (device_t *d) { return d->d_config[0]; }
#endif

#define USART_CSRA(_d)  _SFR_MEM8(usart_iop(_d))
#define USART_CSRB(_d)  _SFR_MEM8(usart_iop(_d) + 1)
#define USART_CSRC(_d)  _SFR_MEM8(usart_iop(_d) + 2)
#define USART_BRR(_d)   _SFR_MEM16(usart_iop(_d) + 4)
#define USART_DR(_d)    _SFR_MEM8(usart_iop(_d) + 6)

__BEGIN_DECLS

void    usart_isr_udre  (device_t *dev);

__END_DECLS

#endif
