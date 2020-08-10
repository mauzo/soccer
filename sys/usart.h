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
#ifndef USART_FIXED_IOP
    byte        us_iop;
#endif
    c_buffer    us_wr_buf;
};

extern devsw_t usart_devsw;

#ifdef USART_FIXED_IOP
/* Make sure we 'use' _c (-Wunused-value/variable) */
# define USART_IOP(_c) ((void)(_c), USART_FIXED_IOP)
#else
# define USART_IOP(_c) (_c)->us_iop
#endif

#define USART_CSRA(_c)  _SFR_MEM8(USART_IOP(_c))
#define USART_CSRB(_c)  _SFR_MEM8(USART_IOP(_c) + 1)
#define USART_CSRC(_c)  _SFR_MEM8(USART_IOP(_c) + 2)
#define USART_BRR(_c)   _SFR_MEM16(USART_IOP(_c) + 4)
#define USART_DR(_c)    _SFR_MEM8(USART_IOP(_c) + 6)

__BEGIN_DECLS

void    usart_isr_udre  (device_t *dev);

__END_DECLS

#endif
