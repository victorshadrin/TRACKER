
#include "program.h"
#include "sleepmode.h"
#include "main.h"
#include "eeprom.h"
#include "string.h"
#include "tinygps.h"
#include "gpsmodule.h"


#define INIT_ALL() MAIN_USART2_UART_Init();MAIN_I2C1_Init();MAIN_USART1_UART_Init();  
#define DEINIT_ALL() HAL_UART_DeInit(CMD_UART);  HAL_UART_DeInit(GPS_UART);  HAL_I2C_DeInit(MEMORY_I2C);;  
#define LED_SIGNAL() int counter = 0; \
                    while (counter++ < 4) { \
                      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); \
                      HAL_Delay(100); \
                      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET); \
                      HAL_Delay(100); \
                    }

__IO ProgramState programState = STATE_SLEEPING;

bool is_trace;
bool cmd_ready;                    
                    
uint8_t command_buffer[10];
uint8_t command_buffer_counter;
uint8_t command_char;
uint8_t nmea_char;


void program_setup() {  
  __HAL_RCC_PWR_CLK_ENABLE();  
}

void process_command(Command command) {
  switch(command) {
    case COMMAND_START: 
      if (programState == STATE_WAITING) {
          readIndex();
          writeCurrentData(RECORD_BEGIN);
          programState = STATE_RUNNING;
      }
      break;
    case COMMAND_STOP: 
      if (programState == STATE_RUNNING) {
          programState = STATE_WAITING;
          writeCurrentData(RECORD_END);
          writeIndex();
      }
      break;
    case COMMAND_CLEAR:
      clearData();
      break;
    case COMMAND_PRINT:
      if (programState == STATE_WAITING || programState == STATE_SEEKING) {
        printout();
      }
      break;
    default:
      break;
  }
}


void program_loop() {
  switch (programState) {
    case STATE_SLEEPING: 
      program_sleep();
      break;
    case STATE_AWAKENING: 
      MAIN_GPIO_Init();
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
      if (PowerButton_released()) {
        programState = STATE_SLEEPING;
      } else if (PowerButton_pressed(BUTTON_DELAY)) {
        PowerButton_reset();  
        programState = STATE_AWAKENED;
      }
      break;    
    case STATE_AWAKENED: 
      INIT_ALL();
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
      power_on();
      printf("Entered run mode\n");
      programState = STATE_WARMING;
      break;
    case STATE_WARMING: 
    case STATE_RUNNING: 
    case STATE_SEEKING: 
    case STATE_WAITING:
      if (PowerButton_pressed(BUTTON_DELAY)) {
        programState = STATE_SLEEPING;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
      } else {
          if (cmd_ready) {
            cmd_ready = false;
            process_command(parse_command());
          }
      }
  }
}

void power_on(void) {
  HAL_GPIO_WritePin(POWER_PORT, POWER_PIN, GPIO_PIN_SET);
}

void power_off(void) {
  HAL_GPIO_WritePin(POWER_PORT, POWER_PIN, GPIO_PIN_RESET);
}


void program_sleep() {  
  LED_SIGNAL();
  printf("Entering sleep mode\n");
  power_off();
  DEINIT_ALL();
  PowerButton_DeInit();  
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {  
  if (huart->Instance == huart2.Instance) {
    if (command_char == '\n') {    
        command_buffer[command_buffer_counter] = 0;
        command_buffer_counter = 0;
        //printf("HAL_UART_RxCpltCallback: %s\n", command_buffer);
        cmd_ready = true;
    } else {
      command_buffer[command_buffer_counter++] = command_char;    
    }
    REQUEST_COMMAND_CHAR();
  } else if (huart->Instance == huart1.Instance) {
    if (tinygps_encode(nmea_char)) {
        update_current_data();
    }
    REQUEST_NMEA_CHAR();
  }  
}

Command parse_command() {
  if (strncmp("start", (char*)command_buffer, 5)==0) {
      return COMMAND_START;
  } else if (strncmp("stop", (char*)command_buffer, 4)==0) {
       return COMMAND_STOP;
  } else if (strncmp("clear", (char*)command_buffer, 5)==0) {
       return COMMAND_CLEAR;
  } else if (strncmp("print", (char*)command_buffer, 5)==0) {
       return COMMAND_PRINT;
  } else if (strncmp("trace", (char*)command_buffer, 5)==0) {
       return COMMAND_TRACE;
  }        
  return COMMAND_NONE;  
}

void update_current_data() {
   int year;
    tinygps_crack_datetime(&year, &trackerData.month, &trackerData.day,
    &trackerData.hour, &trackerData.minute, &trackerData.second, 
    &trackerData.hundredths, &trackerData.age);
   trackerData.year = year - 2000;
   tinygps_f_get_position(&trackerData.latitude, &trackerData.longitude, &trackerData.fix_age);
   trackerData.ele = tinygps_altitude();
   missedWritings++;
   if (trackerData.latitude != GPS_INVALID_F_ANGLE) {
       programState = STATE_WAITING;
       printf("Ready\n");
   } else if (programState == STATE_WARMING) {
       programState = STATE_SEEKING;
       printf("Start seeking\n");
   }   
} 


