#ifndef __sys_sleep_h
#define __sys_sleep_h

#include <avr/sleep.h>

#define sleep_while(_cond) ({ \
    byte    _old_sreg; \
    \
    set_sleep_mode(SLEEP_MODE_IDLE); \
    _old_sreg   = SREG; \
    cli(); \
    while (_cond) { \
        sleep_enable(); \
        sei(); \
        sleep_cpu(); \
        sleep_disable(); \
        cli(); \
    } \
    SREG = _old_sreg; \
})

#endif
