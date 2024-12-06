/*
 * gestion_moteurs.c
 *
 *  Created on: Oct 30, 2024
 *      Author: sebas
 */

#include "gestion_moteurs.h"
#include "vitesse_affichage.h"

extern int directionD;
extern int directionG;

void Avancer(int Pulse, TIM_HandleTypeDef* htim3){

	htim3->Instance -> CCR2 = 0;
	htim3->Instance -> CCR4 = 0;

	if(Pulse > 499){
		Pulse = 499;
	}

	htim3->Instance -> CCR1 = Pulse;
	htim3->Instance -> CCR3 = htim3->Instance -> CCR1 * 0.9;

	directionD = 0;
	directionG = 0;

	LCD_Vitesse(directionD, directionG);

	return;
}

void Droite(int Pulse, TIM_HandleTypeDef* htim3){

	htim3->Instance -> CCR2 = 0;
	htim3->Instance -> CCR3 = 0;

	if(Pulse > 499){
		Pulse = 499;
	}
	htim3->Instance -> CCR1 = Pulse;
	htim3->Instance -> CCR4 = htim3->Instance -> CCR1 * 0.9;

	directionD = 0;
	directionG = 1;

	LCD_Vitesse(directionD, directionG);

	return;
}

void Gauche(int Pulse, TIM_HandleTypeDef* htim3){

	htim3->Instance -> CCR1 = 0;
	htim3->Instance -> CCR4 = 0;

	if(Pulse > 499){
		Pulse = 499;
	}
	htim3->Instance -> CCR2 = Pulse;
	htim3->Instance -> CCR3 = htim3->Instance -> CCR2 * 0.9;

	directionD = 1;
	directionG = 0;

	LCD_Vitesse(directionD, directionG);

	return;
}

void Reculer(int Pulse,TIM_HandleTypeDef* htim3){

	htim3->Instance -> CCR1 = 0;
	htim3->Instance -> CCR3 = 0;

	if(Pulse > 499){
		Pulse = 499;
	}

	htim3->Instance -> CCR4 = Pulse;
	htim3->Instance -> CCR2 = htim3->Instance -> CCR4 * 0.95;

	directionD = 1;
	directionG = 1;

	LCD_Vitesse(directionD, directionG);

	return;
}
void Stop(TIM_HandleTypeDef* htim3){
	htim3->Instance -> CCR1 = 0;
	htim3->Instance -> CCR2 = 0;
	htim3->Instance -> CCR3 = 0;
	htim3->Instance -> CCR4 = 0;

	directionD = 0;
	directionG = 0;

	LCD_Vitesse(directionD, directionG);

	return;
}
