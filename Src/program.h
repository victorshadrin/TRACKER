

#ifndef __PROGRAM_H
#define __PROGRAM_H

#include "main.h"
#include "power_button.h"

#define POWER_PORT GPIOA
#define POWER_PIN  GPIO_PIN_8

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

#define GPS_UART &huart1
#define CMD_UART &huart2
#define MEMORY_I2C &hi2c1

typedef enum {
	STATE_SLEEPING,
	STATE_RUNNING,
	STATE_AWAKENING,
	STATE_AWAKENED
} ProgramState;

void program_setup(void);
void program_loop(void);
void program_sleep(void);

void power_on(void);
void power_off(void);




#endif
