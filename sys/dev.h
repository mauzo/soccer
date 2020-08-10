#ifndef __sys_dev_h
#define __sys_dev_h

#include <sys/types.h>
#include <sys/config.h>

typedef struct cdev {
    byte    cd_flags;
} cdev_t;

typedef const __flash struct devsw {
    void    (*sw_open)  (cdev_t *c, byte mode);
    void    (*sw_ioctl) (cdev_t *c, ioc_t r, uintptr_t p);
    void    (*sw_read)  (cdev_t *c, byte *b, size_t l);
    void    (*sw_write) (cdev_t *c, const byte *b, size_t l);
} devsw_t;

typedef const __flash struct device {
    cdev_t      *d_cdev;
    devsw_t     *d_devsw;
} device_t;

#define     DEV_OPEN        1
#define     DEV_READING     2
#define     DEV_WRITING     4

extern device_t Devices[];

#define _cdev_downcast(_typ, _mycd, _cd) \
    struct _typ ## _cdev *_mycd = (struct _typ ## _cdev *)(_cd)

__BEGIN_DECLS

_MACRO device_t *devnum2dev (dev_t d)
    { return &Devices[d]; }

_MACRO cdev_t *dev2cdev (dev_t d)
    {   return devnum2dev(d)->d_cdev; }

_MACRO void cdev_set_flag (struct cdev *c, byte f)
    {   c->cd_flags |= f; }

_MACRO void cdev_clr_flag (struct cdev *c, byte f)
    {   c->cd_flags &= ~f; }

void    ioctl   (dev_t d, ioc_t r, uintptr_t p);
void    open    (dev_t d, byte mode);
bool    poll    (dev_t d, byte mode);
void    read    (dev_t d, byte *b, size_t l, byte f);
void    write   (dev_t d, const byte *b, size_t l, byte f);

__END_DECLS

#endif
