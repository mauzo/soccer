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

iovec_t iov_poll    = str2iovf("[poll]");
iovec_t iov_irq     = str2iovf("[irq]");

static void
show_twi_reg (void)
{
    xprintf("twi BR [%u] CR [%x] SR [%x]\n", TWBR, TWCR, TWSR);
}

static void
wait_for_twi ()
{
    print("Waiting for twi...\n");

    while (!(TWCR & TW_INT))
        ;

    show_twi_reg();
}

static void
send_twi_poll (void)
{
    device_t    *d;

    d   = devnum2dev(DEV_iic0);

    print("Sending START\n");
    TWI_CR(d)   = TW_INT|TW_STA|TW_EN;
    wait_for_twi();

    if (TWI_SR(d) != TWI_SR_START) {
        print("Start not sent\n");
        return;
    }

    xprintf("Sending address [%x]\n", TWI_ADDR(TW_SLAVE, TWI_WRITE));
    TWI_DR(d)   = TWI_ADDR(TW_SLAVE, TWI_WRITE);
    TWI_CR(d)   = TW_INT|TW_EN;
    wait_for_twi();

    if (TWI_SR(d) != TWI_SR_ADDR_ACK) {
        print("Addr not acked\n");
        goto stop;
    }

    print("Sending data\n");

    while (iov_poll.iov_len) {
        TWI_DR(d)   = *(_FLASH byte *)iov_poll.iov_base;
        TWI_CR(d)   = TW_INT|TW_EN;
        wait_for_twi();

        if (TWI_SR(d) != TWI_SR_DATA_ACK) {
            print("Data not acked\n");
            goto stop;
        }

        iov_poll.iov_base++;
        iov_poll.iov_len--;
    }

  stop:
    print("Sending stop\n");
    TWI_CR(d)   = TW_INT|TW_EN|TW_STO;
    /* don't wait for stop */
}

task_st_t
iictest_run (task_st_t next)
{
    errno_t     err;

    if (next == ST_DONE) {
        task_stop();
        return ST_DONE;
    }

    cons_setup(CONS_FLASH);

    print("Opening device\n");
    twi_setaddr(DEV_iic0, TW_SLAVE);
    err = open(DEV_iic0, O_WRITE);
    panic_if(err, "open iic0");
    show_twi_reg();

    print("Polling write\n");
    send_twi_poll();

    print("Queueing write\n");
    err = write_queue(DEV_iic0, iov_irq.iov_base, iov_irq.iov_len, F_FLASH|F_WAIT);
    panic_if(err, "queue write");
    show_twi_reg();

    print("Polling\n");
    err = write_poll(DEV_iic0, iov_irq.iov_base, iov_irq.iov_len, F_WAIT);
    panic_if(err, "poll write");
    show_twi_reg();

    print("Done\n");

    return ST_DONE;
}
