#include "i2c_bus.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"

#define I2C_PORT    I2C_NUM_0
#define I2C_TIMEOUT pdMS_TO_TICKS(50)

esp_err_t i2c_bus_init(int sda, int scl, uint32_t hz)
{
    i2c_config_t cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,   /* ideal: pull-ups externos de 4.7k */
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = hz,
    };
    esp_err_t err = i2c_param_config(I2C_PORT, &cfg);
    if (err != ESP_OK) return err;
    return i2c_driver_install(I2C_PORT, cfg.mode, 0, 0, 0);
}

esp_err_t i2c_read_regs(uint8_t addr, uint8_t reg, uint8_t *buf, size_t len)
{
    return i2c_master_write_read_device(I2C_PORT, addr, &reg, 1, buf, len, I2C_TIMEOUT);
}

esp_err_t i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t val)
{
    uint8_t d[2] = { reg, val };
    return i2c_master_write_to_device(I2C_PORT, addr, d, 2, I2C_TIMEOUT);
}
