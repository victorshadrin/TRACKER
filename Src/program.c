
#include "program.h"
#include "sleepmode.h"
#include "main.h"



__IO ProgramState programState = PROGRAM_RUNNING;


void program_setup() {	
	__HAL_RCC_PWR_CLK_ENABLE();
	PowerButton_Init(BUTTON_MODE_GPIO);	
	printf("Push button to sleep\n");
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
}

void program_loop() {
	if (programState == PROGRAM_AWAKENED) {
		MAIN_GPIO_Init();
		PowerButton_Init(BUTTON_MODE_GPIO);	
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
		MAIN_USART2_UART_Init();
		MAIN_I2C1_Init();
		MAIN_USART1_UART_Init();	
		printf("Entered run mode\n");
		programState = PROGRAM_RUNNING;
	} else {		
		if (PowerButton_check()) {
			program_sleep();
		}	
	}
}

void program_sleep() {	
	printf("Entering sleep mode\n");	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_UART_DeInit(CMD_UART);
	HAL_UART_DeInit(GPS_UART);
	HAL_I2C_DeInit(MEMORY_I2C);
	SleepMode_enter();	
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {		
	programState = PROGRAM_AWAKENED;		
}

void EXTI15_10_IRQHandler(void) {	
  HAL_GPIO_EXTI_IRQHandler(BUTTON_PIN);
}




