
#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_i2c.h"

#define EEPROM_I2C_ADDRESS 0xa0

void eeprom_write(I2C_HandleTypeDef* hi2c, uint16_t address, void *data, uint16_t size);
void eeprom_write_32(I2C_HandleTypeDef* hi2c, uint16_t address, uint32_t data);
void eeprom_write_8(I2C_HandleTypeDef* hi2c, uint16_t address, uint8_t data);
void eeprom_read(I2C_HandleTypeDef* hi2c, uint16_t address, void *data, uint16_t size);

#endif
