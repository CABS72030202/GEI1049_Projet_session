/*
 * manette.c
 *
 *  Created on: Nov 27, 2024
 *      Author: Yohan Lefebvre
 */

#include "manette.h"
#include "vitesse_affichage.h"

uint8_t initcomm1[2] = {0xF0, 0x55};
uint8_t initcomm2[2] = {0xFB, 0x00};

uint8_t data[6];
int formatted_data[7];
int Xdata;
int Ydata;
int Xpulse;
int Ypulse;
int Cbutton;
int ToggleC = 1;
int Zbutton;

int modeVitesse = 3;
float pulseCoeff = 1.96;
float maxInverse = 500;
float maxInverse2 = 500*0.66;
float maxInverse1 = 500*0.33;
int sendPulseMotor;

int toggle;

void Format_Data() {
	/*FROM LAB7*/
	formatted_data[0] = data[0];
	formatted_data[1] = data[1];
	formatted_data[5] = (data[5] & 0x02);
	if(formatted_data[5] == 2)
		formatted_data[5] = 1;
	formatted_data[6] = (data[5] & 0x01);

	/*MY DATA*/
	Xdata = data[0];
	Ydata = data[1];
	Xpulse = data[0];
	Ypulse = data[1];
	Cbutton = (data[5] & 0x02);
	Zbutton = (data[5] & 0x01);
	/* neutral x and y data: 128
	max x and y data: 1-254 */
}
void Initialisation_manette(){

	HAL_I2C_Master_Transmit(&hi2c1, NUNCHUK_ADDRESS, initcomm1, 2, HAL_MAX_DELAY);
	HAL_Delay(10);
	HAL_I2C_Master_Transmit(&hi2c1, NUNCHUK_ADDRESS, initcomm2, 2, HAL_MAX_DELAY);
}


void Controller(){

	HAL_I2C_Master_Transmit(&hi2c1, NUNCHUK_ADDRESS, 0x00, 1, HAL_MAX_DELAY);
	HAL_Delay(10);
	HAL_I2C_Master_Receive(&hi2c1, NUNCHUK_ADDRESS, data, 6, HAL_MAX_DELAY);
	Format_Data();

	if(Xdata == 0){
		Xdata = 1;
	}
	if(Ydata == 0){
			Ydata = 1;
		}




	if(Cbutton == 0){
		HAL_Delay(10);
		ToggleModeMan();
	}
	if(Zbutton == 0){
		HAL_Delay(100);
		ToggleModeVit();
	}


	if(ToggleC == 1){


		if(Xdata == 128 && Ydata == 128){
				Stop(&htim3);
		}

		else{

			/* Adjustment of data according to mode */

		if(modeVitesse==1){
			Xpulse = Xdata * 0.33 * pulseCoeff;
			Ypulse = Ydata * 0.33 * pulseCoeff;
			maxInverse = maxInverse1;
			LCD_Manuel(1);
		}

		if(modeVitesse==2){
			Xpulse = Xdata * 0.66 * pulseCoeff;
			Ypulse = Ydata * 0.66 * pulseCoeff;
			maxInverse = maxInverse2;
			LCD_Manuel(2);
		}

		if(modeVitesse==3){
			Xpulse = Xdata * pulseCoeff;
			Ypulse = Ydata * pulseCoeff;
			maxInverse = 500;
			LCD_Manuel(3);
		}


		/* Sending pulse to motor */

		if(Xdata>128){
			sendPulseMotor = Xpulse;
			Droite(sendPulseMotor,&htim3);
		}

		else if(Xdata<128) {
				sendPulseMotor = maxInverse - Xpulse;
				Gauche(sendPulseMotor,&htim3);
			}


		if(Ydata>128){
			sendPulseMotor = Ypulse;
			Avancer(sendPulseMotor,&htim3);
		}

		else if(Ydata<128){
				sendPulseMotor = maxInverse - Ypulse;
				Reculer(sendPulseMotor,&htim3);
			}
		}
	}
	else{
		Stop(&htim3);
	}

}


void ToggleModeVit() {
	if(modeVitesse == 3) {
		modeVitesse = 1;
	}
	else {
		modeVitesse++;
	}
}

void ToggleModeMan() {

	if(ToggleC == 1){
		ToggleC = 0;
	}
	else if(ToggleC == 0){
		ToggleC = 1;
	}

}




