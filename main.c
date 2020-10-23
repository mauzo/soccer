#include <sys/config.h>
#include <sys/task.h>
#include <sys/panic.h>

#include <xprintf.h>

task_run_t  ttytest_run;
task_run_t  gpiotest_run;

task_t Tasks[NTASK] = {
    { .tsk_run  = ttytest_run, },
    { .tsk_run  = gpiotest_run, },
};

int
main (void)
{
    byte    i;
    task_t  *t;
    wchan_t *w;

    while (1) {
        for (i = 0; i < NTASK; i++) {
            t   = &Tasks[i];
            w   = &t->tsk_wchan;
            
            switch (w->wc_type) {
            case W_RUN:
                xprintf("Running task %u with %u\n", i, w->wc_next);
                *w  = t->tsk_run(w->wc_next);
                break;

            default:
                panic("Bad wait state");
            }
        }
    }

    panic("Nothing to do!");
}
