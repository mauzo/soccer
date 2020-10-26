/*
 * panic.c: panic!
 *
 */

#include <stdlib.h>

#include <sys/types.h>
#include <sys/dev.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#include <lib/console.h>

static _FLASH char panic_header[] = "\r\n\007PANIC!\r\n";
static _FLASH char panic_footer[] = "\r\nSystem halted.\r\n";

void
cons_write (const char *msg, size_t sz, byte flags)
{
    write(0, (const byte *)msg, sz, flags|F_WAIT);
}

void
_print (_FLASH char *s, size_t sz)
{
    cons_write(s, sz, F_FLASH);
}

void
_panic (_FLASH char *msg, size_t sz)
{
    cons_write(panic_header, sizeof(panic_header), F_FLASH);
    cons_write(msg, sz, F_FLASH);
    cons_write(panic_footer, sizeof(panic_footer), F_FLASH|F_SYNC);

    _delay_ms(5000);
    exit(255);
}
