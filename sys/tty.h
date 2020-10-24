/*
 * sys/tty.h: tty interface.
 */

#ifndef __sys_tty_h
#define __sys_tty_h

/* Bits for tty_[gs]etmode */
#define CS5         0x0     /* 5 data bits */
#define CS6         0x1     /* 6 data bits */
#define CS7         0x2     /* 7 data bits */
#define CS8         0x3     /* 8 data bits */
#define CSIZE       0x3     /* mask for data size values */
#define CSTOPB      0x4     /* 2 stop bits; else 1 */
#define PARENB      0x8     /* enable parity */
#define PARODD      0xa     /* odd parity; else even */

#endif
