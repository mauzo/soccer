/*
 * iictest: test the i2c interface
 */

#include <sys/sleep.h>
#include <sys/task.h>
#include <sys/twi.h>

#include <lib/console.h>
#include <lib/xprintf.h>

enum {
    ST_START = 0,
    ST_DONE,
};

//static byte buf[] = "Hello!";

#define TW_INT  (1<<TWINT)
#define TW_STA  (1<<TWSTA)
#define TW_STO  (1<<TWSTO)
#define TW_EN   (1<<TWEN)

#define TW_SLAVE    0x13

iovec_t iov = str2iovf("Hello world!");

static void
wait_for_twi (device_t *d)
{
    print("Waiting for twi...\n");

    while (!(TWI_CR(d) & TW_INT))
        ;

    xprintf("twi CR [%x] SR [%x]\n", TWCR, TWSR);
}

task_st_t
iictest_run (task_st_t next)
{
    device_t    *d;

    if (next == ST_DONE) {
        task_stop();
        return ST_DONE;
    }

    cons_setup(CONS_FLASH);

    d   = devnum2dev(DEV_iic0);

    xprintf("Setting up bit rate (TWBR [%u])\n", TWBR);
    TWI_SRPS(d) = 0;
    TWI_BR(d)   = 72;
    xprintf("SRPS [%x]\n", TWI_SRPS(d));

    print("Sending START\n");
    TWI_CR(d)   = TW_INT|TW_STA|TW_EN;
    wait_for_twi(d);

    if (TWI_SR(d) != TWI_SR_START) {
        print("Start not sent\n");
        goto done;
    }

    print("Sending address\n");
    TWI_DR(d)   = TWI_ADDR(TW_SLAVE, TWI_WRITE);
    TWI_CR(d)   = TW_INT|TW_EN;
    wait_for_twi(d);

    if (TWI_SR(d) != TWI_SR_ADDR_ACK) {
        print("Addr not acked\n");
        goto done;
    }

    print("Sending data\n");

    while (iov.iov_len) {
        TWI_DR(d)   = *(_FLASH byte *)iov.iov_base;
        TWI_CR(d)   = TW_INT|TW_EN;
        wait_for_twi(d);

        if (TWI_SR(d) != TWI_SR_DATA_ACK) {
            print("Data not acked\n");
            goto done;
        }

        iov.iov_base++;
        iov.iov_len--;
    }

    print("Sending stop\n");
    TWI_CR(d)   = TW_INT|TW_EN|TW_STO;
    /* don't wait for stop */

    print("Done\n");

  done:
    return ST_DONE;
}
