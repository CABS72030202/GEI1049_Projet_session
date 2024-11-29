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
	Paint_DrawString_EN (120, 75, "      ",        &Font24,    MAGENTA,  WHITE);
	}
}

void LCD_Mode()
{
	char charMode[12];
	strcpy(charMode, Get_Mode_String());
	Paint_DrawString_EN (5, 75, charMode,        &Font24,    MAGENTA,  WHITE);
}

void LCD_Vitesse(uint16_t vitesseG, uint16_t vitesseD, int encod_D, int encod_G)
{
	char buffVG[14];
	char buffVD[14];

	if(encod_G == 0)
	{
		sprintf(buffVG,
		  "+%2d",
		   vitesseG);
	}
	else
	{
		sprintf(buffVG,
			"-%2d",
			   vitesseG);
	}

	if(encod_D == 0)
	{
		sprintf(buffVD,
		  "+%2d",
		   vitesseD);
	}
	else
	{
		sprintf(buffVD,
			"-%2d",
			   vitesseD);
	}

	Paint_DrawString_EN (190, 100, buffVD,        &Font16,    MAGENTA,  WHITE);
	Paint_DrawString_EN (190, 125, buffVG,        &Font16,    MAGENTA,  WHITE);
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
	Paint_DrawString_EN (50, 100, "DEMARRAGE DU",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (100, 125, "PROJET",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (70, 150, "VROUM-VROUM",        &Font24,    MAGENTA,  WHITE);
	DEV_Delay_ms(500);
	Paint_Clear(MAGENTA);

	Paint_DrawString_EN (85, 100, "LE PROJET",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (70, 125, "VROUM-VROUM",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (85, 150, "VOUS SALUT",        &Font24,    MAGENTA,  WHITE);
	DEV_Delay_ms(500);
	Paint_Clear(MAGENTA);

	Paint_DrawString_EN (5, 1, "La date : ",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (5, 25, "L'heure : ",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (5, 50, "Mode actuel : ",        &Font24,    MAGENTA,  WHITE);


	LCD_Mode();
	LCD_Manuel(3);

	Paint_DrawString_EN (5, 100, "Vitesse G :   %",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (5, 125, "Vitesse D :   %",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (10, 220, "Attention aux enfants",        &Font20,    YELLOW,  RED);

	DEV_Delay_ms(3000);

	*a = 1;
	//DEV_Module_Exit();

}


