/*
 * auto.c
 *
 *  Created on: Oct 30, 2024
 *      Author: sebas
 */

#include "auto.h"

uint8_t curr_mode = 0;
volatile int timer_count = 0;
float turning_time = 0;

int Get_Mode(int MSB_state, int LSB_state) {
	static const int id_lookup[4] = {MANUAL_ID, CIRCLE_ID, SQUARE_ID, BACK_FORTH_ID};
	int bin = ((MSB_state & 1) << 1) | (LSB_state & 1);
	return id_lookup[bin];
}

char* Get_Mode_String() {
	const size_t buffer_size = 8;
	char* str = malloc(buffer_size);
	switch(curr_mode) {
		case MANUAL_ID:
			str = "MANUAL";
			break;
		case CIRCLE_ID:
			str = "CIRCLE";
			break;
		case BACK_FORTH_ID:
			str = "LINE";
			break;
		case SQUARE_ID:
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
        // Clockwise turn (right): Left track forward, Right track backward
        /*htim3->Instance->CCR1 = 400;  // Set left track forward (CCR1, CCR3)
        htim3->Instance->CCR3 = 0;  // Reduce speed slightly for alignment
        htim3->Instance->CCR4 = 400;  // Set right track backward (CCR2, CCR4)
        htim3->Instance->CCR2 = 200;*/
    	turning_time /= (360.0 * CLOCKWISE_FACTOR);
        Droite(BASE_SPEED, htim3);
    } else {
        // Counterclockwise turn (left): Left track backward, Right track forward
        /*htim3->Instance->CCR2 = 400;  // Set right track forward
        htim3->Instance->CCR4 = 0;  // Reduce speed slightly for alignment
        htim3->Instance->CCR1 = 400;  // Set left track backward
        htim3->Instance->CCR3 = 200;*/
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
    int mid_steps = total_steps / 2;
    int accel_steps = mid_steps / 2;
    int decel_steps = total_steps - accel_steps - mid_steps;

    // Initialize speed
    int current_speed = min_speed;

    // Acceleration phase
    for (int step = 0; step < accel_steps; step++) {
        current_speed = min_speed + (max_speed - min_speed) * step / accel_steps;
        /*htim3->Instance->CCR1 = current_speed;  // Set speed for left track
        htim3->Instance->CCR3 = current_speed;  // Set speed for right track*/
        Avancer(current_speed, htim3);
        HAL_Delay(UPDATE_INTERVAL);            // Wait for update interval
    }

    // Constant speed phase
    for (int step = 0; step < mid_steps; step++) {
        /*htim3->Instance->CCR1 = max_speed;
        htim3->Instance->CCR3 = max_speed;*/
    	Avancer(max_speed, htim3);
        HAL_Delay(UPDATE_INTERVAL);
    }

    // Deceleration phase
    for (int step = 0; step < decel_steps; step++) {
        current_speed = max_speed - (max_speed - min_speed) * step / decel_steps;
        /*htim3->Instance->CCR1 = current_speed;
        htim3->Instance->CCR3 = current_speed;*/
        Avancer(current_speed, htim3);
        HAL_Delay(UPDATE_INTERVAL);
    }

    // Stop the robot
    Stop(htim3);
}

void Auto_Circle(TIM_HandleTypeDef* htim3) {

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

int Calc_ARR(float update_event) {
	const int psc = 59999, clk = 84e6, factor = 100;
	int arr = 0;

	// Convert update_event to ms for precision
	update_event *= factor;

	arr = (clk / (update_event * (psc + 1))) - 1;

	// Adjust ARR from conversion factor
	//arr /= factor;

	return arr;
}
