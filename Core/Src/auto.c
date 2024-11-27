/*
 * auto.c
 *
 *  Created on: Oct 30, 2024
 *      Author: sebas
 */

#include "auto.h"

uint8_t pause = 0;
uint8_t dip_state = 0;
uint8_t curr_mode = 0;
volatile int timer_count = 0;
float turning_time = 0;

int Get_Mode(int MSB_state, int LSB_state) {
	static const int id_lookup[4] = {MANUAL_MODE, CIRCLE_MODE, SQUARE_MODE, BACK_FORTH_MODE};
	int bin = ((MSB_state & 1) << 1) | (LSB_state & 1);
	return id_lookup[bin];
}

char* Get_Mode_String() {
	const size_t buffer_size = 8;
	char* str = malloc(buffer_size);
	switch(curr_mode) {
		case MANUAL_MODE:
			str = "MANUAL";
			break;
		case CIRCLE_MODE:
			str = "CIRCLE";
			break;
		case BACK_FORTH_MODE:
			str = "LINE";
			break;
		case SQUARE_MODE:
			str = "SQUARE";
			break;
		default:
			str = "ERROR";
			break;
	}
	return str;
}

void Auto_Angle(float value, TIM_HandleTypeDef* htim3) {
    // Calculate the duration for the turn
    turning_time = fabs(value) * TRACK_WIDTH;

    // Determine direction of turn
    if (value > 0) {
    	turning_time /= (360.0 * CLOCKWISE_FACTOR);
        Droite(BASE_SPEED, htim3);
    } else {
    	turning_time /= (360.0 * COUNTER_CLW_FACTOR);
    	Gauche(BASE_SPEED, htim3);
    }

    // Convert turning time in µs
    //turning_time = 20;
    turning_time *= 1e6;

    // Delay for calculated turning time
    HAL_TIM_Base_Start_IT(&htim7);
    while(timer_count < (int)turning_time) {
    	// Wait
    }
    HAL_TIM_Base_Stop_IT(&htim7);

    // Reset temporal counter
    timer_count = 0;

    // Stop the motors
    Stop(htim3);
}

void Auto_Line(int dist, int min_speed, int max_speed, TIM_HandleTypeDef* htim3) {
    // Calculate total steps required
    int total_steps = (int)(dist / TRACK_RESOLUTION);

    // Split into acceleration, constant speed, and deceleration
    int accel_steps = 2 * (total_steps / 5);
    int decel_steps = accel_steps;
    int mid_steps = total_steps - accel_steps - decel_steps;

    // Initialize speed
    int current_speed = min_speed;

    // Acceleration phase
    for (int step = 0; step < accel_steps; step++) {
        current_speed = min_speed + (max_speed - min_speed) * step / accel_steps;
        Avancer(current_speed, htim3);
        HAL_TIM_Base_Start_IT(&htim7);
        while(timer_count < UPDATE_INTERVAL * 1e3) {
        	// Wait for update interval
        }
        HAL_TIM_Base_Stop_IT(&htim7);
        timer_count = 0;
    }

    // Constant speed phase
    for (int step = 0; step < mid_steps; step++) {
    	Avancer(max_speed, htim3);
    	HAL_TIM_Base_Start_IT(&htim7);
    	while(timer_count < UPDATE_INTERVAL * 1e3) {
    		// Wait for update interval
    	}
    	HAL_TIM_Base_Stop_IT(&htim7);
    	timer_count = 0;
    }

    // Deceleration phase
    for (int step = 0; step < decel_steps; step++) {
        current_speed = max_speed - (max_speed - min_speed) * step / decel_steps;
        Avancer(current_speed, htim3);
        HAL_TIM_Base_Start_IT(&htim7);
        while(timer_count < UPDATE_INTERVAL * 1e3) {
        	// Wait for update interval
        }
        HAL_TIM_Base_Stop_IT(&htim7);
        timer_count = 0;
    }

    // Stop the robot
    Stop(htim3);
}

void Auto_Circle(TIM_HandleTypeDef* htim3) {
	// Calculate total steps required
	int total_steps = (int)((PI * DISTANCE) / TRACK_RESOLUTION);

	// Calculate wheel inner wheel ratio
	float ratio = 0.9 * ((int)((2 * PI * ((DISTANCE * 0.5) - TRACK_WIDTH)) / TRACK_RESOLUTION) / total_steps);

	// Constant speed phase
	for (int step = 0; step < total_steps; step++) {
		htim3->Instance -> CCR2 = 0;
		htim3->Instance -> CCR4 = 0;
		htim3->Instance -> CCR1 = BASE_SPEED;
		htim3->Instance -> CCR3 = htim3->Instance -> CCR1 * ratio;
		HAL_TIM_Base_Start_IT(&htim7);
		while(timer_count < UPDATE_INTERVAL * 1e3) {
			// Wait for update interval
	    }
	    HAL_TIM_Base_Stop_IT(&htim7);
	    timer_count = 0;
	}

	// Stop the robot
	Stop(htim3);
}

void Auto_Back_Forth(TIM_HandleTypeDef* htim3) {
    // Move forward 1 meter
    Auto_Line(DISTANCE, BASE_SPEED, BASE_SPEED, htim3);

    // Turn 180 degrees
    Auto_Angle(180.0, htim3);

    // Move backward 1 meter
    Auto_Line(DISTANCE, BASE_SPEED, BASE_SPEED, htim3);

    // Turn 180 degrees again to face the original direction
    Auto_Angle(180.0, htim3);
}

void Auto_Square(TIM_HandleTypeDef* htim3) {
    // Move forward and turn 90 degrees four times
    for (int i = 0; i < 4; i++) {
        Auto_Line(DISTANCE, (BASE_SPEED * 0.333), BASE_SPEED, htim3);
        Auto_Angle(90.0, htim3);
    }
}
