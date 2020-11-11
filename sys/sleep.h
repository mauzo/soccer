#ifndef _sys_sleep_h
#define _sys_sleep_h

#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

/* For now define delay in terms of _delay_ms, since I don't entirely
 * understand the limits of _delay_us and the __builtin it uses. Use a
 * us argument for forward-compat. */
#define delay(_us) _delay_ms((_us)/1000.0)

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
