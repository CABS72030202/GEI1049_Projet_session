/*
 * vitesse_affichage.h
 *
 *  Created on: Oct 30, 2024
 *      Author: sebas
 */

#ifndef INC_VITESSE_AFFICHAGE_H_
#define INC_VITESSE_AFFICHAGE_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

void LCD_Mode();
void LCD_Vitesse(uint16_t vitesseG, uint16_t vitesseD, int encod_D, int encod_G);
void LCD_Init(uint8_t* a);
void LCD_Manuel(int a);

#endif /* INC_VITESSE_AFFICHAGE_H_ */
