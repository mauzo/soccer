/*
 * tasks.c: define the tasks for this build.
 */

#include <sys/config.h>
#include <sys/task.h>

task_run_t  ttytest_run;
task_run_t  gpiotest_run;

task_t Tasks[NTASK] = {
    [TASK_ttytest]  = 
        { .tsk_run  = ttytest_run, },
    [TASK_gpiotest] =
        { .tsk_run  = gpiotest_run, },
};

