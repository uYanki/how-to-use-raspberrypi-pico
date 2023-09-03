#include "i2c.h"

uint8_t i2cmst_scan(i2c_dev_t* dev)
{
    uint8_t cnt = 0;
    uint8_t step;

    i2c_msg_t msg = {
        .addr = 0,
        .dat  = nullptr,
        .len  = 0,
        .flgs =
            I2C_MASTER_RAED |
            I2C_MASTER_ADDRESS_MODE_7BIT |
            I2C_MASTER_GENERATE_START_SINGAL |
            I2C_MASTER_GENERATE_STOP_SINGAL |
            I2C_MASTER_GENERATE_ACK_NACK_SINGAL,
    };

    println("i2cdev 7-bit address detector:");
    println("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f");

    while (msg.addr < 0x80) {
        printf("%02x: ", msg.addr);

        step = 0x10;
        do {
            if (IS_OK(i2cmst_xfer(dev, &msg, 1))) {
                printf("%02x ", msg.addr);
                ++cnt;
            } else {
                printf("-- ");
            }
            ++msg.addr;
        } while (--step);

        println("");
    }

    println(">> %d devices detected in this scan", cnt);

    return cnt;
}

err_t i2cmst_write_bytes(i2c_dev_t* dev, uint16_t addr, uint8_t reg, uint8_t* dat, uint16_t len)
{
    i2c_msg_t msg[2];

    msg[0].addr = addr;
    msg[0].flgs =
        I2C_MASTER_WRITE |
        I2C_MASTER_ADDRESS_MODE_7BIT |
        I2C_MASTER_GENERATE_START_SINGAL;
    msg[0].dat = &reg;
    msg[0].len = 1;

    msg[1].addr = addr;
    msg[1].flgs =
        I2C_MASTER_WRITE |
        I2C_MASTER_ADDRESS_MODE_7BIT |
        I2C_MASTER_GENERATE_STOP_SINGAL;
    msg[1].dat = dat;
    msg[1].len = len;

    return i2cmst_xfer(dev, msg, 2);
}

