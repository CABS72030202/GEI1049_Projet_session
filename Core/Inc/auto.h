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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "gestion_moteurs.h"

// Constants
#define MANUAL_ID 0
#define CIRCLE_ID 1
#define BACK_FORTH_ID 2
#define SQUARE_ID 3

#define TRACK_RESOLUTION 2		// Distance per encoder step in mm
#define TRACK_WIDTH 272     	// Distance between the two tracks in mm
#define UPDATE_INTERVAL 50		// Time interval for speed updates in ms
#define SPEED_FACTOR 0.03     	// Speed factor

#define BASE_SPEED 450
#define CIRCLE_CIRCUMFERENCE 6280
#define DISTANCE 100

// Variables
extern uint8_t curr_mode;		// Current mode

// Prototypes
int Get_Mode(int, int);			// Converts the binary input into active mode
char* Get_Mode_String();		// Convert the current mode to a string
void Auto_Angle(int, TIM_HandleTypeDef*);
void Auto_Line(int, int, int, TIM_HandleTypeDef*);
void Auto_Circle(TIM_HandleTypeDef*);
void Auto_Back_Forth(TIM_HandleTypeDef*);
void Auto_Square(TIM_HandleTypeDef*);


#endif /* INC_AUTO_H_ */
