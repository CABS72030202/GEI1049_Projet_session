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
void LCD_Vitesse(/*uint16_t vitesseG, uint16_t vitesseD, */int encod_D, int encod_G);
void LCD_Init(uint8_t* a);
void LCD_Manuel(int a);
void init_RTC_Time();
void refresh_RTC_Time();
void affich_RTC_Date();
void int_to_str(int num, char *str);
void format_date(int date, int month, int year, char *buffDate);
void format_vitesse(int vitesse, char *buffVitesse, int negatif);
void moyPulse(uint16_t pulseD, uint16_t pulseG, uint16_t *moyD, uint16_t *moyG);



#endif /* INC_VITESSE_AFFICHAGE_H_ */
