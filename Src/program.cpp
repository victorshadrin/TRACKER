
#include "program.h"
#include "sleepmode.h"
#include "main.h"
#include "eeprom.h"
#include "tinygps.h"


#define INIT_ALL() 	MAIN_GPIO_Init(); MAIN_USART2_UART_Init();MAIN_I2C1_Init();MAIN_USART1_UART_Init();	
#define DEINIT_ALL() HAL_UART_DeInit(CMD_UART);	HAL_UART_DeInit(GPS_UART);	HAL_I2C_DeInit(MEMORY_I2C);;	

__IO ProgramState programState = STATE_SLEEPING;


void program_setup() {	
	__HAL_RCC_PWR_CLK_ENABLE();
	int counter = 0;
	while (counter++ < 4) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(100);
	}
}

void program_loop() {
	if (programState == STATE_SLEEPING) {
		program_sleep();
	} else if (programState == STATE_AWAKENING) {
		if (PowerButton_released()) {
			programState = STATE_SLEEPING;
		} else if (PowerButton_pressed(BUTTON_DELAY)) {
			PowerButton_reset();	
			programState = STATE_AWAKENED;
		}		
	} else if (programState == STATE_AWAKENED) {
		INIT_ALL();
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
		power_on();
		printf("Entered run mode\n");
		programState = STATE_RUNNING;
	} else if (PowerButton_pressed(BUTTON_DELAY)) {
		programState = STATE_SLEEPING;
	}	
}

void power_on(void) {
	HAL_GPIO_WritePin(POWER_PORT, POWER_PIN, GPIO_PIN_SET);
}

void power_off(void) {
	HAL_GPIO_WritePin(POWER_PORT, POWER_PIN, GPIO_PIN_RESET);
}


void program_sleep() {	
	printf("Entering sleep mode\n");	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	power_off();
	DEINIT_ALL();	
	SleepMode_enter();	
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	PowerButton_DeInit();
	PowerButton_Init(BUTTON_MODE_GPIO);	
	programState = STATE_AWAKENING;
}

void EXTI15_10_IRQHandler(void) {	
  HAL_GPIO_EXTI_IRQHandler(BUTTON_PIN);
}




