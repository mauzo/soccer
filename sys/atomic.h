/*
 * sys/atomic.h: atomic operations and critical sections.
 */

#ifndef __SYS_ATOMIC_H
#define __SYS_ATOMIC_H

/* Use like this:
 *  CRIT_START {
 *      ...;
 *  } CRIT_END;
 */

#define CRIT_START ({ \
    byte _old_SREG = SREG; \
    cli();

#define CRIT_END \
    SREG = _old_SREG; })

#endif
