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

typedef struct usart_cdev {
    struct cdev us_cdev;
    byte        us_iop;
    c_buffer    us_wr_buf;
} usart_cdev;

extern devsw_t usart_devsw;

#define USART_CSRA(_c)  _SFR_MEM8((_c)->us_iop)
#define USART_CSRB(_c)  _SFR_MEM8((_c)->us_iop + 1)
#define USART_CSRC(_c)  _SFR_MEM8((_c)->us_iop + 2)
#define USART_BRR(_c)   _SFR_MEM16((_c)->us_iop + 4)
#define USART_DR(_c)    _SFR_MEM8((_c)->us_iop + 6)

__BEGIN_DECLS

void    usart_isr_udre  (device_t *dev);

__END_DECLS

#endif
