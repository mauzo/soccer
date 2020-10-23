.include "config.mk"
.include "../avr-mk/avr.mk"

PROG=	gpiotest

SRCS+=	gpiotest.c ttytest.c
SRCS+=	console.c devices.c devops.c xprintf.c main.c
SRCS+=	${SRCS.y}

mk_gpio=	y
mk_usart=	y

SRCS.${mk_gpio}+=	gpio.c
SRCS.${mk_usart}+=	usart.c

TTY_SPEED=	9600

CFLAGS+=	-Os -ffunction-sections -fdata-sections -std=gnu99
CFLAGS+=	-Werror -Wall -Wno-parentheses -Wno-error=unused-function
CFLAGS+=	-I${.CURDIR}

LDFLAGS+=	-Os -Wl,--gc-sections

#USE_ARDUINO=

defines: ${F}
	${CC} ${CFLAGS} -dM -E ${F:D${.ALLSRC}:U-x c /dev/null} | sort

.include <avr.arduino.mk>
.include <avr.arduino.board.mk>
.include <avr.prog.mk>
