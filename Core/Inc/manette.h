/*
 * manette.h
 *
 *  Created on: Oct 30, 2024
 *      Author: sebas
 */

#ifndef INC_MANETTE_H_
#define INC_MANETTE_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "gestion_moteurs.h"

#define NUNCHUK_ADDRESS (0x52<<1)
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim3;

extern uint8_t initcomm1[2];
extern uint8_t initcomm2[2];

extern uint8_t data[6];
extern int formatted_data[7];
extern int Xdata;
extern int Ydata;
extern int Xpulse;
extern int Ypulse;
extern int Cbutton;
extern int Zbutton;

extern int modeVitesse;
extern float pulseCoeff;
extern int maxInverse;
extern int sendPulseMotor;

extern int toggle;

void Format_Data();
void Initialisation_manette();
void Controller();
void ToggleModeVit();

#endif /* INC_MANETTE_H_ */
