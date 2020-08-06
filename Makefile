.include "config.mk"
.include "../avr-mk/avr.mk"

PROG=	uart

SRCS=	usart.c

TTY_SPEED=	9600

CFLAGS=		-Os

defines: ${F}
	${CC} ${CFLAGS} -dM -E ${F:D${.ALLSRC}:U-x c /dev/null} | sort

.include <avr.arduino.board.mk>
.include <avr.prog.mk>
