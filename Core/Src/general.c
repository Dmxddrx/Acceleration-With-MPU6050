#include "oled.h"
#include "mpu6050.h"
#include <stdio.h>
#include <math.h>

extern I2C_HandleTypeDef hi2c1;

// Helper: convert float -> string with 'decimals' digits, no %f used
static void float_to_str(float v, char *out, size_t out_len, int decimals)
{
    if (out_len == 0) return;
    if (decimals < 0) decimals = 0;
    if (decimals > 5) decimals = 5; // limit

    int sign = 0;
    if (v < 0.0f) { sign = 1; v = -v; }

    int mult = 1;
    for (int i = 0; i < decimals; ++i) mult *= 10;

    int whole = (int)v;
    int frac = (int)((v - (float)whole) * (float)mult + 0.5f); // round

    if (frac >= mult) { whole += 1; frac -= mult; }

    if (sign) {
        if (decimals == 0)
            snprintf(out, out_len, "-%d", whole);
        else
            snprintf(out, out_len, "-%d.%0*d", whole, decimals, frac);
    } else {
        if (decimals == 0)
            snprintf(out, out_len, "%d", whole);
        else
            snprintf(out, out_len, "%d.%0*d", whole, decimals, frac);
    }
}

// General run for logic
void General_Run(void) {
    OLED_Init(&hi2c1);
    MPU6050_Init(&hi2c1);

    float ax, ay, az, gx, gy, gz;

    char line[32];
    char fstr[16];

    OLED_Clear();   // clear only once at start
    OLED_Print(39, 0, "MPU6050");
    OLED_Rectangle(0, 12, 128, 1);
    OLED_Print(0, 16, "ACC:");
    OLED_Rectangle(0, 38, 128, 1);
    OLED_Print(0, 42, "GYR:");
    OLED_Update();

    while (1) {
    	// Read Sensors
        MPU6050_ReadAccelGyro(&ax, &ay, &az, &gx, &gy, &gz);
        // calculate magnitude here
        float accMag = sqrtf(ax*ax + ay*ay + az*az);
        float gyrMag = sqrtf(gx*gx + gy*gy + gz*gz);


        // ------------------ ACCELERATION ------------------
        // Acceleration magnitude
        float_to_str(accMag, fstr, sizeof(fstr), 2);
        snprintf(line, sizeof(line), "%s g", fstr);
        OLED_ClearArea(30, 16, 98, 10);
        OLED_Print(30, 16, line);

        //Acceleration raw values
        float ax_dps = ax / 16384.0f;
        float ay_dps = ay / 16384.0f;
        float az_dps = az / 16384.0f;

        char ax_str[12], ay_str[12], az_str[12];

        float_to_str(ax_dps, ax_str, sizeof(ax_str), 1);
        float_to_str(ay_dps, ay_str, sizeof(ay_str), 1);
        float_to_str(az_dps, az_str, sizeof(az_str), 1);

        OLED_ClearArea(0, 27, 128, 10);

        // Print accRaw
        OLED_Print(0, 27, ax_str);   // AX
        OLED_Print(42, 27, ay_str);   // AY
        OLED_Print(84, 27, az_str);   // AZ


        // ------------------ ANGULAR VELOCITY ------------------
        //Gyroscope magnitude
        float_to_str(gyrMag, fstr, sizeof(fstr), 2);
		snprintf(line, sizeof(line), "%s dec/s", fstr);
		OLED_ClearArea(30, 42, 128, 10);
		OLED_Print(30, 42, line);

		//Gyroscope raw values
        float gx_dps = gx / 131.0f;
        float gy_dps = gy / 131.0f;
        float gz_dps = gz / 131.0f;

        char gx_str[12], gy_str[12], gz_str[12];

        float_to_str(gx_dps, gx_str, sizeof(gx_str), 1);
        float_to_str(gy_dps, gy_str, sizeof(gy_str), 1);
        float_to_str(gz_dps, gz_str, sizeof(gz_str), 1);

        OLED_ClearArea(0, 53, 128, 10);

        // Print gyroRaw
        OLED_Print(0, 53, gx_str);   // GX
        OLED_Print(42, 53, gy_str);   // GY
        OLED_Print(84, 53, gz_str);   // GZ

        OLED_Update();   // refresh once per loop

    }
}
