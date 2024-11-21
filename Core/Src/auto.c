/*
 * auto.c
 *
 *  Created on: Oct 30, 2024
 *      Author: sebas
 */

#include "auto.h"

uint8_t curr_mode = 0;

int Get_Mode(int MSB_state, int LSB_state) {
	static const int id_lookup[4] = {MANUAL_ID, CIRCLE_ID, SQUARE_ID, BACK_FORTH_ID};
	int bin = ((MSB_state & 1) << 1) | (LSB_state & 1);
	return id_lookup[bin];
}

void Auto_Angle(int value, TIM_HandleTypeDef* htim3) {
    // Calculate the duration for the turn
    double turning_time = (fabs(value) * TRACK_WIDTH) / (360.0 * SPEED_FACTOR);

    // Determine direction of turn
    if (value > 0) {
        // Clockwise turn (right): Left track forward, Right track backward
        htim3->Instance->CCR1 = 400;  // Set left track forward (CCR1, CCR3)
        htim3->Instance->CCR3 = 360;  // Reduce speed slightly for alignment
        htim3->Instance->CCR4 = 400;  // Set right track backward (CCR2, CCR4)
        htim3->Instance->CCR2 = 380;
    } else {
        // Counterclockwise turn (left): Left track backward, Right track forward
        htim3->Instance->CCR2 = 400;  // Set right track forward
        htim3->Instance->CCR4 = 360;  // Reduce speed slightly for alignment
        htim3->Instance->CCR1 = 400;  // Set left track backward
        htim3->Instance->CCR3 = 380;
    }

    // Delay for calculated turning time
    HAL_Delay((int)turning_time);

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
        htim3->Instance->CCR1 = current_speed;  // Set speed for left track
        htim3->Instance->CCR2 = current_speed;  // Set speed for right track
        HAL_Delay(UPDATE_INTERVAL);            // Wait for update interval
    }

    // Constant speed phase
    for (int step = 0; step < mid_steps; step++) {
        htim3->Instance->CCR1 = max_speed;
        htim3->Instance->CCR2 = max_speed;
        HAL_Delay(UPDATE_INTERVAL);
    }

    // Deceleration phase
    for (int step = 0; step < decel_steps; step++) {
        current_speed = max_speed - (max_speed - min_speed) * step / decel_steps;
        htim3->Instance->CCR1 = current_speed;
        htim3->Instance->CCR2 = current_speed;
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
    Auto_Angle(180, htim3);

    // Move backward 1 meter
    Auto_Line(DISTANCE, BASE_SPEED, BASE_SPEED, htim3);

    // Turn 180 degrees again to face the original direction
    Auto_Angle(180, htim3);
}

void Auto_Square(TIM_HandleTypeDef* htim3) {
    // Move forward and turn 90 degrees four times
    for (int i = 0; i < 4; i++) {
        Auto_Line(DISTANCE, (BASE_SPEED * SPEED_FACTOR), BASE_SPEED, htim3);
        Auto_Angle(90, htim3);
    }
}
