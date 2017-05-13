
#include "power_button.h"

void PowerButton_Init(ButtonMode_TypeDef ButtonMode)
{ 
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Enable the BUTTON Clock */
	__HAL_RCC_GPIOC_CLK_ENABLE();
  
  if(ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);
  }
  
  if(ButtonMode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; 
    HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);
    
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(EXTI15_10_IRQn), 0x0F, 0x00);
    HAL_NVIC_EnableIRQ((IRQn_Type)(EXTI15_10_IRQn));
  }
}

void PowerButton_DeInit()
{
    GPIO_InitTypeDef gpio_init_structure;

    gpio_init_structure.Pin = BUTTON_PIN;
    HAL_NVIC_DisableIRQ((IRQn_Type)(EXTI15_10_IRQn));
    HAL_GPIO_DeInit(BUTTON_PORT, gpio_init_structure.Pin);
}