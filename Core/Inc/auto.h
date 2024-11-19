/*
 * auto.h
 *
 *  Created on: Oct 30, 2024
 *      Author: sebas
 */

#ifndef INC_AUTO_H_
#define INC_AUTO_H_

// Includes
#include <stdint.h>
#include <math.h>
#include "stm32f4xx_hal.h"
#include "gestion_moteurs.h"

// Constants
#define MANUAL_ID 0
#define CIRCLE_ID 1
#define SQUARE_ID 2
#define LINE_ID	3

#define TRACK_WIDTH 100     	// Distance between the two tracks in mm
#define SPEED_FACTOR 0.1     	// Speed factor

// Variables
extern uint8_t curr_mode;		// Current mode

// Prototypes
int Get_Mode(int, int);			// Converts the binary input into active mode
void Auto_Angle(int, TIM_HandleTypeDef);
void Auto_Circle();
void Auto_Square();
void Auto_Line();


#endif /* INC_AUTO_H_ */
