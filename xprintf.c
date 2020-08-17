#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <xprintf.h>

#include <sys/dev.h>

static char buf[64];

void
xprintf (const char *f, ...)
{
    va_list     ap;

    /* We must poll before formatting or we overwrite the buffer */
    poll(DEV_tty0, DEV_WRITING);

    va_start(ap, f);
    vsnprintf(buf, sizeof(buf), f, ap);
    va_end(ap);

    write(DEV_tty0, (byte *)buf, strlen(buf), 0);
}

void
print (const char *s)
{
    write(DEV_tty0, (const byte *)s, strlen(s), F_WAIT);
}

