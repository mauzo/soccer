/*
 * sys/usart-isr.h: inline ISR definitions for the USART.
 * Making these inline avoids forcing the compiler to push/pop the whole
 * register set when calling.
 */

#ifndef _sys_usart_isr_h
#define _sys_usart_isr_h

#include <sys/cdefs.h>
#include <sys/usart.h>

_MACRO void
usart_isr_rxc (device_t *dev)
{
    cdev_rw_t   *cd     = (cdev_rw_t *)dev->d_cdev;
    iovec_t     *iov    = &cd->cd_reading;

    if (iov->iov_len) {
        *(byte *)iov->iov_base++    = USART_DR(dev);
        if (--iov->iov_len)
            return;
    }

    if (cd->cd_rd_flags & DEV_RD_NEXT) {
        cd->cd_reading  = cd->cd_rd_next;
        cd->cd_rd_flags &= ~DEV_RD_NEXT;
        cd->cd_rd_next  = (iovec_t){ 0 };
        return;
    }

    USART_CSRB(dev) &= ~USART_IRQ_RXC;
    cd->cd_rd_flags &= ~DEV_READING;
}

_MACRO void
usart_isr_udre (device_t *dev)
{
    cdev_rw_t   *cd     = (cdev_rw_t *)dev->d_cdev;
    iovec_t     *iov    = &cd->cd_writing;

    if (iov->iov_len) {
        if (cd->cd_wr_flags & DEV_WR_FLASH)
            USART_DR(dev)   = *(_FLASH byte *)iov->iov_base++;
        else
            USART_DR(dev)   = *(const byte *)iov->iov_base++;
        iov->iov_len--;
    }
    if (!iov->iov_len) {
        USART_CSRB(dev) &= ~USART_IRQ_UDRE;
        cd->cd_wr_flags &= ~DEV_WRITING;
    }
}

#endif
