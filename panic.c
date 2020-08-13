/*
 * panic.c: panic!
 *
 */

#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/dev.h>

void
panic (const char *msg)
{
    write(0, (const byte *)"\r\nPANIC!\r\n", 10, F_WAIT);
    write(0, (const byte *)msg, strlen(msg), F_WAIT|F_SYNC);

    exit(255);
}
