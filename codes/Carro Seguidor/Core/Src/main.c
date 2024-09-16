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
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <string.h>
#include "ultrassonico.h"
#include "lcd_hd44780_i2c.h"
#include "communication.h"
#include "motors.h"
#include "led.h"
#include "buzzer.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{ Bit_RESET = 0,
  Bit_SET
}BitAction;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//    Timer & Counters

TIM_HandleTypeDef* pTimerPWMTrigger = &htim20;
TIM_HandleTypeDef* pTimerEcoUltrassonicoFrontal = &htim3;

TIM_HandleTypeDef* pTimerBuzzer = &htim5;
TIM_HandleTypeDef* pTimerPWMBuzzer = &htim8;

//   Ints
//flags
char CountMode=0;
char cBuzzerState=0;

uint16_t uiAuxDistanceUltrassonicoFrontal1=0;
uint16_t uiAuxDistanceUltrassonicoFrontal2=0;

uint16_t uiAuxDistanceUltrassonicoEsquerdo1=0;
uint16_t uiAuxDistanceUltrassonicoEsquerdo2=0;
//Floats

float fDistanceFrontal=0;
float fDistanceEsquerdo=0;

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
	unsigned char ucLCD0Msg[17], ucLCD1Msg[17];

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LPUART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM20_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  lcdInit(&hi2c2, (uint8_t)0x27, (uint8_t)2, (uint8_t)16);

  vUltrassonicoInit(pTimerEcoUltrassonicoFrontal,pTimerPWMTrigger) ;
  vCommunicationInit();

  vMotorsInit();

  vBuzzerConfig(100, 100, pTimerPWMBuzzer, pTimerBuzzer);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  lcdSetCursorPosition(0, 0);
  sprintf((char *)ucLCD0Msg, "Dist (cm)");
  lcdPrintStr((uint8_t*)ucLCD0Msg, strlen((char *)ucLCD0Msg));


  lcdSetCursorPosition(0,1);
  sprintf((char *)ucLCD1Msg, "Sensor Frontal");
  lcdPrintStr((uint8_t*)ucLCD1Msg, strlen((char *)ucLCD1Msg));




  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_Delay(500);
	  lcdSetCursorPosition(10, 0);
	  lcdPrintStr((uint8_t*)pCommunicationFloatToString(fDistanceFrontal, 2), strlen((char *)pCommunicationFloatToString(fDistanceFrontal, 2)));



	  	  if(fDistanceFrontal<4.0){
		  while(fDistanceFrontal<4.0){
		  //vMotorsSetPWM(left, 0.6, 0);
		  //vMotorsSetPWM(right, 0.6, 0);
		  vLedWriteLed(0b001);
		  //vBuzzerPlay();
		  //cBuzzerState=1;
		  }
	  }

	  else if(20.0>fDistanceFrontal && fDistanceFrontal >4.0){

		  //vMotorsSetOff(left);
		  //vMotorsSetOff(right);
		  vLedWriteLed(0b100);
	  }
	  else if(20.0<fDistanceFrontal){
		  //vMotorsSetPWM(left, 0.6, 1);
		  //vMotorsSetPWM(right, 0.6, 1);
		  vLedWriteLed(0b010);
	  }

  }
  /* USER CODE END 3 */
}

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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef * htim){

  if(0==CountMode){
	  uiAuxDistanceUltrassonicoFrontal1= HAL_TIM_ReadCapturedValue(pTimerEcoUltrassonicoFrontal,TIM_CHANNEL_1);
	  uiAuxDistanceUltrassonicoEsquerdo1=HAL_TIM_ReadCapturedValue(pTimerEcoUltrassonicoFrontal,TIM_CHANNEL_2);
	  CountMode=1;
  }else{
	  uiAuxDistanceUltrassonicoFrontal2 = HAL_TIM_ReadCapturedValue(pTimerEcoUltrassonicoFrontal,TIM_CHANNEL_1);
	  uiAuxDistanceUltrassonicoEsquerdo2=HAL_TIM_ReadCapturedValue(pTimerEcoUltrassonicoFrontal,TIM_CHANNEL_2);

	  fDistanceFrontal=fUltrassonicoGetDistance( uiAuxDistanceUltrassonicoFrontal1,uiAuxDistanceUltrassonicoFrontal2);
	  fDistanceEsquerdo=fUltrassonicoGetDistance( uiAuxDistanceUltrassonicoEsquerdo1 , uiAuxDistanceUltrassonicoEsquerdo2);
	  CountMode=0;
  }
  if(fDistanceFrontal<0){
	  fDistanceFrontal=fDistanceFrontal*(-1);
  }
  if(fDistanceEsquerdo<0)
	  fDistanceEsquerdo=fDistanceEsquerdo*(-1);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin==GPIO_PIN_7){
	vMotorsSetPWM(left, 1, 1);
	vMotorsSetPWM(right, 1, 0);
	}
	if(GPIO_Pin==GPIO_PIN_5){
	vMotorsSetOff(left);
	vMotorsSetOff(right);
	}


}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim){
	  //Fim do timer do Buzzer = Buzzer->OFF
	  if (htim == pTimerBuzzer)
		  vBuzzerStop();
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
