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
    c_buffer    us_wr_buf;
} usart_cdev;

extern devsw_t usart_devsw;

#endif
