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

void General_Run(void) {
    OLED_Init(&hi2c1);
    MPU6050_Init(&hi2c1);

    float ax, ay, az, gx, gy, gz;

    char line[32];
    char fstr[16];



    OLED_Clear();   // clear only once at start
    OLED_Rectangle(74, 0, 1, 12);
    OLED_Rectangle(0, 13, 128, 1);
    OLED_Print(0, 0, "ACC:");
    OLED_Print(0, 16, "GPS:");
    OLED_Print(0, 27, "AnV:");
    OLED_Update();

    while (1) {
    	// Read Sensors
        MPU6050_ReadAccelGyro(&ax, &ay, &az, &gx, &gy, &gz);
        // calculate magnitude here
        float accMag = sqrtf(ax*ax + ay*ay + az*az);
        float gyrMag = sqrtf(gx*gx + gy*gy + gz*gz);

        // Update Status line
        OLED_ClearArea(78, 0, 50, 10); // erase old text

        // --- Update magnitude ---
        float_to_str(accMag, fstr, sizeof(fstr), 2);
        snprintf(line, sizeof(line), "%sg", fstr);
        OLED_ClearArea(30, 0, 42, 10);
        OLED_Print(30, 0, line);

        float_to_str(gyrMag, fstr, sizeof(fstr), 2);
		snprintf(line, sizeof(line), "%s dec/s", fstr);
		OLED_ClearArea(30, 27, 98, 10);
		OLED_Print(30, 27, line);

        // --- Update GPS ---
		OLED_ClearArea(30, 16, 49, 10);

        // ------------------ ANGULAR VELOCITY ------------------
        float gx_dps = gx / 131.0f;
        float gy_dps = gy / 131.0f;
        float gz_dps = gz / 131.0f;

        char gx_str[12], gy_str[12], gz_str[12];

        float_to_str(gx_dps, gx_str, sizeof(gx_str), 1);
        float_to_str(gy_dps, gy_str, sizeof(gy_str), 1);
        float_to_str(gz_dps, gz_str, sizeof(gz_str), 1);

        // Clear full area where GX, GY, GZ will print
        OLED_ClearArea(30, 38, 98, 10);

        // Print in required user positions
        OLED_Print(0, 38, gx_str);   // GX
        OLED_Print(42, 38, gy_str);   // GY
        OLED_Print(84, 38, gz_str);   // GZ */

        OLED_Update();   // refresh once per loop

    }
}
