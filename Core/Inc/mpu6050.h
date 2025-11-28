#ifndef MPU6050_H
#define MPU6050_H

#include "main.h"
#include <stdbool.h>

void MPU6050_Init(I2C_HandleTypeDef *hi2c);
void MPU6050_ReadAccelGyro(float *ax, float *ay, float *az, float *gx, float *gy, float *gz);

#endif
