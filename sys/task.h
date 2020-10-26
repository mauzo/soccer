#ifndef _sys_task_h
#define _sys_task_h

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/config.h>
#include <sys/errno.h>

struct task;
struct wchan;

typedef byte            tid_t;
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
    W_STOPPED,  /* Task is stopped, wake with task_wake */
    W_IRQ,      /* Waiting for an IRQ */
    W_SWI,      /* Waiting for a SWI */
    W_DEV,      /* Waiting for a device */
    W_TIME,     /* Waiting for a timer */
};

extern task_t Tasks[NTASK];

_MACRO task_t *
tid2task (tid_t t)
{
    if (t > NTASK) return NULL;
    return &Tasks[t];
}

_MACRO errno_t
task_wake (tid_t t)
{
    task_t  *task   = tid2task(t);

    if (!task) return ESRCH;
    task->tsk_wchan.wc_type = W_RUN;
    return 0;
}

_MACRO wchan_t
yield (byte next) 
{
    return (wchan_t){ .wc_next = next }; 
}

_MACRO wchan_t
wait (byte type, uint16_t detail, byte next)
{
    return (wchan_t){
        .wc_next    = next,
        .wc_type    = type,
        .wc_detail  = detail,
    };
}

#endif
