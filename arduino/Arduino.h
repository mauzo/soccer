#ifndef __Arduino_h
#define __Arduino_h

#include <string.h>
#include <sys/usart.h>
#include <util/delay.h>

#define delay(x) _delay_ms(x)

class HardwareSerial
{
    public:
        void    begin   (unsigned long baud) {
            usart_init(baud);
        }
        size_t  write   (const uint8_t *buf, size_t size) {
            usart_write((uint8_t *)buf, size);
            while (usart_poll(DEV_WRITING)) ;
            return size;
        }
        size_t  write   (const char *str) {
            if (str == NULL) return 0;
            return write((const uint8_t *)str, strlen(str));
        }
};

extern HardwareSerial Serial;

#endif
