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

#define TRACK_RESOLUTION 4.652		// Distance per encoder step in mm
#define TRACK_WIDTH 272.0     		// Distance between the two tracks in mm
#define BASE_SPEED 450
#define DISTANCE 1000

// Variables
extern uint8_t pause;				// Detect blue button press
extern uint8_t dip_state;			// Current dipswitch state
extern uint8_t curr_mode;			// Current mode
extern uint8_t curr_step;			// Current step to execute
extern int total_time;
extern int accel_time;
extern int decel_time;
extern int mid_time;
extern int current_speed;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim7;
extern volatile int timer_count;	// Elapsed time in µs
extern float turning_time;			// Target time in sec
extern int save[4];					// Save CCR register values on pause
extern float RATIO;
extern float CLOCKWISE_FACTOR; 		// Turning factor [t_moy(180) = 2.50s]
extern float COUNTER_CLW_FACTOR; 	// Turning factor [t_moy(-180) = 2.22s]

// Prototypes
int Get_Mode(int, int);				// Converts the binary input into active mode
char* Get_Mode_String();			// Convert the current mode to a string
void Auto_Angle(float);
void Auto_Line(int, int, int);
void Auto_Circle();
void Auto_Back_Forth();
void Auto_Square();
void Pause();
void Resume();

/* Use exclusively for debugging: increment constant values every time
 * blue button is pressed and make a 90 degree turn afterwards. Note
 * that all automatic modes are disabled when this feature is enabled.
 *
 * To enable this feature :
 *  - Change DEBUG_MODE value to 1 for clockwise turns, to -1 for
 *    counter-clockwise turns and to 2 for auto circle ratio
 *  - Declare constants as variables without changing their names
 *  - Define the starting value and the step value
 *
 * When using this feature :
 * 	- Press on the blue button until the robot can make a precise 90
 * 	  degrees angle
 * 	- If one press makes it suddenly turn too much, change the starting
 * 	  point value to the current value using the following formula :
 * 	  (STARTING_VALUE + STEP_VALUE * number of presses) and reduce step
 * 	  value.
 * 	- Repeat until satisfaction, then change DEBUG_MODE value to 0 and
 * 	  change defined value of tested constant
 */
#define DEBUG_MODE 0
void Constant_Tuning_Mode();
#define STARTING_VALUE 0.8
#define STEP_VALUE -0.01

#endif /* INC_AUTO_H_ */
