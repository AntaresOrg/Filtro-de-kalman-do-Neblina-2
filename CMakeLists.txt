idf_component_register(SRCS "main.c" "ekf.c" "mpu6050.c" "bmp280.c" "i2c_bus.c"
                       INCLUDE_DIRS "."
                       REQUIRES driver esp_timer)
