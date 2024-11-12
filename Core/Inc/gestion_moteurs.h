/*
 * gestion_moteurs.h
 *
 *  Created on: Oct 30, 2024
 *      Author: sebas
 */

#ifndef INC_GESTION_MOTEURS_H_
#define INC_GESTION_MOTEURS_H_

#include "stm32f4xx_hal.h"

void Avancer(int, TIM_HandleTypeDef);
void Droite(int, TIM_HandleTypeDef);
void Gauche(int, TIM_HandleTypeDef);
void Reculer(int, TIM_HandleTypeDef);
void Stop(TIM_HandleTypeDef);

#endif /* INC_GESTION_MOTEURS_H_ */
