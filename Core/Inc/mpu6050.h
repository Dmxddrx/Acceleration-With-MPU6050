#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

//#define MPU6050_ADDR       (0x68 << 1) // HAL expects 8-bit address
// Registers
#define MPU6050_REG_PWR_MGMT_1  0x6B
#define MPU6050_REG_SMPLRT_DIV  0x19
#define MPU6050_REG_CONFIG      0x1A
#define MPU6050_REG_GYRO_CONFIG 0x1B
#define MPU6050_REG_ACCEL_CONFIG 0x1C
#define MPU6050_REG_ACCEL_XOUT_H 0x3B

typedef struct {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    int16_t temp;
} mpu6050_raw_t;

typedef struct {
    float ax_g, ay_g, az_g;   // in g
    float gx_dps, gy_dps, gz_dps; // in °/s
    float temp_c;
} mpu6050_f_t;

bool MPU6050_Init(void);
bool MPU6050_ReadRaw(mpu6050_raw_t *raw);
void MPU6050_RawToFloat(const mpu6050_raw_t *raw, mpu6050_f_t *out);

#endif // MPU6050_H
