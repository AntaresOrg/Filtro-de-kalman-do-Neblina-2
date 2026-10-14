#pragma once
#include <stddef.h>
#include <stdint.h>
#include "esp_err.h"

esp_err_t i2c_bus_init(int sda, int scl, uint32_t hz);
esp_err_t i2c_read_regs(uint8_t addr, uint8_t reg, uint8_t *buf, size_t len);
esp_err_t i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t val);
