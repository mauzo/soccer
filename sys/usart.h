/*
 * usart.h
 * Interface to the USART on AVR ATmega chips.
 *
 */

#ifndef __SOC_USART_H
#define __SOC_USART_H

#include <sys/types.h>
#include <avr/io.h>

#define     USART_TX    (1<<TXEN0)
#define     USART_UDRE  (1<<UDRIE0)

void    usart_init      (uint16_t baud, uint8_t mode);
void    usart_busy_tx   (byte data);
void    usart_set_buf   (byte *buf, size_t len);

#endif
