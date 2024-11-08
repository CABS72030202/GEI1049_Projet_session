/*
 * gestion_moteurs.c
 *
 *  Created on: Oct 30, 2024
 *      Author: sebas
 */

#include "gestion_moteurs.h"

#define max 200
#define min -200

void VitCommande(int Vgc, int Vdc, int *Vgv, int *Vdv, TIM_HandleTypeDef htim7){

	if(Vgc >= max){
		*Vgv = max;
	}
	else if(Vgc <= min){
		*Vgv = min;
	}
	else{
		*Vgv = Vgc;
	}

	if(Vdc >= max){
		*Vdv = max;
	}
	else if(Vdc <= min){
		*Vdv = min;
	}
	else{
		*Vdv = Vdc;
	}
	HAL_TIM_Base_Start_IT(&htim7);
}
