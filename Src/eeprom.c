#include "eeprom.h"

void eeprom_write(I2C_HandleTypeDef* hi2c, uint16_t address, void *data, uint16_t size){
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hi2c, EEPROM_I2C_ADDRESS, address, I2C_MEMADD_SIZE_16BIT, (uint8_t*)data, size, 10000);
	if (status != HAL_OK) {
		printf("Error writing to eeprom: %d\n", status);
	}
}

void eeprom_read(I2C_HandleTypeDef* hi2c, uint16_t address, void *data, uint16_t size) {
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, EEPROM_I2C_ADDRESS, address, I2C_MEMADD_SIZE_16BIT, (uint8_t*)data, size, 10000);
	if (status != HAL_OK) {
		printf("Error reading from eeprom: %d\n", status);
	}
}

void eeprom_write_32(I2C_HandleTypeDef* hi2c, uint16_t address, uint32_t data) {
	eeprom_write(hi2c, address, &data, 4);
}

void eeprom_write_8(I2C_HandleTypeDef* hi2c, uint16_t address, uint8_t data) {
	eeprom_write(hi2c, address, &data, 1);
}
