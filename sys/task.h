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
typedef byte            task_st_t;
typedef task_st_t       task_run_t(task_st_t next);

struct task {
    task_run_t  *tsk_run;
    byte        tsk_state;
    task_st_t   tsk_next;
};

/* Values for wc_type */
enum task_state {
    W_RUN,      /* Task is runnable now */
    W_SLEEP,    /* Task is waiting for a device */
    W_STOPPED,  /* Task is stopped, wake with task_wake */
};

extern task_t   Tasks[NTASK];
extern tid_t    Currtask;

_MACRO task_t *
tid2task (tid_t t)
{
    if (t > NTASK) return NULL;
    return &Tasks[t];
}

_MACRO void
task_sleep (void)
{
    task_t  *task   = tid2task(Currtask);
    task->tsk_state = W_SLEEP;
}

_MACRO void
task_stop (void)
{
    task_t  *task   = tid2task(Currtask);
    task->tsk_state = W_STOPPED;
}

_MACRO errno_t
task_wake (tid_t t)
{
    task_t  *task   = tid2task(t);

    if (!task) return ESRCH;
    task->tsk_state = W_RUN;
    return 0;
}

#endif
