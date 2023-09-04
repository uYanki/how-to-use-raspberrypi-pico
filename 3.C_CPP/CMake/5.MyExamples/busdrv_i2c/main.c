

#include "hardware/gpio.h"

#include "swi2c.h"

static void swi2c_set_sda(void* port, pin_lvl_e level)
{
    swi2c_cfg_t* cfg = (swi2c_cfg_t*)port;
    gpio_put(cfg->sda, level == LOW ? 0 : 1);
}

static void swi2c_set_scl(void* port, pin_lvl_e level)
{
    swi2c_cfg_t* cfg = (swi2c_cfg_t*)port;
    gpio_put(cfg->scl, level == LOW ? 0 : 1);
}

static pin_lvl_e swi2c_get_sda(void* port)
{
    swi2c_cfg_t* cfg = (swi2c_cfg_t*)port;
    return gpio_get(cfg->sda) == 0 ? LOW : HIGH;
}

static pin_lvl_e swi2c_get_scl(void* port)
{
    swi2c_cfg_t* cfg = (swi2c_cfg_t*)port;
    return gpio_get(cfg->scl) == 0 ? LOW : HIGH;
}

void swi2c_cfg_sda(void* port, pin_dir_e dir)
{
    swi2c_cfg_t* cfg = (swi2c_cfg_t*)port;
    gpio_set_dir(cfg->sda, dir == OUT ? GPIO_OUT : GPIO_IN);
}

void swi2c_cfg_scl(void* port, pin_dir_e dir)
{
    swi2c_cfg_t* cfg = (swi2c_cfg_t*)port;
    gpio_set_dir(cfg->scl, dir == OUT ? GPIO_OUT : GPIO_IN);
}

void swi2c_demo(void)
{
    swi2c_cfg_t cfg = {
        .name = "swi2c-bus",
        .sda  = 1,
        .scl  = 2,
    };

    swi2c_drv_t drv = {
        .port     = &cfg,
        .cfg_scl  = swi2c_cfg_scl,
        .cfg_sda  = swi2c_cfg_sda,
        .set_sda  = swi2c_set_sda,
        .set_scl  = swi2c_set_scl,
        .get_sda  = swi2c_get_sda,
        .get_scl  = swi2c_get_scl,
        .delay    = sleep_us,
        .interval = 5,
        .timeout  = 100,
    };

    i2c_bus_t bus = {
        .drv = &drv,
        .ops = &g_swi2c_ops,
    };

    // ssd1306
    i2c_dev_t dev = {
        .bus  = &bus,
        .addr = 0x3C,
    };

    println("%s", cfg.name);

    gpio_init(cfg.sda);
    gpio_init(cfg.scl);

    gpio_pull_up(cfg.sda);
    gpio_pull_up(cfg.scl);

    uint8_t demo = 3;

    while (1) {
        switch (demo) {
            case 1: {
                // address scan
                i2cmst_scanner(&bus);
                break;
            }
            case 2: {
                // transmit test (use logic-analyzer)
                i2c_msg_t msg = {
                    .addr = 0x10,
                    .dat  = (uint8_t[]){0x11, 0x22, 0x33, 0x44},
                    .len  = 4,
                    .flgs = I2CMST_WRITE | I2CMST_ADDR_7BIT,
                };

                i2cmst_xfer(&bus, &msg, 1);
                sleep_ms(200);
                msg.flgs |= I2CMST_IGNORE_NACK;
                i2cmst_xfer(&bus, &msg, 1);

                printf(".");
                break;
            }
            case 3: {
                extern void ssd1306_init(i2c_dev_t * dev);
                extern void ssd1306_fill(i2c_dev_t * dev, uint8_t color);

                static bool init = false;

                if (!init) {
                    init = true;
                    ssd1306_init(&dev);
                }

                static uint8_t color = 0x0F;
                ssd1306_fill(&dev, color);
                color = ~color;
            }
            default: {
                break;
            }
        }

        sleep_ms(2000);
    }
}

void hwi2c_init()
{
    // i2c_init(cfg.handle, 100 * 1000);

    // gpio_init(cfg.sda);
    // gpio_init(cfg.scl);

    // gpio_pull_up(cfg.sda);
    // gpio_pull_up(cfg.scl);

    // gpio_set_function(cfg.sda, GPIO_FUNC_I2C);
    // gpio_set_function(cfg.scl, GPIO_FUNC_I2C);

    // bi_decl(bi_2pins_with_func(cfg.sda, cfg.scl, GPIO_FUNC_I2C));
}

int main()
{
    stdio_init_all();
    // getchar();
    swi2c_demo();
    return 0;
}

void ssd1306_init(i2c_dev_t* dev)
{
    uint8_t cmd[] = {
        0xae,
        0xd5,
        0x80,
        0xa8,
        0x3f,
        0xd3,
        0x00,
        0x40,
        0x20,
        0x00,
        0xa1,
        0xc8,
        0xda,
        0x12,
        0x81,
        0xcf,
        0xd9,
        0xf1,
        0xdb,
        0x30,
        0x8d,
        0x14,
        0x2e,
        0xa4,
        0xa6,
        0xaf,
    };

    i2cdev_write_bytes(dev, 0x00, cmd, sizeof(cmd));
}

void ssd1306_fill(i2c_dev_t* dev, uint8_t color)
{
    uint8_t x0 = 0;
    uint8_t x1 = 127;
    uint8_t y0 = 0;
    uint8_t y1 = 7;

    uint8_t cmd[] = {
        0x21, x0, x1,  // col
        0x22, y0, y1,  // row
    };

    i2cdev_write_bytes(dev, 0x00, cmd, sizeof(cmd));

    uint8_t dat[16];
    uint8_t cnt = sizeof(dat);
    while (cnt--) {
        dat[cnt] = color;
    }

    cnt = 64;
    while (cnt--) {
        i2cdev_write_bytes(dev, 0x40, dat, sizeof(dat));
    }
}
