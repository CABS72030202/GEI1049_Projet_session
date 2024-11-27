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
#define PI 3.14159
#define MANUAL_MODE 0
#define CIRCLE_MODE 1
#define BACK_FORTH_MODE 2
#define SQUARE_MODE 3

#define TRACK_RESOLUTION 6.5		// Distance per encoder step in mm
#define TRACK_WIDTH 272     		// Distance between the two tracks in mm
#define UPDATE_INTERVAL 50			// Time interval for speed updates in ms (time per step)
#define CLOCKWISE_FACTOR 53.4522 	// Turning factor
#define COUNTER_CLW_FACTOR 50.0000 	// Turning factor

#define BASE_SPEED 450
#define DISTANCE 1000

// Variables
extern uint8_t pause;				// Detect blue button press
extern uint8_t dip_state;			// Current dipswitch state
extern uint8_t curr_mode;			// Current mode
extern TIM_HandleTypeDef htim7;
extern volatile int timer_count;	// Elapsed time in µs
extern float turning_time;			// Target time in sec

// Prototypes
int Get_Mode(int, int);				// Converts the binary input into active mode
char* Get_Mode_String();			// Convert the current mode to a string
void Auto_Angle(float, TIM_HandleTypeDef*);
void Auto_Line(int, int, int, TIM_HandleTypeDef*);
void Auto_Circle(TIM_HandleTypeDef*);
void Auto_Back_Forth(TIM_HandleTypeDef*);
void Auto_Square(TIM_HandleTypeDef*);


#endif /* INC_AUTO_H_ */
