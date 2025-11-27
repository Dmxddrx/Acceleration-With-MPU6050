#ifndef GENERAL_H
#define GENERAL_H

#include "stm32f1xx_hal.h"

void General_Run(void);
void delay_ms(uint32_t ms);
void Error_Handler_Notify(const char *msg);

#endif
