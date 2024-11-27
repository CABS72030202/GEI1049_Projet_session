/*
 * manette.c
 *
 *  Created on: Nov 27, 2024
 *      Author: Yohan Lefebvre
 */

#include "manette.h"

#define NUNCHUK_ADDRESS (0x52<<1)
extern I2C_HandleTypeDef hi2c1;

uint8_t initcomm1[2] = {0xF0, 0x55};
uint8_t initcomm2[2] = {0xFB, 0x00};

uint8_t data[6];
int formatted_data[7];


void Format_Data() {
	formatted_data[0] = data[0];
	formatted_data[1] = data[1];
	formatted_data[2] = (data[2] << 2) | ((data[5] >> 0) & 0x03);
	formatted_data[3] = (data[3] << 2) | ((data[5] >> 2) & 0x03);
	formatted_data[4] = (data[4] << 2) | ((data[5] >> 4) & 0x03);
	formatted_data[5] = (data[5] & 0x02);
	if(formatted_data[5] == 2)
		formatted_data[5] = 1;
	formatted_data[6] = (data[5] & 0x01);
}


void Initialisation_manette(){

	HAL_I2C_Master_Transmit(&hi2c1, NUNCHUK_ADDRESS, initcomm1, 2, HAL_MAX_DELAY);
	HAL_Delay(10);
	HAL_I2C_Master_Transmit(&hi2c1, NUNCHUK_ADDRESS, initcomm2, 2, HAL_MAX_DELAY);

}


void MettreDansWhile(){

	HAL_I2C_Master_Transmit(&hi2c1, NUNCHUK_ADDRESS, 0x00, 1, HAL_MAX_DELAY);
	HAL_Delay(10);
	HAL_I2C_Master_Receive(&hi2c1, NUNCHUK_ADDRESS, data, 6, HAL_MAX_DELAY);

	Format_Data();
}


