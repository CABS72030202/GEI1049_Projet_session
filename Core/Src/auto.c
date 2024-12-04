/*
 * auto.c
 *
 *  Created on: Oct 30, 2024
 *      Author: sebas
 */

#include "auto.h"
#include "vitesse_affichage.h"

uint8_t pause = 0;
uint8_t dip_state = 0;
uint8_t curr_mode = 0;
uint8_t curr_step = 1;
int total_time = 0;
int accel_time = 0;
int decel_time = 0;
int mid_time = 0;
int current_speed = 0;
volatile int timer_count = 0;
float turning_time = 0;
int save[4] = {0, 0, 0, 0};
float RATIO = 0.9;
float CLOCKWISE_FACTOR = 66.879;
float COUNTER_CLW_FACTOR = 0;


int Get_Mode(int MSB_state, int LSB_state) {
	static const int id_lookup[4] = {MANUAL_MODE, CIRCLE_MODE,  BACK_FORTH_MODE, SQUARE_MODE};
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

void Auto_Angle(float value) {
	// Initial setup
	if(timer_count == 0) {
		LCD_Manuel(0);
		LCD_Mode();

		// Calculate the duration for the turn
		turning_time = fabs(value) * TRACK_WIDTH;

		// Determine direction of turn
		if (value > 0) {
			turning_time /= (360.0 * CLOCKWISE_FACTOR);
			Droite(BASE_SPEED, &htim3);
		}
		else {
			turning_time /= (360.0 * COUNTER_CLW_FACTOR);
			Gauche(BASE_SPEED, &htim3);
		}

		// Convert turning time in µs
		turning_time *= 1e6;

		// Start timer
	    HAL_TIM_Base_Start_IT(&htim7);
	}

    // Stop when finished
	if(timer_count > (int)turning_time) {

		// Stop timer
		HAL_TIM_Base_Stop_IT(&htim7);

		// Stop the motors
		Stop(&htim3);

		// Reset temporal counter
		timer_count = 0;

		// Ready for next step
		curr_step++;
	}

	return;
}

void Auto_Line(int dist, int min_speed, int max_speed) {
    // Constants
    const float accel_fraction = 0.4; // 40% of total time for acceleration
    const float decel_fraction = 0.4; // 40% of total time for deceleration

    // Initial setup
    if (timer_count == 0) {
    	LCD_Manuel(0);
    	LCD_Mode();

    	// Calculate total pulses required
    	float total_pulses = dist / TRACK_RESOLUTION;

        // Calculate total time
        turning_time = (min_speed + max_speed) / 2.0;
        total_time = (int)((total_pulses / turning_time) * 1e7);

        // Calculate time for each phase
        accel_time = (int)(total_time * accel_fraction);
        decel_time = (int)(total_time * decel_fraction);
        mid_time = total_time - accel_time - decel_time;

        // Start timer
        HAL_TIM_Base_Start_IT(&htim7);
    }

    // Acceleration phase
    if (timer_count < accel_time) {
        current_speed = min_speed + (max_speed - min_speed) * timer_count / accel_time;
    }

    // Constant speed phase
    else if (timer_count < accel_time + mid_time) {
        current_speed = max_speed;
    }

    // Deceleration phase
    else if (timer_count < total_time) {
        int decel_time_elapsed = timer_count - (accel_time + mid_time);
        current_speed = max_speed - (max_speed - min_speed) * decel_time_elapsed / decel_time;
    }

    // Stop when finished
    else {
        // Stop timer
        HAL_TIM_Base_Stop_IT(&htim7);

        // Stop the motors
        Stop(&htim3);

        // Reset temporal counter
        timer_count = 0;

        // Ready for next step
        curr_step++;

        // Exit
        return;
    }

    // Adjust speed every iteration
    if(!pause)
    	Avancer(current_speed, &htim3);

    return;
}


void Auto_Circle() {
	// Initial setup
	if(timer_count == 0) {
		LCD_Manuel(0);
		LCD_Mode();

		// Calculate total outer wheel distance
		float outer_circumference = (PI*DISTANCE) / TRACK_RESOLUTION;//785
		float inner_circumference = outer_circumference - TRACK_WIDTH;//513

		// Calculate total time
		total_time = (int)((outer_circumference / BASE_SPEED) * 1e7);//17.4s

		// Calculate wheel inner wheel speed
		float factor = RATIO * (inner_circumference / outer_circumference);

		// Constant speed
		htim3.Instance -> CCR2 = 0;
		htim3.Instance -> CCR4 = 0;
		htim3.Instance -> CCR1 = BASE_SPEED;
		htim3.Instance -> CCR3 = (factor * BASE_SPEED);
		HAL_TIM_Base_Start_IT(&htim7);
	}

	// Stop when finished
	if(timer_count > (int)total_time) {

		// Stop timer
		HAL_TIM_Base_Stop_IT(&htim7);

		// Stop the motors
		Stop(&htim3);

		// Reset temporal counter
		timer_count = 0;

		// End of sequence : reset current step and set to manual mode after drawing shape
		curr_mode = MANUAL_MODE;
		LCD_Mode();
		LCD_Manuel(3);
	}

	return;
}

void Auto_Back_Forth() {
	// Manage ongoing step
	switch(curr_step) {
	case 1:
		// Step 1: Move forward 1 meter
		Auto_Line(DISTANCE, BASE_SPEED, BASE_SPEED);
		break;

	case 2:
		// Step 2: Turn 180 degrees
		Auto_Angle(180.0);
		break;

	case 3:
		// Step 3: Move backward 1 meter
		Auto_Line(DISTANCE, BASE_SPEED, BASE_SPEED);
		break;

	case 4:
		// Step 4: Turn 180 degrees again to face the original direction
	    Auto_Angle(180.0);
		break;

	default:
		// End of sequence : reset current step and set to manual mode after drawing shape
		curr_step = 1;
		curr_mode = MANUAL_MODE;
		LCD_Mode();
		LCD_Manuel(3);
		break;
	}
	return;
}

void Auto_Square() {
	// Manage ongoing step
	switch(curr_step) {
	case 1:
	case 3:
	case 5:
	case 7:
		// Odd steps: Move forward
		Auto_Line(DISTANCE, (BASE_SPEED * 0.333), BASE_SPEED);
		break;

	case 2:
	case 4:
	case 6:
	case 8:
		// Even steps: Turn 90 degrees
		Auto_Angle(90.0);
		break;

	default:
		// End of sequence: reset current step and set to manual mode after drawing shape
		curr_step = 1;
		curr_mode = MANUAL_MODE;
		LCD_Mode();
		LCD_Manuel(3);
		break;
	}
	return;
}

void Pause() {
	pause = pause ^ 1;

	// Save currrent CCR values
	save[0] = htim3.Instance -> CCR1;
	save[1] = htim3.Instance -> CCR2;
	save[2] = htim3.Instance -> CCR3;
	save[3] = htim3.Instance -> CCR4;

	// Stop
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
	HAL_TIM_Base_Stop_IT(&htim7);

	return;
}

void Resume() {
	pause = pause ^ 1;

	// Restore saved CCR values
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	htim3.Instance -> CCR1 = save[0];
	htim3.Instance -> CCR2 = save[1];
	htim3.Instance -> CCR3 = save[2];
	htim3.Instance -> CCR4 = save[3];
	HAL_TIM_Base_Start_IT(&htim7);

	return;
}

void Constant_Tuning_Mode() {
	// If robot is not moving, do nothing
	if(curr_mode == MANUAL_MODE)
		return;

	// Testing turning factors
	if(abs(DEBUG_MODE) == 1)
		// Manage ongoing step
		switch(curr_step) {
			case 1:
			case 3:
				// Steps 1 and 3: Move forward
				Auto_Line(100, BASE_SPEED, BASE_SPEED);
				break;

			case 2:
				// Step 2: Turn 90 degrees
				Auto_Angle(90.0 * DEBUG_MODE);
				break;

			default:
				// End of sequence: Reset current step and set to manual mode after drawing shape
				curr_step = 1;
				curr_mode = MANUAL_MODE;
				break;
		}

	// Testing auto circle ratio
	else if(DEBUG_MODE == 2)
		Auto_Circle();
	return;
}
