.include "config.mk"
.include "../avr-mk/avr.mk"

PROG=	ttytest

SRCS+=	ttytest.c
SRCS+=	devices.c syscalls.c
SRCS+=	${SRCS.y}

SRCS.${mk_usart}=	usart.c

TTY_SPEED=	9600

CFLAGS+=	-ffunction-sections -fdata-sections -Os -std=gnu99
CFLAGS+=	-Wall -Wno-parentheses
CFLAGS+=	-I${.CURDIR}

LDFLAGS+=	-Os -Wl,--gc-sections

#USE_ARDUINO=

defines: ${F}
	${CC} ${CFLAGS} -dM -E ${F:D${.ALLSRC}:U-x c /dev/null} | sort

.include <avr.arduino.mk>
.include <avr.arduino.board.mk>
.include <avr.prog.mk>
