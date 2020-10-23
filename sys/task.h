#ifndef _task_h
#define _task_h

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/config.h>

struct task;
struct wchan;

typedef struct task     task_t;
typedef struct wchan    wchan_t;
typedef wchan_t         task_run_t(byte next);

/* Keep this below 4 bytes. Any more than that and the AVR doesn't like
 * keeping it in registers when we return it as a literal. */
struct wchan {
    byte        wc_next;    /* task-private; passed to run() */
    byte        wc_type;    /* what we are waiting for */
    uint16_t    wc_detail;  /* which ^ are we waiting for */
};

struct task {
    wchan_t     tsk_wchan;
    task_run_t  *tsk_run;
};

/* Values for wc_type */
enum wchan_type {
    W_RUN,      /* Task is runnable now */
    W_IRQ,      /* Waiting for an IRQ */
    W_SWI,      /* Waiting for a SWI */
    W_DEV,      /* Waiting for a device */
    W_TIME,     /* Waiting for a timer */
};

extern task_t Tasks[NTASK];

static inline wchan_t
yield (byte next) { 
    return (wchan_t){ .wc_next = next }; 
}

static inline wchan_t
wait (byte type, uint16_t detail, byte next) {
    return (wchan_t){
        .wc_next    = next,
        .wc_type    = type,
        .wc_detail  = detail,
    };
}

#endif
