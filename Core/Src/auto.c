/*
 * auto.c
 *
 *  Created on: Oct 30, 2024
 *      Author: sebas
 */

#include "auto.h"

uint8_t curr_mode = 0;

int Get_Mode(int MSB_state, int LSB_state) {
	static const int id_lookup[4] = {MANUAL_ID, CIRCLE_ID, SQUARE_ID, LINE_ID};
	int bin = ((MSB_state & 1) << 1) | (LSB_state & 1);
	return id_lookup[bin];
}

void Auto_Angle(int value, TIM_HandleTypeDef htim3) {
    // Calculate the duration for the turn
    double turning_time = (fabs(value) * TRACK_WIDTH) / (360.0 * SPEED_FACTOR);

    // Determine direction of turn
    if (value > 0) {
        // Clockwise turn (right): Left track forward, Right track backward
        htim3.Instance->CCR1 = 400;  // Set left track forward (CCR1, CCR3)
        htim3.Instance->CCR3 = 360;  // Reduce speed slightly for alignment
        htim3.Instance->CCR4 = 400;  // Set right track backward (CCR2, CCR4)
        htim3.Instance->CCR2 = 380;
    } else {
        // Counterclockwise turn (left): Left track backward, Right track forward
        htim3.Instance->CCR2 = 400;  // Set right track forward
        htim3.Instance->CCR4 = 360;  // Reduce speed slightly for alignment
        htim3.Instance->CCR1 = 400;  // Set left track backward
        htim3.Instance->CCR3 = 380;
    }

    // Delay for calculated turning time
    HAL_Delay((int)turning_time);

    // Stop the motors
    Stop(htim3);
}

void Auto_Circle() {

}

void Auto_Square() {

}

void Auto_Line() {

}
