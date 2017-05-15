
#ifndef __UART_PRINTF_H
#define __UART_PRINTF_H

#include "main.h"

extern UART_HandleTypeDef huart2;

namespace std {

int fputc(int ch, FILE* f) {
	/* Send over USART */
	 HAL_UART_Transmit(&huart2, (uint8_t*) &ch, 1, 1);	
	/* Return character */
	return ch;
}

}


#endif
