#include <sys/config.h>
#include <sys/task.h>

#include <lib/console.h>
#include <lib/xprintf.h>

tid_t   Currtask;

int
main (void)
{
    tid_t       i;
    task_t      *t;

    while (1) {
        for (i = 0; i < NTASK; i++) {
            Currtask    = i;
            t           = &Tasks[i];
            
            switch (t->tsk_state) {
            case W_RUN:
                //xprintf("Running task %u with %u\n", i, w->wc_next);
                t->tsk_next  = t->tsk_run(t->tsk_next);
                break;

            case W_SLEEP:
            case W_STOPPED:
                break;

            default:
                panic("Bad wait state");
            }
        }
    }

    panic("Nothing to do!");
}
