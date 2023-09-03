#ifndef __I2C_H__
#define __I2C_H__

#include "types.h"

//-----------------------------------------------------------------------------------
// Configuration
//-----------------------------------------------------------------------------------

typedef struct i2c_msg i2c_msg_t;
typedef struct i2c_dev i2c_dev_t;
typedef struct i2c_ops i2c_ops_t;
typedef struct i2c_cli i2c_cli_t;

// address mode
#define I2C_MASTER_ADDRESS_MODE_7BIT        BV(0)  // 7-bits address
#define I2C_MASTER_ADDRESS_MODE_10BIT       BV(1)  // 10-bits address

// data transmission direction
#define I2C_MASTER_WRITE                    BV(0)  // write data from master to slave
#define I2C_MASTER_RAED                     BV(2)  // read data from slave to master

// generate start or stop singal
#define I2C_MASTER_GENERATE_START_SINGAL    BV(3)
#define I2C_MASTER_GENERATE_STOP_SINGAL     BV(4)

// send ack when the current read length is less than the target read length
// send nack when the current read length reaches the target read length
#define I2C_MASTER_GENERATE_ACK_NACK_SINGAL BV(5)

// dont't generate stop singal when error occurs
#define I2C_MASTER_ERROR_TRIGGER_STOP       BV(0)
#define I2C_MASTER_ERROR_TRIGGER_NONE       BV(6)

// ignore nack singal when write address or data
#define I2C_MASTER_IGNORE_NACK              BV(7)

struct i2c_msg {
    uint16_t addr;  // slave address
    uint8_t* dat;
    uint16_t len;
    uint16_t flgs;  // flags: I2C_MASTER_xxx
};

// operations
struct i2c_ops {
    err_t (*xfer)(i2c_dev_t* dev, i2c_msg_t* msgs, uint16_t cnt);
    err_t (*ioctl)(int32_t cmd, void* args);
};

// device
struct i2c_dev {
    void*      drv;
    i2c_ops_t* ops;
};

struct i2c_cli {
    i2c_dev_t* dev;
    uint16_t   addr;
    void*      args;
};

//-----------------------------------------------------------------------------------
// Function
//-----------------------------------------------------------------------------------

static inline err_t i2cmst_xfer(i2c_dev_t* dev, i2c_msg_t* msgs, uint16_t cnt) { return dev->ops->xfer(dev, msgs, cnt); }

uint8_t i2cmst_scan(i2c_dev_t* dev);

err_t i2cmst_write_bytes(i2c_dev_t* dev, uint16_t addr, uint8_t reg, uint8_t* dat, uint16_t len);
err_t i2cmst_read_bytes(i2c_dev_t* dev, uint16_t addr, uint8_t reg, uint8_t* dat, uint16_t len);

static inline err_t i2cmst_read_byte(i2c_dev_t* dev, uint16_t addr, uint8_t reg, uint8_t* dat) { return i2cmst_read_bytes(dev, addr, reg, dat, 1); }
static inline err_t i2cmst_write_byte(i2c_dev_t* dev, uint16_t addr, uint8_t reg, uint8_t dat) { return i2cmst_write_bytes(dev, addr, reg, &dat, 1); }

static inline err_t i2cmst_read_word(i2c_dev_t* dev, uint16_t addr, uint16_t* dat);
static inline err_t i2cmst_write_word(i2c_dev_t* dev, uint16_t addr, uint16_t dat);

err_t i2cmst_read_bit(i2c_dev_t* dev, uint16_t addr, uint8_t reg, uint8_t stb, uint8_t* bit);
err_t i2cmst_write_bit(i2c_dev_t* dev, uint16_t addr, uint8_t reg, uint8_t stb, uint8_t bit);

err_t i2cmst_read_bits(i2c_dev_t* dev, uint16_t addr, uint8_t reg, uint8_t stb, uint8_t* bit, uint8_t len);
err_t i2cmst_write_bits(i2c_dev_t* dev, uint16_t addr, uint8_t reg, uint8_t stb, uint8_t bit, uint8_t len);

err_t i2cmst_read_mask(i2c_dev_t* dev, uint16_t addr, uint8_t reg, uint8_t msk, uint8_t* dat)
{
    uint8_t buf;
    err_t   ret;

    ret  = i2cmst_read_byte(dev, addr, reg, msk, &buf);
    *dat = IS_OK(ret) ? (buf & msk) : (0x00);

    return ret;
}

err_t i2cmst_write_mask(i2c_dev_t* dev, uint16_t addr, uint8_t reg, uint8_t msk, uint8_t dat)
{
    uint8_t buf;
    err_t   ret;

    ret = i2cmst_read_byte(dev, addr, reg, msk, &buf);

    if (IS_OK(ret)) {
        dat &= msk;
        buf &= ~msk;
        buf |= dat;
        ret = i2cmst_write_byte(dev, addr, reg, buf);
    }

    return ret;
}

#endif