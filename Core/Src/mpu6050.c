#include "mpu6050.h"
#include "general.h"

static uint8_t write_reg(uint8_t reg, uint8_t val) {
    return HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
}

static uint8_t read_regs(uint8_t reg, uint8_t *buf, uint16_t len) {
    return HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 100);
}

bool MPU6050_Init(void) {
    HAL_StatusTypeDef ret;
    uint8_t tmp;

    // Wake up device (clear sleep bit)
    tmp = 0x00;
    ret = HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_REG_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    if (ret != HAL_OK) return false;

    // Sample rate divider = 0 (sample rate = gyro output rate)
    tmp = 0x00;
    ret = HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_REG_SMPLRT_DIV, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    if (ret != HAL_OK) return false;

    // DLPF = 0 (no low-pass) or 3 (recommended)
    tmp = 0x03;
    ret = HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_REG_CONFIG, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    if (ret != HAL_OK) return false;

    // Gyro full scale ±250 °/s
    tmp = 0x00;
    ret = HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_REG_GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    if (ret != HAL_OK) return false;

    // Accel full scale ±2g
    tmp = 0x00;
    ret = HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_REG_ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 100);
    if (ret != HAL_OK) return false;

    return true;
}

bool MPU6050_ReadRaw(mpu6050_raw_t *raw) {
    uint8_t buf[14];
    if (HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, MPU6050_REG_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, buf, 14, 100) != HAL_OK) {
        return false;
    }
    raw->ax = (int16_t)(buf[0] << 8 | buf[1]);
    raw->ay = (int16_t)(buf[2] << 8 | buf[3]);
    raw->az = (int16_t)(buf[4] << 8 | buf[5]);
    raw->temp = (int16_t)(buf[6] << 8 | buf[7]);
    raw->gx = (int16_t)(buf[8] << 8 | buf[9]);
    raw->gy = (int16_t)(buf[10] << 8 | buf[11]);
    raw->gz = (int16_t)(buf[12] << 8 | buf[13]);
    return true;
}

void MPU6050_RawToFloat(const mpu6050_raw_t *raw, mpu6050_f_t *out) {
    // For default ±2g and ±250 dps
    const float accel_scale = 16384.0f; // LSB/g
    const float gyro_scale = 131.0f;    // LSB/(°/s)

    out->ax_g = raw->ax / accel_scale;
    out->ay_g = raw->ay / accel_scale;
    out->az_g = raw->az / accel_scale;

    out->gx_dps = raw->gx / gyro_scale;
    out->gy_dps = raw->gy / gyro_scale;
    out->gz_dps = raw->gz / gyro_scale;

    // Temp conversion per datasheet: Temp_in_C = (raw_temp / 340) + 36.53
    out->temp_c = (raw->temp / 340.0f) + 36.53f;
}
