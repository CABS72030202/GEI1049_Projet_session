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

void Auto_Circle() {

}

void Auto_Square() {

}

void Auto_Line() {

}
