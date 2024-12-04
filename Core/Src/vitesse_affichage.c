/*
 * vitesse_affichage.c
 *
 *  Created on: Oct 30, 2024
 *      Author: sebas
 */

#include "vitesse_affichage.h"

#include "image.h"
#include "LCD_Test.h"
#include "LCD_2inch.h"
#include "DEV_Config.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "auto.h"
#include "rtc.h"
#include "stdint.h"
#define INDICE_MEM_MAX 4

	char buffDate[15] = {0};
	char buffTime[10] = {0};
	char buffTimeRefresh[2] = {0};
	char buffVG[5] = {0};
	char buffVD[5] = {0};
    int ind = 0;
	RTC_TimeTypeDef currentTime = {0};
	RTC_DateTypeDef currentDate = {0};
    uint8_t hours = 0, pastHours = 0;
    uint8_t minutes = 0, pastMinutes = 0;
    uint8_t seconds = 0, pastSeconds = 0;
    uint8_t largeurFont24 = 17;

	uint16_t memPulseD[(INDICE_MEM_MAX + 1)] = {0};
	uint16_t memPulseG[(INDICE_MEM_MAX + 1)] = {0};
	uint8_t indiceMem = 0;
	uint8_t sumG = 0;
	uint8_t sumD = 0;



    extern volatile uint16_t vitesseD, vitesseG; //en mm par seconde

void moyPulse(uint16_t pulseD, uint16_t pulseG, uint16_t *moyD, uint16_t *moyG){

	sumD = 0;
	sumG = 0;

    if(indiceMem < INDICE_MEM_MAX){
        indiceMem++;
    }
    else{
        indiceMem = 0;
    }

	memPulseD[indiceMem] = pulseD;
	memPulseG[indiceMem] = pulseG;

	for (ind = 0; ind < (INDICE_MEM_MAX+1); ind++){
		sumD += memPulseD[ind];
		sumG += memPulseG[ind];
	}

	*moyD = sumD / (INDICE_MEM_MAX + 1 );
	*moyG = sumG / (INDICE_MEM_MAX + 1);
}


void int_to_str(int num, char *str) {
	    int i = 0, j, digit;
	    char temp;

	    if (num == 0) {
	        str[i++] = '0';
	    } else {
	        while (num > 0) {
	            digit = num % 10;
	            str[i++] = digit + '0';
	            num /= 10;
	        }
	    }
	    str[i] = '\0';

	    // Inverser la chaîne
	    for (j = 0; j < i / 2; j++) {
	        temp = str[j];
	        str[j] = str[i - 1 - j];
	        str[i - 1 - j] = temp;
	    }
	}

void format_date(int date, int month, int year, char *buffDate) {
	    ind = 0;

	    int_to_str(date, buffDate + ind);
	    while (buffDate[ind] != '\0') ind++;
	    buffDate[ind++] = ',';

	    int_to_str(month, buffDate + ind);
	    while (buffDate[ind] != '\0') ind++;
	    buffDate[ind++] = ',';

	    buffDate[ind++] = '2';
	    buffDate[ind++] = '0';
	    int_to_str(year, buffDate + ind);
	}

void format_vitesse(int vitesse, char *buffVitesse, int negatif) {
	    ind = 0;
	    if(negatif)
	    {
		buffVitesse[ind] = '-';
	    }
	    else
	    {
	    buffVitesse[ind] = '+';
	    }

	    ind++;
	    if(vitesse < 100){
	    	buffVitesse[ind] = 48;
	    	ind++;

		    if(vitesse < 10){
		    	buffVitesse[ind] = 48;
		    	ind++;
		    }
	    }

	    int_to_str(vitesse, buffVitesse + ind);
	    buffVitesse[4] = 0;

	}

void init_RTC_Time()
{
	if (HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN) == HAL_OK)
	{
		HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
	    hours = currentTime.Hours;
	    minutes = currentTime.Minutes;
	    seconds = currentTime.Seconds;
	}
	else
	{
		Error_Handler();
	}
		ind = 0;

		if(hours/10 == 0){
			buffTime[ind] = '0';
			ind++;
		}

	    int_to_str(hours, buffTime + ind);
	    while (buffTime[ind] != '\0') ind++;
	    buffTime[ind++] = 58;

	    if(minutes/10 ==0){
			buffTime[ind] = '0';
			ind++;
	    	}

	    int_to_str(minutes, buffTime + ind);
	    while (buffTime[ind] != '\0') ind++;
	    buffTime[ind++] = 58;

	    if(seconds/10 == 0){
	    	buffTime[ind] = '0';
	    	ind++;
	    	}

	    int_to_str(seconds, buffTime + ind);

		Paint_DrawString_EN (5, 25, buffTime,        &Font24,    MAGENTA,  WHITE);
		pastHours = hours;
		pastMinutes = minutes;
		pastSeconds = seconds;

}

void refresh_RTC_Time(){
	if (HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN) == HAL_OK)
	{
		HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
	    hours = currentTime.Hours;
	    minutes = currentTime.Minutes;
	    seconds = currentTime.Seconds;
	}
	else
	{
		Error_Handler();
	}

if(pastSeconds != seconds){
	ind = 0;
    if(seconds/10 == 0){
    	buffTimeRefresh[ind] = '0';
    	ind++;
    	}
    int_to_str(seconds, buffTimeRefresh + ind);
    Paint_DrawString_EN ((5 + 6 * largeurFont24), 25, buffTimeRefresh,        &Font24,    MAGENTA,  WHITE);
	pastSeconds = seconds;
}

if(pastMinutes != minutes){
	ind = 0;
    if(minutes/10 == 0){
    	buffTimeRefresh[ind] = '0';
    	ind++;
    	}
    int_to_str(minutes, buffTimeRefresh + ind);
    Paint_DrawString_EN ((5 + 3 * largeurFont24), 25, buffTimeRefresh,        &Font24,    MAGENTA,  WHITE);
	pastMinutes = minutes;
}

if(pastHours != hours){
	ind = 0;
    if(hours/10 == 0){
    	buffTimeRefresh[ind] = '0';
    	ind++;
    	}
    int_to_str(hours, buffTimeRefresh + ind);
    Paint_DrawString_EN (5, 25, buffTimeRefresh,        &Font24,    MAGENTA,  WHITE);
	pastHours = hours;
}





}

void affich_RTC_Date()
{
	uint8_t year = 0;
	uint8_t month = 0;
	uint8_t date = 0;
	uint8_t weekday = 0;

	if (HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN) == HAL_OK)
	{
	    year = currentDate.Year;
	    month = currentDate.Month;
	    date = currentDate.Date;
	    weekday = currentDate.WeekDay;
	}
	else
	{
		Error_Handler();
	}


	switch(weekday)
	{
			case 1:
				Paint_DrawString_EN (5, 1, "Lundi le ",        &Font24,    MAGENTA,  WHITE);
	            break;
	        case 2:
				Paint_DrawString_EN (5, 1, "Mardi le ",        &Font24,    MAGENTA,  WHITE);
	            break;
	        case 3:
				Paint_DrawString_EN (5, 1, "Mercredi le ",        &Font24,    MAGENTA,  WHITE);
	            break;
	        case 4:
	        	Paint_DrawString_EN (5, 1, "Jeudi le ",        &Font24,    MAGENTA,  WHITE);
	            break;
	        case 5:
	        	Paint_DrawString_EN (5, 1, "Vendredi le ",        &Font24,    MAGENTA,  WHITE);
	            break;
	        case 6:
	        	Paint_DrawString_EN (5, 1, "Samedi le ",        &Font24,    MAGENTA,  WHITE);
	            break;
	        case RTC_WEEKDAY_SUNDAY:
	        	Paint_DrawString_EN (5, 1, "Dimanche le ",        &Font24,    MAGENTA,  WHITE);
	            break;
	        default:
	        	Paint_DrawString_EN (5, 1, "Journee ",        &Font24,    MAGENTA,  WHITE);
	            break;
	}

	char buffDate[15];
	format_date(date, month, year, buffDate);;


	Paint_DrawString_EN (210, 6, buffDate,        &Font16,    MAGENTA,  WHITE);

}


void LCD_Manuel(int a)
{
	if (a == 1){
	Paint_DrawString_EN (120, 75, "1",        &Font24,    MAGENTA,  WHITE);
	}
	else if (a == 2){
	Paint_DrawString_EN (120, 75, "2",        &Font24,    MAGENTA,  WHITE);
	}
	else if (a==3){
	Paint_DrawString_EN (120, 75, "3",        &Font24,    MAGENTA,  WHITE);
	}
	else{
	Paint_DrawString_EN (120, 75, "   ",        &Font24,    MAGENTA,  WHITE);
	}
}

void LCD_Mode()
{
	char charMode[12];
	strcpy(charMode, Get_Mode_String());
	Paint_DrawString_EN (5, 75, charMode,        &Font24,    MAGENTA,  WHITE);
}

void LCD_Vitesse(int encod_D, int encod_G)
{


	if(encod_G == 0)
	{
		format_vitesse(vitesseG, buffVG, 0);
	}
	else
	{
		format_vitesse(vitesseG, buffVG, 1);
	}

	if(encod_D == 0)
	{
		format_vitesse(vitesseD, buffVD, 0);
	}
	else
	{
		format_vitesse(vitesseD, buffVD, 1);
	}

	Paint_DrawString_EN (180, 105, buffVD,        &Font16,    MAGENTA,  WHITE);
	Paint_DrawString_EN (180, 130, buffVG,        &Font16,    MAGENTA,  WHITE);
}

void LCD_Init(uint8_t* a)
{
	DEV_Module_Init();


	LCD_2IN_SetBackLight(1000);
	LCD_2IN_Init();
	LCD_2IN_Clear(WHITE);


	Paint_NewImage(LCD_2IN_WIDTH,LCD_2IN_HEIGHT, ROTATE_90, WHITE);


	Paint_SetClearFuntion(LCD_2IN_Clear);
	Paint_SetDisplayFuntion(LCD_2IN_DrawPaint);

  printf("Paint_Clear\r\n");
	Paint_Clear(MAGENTA);
	DEV_Delay_ms(500);


	Paint_SetRotate(ROTATE_270);
	/*Paint_DrawString_EN (50, 100, "DEMARRAGE DU",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (100, 125, "PROJET",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (70, 150, "VROUM-VROUM",        &Font24,    MAGENTA,  WHITE);
	DEV_Delay_ms(500);
	Paint_Clear(MAGENTA);

	Paint_DrawString_EN (85, 100, "LE PROJET",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (70, 125, "VROUM-VROUM",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (85, 150, "VOUS SALUT",        &Font24,    MAGENTA,  WHITE);
	DEV_Delay_ms(500);
	Paint_Clear(MAGENTA);
*/
	affich_RTC_Date();
	init_RTC_Time();
	Paint_DrawString_EN (5, 50, "Mode actuel : ",        &Font24,    MAGENTA,  WHITE);


	LCD_Mode();
	LCD_Manuel(3);

	Paint_DrawString_EN (5, 100, "Vitesse G:    mm/s",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (5, 125, "Vitesse D:    mm/s",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (10, 220, "Attention aux enfants",        &Font20,    YELLOW,  RED);

	LCD_Vitesse(0, 0);

	DEV_Delay_ms(500);

	*a = 1;
	//DEV_Module_Exit();

}


