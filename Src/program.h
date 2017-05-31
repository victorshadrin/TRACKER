

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

#define REQUEST_COMMAND_CHAR() HAL_UART_Receive_IT(CMD_UART, &command_char, 1)
#define REQUEST_NMEA_CHAR() HAL_UART_Receive_IT(GPS_UART, &nmea_char, 1)


typedef enum  {
    COMMAND_NONE,
    COMMAND_START,
    COMMAND_STOP,
    COMMAND_PRINT,
    COMMAND_CLEAR,
    COMMAND_TRACE
} Command;


typedef enum {
	STATE_SLEEPING,
	STATE_AWAKENING,
	STATE_AWAKENED,
	STATE_WARMING,
    STATE_SEEKING,
	STATE_WAITING,
	STATE_RUNNING
} ProgramState;


void program_setup(void);
void program_loop(void);
void program_sleep(void);

void power_on(void);
void power_off(void);

Command commands_read(UART_HandleTypeDef* uart);
Command parse_command(void);
void update_current_data(void);

#endif
