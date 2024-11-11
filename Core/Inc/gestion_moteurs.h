/*
 * gestion_moteurs.h
 *
 *  Created on: Oct 30, 2024
 *      Author: sebas
 */

#ifndef INC_GESTION_MOTEURS_H_
#define INC_GESTION_MOTEURS_H_

#include "stm32f4xx_hal.h"

void VitCommande(int, int, int *, int *, TIM_HandleTypeDef);

#endif /* INC_GESTION_MOTEURS_H_ */
