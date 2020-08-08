#ifndef __sys_dev_h
#define __sys_dev_h

typedef const __flash struct device {
    void    *cdev;
} device;

#define DEV_tty0    0
#define NDEV        1

extern device Devices[NDEV];

#define dev2cdev(_d) (Devices[_d].cdev)

#endif
