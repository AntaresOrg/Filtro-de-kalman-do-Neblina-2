#pragma once
#include "esp_err.h"

#define MPU6050_ADDR 0x68   /* AD0 = GND; 0x69 se AD0 = VCC */

esp_err_t mpu6050_init(void);
/* gyro em rad/s, acc em m/s^2 (forca especifica) */
esp_err_t mpu6050_read(float gyro[3], float acc[3]);
