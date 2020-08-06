.include "config.mk"
.include "../avr-mk/avr.mk"

PROG=	uart

SRCS=	uart.c

TTY_SPEED=	9600

defines: ${F}
	${CC} ${CFLAGS} -dM -E ${F:D${.ALLSRC}:U-x c /dev/null} | sort

.include <avr.arduino.board.mk>
.include <avr.prog.mk>
