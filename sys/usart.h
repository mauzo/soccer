/*
 * usart.h
 * Interface to the USART on AVR ATmega chips.
 *
 */

#ifndef __sys_usart_h
#define __sys_usart_h

#include <sys/cdefs.h>
#include <sys/types.h>

#include <avr/io.h>

#define USART_ENABLE_TX     (1<<TXEN0)
#define USART_ENABLE_DRE    (1<<UDRIE0)

#define     DEV_OPEN        1
#define     DEV_READING     2
#define     DEV_WRITING     4

typedef struct buffer {
    byte    *bf_ptr;
    size_t  bf_len;
    byte    *bf_xptr;
    size_t  bf_xlen;
} buffer;

typedef struct usart_dev {
    byte    us_flags;
    buffer  us_wr_buf;
} usart_dev;

__BEGIN_DECLS

void    usart_init      (uint16_t baud);
void    usart_write     (byte *ptr, size_t len);
bool    usart_poll      (byte mode);

__END_DECLS

#endif
