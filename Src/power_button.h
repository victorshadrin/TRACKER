
#ifndef __POWER_BUTTON_H
#define __POWER_BUTTON_H

#include "main.h"
#include "stm32f3xx_hal.h"

#define BUTTON_PORT GPIOC
#define BUTTON_PIN  GPIO_PIN_13

#define BUTTON_DELAY 2000

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

void PowerButton_Init(ButtonMode_TypeDef ButtonMode);
void PowerButton_DeInit(void);
int PowerButton_check(void);

#endif
