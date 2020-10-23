/*
 * panic.c: panic!
 *
 */

#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/dev.h>

void
_panic (_FLASH char *msg, size_t sz)
{
    write(0, (const byte *)_F("\r\nPANIC!\r\n"), 10, F_FLASH|F_WAIT);
    write(0, (const byte *)msg, sz, F_FLASH|F_WAIT|F_SYNC);

    exit(255);
}
