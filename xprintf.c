#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <xprintf.h>

#include <sys/dev.h>

#define END(_b) ((_b) + sizeof(_b))

static _FLASH char digits[16] = "0123456789abcdef";

static void
do_write (const char *buf, size_t len)
{
    write(DEV_tty0, (const byte *)buf, len, F_WAIT);
}

static char *
fmt_u (char *buf, unsigned int u, int base)
{
    while (1) {
        *--buf  = digits[u % base];
        if (u < base)
            return buf;
        u       = u / base;
    }
}

static char *
fmt_i (char *buf, int i)
{
    if (i < 0) {
        buf     = fmt_u(buf, -i, 10);
        *--buf  = '-';
    }
    else {
        buf     = fmt_u(buf, i, 10);
    }
    return buf;
}

void
xprintf (const char *f, ...)
{
    va_list     ap;
    const char  *p, *mark;
    char        buf[6];

    va_start(ap, f);

    p = mark = f;
    while (1) {
        /* An ordinary char: move along */
        if (*p != '\0' && *p != '%') {
            p++;
            continue;
        }

        /* Write out a constant section */
        if (p != mark)
            do_write(mark, p - mark);

        /* We've finished */
        if (*p == '\0')
            return;

        /* We've reached a % */
        p++;
        switch (*p) {
        /* Ignore a lone % at the end */
        case '\0':
            return;

        case '%':
            buf[0]  = '%';
            do_write(buf, 1);
            break;

        case 's':
            mark    = va_arg(ap, char *);
            do_write(mark, strlen(mark));
            break;

        /* (u)char is promoted to int */
        case 'c':
        case 'd':
            mark    = fmt_i(END(buf), va_arg(ap, int));
            do_write(mark, END(buf) - mark);
            break;

        case 'u':
            mark    = fmt_u(END(buf), va_arg(ap, unsigned int), 10);
            do_write(mark, END(buf) - mark);
            break;

        case 'x':
            mark    = fmt_u(END(buf), va_arg(ap, unsigned int), 16);
            do_write(mark, END(buf) - mark);
            break;

        default:
            buf[0] = '%';
            buf[1] = *p;
            do_write(buf, 2);
            break;
        }

        p++;
        mark = p;
    }

    va_end(ap);
}

void
print (const char *s)
{
    do_write(s, strlen(s));
}

