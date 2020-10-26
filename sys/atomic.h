/*
 * sys/atomic.h: atomic operations and critical sections.
 */

#ifndef _sys_atomic_h
#define _sys_atomic_h

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
