/*
 * sys/gpio.h: definitions for using GPIO pins.
 */

#ifndef __sys_gpio_h
#define __sys_gpio_h

#define GPIO_IOP        0x23
#define GPIO_IOPx(_p)   (GPIO_IOP + ((_p) / 8) * 3)
#define GPIO_PIN(_p)    _SFR_MEM8(GPIO_IOPx(_p))
#define GPIO_DDR(_p)    _SFR_MEM8(GPIO_IOPx(_p) + 1)
#define GPIO_PORT(_p)   _SFR_MEM8(GPIO_IOPx(_p) + 2)
#define GPIO_BIT(_p)    (1 << ((_p) % 8))

#endif
