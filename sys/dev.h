#ifndef __sys_dev_h
#define __sys_dev_h

#include <sys/types.h>
#include <sys/config.h>

struct cdev;
struct devsw;
struct device;

typedef struct cdev             cdev_t;
typedef _FLASH struct devsw     devsw_t;
typedef _FLASH struct device    device_t;

struct cdev {
    byte    cd_flags;
};

struct devsw {
    void    (*sw_open)  (device_t *d, byte mode);
    void    (*sw_ioctl) (device_t *d, ioc_t r, uintptr_t p);
    void    (*sw_read)  (device_t *d, byte *b, size_t l);
    void    (*sw_write) (device_t *d, const byte *b, size_t l);
};

struct device {
    devsw_t     *d_devsw;
    cdev_t      *d_cdev;
    union {
        _FLASH void *d_fdev;
        byte        d_config[2];
    };
};

#define     DEV_OPEN        1
#define     DEV_READING     2
#define     DEV_WRITING     4

extern device_t Devices[];

#define _device_cdev(_typ, _cd, _dev) \
    struct _typ ## _cdev *_cd = \
        (struct _typ ## _cdev *)(_dev)->d_cdev

__BEGIN_DECLS

_MACRO device_t *devnum2dev (dev_t d)
    { return &Devices[d]; }

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
