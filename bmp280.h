#pragma once
#include <stdint.h>
#include "esp_err.h"

#define BMP280_ADDR_LOW  0x76   /* SDO = GND */
#define BMP280_ADDR_HIGH 0x77   /* SDO = VCC */

typedef struct {
    uint8_t  addr;
    uint16_t T1; int16_t T2, T3;
    uint16_t P1; int16_t P2, P3, P4, P5, P6, P7, P8, P9;
    int32_t  t_fine;
} bmp280_t;

esp_err_t bmp280_init(bmp280_t *b, uint8_t addr);
esp_err_t bmp280_read(bmp280_t *b, float *pressure_pa, float *temp_c);
float     bmp280_altitude(float pressure_pa, float p0_pa);   /* altitude relativa a p0 */
