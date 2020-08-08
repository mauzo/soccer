/*
 * usart.h
 * Interface to the USART on AVR ATmega chips.
 *
 */

#ifndef __sys_usart_h
#define __sys_usart_h

#include <sys/types.h>
#include <sys/dev.h>

#include <avr/io.h>

#define USART_ENABLE_TX     (1<<TXEN0)
#define USART_ENABLE_DRE    (1<<UDRIE0)

typedef struct buffer {
    byte    *bf_ptr;
    size_t  bf_len;
} buffer;

typedef struct c_buffer {
    const byte  *bf_ptr;
    size_t      bf_len;
} c_buffer;

typedef struct usart_cdev {
    struct cdev us_cdev;
    c_buffer    us_wr_buf;
} usart_cdev;

__BEGIN_DECLS

void    usart_ioctl     (dev_t d, ioc_t r, uintptr_t p);
void    usart_open      (dev_t d, byte mode);
void    usart_write     (dev_t d, const byte *ptr, size_t len);

__END_DECLS

#endif
