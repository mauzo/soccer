.include "config.mk"
.include "../avr-mk/avr.mk"

PROG=	gpiotest

SRCS+=	gpiotest.c
SRCS+=	devices.c devops.c gpio.c panic.c
SRCS+=	${SRCS.y}

SRCS.${mk_usart}=	usart.c

TTY_SPEED=	9600

CFLAGS+=	-Os -ffunction-sections -fdata-sections -std=gnu99
CFLAGS+=	-Wall -Wno-parentheses
CFLAGS+=	-I${.CURDIR}

LDFLAGS+=	-Os -Wl,--gc-sections
LIBS+=		-Wl,-u,vfprintf -lprintf_min

#USE_ARDUINO=

defines: ${F}
	${CC} ${CFLAGS} -dM -E ${F:D${.ALLSRC}:U-x c /dev/null} | sort

.include <avr.arduino.mk>
.include <avr.arduino.board.mk>
.include <avr.prog.mk>
