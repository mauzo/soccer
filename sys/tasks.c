/*
 * tasks.c: define the tasks for this build.
 */

#include <sys/config.h>
#include <sys/task.h>

task_run_t  ttytest_run;
task_run_t  gpiotest_run;
task_run_t  pkttest_run;
task_run_t  gpioctl_run;
task_run_t  iictest_run;

task_t Tasks[NTASK] = {
    [TASK_iictest]  = { .tsk_run = iictest_run },
};

