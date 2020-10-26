.include "config.mk"
.include "../avr-mk/avr.mk"

PROG=	gpiotest

SRCS+=	task/gpiotest.c task/ttytest.c
SRCS+=  sys/devops.c sys/main.c sys/devices.c sys/tasks.c
SRCS+=  lib/console.c lib/xprintf.c
SRCS+=	${SRCS.y}

mk_gpio=	y
mk_usart=	y

SRCS.${mk_gpio}+=	sys/gpio.c
SRCS.${mk_usart}+=	sys/usart.c

TTY_SPEED=	10

CFLAGS+=	-Os -ffunction-sections -fdata-sections -std=gnu99
CFLAGS+=	-Werror -Wall -Wno-parentheses -Wno-error=unused-function
CFLAGS+=	-I${.CURDIR}
CFLAGS+=	-DTTY_SPEED=${TTY_SPEED}

LDFLAGS+=	-Os -Wl,--gc-sections

#USE_ARDUINO=

defines: ${F}
	${CC} ${CFLAGS} -dM -E ${F:D${.ALLSRC}:U-x c /dev/null} | sort

.include <avr.arduino.mk>
.include <avr.arduino.board.mk>
.include <avr.prog.mk>
