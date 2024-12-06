/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "vitesse_affichage.h"
#include "image.h"
#include "LCD_Test.h"
#include "LCD_2inch.h"
#include "DEV_Config.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PULSE_PAR_TOUR 74
#define CIRCONFERENCE 157 // en mm
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint16_t arrTimerVitesse = 199, nbPulseD = 0, nbPulseG = 0;
uint16_t moyNbPulseG = 0, moyNbPulseD = 0;
volatile uint16_t vitesseD = 0, vitesseG = 0; //en mm par seconde
uint8_t LCD_Init_OK = 0, delais_LCD = 0;
int encod_D = 0, encod_G = 0;

int timeRefresh = 0;
int vitesseRefresh = 0;

//GPIO_PinState Encod_B_D;
//GPIO_PinState Encod_B_G;

int directionD = 0;
int directionG = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  Initialisation_manette();

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  MX_TIM3_Init();
  MX_TIM7_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  MX_RTC_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim10);

  if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK)
  {
      Error_Handler();
  }

  if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2) != HAL_OK)
  {
      Error_Handler();
  }

  if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK)
  {
      Error_Handler();
  }

  if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4) != HAL_OK)
  {
      Error_Handler();
  }
  arrTimerVitesse = __HAL_TIM_GET_AUTORELOAD(&htim6);
  curr_mode = MANUAL_MODE;
  curr_step = 1;
  if(DEBUG_MODE) {
	  CLOCKWISE_FACTOR = STARTING_VALUE;
	  COUNTER_CLW_FACTOR = STARTING_VALUE;
	  RATIO = STARTING_VALUE;
  } else {
	  RATIO = 0.61;
	  CLOCKWISE_FACTOR = 56.75;
	  COUNTER_CLW_FACTOR = 0;
  }
  LCD_Init(&LCD_Init_OK);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	if(timeRefresh && curr_mode == MANUAL_MODE){
	refresh_RTC_Time();
	timeRefresh = 0;
	}

	if(DEBUG_MODE)							// Find constant values
		  Constant_Tuning_Mode();
	else if(curr_mode != MANUAL_MODE) { 	// Toggle auto mode
		switch(curr_mode) {
			case CIRCLE_MODE:
				Auto_Circle();
				break;
			case BACK_FORTH_MODE:
				Auto_Back_Forth();
				break;
			case SQUARE_MODE:
				Auto_Square();
				break;
		}
	}
	else { 	// Manual mode
		Controller();
	}
	if(vitesseRefresh >= 3 && curr_mode == MANUAL_MODE){
		LCD_Vitesse(directionD, directionG);
		vitesseRefresh = 0;
	}
  }
}

  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	if(GPIO_Pin == GPIO_PIN_9)
	{
		nbPulseD++ ; // compte les pulses de lencodeur droit
    	//Encod_B_D = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7);
	}

	if(GPIO_Pin == GPIO_PIN_8)
	{
		nbPulseG++ ; // compte les pulses de lencodeur droit
    	//Encod_B_G = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6);
	}


	if(GPIO_Pin == Blue_Button_Pin) {
		if(DEBUG_MODE && curr_mode == MANUAL_MODE) {
			CLOCKWISE_FACTOR += STEP_VALUE;
			COUNTER_CLW_FACTOR += STEP_VALUE;
			RATIO += STEP_VALUE;
			curr_mode = DEBUG_MODE;
			char buffer[8];
			sprintf(buffer, "%.4f", CLOCKWISE_FACTOR);
			Paint_DrawString_EN (120, 150, buffer, &Font16, MAGENTA, WHITE);
			return;
		}

		// Resume if auto mode paused
		if(pause)
			Resume();

		// Pause auto mode if button pressed while auto mode on
		else if(curr_mode != MANUAL_MODE)
			Pause();

		// Change current mode only on manual mode
		if(curr_mode == MANUAL_MODE) {
			dip_state = Get_Mode(HAL_GPIO_ReadPin(Dipswitch_MSB_GPIO_Port, Dipswitch_MSB_Pin), HAL_GPIO_ReadPin(Dipswitch_LSB_GPIO_Port, Dipswitch_LSB_Pin));
			curr_mode = dip_state;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {

    	moyPulse(nbPulseD, nbPulseG, &moyNbPulseD, &moyNbPulseG);

    	vitesseD = CIRCONFERENCE * moyNbPulseD / PULSE_PAR_TOUR * 2000 / (arrTimerVitesse + 1) ; // calcule la vitesse de la chenille droite en m/s
    	vitesseG = CIRCONFERENCE * moyNbPulseG / PULSE_PAR_TOUR * 2000 / (arrTimerVitesse + 1) ;
    	nbPulseD = 0;
    	nbPulseG = 0;
    	vitesseRefresh++;

    	   /* if (Encod_B_D == GPIO_PIN_SET)
    	    {
    	        directionD = 1;
    	    }
    	    else
    	    {
    	    	directionD = 0;
    	    }

    	    if (Encod_B_G == GPIO_PIN_SET)
    	    {
    	        directionG = 1;
    	    }
    	    else
    	    {
    	    	directionG = 0;
    	    }*/
    	}



    if(htim->Instance == TIM7) {	// Triggered every 10 µs
    	timer_count += 10;
    }
    if(htim->Instance == TIM10){
    	timeRefresh = 1;
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
