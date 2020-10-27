/*
 * sys/usart.h
 * Interface to the USART on AVR ATmega chips.
 *
 */

#ifndef _sys_usart_h
#define _sys_usart_h

#include <sys/types.h>
#include <sys/config.h>
#include <sys/dev.h>
#include <sys/uio.h>

#include <avr/io.h>

#define USART_ENABLE_TX     (1<<TXEN0)
#define USART_ENABLE_RX     (1<<RXEN0)
#define USART_IRQ_UDRE      (1<<UDRIE0)
#define USART_IRQ_TXC       (1<<TXCIE0)
#define USART_IRQ_RXC       (1<<RXCIE0)

struct usart_cdev {
    struct cdev_rw  us_cdev;
};

struct usart_softc {
    byte        us_iop;
};

extern devsw_t usart_devsw;

_HANDLE_FIXED_SOFTC(usart);

_MACRO byte usart_iop (device_t *d) { return usart_softc(d)->us_iop; }

#define USART_CSRA(_d)  _SFR_MEM8(usart_iop(_d))
#define USART_CSRB(_d)  _SFR_MEM8(usart_iop(_d) + 1)
#define USART_CSRC(_d)  _SFR_MEM8(usart_iop(_d) + 2)
#define USART_BRR(_d)   _SFR_MEM16(usart_iop(_d) + 4)
#define USART_DR(_d)    _SFR_MEM8(usart_iop(_d) + 6)

__BEGIN_DECLS

errno_t usart_setbaud   (dev_t d, uint16_t spd);
errno_t usart_setmode   (dev_t d, byte mode);

__END_DECLS

#endif
