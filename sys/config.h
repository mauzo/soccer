/*
 * sys/config.h: system configuration.
 */

#ifndef _sys_config_h
#define _sys_config_h

#define SECS_PER_CYCLE  (F_CPU / 320)

#define CONS_TTY_DEV    DEV_tty0
#define CONS_GPIO_DEV   DEV_gpio0
#define CONS_GPIO_PIN   5           /* builtin led */

enum {
    DEV_tty0,
    DEV_gpio0,
    DEV_iic0,
    NDEV,
};

#define usart_HAVE_FIXED_SOFTC 1
#define usart_INIT_FIXED_SOFTC { .us_iop = 0xC0 }

#define gpio_HAVE_FIXED_SOFTC 1
#define gpio_INIT_FIXED_SOFTC { .gp_iop = 0x23, .gp_maxpin = 23 }

#define twi_HAVE_FIXED_SOFTC 1
#define twi_INIT_FIXED_SOFTC { .tw_iop = 0xB8 }

enum {
    TASK_iictest,
    NTASK,
};

#endif
