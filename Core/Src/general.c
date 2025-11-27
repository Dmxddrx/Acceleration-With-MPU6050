#include "general.h"
#include "oled.h"
#include "mpu6050.h"
#include "ssd1306.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

/* Delay helper */
void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

/* Simple error notifier */
void Error_Handler_Notify(const char *msg)
{
    (void)msg;
    Error_Handler();
}

/* MAIN APPLICATION LOGIC */
void General_Run(void)
{
    OLED_Init();

    if (!MPU6050_Init())
    {
        OLED_Clear();
        OLED_Print("MPU init failed");
        Error_Handler_Notify("MPU6050 init fail");
    }
    else
    {
        OLED_Clear();
        OLED_Print("MPU OK");
        HAL_Delay(500);
    }

    mpu6050_raw_t raw;
    mpu6050_f_t f;

    while (1)
    {
        if (MPU6050_ReadRaw(&raw))
        {
            MPU6050_RawToFloat(&raw, &f);

            OLED_Clear();

            ssd1306_SetCursor(0, 0);
            char line1[32];
            snprintf(line1, sizeof(line1),
                     "AX: %.2fg AY: %.2fg",
                     f.ax_g, f.ay_g);
            ssd1306_WriteString(line1, Font_7x10, White);

            ssd1306_SetCursor(0, 2);
            char line2[32];
            snprintf(line2, sizeof(line2),
                     "GX: %.1fdps GY: %.1fdps",
                     f.gx_dps, f.gy_dps);
            ssd1306_WriteString(line2, Font_7x10, White);

            ssd1306_UpdateScreen();
        }
        else
        {
            OLED_Clear();
            OLED_Print("MPU read err");
        }

        HAL_Delay(200);
    }
}
