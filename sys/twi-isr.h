/*
 * sys/twi-isr.h: inline ISR definitions for the TWI.
 * Making these inline avoids forcing the compiler to push/pop the whole
 * register set when calling.
 */

#ifndef _sys_twi_isr_h
#define _sys_twi_isr_h

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/twi.h>

#include <lib/console.h>

_MACRO void
twi_write_data(device_t *dev, cdev_rw_t *cd);

_MACRO void
twi_isr (device_t *dev)
{
    twi_cdev_t  *cd     = (twi_cdev_t *)dev->d_cdev;

    panic("twi_isr called!");

    switch (TWI_SR(dev)) {
    case TWI_SR_START:
        TWI_DR(dev)     = cd->tw_addr;
        TWI_CR(dev)     = TWI_CR_OK;
        return;

    case TWI_SR_ADDR_ACK:
    case TWI_SR_DATA_ACK:
        twi_write_data(dev, &cd->tw_buf);
        return;

    default:
        cons_flash();
        return;
    }
}

_MACRO void
twi_write_data (device_t *dev, cdev_rw_t *cd)
{
    iovec_t     *iov    = &cd->cd_writing;

    if (iov->iov_len) {
        if (cd->cd_wr_flags & DEV_WR_FLASH)
            TWI_DR(dev) = *(_FLASH byte *)iov->iov_base++;
        else
            TWI_DR(dev) = *(const byte *)iov->iov_base++;
        iov->iov_len--;

        TWI_CR(dev)     = TWI_CR_OK;
    }
    else {
        TWI_CR(dev)     = TWI_CR_OK|TWI_CR_STOP;
    }
}

#if 0
_MACRO void
twi_isr_udre (device_t *dev)
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

_MACRO void
twi_isr_rxc (device_t *dev)
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
#endif

#endif
