#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "general.h"

void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define SIM900_PWRKEY_Pin GPIO_PIN_0
#define SIM900_PWRKEY_GPIO_Port GPIOB
#define SIM900_STATUS_Pin GPIO_PIN_1
#define SIM900_STATUS_GPIO_Port GPIOB
#define GPS_PPS_Pin GPIO_PIN_5
#define GPS_PPS_GPIO_Port GPIOB
#define MPU_INT_Pin GPIO_PIN_8
#define MPU_INT_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
