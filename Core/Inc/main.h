/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include "auto.h"
#include "gestion_moteurs.h"
#include "manette.h"
#include "vitesse_affichage.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Blue_Button_Pin GPIO_PIN_13
#define Blue_Button_GPIO_Port GPIOC
#define Blue_Button_EXTI_IRQn EXTI15_10_IRQn
#define Dipswitch_MSB_Pin GPIO_PIN_2
#define Dipswitch_MSB_GPIO_Port GPIOC
#define Dipswitch_LSB_Pin GPIO_PIN_3
#define Dipswitch_LSB_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define Gauche_av_Pin GPIO_PIN_6
#define Gauche_av_GPIO_Port GPIOA
#define Gauche_re_Pin GPIO_PIN_7
#define Gauche_re_GPIO_Port GPIOA
#define Droit_av_Pin GPIO_PIN_0
#define Droit_av_GPIO_Port GPIOB
#define Droit_re_Pin GPIO_PIN_1
#define Droit_re_GPIO_Port GPIOB
#define SPI_RST_Pin GPIO_PIN_13
#define SPI_RST_GPIO_Port GPIOB
#define SPI_CS_Pin GPIO_PIN_14
#define SPI_CS_GPIO_Port GPIOB
#define SPI_DC_Pin GPIO_PIN_15
#define SPI_DC_GPIO_Port GPIOB
#define Encodeur_D_B_Pin GPIO_PIN_7
#define Encodeur_D_B_GPIO_Port GPIOC
#define Encodeur_G_A_Pin GPIO_PIN_8
#define Encodeur_G_A_GPIO_Port GPIOA
#define Encodeur_G_A_EXTI_IRQn EXTI9_5_IRQn
#define Encodeur_D_A_Pin GPIO_PIN_9
#define Encodeur_D_A_GPIO_Port GPIOA
#define Encodeur_D_A_EXTI_IRQn EXTI9_5_IRQn
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define Encodeur_G_B_Pin GPIO_PIN_6
#define Encodeur_G_B_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_7
#define LCD_BL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
