#include "mpu6050.h"
#include "i2c_bus.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define REG_SMPLRT_DIV   0x19
#define REG_CONFIG       0x1A
#define REG_GYRO_CONFIG  0x1B
#define REG_ACCEL_CONFIG 0x1C
#define REG_ACCEL_XOUT_H 0x3B
#define REG_PWR_MGMT_1   0x6B
#define REG_WHO_AM_I     0x75

#define GYRO_LSB_PER_DPS 65.5f      /* +-500 dps */
#define ACC_LSB_PER_G    8192.0f    /* +-4 g     */
#define DEG2RAD          0.017453292519943295f
#define G_STD            9.80665f

static const char *TAG = "mpu6050";

esp_err_t mpu6050_init(void)
{
    uint8_t id = 0;
    esp_err_t err = i2c_read_regs(MPU6050_ADDR, REG_WHO_AM_I, &id, 1);
    if (err != ESP_OK) return err;
    if (id != 0x68) ESP_LOGW(TAG, "WHO_AM_I=0x%02X (esperado 0x68; clones usam outros valores)", id);

    ESP_ERROR_CHECK(i2c_write_reg(MPU6050_ADDR, REG_PWR_MGMT_1, 0x80));   /* reset */
    vTaskDelay(pdMS_TO_TICKS(100));
    ESP_ERROR_CHECK(i2c_write_reg(MPU6050_ADDR, REG_PWR_MGMT_1, 0x01));   /* clock = PLL gyro X */
    vTaskDelay(pdMS_TO_TICKS(10));
    ESP_ERROR_CHECK(i2c_write_reg(MPU6050_ADDR, REG_CONFIG, 0x03));       /* DLPF ~44 Hz, base 1 kHz */
    ESP_ERROR_CHECK(i2c_write_reg(MPU6050_ADDR, REG_SMPLRT_DIV, 9));      /* 100 Hz */
    ESP_ERROR_CHECK(i2c_write_reg(MPU6050_ADDR, REG_GYRO_CONFIG, 0x08));  /* +-500 dps */
    ESP_ERROR_CHECK(i2c_write_reg(MPU6050_ADDR, REG_ACCEL_CONFIG, 0x08)); /* +-4 g */
    return ESP_OK;
}

esp_err_t mpu6050_read(float gyro[3], float acc[3])
{
    uint8_t b[14];
    esp_err_t err = i2c_read_regs(MPU6050_ADDR, REG_ACCEL_XOUT_H, b, sizeof(b));
    if (err != ESP_OK) return err;

    int16_t ax = (int16_t)((b[0] << 8) | b[1]);
    int16_t ay = (int16_t)((b[2] << 8) | b[3]);
    int16_t az = (int16_t)((b[4] << 8) | b[5]);
    int16_t gx = (int16_t)((b[8] << 8) | b[9]);
    int16_t gy = (int16_t)((b[10] << 8) | b[11]);
    int16_t gz = (int16_t)((b[12] << 8) | b[13]);

    acc[0] = ax / ACC_LSB_PER_G * G_STD;
    acc[1] = ay / ACC_LSB_PER_G * G_STD;
    acc[2] = az / ACC_LSB_PER_G * G_STD;
    gyro[0] = gx / GYRO_LSB_PER_DPS * DEG2RAD;
    gyro[1] = gy / GYRO_LSB_PER_DPS * DEG2RAD;
    gyro[2] = gz / GYRO_LSB_PER_DPS * DEG2RAD;
    return ESP_OK;
}
