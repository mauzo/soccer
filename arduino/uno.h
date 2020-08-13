/*
 * arduino/uno.h: defines for using the Arduino Uno.
 *
 * The GPIO pin numbering on the Arduino boards is weird. Provide some
 * defines to make this a bit easier.
 */

#ifndef __arduino_uno_h
#define __arduino_uno_h

#define GPIO_PIN_0      16  /* port D, pins 0-7 */
#define GPIO_PIN_1      17
#define GPIO_PIN_2      18
#define GPIO_PIN_3      19
#define GPIO_PIN_4      20
#define GPIO_PIN_5      21
#define GPIO_PIN_6      22
#define GPIO_PIN_7      23

#define GPIO_PIN_8      0   /* port B, pins 0-5 */
#define GPIO_PIN_9      1
#define GPIO_PIN_10     2
#define GPIO_PIN_11     3
#define GPIO_PIN_12     4
#define GPIO_PIN_13     5

#define GPIO_PIN_A0     8   /* port C, pins 0-5 */
#define GPIO_PIN_A1     9
#define GPIO_PIN_A2     10
#define GPIO_PIN_A3     11
#define GPIO_PIN_A4     12
#define GPIO_PIN_A5     13

#define GPIO_LED_BUILTIN    GPIO_PIN_13

#endif
