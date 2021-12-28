/*
 * Copyright (c) YYYY YOUR NAME HERE <user@your.dom.ain>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#include <esp_err.h>
#include <esp_log.h>

#include "ip5306.h"

#define I2C_FREQ_HZ 400000

#define IP5306_DEFAULT_SETUP    0x37

#define IP5306_REG_SYS_CTL0     0x00  // initialize
#define IP5306_REG_READ0        0x70  // charge en
#define IP5306_REG_READ1        0x71  // charge full
#define IP5306_REG_READ3        0x78  // bat level

#define CHECK_ARG(ARG) do { if (!(ARG)) return ESP_ERR_INVALID_ARG; } while (0)

esp_err_t ip5306_init_desc(i2c_dev_t *dev, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio)
{
    CHECK_ARG(dev);

    dev->port = port;
    dev->addr = IP5306_I2C_ADDR;
    dev->cfg.sda_io_num = sda_gpio;
    dev->cfg.scl_io_num = scl_gpio;
#if HELPER_TARGET_IS_ESP32
    dev->cfg.master.clk_speed = I2C_FREQ_HZ;
#endif
    return i2c_dev_create_mutex(dev);
}

esp_err_t ip5306_setup(i2c_dev_t *dev, ip5306_setup_params_t params) {
    CHECK_ARG(dev);

    uint8_t init_params = params.power_boost_enabled
            ? 0x37
            : 0x35;

    I2C_DEV_TAKE_MUTEX(dev);
    I2C_DEV_CHECK(dev, i2c_dev_write_reg(dev, IP5306_REG_SYS_CTL0, &init_params, 1));
    I2C_DEV_GIVE_MUTEX(dev);

    return ESP_OK;
}

esp_err_t ip5306_free_desc(i2c_dev_t *dev)
{
    CHECK_ARG(dev);

    return i2c_dev_delete_mutex(dev);
}

esp_err_t ip5306_is_charger_connected(i2c_dev_t *dev, bool *connected)
{
    CHECK_ARG(dev && connected);

    uint8_t val;

    I2C_DEV_TAKE_MUTEX(dev);
    I2C_DEV_CHECK(dev, i2c_dev_read_reg(dev, IP5306_REG_READ0, &val, 1));
    I2C_DEV_GIVE_MUTEX(dev);

    *connected = val & (1 << 3);

    return ESP_OK;
}

esp_err_t ip5306_is_charge_full(i2c_dev_t *dev, bool *full)
{
    CHECK_ARG(dev && full);

    uint8_t val;

    I2C_DEV_TAKE_MUTEX(dev);
    I2C_DEV_CHECK(dev, i2c_dev_read_reg(dev, IP5306_REG_READ1, &val, 1));
    I2C_DEV_GIVE_MUTEX(dev);

    *full = val & (1 << 3);

    return ESP_OK;
}

static uint8_t map_battery_level(uint8_t val)
{
    switch (val & 0xF0) {
        case 0xE0: return 25;
        case 0xC0: return 50;
        case 0x80: return 75;
        case 0x00: return 100;
        default: return 0;
    }
}

esp_err_t ip5306_get_battery_level(i2c_dev_t *dev, uint8_t *battery_level)
{
    CHECK_ARG(dev && battery_level);

    uint8_t val;

    I2C_DEV_TAKE_MUTEX(dev);
    I2C_DEV_CHECK(dev, i2c_dev_read_reg(dev, IP5306_REG_READ3, &val, 1));
    I2C_DEV_GIVE_MUTEX(dev);

    *battery_level = map_battery_level(val);

    return ESP_OK;
}
