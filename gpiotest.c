/*
 * gpiotest
 */

#include <string.h>

#include <sys/types.h>
#include <sys/buf.h>
#include <sys/dev.h>
#include <sys/tty.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#define _S(_s) (const byte *)(_s), sizeof(_s)-1

/* Port B, pin 5 */

#define PIN (1<<5)

void
print (const char *s)
{
    write(DEV_tty0, (const byte *)s, strlen(s), F_WAIT);
}

int
main (void)
{
    open(DEV_tty0, DEV_WRITING);
    ioctl(DEV_tty0, TIOCSETBAUD, 9600);
    ioctl(DEV_tty0, TIOCSETMODE, CS8);
    sei();
    _delay_ms(1000);
    print("Starting...\r\n");

    print("Setting pin B7 to output low.\r\n");
    PORTB   &= ~PIN;
    DDRB    |= PIN;

    while (1) {
        _delay_ms(1000);
        print("Setting pin high.\r\n");
        PORTB   |= PIN;

        _delay_ms(1000);
        print("Setting pin low.\r\n");
        PORTB   &= ~PIN;
    }

    write(DEV_tty0, _S("Finished.\r\n"), F_WAIT|F_SYNC);
}
