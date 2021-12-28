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

/**
 * @file ip5306.h
 * @defgroup example example
 * @{
 *
 * An example component.
 *
 */

#include <i2cdev.h>
#include <esp_err.h>
#include <stdbool.h>

#if !defined(__IP5306__H__)
#define __IP5306__H__

#ifdef __cplusplus

extern "C" {
#endif

#define IP5306_I2C_ADDR 0x75

typedef struct {
    bool power_boost_enabled;
} ip5306_setup_params_t;

/**
 * @brief Initialize device descriptor.
 *
 * @param dev      Device descriptor
 * @param port     I2C port
 * @param sda_gpio SDA GPIO
 * @param scl_gpio SCL GPIO
 * @return         `ESP_OK` on success
 */
esp_err_t ip5306_init_desc(i2c_dev_t *dev, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio);

/**
 * @brief Setup IP5306 control registers
 *
 * @param dev Device descriptor
 * @param params Setup parameters
 * @return    `ESP_OK` on success
 */
esp_err_t ip5306_setup(i2c_dev_t *dev, ip5306_setup_params_t params);

/**
 * @brief Free device descriptor.
 *
 * @param dev Device descriptor
 * @return    `ESP_OK` on success
 */
esp_err_t ip5306_free_desc(i2c_dev_t *dev);

/**
 * @brief Check whether charger is connected
 *
 * @param dev Device descriptor
 * @param[out] connected true if charger is connected
 * @return `ESP_OK` on success
 */
esp_err_t ip5306_is_charger_connected(i2c_dev_t *dev, bool *connected);

/**
 * @brief Check whether the battery is charged
 *
 * @param dev Device descriptor
 * @param[out] full true if battery is charged
 * @return `ESP_OK` on success
 */
esp_err_t ip5306_is_charge_full(i2c_dev_t *dev, bool *full);

/**
 * @brief Get the battery level
 *
 * @param dev Device descriptor
 * @param[out] battery_level Value of battery charge level
 * @return `ESP_OK` on success
 */
esp_err_t ip5306_get_battery_level(i2c_dev_t *dev, uint8_t *battery_level);


#ifdef __cplusplus
}
#endif

/**@}*/

#endif // __IP5306__H__
