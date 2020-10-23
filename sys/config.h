/*
 * sys/config.h: system configuration.
 */

#ifndef __sys_config_h
#define __sys_config_h

#define DEV_tty0    0
#define DEV_gpio0   1
#define NDEV        2

#define usart_HAVE_FIXED_SOFTC 0
#define usart_INIT_FIXED_SOFTC { us_iop: 0xC0 }

#define gpio_HAVE_FIXED_SOFTC 1
#define gpio_INIT_FIXED_SOFTC { .gp_iop = 0x23, .gp_maxpin = 23 }

#define NTASK       2

#endif
