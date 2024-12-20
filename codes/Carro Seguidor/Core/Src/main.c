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
#include "timer.h"
#include "pid.h"
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

TIM_HandleTypeDef * pTimerPWMTrigger = &htim20;
TIM_HandleTypeDef * pTimerEcoUltrassonicoFrontal = &htim3;
UART_HandleTypeDef * pBleCtrlMain = &huart1;
UART_HandleTypeDef * pV2V = &huart3;
TIM_HandleTypeDef * pTimDurationMotor = &htim5;
TIM_HandleTypeDef * pTimPWMMotor = &htim1;
//TIM_HandleTypeDef* pTimerPWMBuzzer = &htim8;
TIM_HandleTypeDef* pTimerBuzzer = &htim6;

//Timer de uso geral - conta 10ms
TIM_HandleTypeDef* pTimerGeral = &htim2;

// encoders
TIM_HandleTypeDef* pTimFreqFixa_esq = &htim16;
TIM_HandleTypeDef* pTimFreqFixa_dir = &htim17;
uint32_t uiLeftCapturedTime, uiRightCapturedTime;
unsigned int uiLeftTimeBurst, uiRightTimeBurst;
unsigned long long int ullLeftTimeRan, ullRightTimeRan;
float fLeftSpeed, fRightSpeed, fSpeedMedia, fRightMedia, fLeftMedia;

//flags
char CountModeF=0;
char cBuzzerState=0;

uint16_t uiAuxDistanceUltrassonicoFrontal1=0;
uint16_t uiAuxDistanceUltrassonicoFrontal2=0;
float fDistance=0;

//Controller PID Motors
pid_data_type xPidMotorRight;
pid_data_type xPidMotorLeft;
pid_data_type xPidDistance;
pid_data_type xPidAngle;
float fActuatorValue_left, fActuatorValue_right=0;
int iContaOutlier_left = 0, iContaOutlier_right = 0;

float fLeftMotorKp = 0.0017; // a
float fLeftMotorKi = 0.02; // b
float fRightMotorKp = 0.0017; // c
float fRightMotorKi = 0.02; // d
float fSetPoint_left = 0; //e
float fSetPoint_right = 0; //f


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
  MX_USART3_UART_Init();
  MX_TIM5_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  lcdInit(&hi2c2, (uint8_t)0x27, (uint8_t)2, (uint8_t)16);
  vUltrassonicoInit(pTimerEcoUltrassonicoFrontal,pTimerPWMTrigger) ;
  vCommunicationInit(pBleCtrlMain);

  vMotorsInit(pTimPWMMotor, pTimDurationMotor);
  vCommStateMachineInit(pBleCtrlMain, pV2V);

  vTimerInit();
  iSetTimer(100);

// lider
//  xPidMotorLeft = vPidInit(xPidMotorLeft, 0.005, 0.06, 0, 100, 1);
//  xPidMotorRight = vPidInit(xPidMotorRight, 0.005, 0.06, 0, 100, 1);
// seguidor
   xPidMotorLeft = vPidInit(xPidMotorLeft, fLeftMotorKp, fLeftMotorKi, 0, 100, 0.7);
   xPidMotorRight = vPidInit(xPidMotorRight, fRightMotorKp, fRightMotorKi, 0, 100, 0.7);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  HAL_TIM_Base_Start_IT(pTimDurationMotor);
  HAL_TIM_Base_Start_IT(pTimFreqFixa_esq);
  HAL_TIM_Base_Start_IT(pTimFreqFixa_dir);
  HAL_TIM_Base_Start_IT(pTimerGeral);
  HAL_TIM_IC_Start_IT(pTimFreqFixa_esq, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(pTimFreqFixa_dir, TIM_CHANNEL_1);

  lcdSetCursorPosition(0, 0);
  sprintf((char *)ucLCD0Msg, "Esq(mm/s):");
  lcdPrintStr((uint8_t*)ucLCD0Msg, strlen((char *)ucLCD0Msg));
  lcdSetCursorPosition(0, 1);
  sprintf((char *)ucLCD0Msg, "Dir(mm/s):");
  lcdPrintStr((uint8_t*)ucLCD0Msg, strlen((char *)ucLCD0Msg));

  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
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


void vPeriodicControlTask(){
		//lider
//	  fActuatorValue_left = fPidUpdateData(xPidMotorLeft,fLeftSpeed, 400);
//	  fActuatorValue_left = 0.46 + fActuatorValue_left*0.5;
//	  vMotorsSetPWM(left, fActuatorValue_left, 1);
//
//	  fActuatorValue_right = fPidUpdateData(xPidMotorRight, fRightSpeed, 400);
//	  fActuatorValue_right = 0.46 + fActuatorValue_right*0.5;
//	  vMotorsSetPWM(right, fActuatorValue_right, 1);
	   //seguidor
	  fActuatorValue_left = fPidUpdateData(xPidMotorLeft,fLeftSpeed , abs(fSetPoint_left));
	  fActuatorValue_left = 0.3 + fActuatorValue_left;
	  if (fSetPoint_left == 0)
		  fActuatorValue_left = 0;
	  else if (fSetPoint_left < 0)
		  vMotorsSetPWM(left, fActuatorValue_left, 0);
	  else if (fSetPoint_left>0)
		  vMotorsSetPWM(left, fActuatorValue_left, 1);


	  fActuatorValue_right = fPidUpdateData(xPidMotorRight,fRightSpeed , abs(fSetPoint_right));
	  fActuatorValue_right = 0.3 + fActuatorValue_right;
	  if (fSetPoint_right == 0)
		  fActuatorValue_right = 0;
	  else if (fSetPoint_right < 0)
		  vMotorsSetPWM(right, fActuatorValue_right, 0);
	  else if (fSetPoint_right > 0)
		  vMotorsSetPWM(right, fActuatorValue_right, 1);
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim){
	if (htim == pTimDurationMotor)
	{
	vMotorsDurationCallback();
	}

	//conta 10 ms
	if (htim == pTimerGeral) {

		if (fRightSpeed > 700) {
			iContaOutlier_right++;
		}

		if (fLeftSpeed > 700) {
			iContaOutlier_left++;
		}

		//conta 100 ms
		if (cCountTime(10, 0)) {
			vPeriodicControlTask();
		}

	}

	if (htim == pTimFreqFixa_esq){
		if(uiLeftTimeBurst > 30){
			uiLeftTimeBurst = 1;
			fLeftSpeed = 0;
		}else{
		uiLeftTimeBurst++;
		}
	}

	if (htim == pTimFreqFixa_dir){
		if(uiRightTimeBurst > 30){
			uiRightTimeBurst = 1;
			fRightSpeed = 0;
		}else{
		uiRightTimeBurst++;
		}
	}

	if (htim == pTimerBuzzer){
//		vBuzzerStop();
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef * htim){
//ENCODERS
  if (htim == pTimFreqFixa_esq)
  	{
  		uiLeftCapturedTime =  HAL_TIM_ReadCapturedValue(htim,  TIM_CHANNEL_1);
  		if (uiLeftTimeBurst != 0)
  			ullLeftTimeRan = uiLeftCapturedTime + uiLeftTimeBurst*10000;
  		else
  			ullLeftTimeRan = uiLeftCapturedTime;
  		__HAL_TIM_SET_COUNTER(htim, 0);
  		uiLeftTimeBurst = 0;
  		fLeftSpeed = 10.0f/(ullLeftTimeRan/1000000.0f); //velocidade em mm/s
  	}

  if (htim == pTimFreqFixa_dir)
  	{
  		uiRightCapturedTime =  HAL_TIM_ReadCapturedValue(htim,  TIM_CHANNEL_1);
  		if (uiRightTimeBurst != 0)
  		{
  			ullRightTimeRan = uiRightCapturedTime + uiRightTimeBurst*10000;
  		}
  		else
  		{
  			ullRightTimeRan = uiRightCapturedTime;
  		}
  		__HAL_TIM_SET_COUNTER(htim, 0);
  		uiRightTimeBurst = 0;
  		fRightSpeed = 10.0f/(ullRightTimeRan/1000000.0f); //velocidade em mm/s
  	}
  if(htim == pTimerEcoUltrassonicoFrontal){

	  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  	      {
  		  if(0==CountModeF){
  			  uiAuxDistanceUltrassonicoFrontal1= HAL_TIM_ReadCapturedValue(pTimerEcoUltrassonicoFrontal,TIM_CHANNEL_1);
  			  CountModeF=1;
  		  }else{
  			  uiAuxDistanceUltrassonicoFrontal2 = HAL_TIM_ReadCapturedValue(pTimerEcoUltrassonicoFrontal,TIM_CHANNEL_1);
      		  //if(!(fUltrassonicoGetDistance( uiAuxDistanceUltrassonicoFrontal1 , uiAuxDistanceUltrassonicoFrontal2)-fDistanceFrontal>50 ||fUltrassonicoGetDistance( uiAuxDistanceUltrassonicoFrontal1 , uiAuxDistanceUltrassonicoFrontal2)-fDistanceFrontal<-50) )
  			   fDistance=fUltrassonicoGetDistance( uiAuxDistanceUltrassonicoFrontal1 , uiAuxDistanceUltrassonicoFrontal2);
  			  CountModeF=0;
  		  }
  	 }
  }
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
