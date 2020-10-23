#ifndef __sys_dev_h
#define __sys_dev_h

#include <sys/types.h>
#include <sys/config.h>
#include <sys/errno.h>
#include <sys/uio.h>

struct cdev;
struct devsw;
struct device;

typedef struct cdev             cdev_t;
typedef struct cdev_rw          cdev_rw_t;
typedef _FLASH struct devsw     devsw_t;
typedef _FLASH void             softc_t;
typedef _FLASH struct device    device_t;

/* The cdev lives in SRAM and holds the device-specific status. This
 * structure holds the generic device status; it is extended by devices
 * that need to keep their own data.
 */
struct cdev {
    byte    cd_flags;
};

struct cdev_rw {
    byte            cd_flags;
    struct iovec    cd_reading;
    struct iovec    cd_writing;
};

/* The devsw lives in flash and holds function pointers to the device
 * methods. Currently there is only one kind of devsw but some methods
 * may not be implemented by some devices.
 */
struct devsw {
    errno_t (*sw_open)  (device_t *d, byte mode);
    errno_t (*sw_ioctl) (device_t *d, ioc_t r, iocp_t p);
    errno_t (*sw_read)  (device_t *d);
    errno_t (*sw_write) (device_t *d);
};

/* The softc lives in flash and holds the device-specific configuration
 * (iops and such). There is no generic form, so softc_t* is void*.
 */

/* The device list lives in flash and device numbers index into the
 * list.
 */
struct device {
    devsw_t     *d_devsw;
    cdev_t      *d_cdev;
    union {
        softc_t *d_softc;
        byte    d_inplace[sizeof(softc_t *)];
    };
};

/* Flags for open/poll */
#define     DEV_OPEN        0x1     /* dev is ready for use */
#define     DEV_READING     0x2     /* dev is currently reading */
#define     DEV_WRITING     0x4     /* dev is currently writing */
#define     DEV_WR_FLASH    0x8     /* dev is writing from flash */

/* Flags for read/write */
#define     F_WAIT          0x1     /* wait for device to be ready */
#define     F_SYNC          0x2     /* wait for read/write to finish */
#define     F_FLASH         0x4     /* write from flash */

extern device_t Devices[];

/* Pull a cdev out of a device_t */
#define _device_cdev(_typ, _cd, _dev) \
    struct _typ ## _cdev *_cd = \
        (struct _typ ## _cdev *)(_dev)->d_cdev

/* This orrible mess deals with the (common) possibility that we have
 * only one device of a given type and we want to compile in all the
 * register accesses as constants. So, we can keep the softc in one of
 * three different places:
 *
 *  - If dev_HAVE_FIXED_SOFTC is defined in sys/config.h, we are using a
 *  fixed configuration. In this case dev_INIT_FIXED_SOFTC must also be
 *  defined to an initialiser suitable for struct dev_softc. This will
 *  be declared here as a (const) static variable, so under normal
 *  circumstances all references to it will be removed by the compiler,
 *  and it will not appear in the object file. This will not happen if
 *  pointers to it are passed around, so don't do that; use the device_t
 *  instead.
 *
 *  - Otherwise, if the softc is smaller than a (flash) pointer, keep in
 *  in the device_t where d_softc should be.
 *
 *  - Otherwise, we're out of options and we have to allocate a struct
 *  in flash in devices.c.
 *
 *  The macro _HANDLE_FIXED_SOFTC must be called in every device's .h
 *  file. It creates an inline function dev_softc(device_t *) which
 *  returns a pointer to the appropriate softc struct.
 */

#define _softc_struct(_typ) _FLASH struct _typ ## _softc
#define _softc_fixed(_typ)  _ ## _typ ## _fixed_softc

#define _HANDLE_FIXED_SOFTC(_typ) \
    static _softc_struct(_typ) _softc_fixed(_typ) \
        = _typ ## _INIT_FIXED_SOFTC; \
     \
    _MACRO _softc_struct(_typ) * \
    _typ ## _softc (device_t *d _UNUSED) \
    {  \
        if (_typ ## _HAVE_FIXED_SOFTC) \
            return &_softc_fixed(_typ); \
         \
        if (sizeof(_softc_struct(_typ)) > sizeof(softc_t *)) \
            return d->d_softc; \
         \
        return (_softc_struct(_typ) *)&d->d_softc; \
    } \
    \
    typedef _softc_struct(_typ) _typ ## _softc_t

__BEGIN_DECLS

_MACRO device_t *devnum2dev (dev_t d)
    { return &Devices[d]; }

_MACRO void cdev_set_flag (struct cdev *c, byte f)
    {   c->cd_flags |= f; }

_MACRO void cdev_clr_flag (struct cdev *c, byte f)
    {   c->cd_flags &= ~f; }

errno_t ioctl   (dev_t d, ioc_t r, iocp_t p);
errno_t open    (dev_t d, byte mode);
bool    poll    (dev_t d, byte mode);
errno_t read    (dev_t d, byte *b, size_t l, byte f);
errno_t write   (dev_t d, const byte *b, size_t l, byte f);

__END_DECLS

#endif
