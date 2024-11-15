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

void LCD_Mode(uint8_t modeManuel)
{
	char buffModeMan[10] = {0};

    switch(modeManuel) {
        case 1:
            strcpy(buffModeMan, "M1");
            break;
        case 2:
            strcpy(buffModeMan, "M2");
            break;
        case 3:
            strcpy(buffModeMan, "M3");
            break;
        default:
            sprintf(buffModeMan, "M %d", modeManuel);
            break;
    }
 	Paint_DrawString_EN (250, 50, buffModeMan,        &Font24,    MAGENTA,  WHITE);
}

void LCD_Vitesse(uint16_t vitesseG, uint16_t vitesseD)
{
	char buffVG[11];
	char buffVD[11];
	sprintf(buffVG,
		  "%3d mm/s",
		   vitesseG);

	sprintf(buffVD,
		  "%3d mm/s",
		   vitesseD);

	Paint_DrawString_EN (190, 100, buffVG,        &Font20,    MAGENTA,  WHITE);
	Paint_DrawString_EN (190, 125, buffVD,        &Font20,    MAGENTA,  WHITE);
}

void LCD_Init(uint8_t* a, uint8_t modeA, uint8_t modeB)
{
//	printf("LCD_2IN_test Demo\r\n");
	DEV_Module_Init();

//  printf("LCD_2IN_ Init and Clear...\r\n");
	LCD_2IN_SetBackLight(1000);
	LCD_2IN_Init();
	LCD_2IN_Clear(WHITE);

//  printf("Paint_NewImage\r\n");
	Paint_NewImage(LCD_2IN_WIDTH,LCD_2IN_HEIGHT, ROTATE_90, WHITE);

//  printf("Set Clear and Display Funtion\r\n");
	Paint_SetClearFuntion(LCD_2IN_Clear);
	Paint_SetDisplayFuntion(LCD_2IN_DrawPaint);

  printf("Paint_Clear\r\n");
	Paint_Clear(MAGENTA);
	DEV_Delay_ms(500);

	//  printf("Painting...\r\n");
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

	if(modeA == 0 && modeB == 0)
	{
		Paint_DrawString_EN (250, 50, "A1",        &Font24,    MAGENTA,  WHITE);
	}
	else if(modeA == 1 && modeB == 0)
	{
		Paint_DrawString_EN (250, 50, "A2",        &Font24,    MAGENTA,  WHITE);
	}
	else if(modeA == 0 && modeB == 1)
	{
		Paint_DrawString_EN (250, 50, "A3",        &Font24,    MAGENTA,  WHITE);
	}
	else if(modeA == 1 && modeB == 1)
	{
		Paint_DrawString_EN (250, 50, "M1",        &Font24,    MAGENTA,  WHITE);
	}
	Paint_DrawString_EN (5, 100, "Vitesse G : ",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (5, 125, "Vitesse D : ",        &Font24,    MAGENTA,  WHITE);
	Paint_DrawString_EN (10, 220, "Attention aux enfants",        &Font20,    YELLOW,  RED);

	DEV_Delay_ms(3000);

	*a = 1;
	//DEV_Module_Exit();

}
