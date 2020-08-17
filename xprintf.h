#ifndef __xprintf_h
#define __xprintf_h

#include <sys/types.h>

#define _S(_s)  (const byte *)(_s), sizeof(_s)

__BEGIN_DECLS

void    xprintf     (const char *fmt, ...);
void    print       (const char *s);

__END_DECLS

#endif
