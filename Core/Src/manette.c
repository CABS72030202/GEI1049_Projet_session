/*
 * manette.c
 *
 *  Created on: Nov 27, 2024
 *      Author: Yohan Lefebvre
 */

#include "manette.h"

#define NUNCHUK_ADDRESS (0x52<<1)
I2C_HandleTypeDef hi2c1;

	uint8_t initcomm1[2] = {0xF0, 0x55};
	uint8_t initcomm2[2] = {0xFB, 0x00};

void Initialisation_manette(){





	HAL_I2C_Master_Transmit(&hi2c1, NUNCHUK_ADDRESS, initcomm1, 2, HAL_MAX_DELAY);
	HAL_Delay(10);
	HAL_I2C_Master_Transmit(&hi2c1, NUNCHUK_ADDRESS, initcomm2, 2, HAL_MAX_DELAY);

}


