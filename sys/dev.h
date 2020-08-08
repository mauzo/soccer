#ifndef __sys_dev_h
#define __sys_dev_h

#include <sys/types.h>

typedef byte    dev_t;

typedef const __flash struct device {
    void    *d_cdev;
} device;

struct cdev {
    byte    cd_flags;
};

#define DEV_tty0    0
#define NDEV        1

#define     DEV_OPEN        1
#define     DEV_READING     2
#define     DEV_WRITING     4

extern device Devices[NDEV];

__BEGIN_DECLS

_MACRO void *dev2cdev (dev_t d)
{   
    /* XXX we don't have panic() yet */
    //if (d >= NDEV) panic();
    return Devices[d].d_cdev; 
}

_MACRO void cdev_set_flag (struct cdev *c, byte f)
    {   c->cd_flags |= f; }

_MACRO void cdev_clr_flag (struct cdev *c, byte f)
    {   c->cd_flags &= ~f; }

bool    poll    (dev_t d, byte mode);

__END_DECLS

#endif
