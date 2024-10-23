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
#include <math.h>
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
#include "pid.h"
#include "pid_direita.h"

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

TIM_HandleTypeDef * pTimDurationMotor = &htim5;
TIM_HandleTypeDef * pTimPWMMotor = &htim1;

//Timer de uso geral - conta 10ms
TIM_HandleTypeDef* pTimerGeral = &htim2;

// encoders
TIM_HandleTypeDef* pTimFreqFixa_esq = &htim16;
TIM_HandleTypeDef* pTimFreqFixa_dir = &htim17;
uint32_t uiLeftCapturedTime, uiRightCapturedTime;
unsigned int uiLeftTimeBurst, uiRightTimeBurst;
unsigned long long int ullLeftTimeRan, ullRightTimeRan;
float fLeftSpeed, fRightSpeed, fSpeedMedia, fRightMedia, fLeftMedia,fRightMotorPWM,fLeftMotorPWM;


//   Ints
//flags
char CountModeF=0;
char CountModeE=0;
char CountModeD=0;
char cBuzzerState=0;

uint16_t uiAuxDistanceUltrassonicoFrontal1=0;
uint16_t uiAuxDistanceUltrassonicoFrontal2=0;

uint16_t uiAuxDistanceUltrassonicoEsquerdo1=0;
uint16_t uiAuxDistanceUltrassonicoEsquerdo2=0;

uint16_t uiAuxDistanceUltrassonicoDireito1=0;
uint16_t uiAuxDistanceUltrassonicoDireito2=0;
//Floats

float fDistanceFrontal,fEsq,Fdir=0;
float fDistanceEsquerdo=0;
float fDistanceDireito=0;
float dif;
float fVeloEsqSetPoint, fVeloDirSetPoint;


//Controller PID
float fActuatorValue,fActuatorValue_old = 0.0;
int iConta100 = 0;
float fSetPoint = 400;
float fMediaVelEsq;
float fPWMReading;

float fActuatorValue_dir, fActuatorValue_dir_old = 0.0;
float fMediaVelDir;

// fazer classe encoder e encapsular tudo
float fVetorVelocidadesLeft[20];
float fVetorVelocidadesRight[20];
int iIndexVeloLeft = 0, iIndexVeloRight = 0;
float fVetorVelocidadesLeft_filtrado[20];
float fVetorVelocidadesRight_filtrado[20];
int iContaMediasEsq , iContaMediasDir;
float fMediaDirParaVer,fMediaEsqParaVer;
float fVeloEsqSet,fVeloDirSet;
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
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */



  //lcdInit(&hi2c2, (uint8_t)0x27, (uint8_t)2, (uint8_t)16);

  vUltrassonicoInit(pTimerEcoUltrassonicoFrontal,pTimerPWMTrigger) ;
  vCommunicationInit();

  vMotorsInit(pTimPWMMotor, pTimDurationMotor);
  vPidInit(0.002, 0.06 ,0.0, 100, 1);
  vPidInit_direita(0.002, 0.06, 0.0, 100, 1);


  vBuzzerConfig(100, 100, pTimerPWMBuzzer, pTimerBuzzer);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  HAL_TIM_Base_Start_IT(pTimDurationMotor);
  HAL_TIM_Base_Start_IT(pTimFreqFixa_esq);
  HAL_TIM_Base_Start_IT(pTimFreqFixa_dir);
  HAL_TIM_Base_Start_IT(pTimerGeral);
  HAL_TIM_IC_Start_IT(pTimFreqFixa_esq, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(pTimFreqFixa_dir, TIM_CHANNEL_1);



  clear_velocidades();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	  HAL_Delay(500);
//
//	  lcdSetCursorPosition(8,0);
//	  sprintf((char *)ucLCD1Msg, "D:");
//	  lcdPrintStr((uint8_t*)ucLCD1Msg, strlen((char *)ucLCD1Msg));
//
//	  lcdSetCursorPosition(0,1);
//	  sprintf((char *)ucLCD1Msg, "E:");
//	  lcdPrintStr((uint8_t*)ucLCD1Msg, strlen((char *)ucLCD1Msg));
//
//	  lcdSetCursorPosition(2, 0);
//
//	  lcdPrintStr((uint8_t*)pCommunicationFloatToString(fDistanceFrontal, 1), strlen((char *)pCommunicationFloatToString(fDistanceFrontal, 1)));
//
//	  lcdSetCursorPosition(10, 0);
//	  lcdPrintStr((uint8_t*)pCommunicationFloatToString(fDistanceDireito, 1), strlen((char *)pCommunicationFloatToString(fDistanceDireito, 1)));
//
//	  lcdSetCursorPosition(3, 1);
//	  lcdPrintStr((uint8_t*)pCommunicationFloatToString(fDistanceEsquerdo, 1), strlen((char *)pCommunicationFloatToString(fDistanceEsquerdo, 1)));
  	  if(fDistanceEsquerdo<0)
  		fDistanceEsquerdo=fDistanceEsquerdo*-1.0;
  	  if(fDistanceDireito<0)
  		fDistanceDireito=fDistanceDireito*-1.0;
  	  if(fDistanceFrontal<0)
  		fDistanceFrontal=fDistanceFrontal*-1.0;

  	  dif=fDistanceEsquerdo-fDistanceDireito;


	  if(12.0>fDistanceFrontal || fDistanceEsquerdo<10  || fDistanceDireito<10){
		  fVeloDirSet=0;
		  fVeloEsqSet=0;
		 // vLedWriteLed(0b100);
	  }

	  else if(12.0<fDistanceFrontal  ){


	  	  if(dif<-6.00 || dif>6.00){
	  		  if(dif<-6.00){
	  			  fVeloDirSet=650;
	  			  fVeloEsqSet=400;
	  		  }
	  		  else{
	  			  fVeloDirSet=400;
	  			  fVeloEsqSet=650;
	  		  }
	  	  }
	  	  else{
  			  fVeloDirSet=450;
  			  fVeloEsqSet=450;
	  	  }
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


void clear_velocidades() {
  int i;
  for (i = 0;i<20;i++) {
	  fVetorVelocidadesLeft[i] = 0;
	  fVetorVelocidadesRight[i] = 0;
	  fVetorVelocidadesLeft_filtrado[i] = 0;
	  fVetorVelocidadesRight_filtrado[i] = 0;
  }

  iIndexVeloRight = 0;
  iIndexVeloLeft = 0;
}
void vPeriodicControlTask(){ //pela velocidade
	  fActuatorValue_old=fActuatorValue;
	  fActuatorValue = fPidUpdateData(fMediaVelEsq,fVeloEsqSet);
	  //mapping da entrada PWM

	  fActuatorValue =  0.42+fActuatorValue*0.5;
	  if(12.0>fDistanceFrontal || fDistanceEsquerdo<10  || fDistanceDireito<10)
		  fActuatorValue=0;
	  vMotorsSetPWM(left, fActuatorValue, 1);




	  fActuatorValue_dir_old=fActuatorValue_dir;
	  fActuatorValue_dir = fPidUpdateData_direita(fMediaVelDir, fVeloDirSet); //right speed pq ta trocado left com right
	  //mapping da entrada PWM
	  fActuatorValue_dir = 0.42 + fActuatorValue_dir*0.5;
	  if(12.0>fDistanceFrontal || fDistanceEsquerdo<10  || fDistanceDireito<10)
		  fActuatorValue_dir=0;
	  vMotorsSetPWM(right, fActuatorValue_dir, 1);

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef * htim){
 if(htim == pTimerEcoUltrassonicoFrontal){

  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
      {
	  if(0==CountModeF){
		  uiAuxDistanceUltrassonicoFrontal1= HAL_TIM_ReadCapturedValue(pTimerEcoUltrassonicoFrontal,TIM_CHANNEL_1);
		  CountModeF=1;
	  }else{
		  uiAuxDistanceUltrassonicoFrontal2 = HAL_TIM_ReadCapturedValue(pTimerEcoUltrassonicoFrontal,TIM_CHANNEL_1);
		  fDistanceFrontal=fUltrassonicoGetDistance( uiAuxDistanceUltrassonicoFrontal1,uiAuxDistanceUltrassonicoFrontal2);
		  CountModeF=0;
	  }
      }

      else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
      {
    	  if(0==CountModeE){
    		  uiAuxDistanceUltrassonicoEsquerdo1=HAL_TIM_ReadCapturedValue(pTimerEcoUltrassonicoFrontal,TIM_CHANNEL_2);
    		  CountModeE=1;
    	  }else{
    		  uiAuxDistanceUltrassonicoEsquerdo2=HAL_TIM_ReadCapturedValue(pTimerEcoUltrassonicoFrontal,TIM_CHANNEL_2);
    		  fDistanceEsquerdo=fUltrassonicoGetDistance( uiAuxDistanceUltrassonicoEsquerdo1 , uiAuxDistanceUltrassonicoEsquerdo2);
    		  CountModeE=0;
    	  }

      }
      else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
      {
    	  if(0==CountModeD){
    		  uiAuxDistanceUltrassonicoDireito1=HAL_TIM_ReadCapturedValue(pTimerEcoUltrassonicoFrontal,TIM_CHANNEL_3);
    		  CountModeD=1;
    	  }else{
    		  uiAuxDistanceUltrassonicoDireito2=HAL_TIM_ReadCapturedValue(pTimerEcoUltrassonicoFrontal,TIM_CHANNEL_3);
    		  fDistanceDireito=fUltrassonicoGetDistance( uiAuxDistanceUltrassonicoDireito1 , uiAuxDistanceUltrassonicoDireito2);
    		  CountModeD=0;
    	  }
      }


  if(fDistanceFrontal<0){
	  fDistanceFrontal=fDistanceFrontal*(-1);
  }
  if(fDistanceEsquerdo<0)
	  fDistanceEsquerdo=fDistanceEsquerdo*(-1);

  }
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
  		fVetorVelocidadesLeft[iIndexVeloLeft] = fLeftSpeed;
  		iIndexVeloLeft++;
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
  		fVetorVelocidadesRight[iIndexVeloRight] = fRightSpeed;
  		iIndexVeloRight++;
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




void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim){




	if (htim == pTimerGeral) { //conta 10 ms (acho)

		iConta100++;

		if (fRightSpeed < 700) {
			fMediaVelDir += fRightSpeed;
			iContaMediasDir++;
			fVetorVelocidadesRight_filtrado[iContaMediasDir] = fVetorVelocidadesRight[iIndexVeloRight-1];
		}

		if (fLeftSpeed < 700) {
			fMediaVelEsq += fLeftSpeed;
			iContaMediasEsq++;
			fVetorVelocidadesLeft_filtrado[iContaMediasEsq] = fVetorVelocidadesLeft[iIndexVeloLeft-1];
		}

		if (iConta100 >= 10) {
			fMediaVelEsq /= iContaMediasEsq;
			fMediaVelDir /= iContaMediasDir;
			fMediaDirParaVer = fMediaVelDir;
			fMediaEsqParaVer = fMediaVelEsq;
			vPeriodicControlTask();
			iConta100 = 0;
			fMediaVelEsq = 0;
			fMediaVelDir = 0;
			iContaMediasDir = 0;
			iContaMediasEsq = 0;

			clear_velocidades();
		}



	}

	if (htim == pTimFreqFixa_esq){
		if(uiLeftTimeBurst > 200){
			uiLeftTimeBurst = 1;
			fLeftSpeed = 0;
		}else{
		uiLeftTimeBurst++;
		}
	}

	if (htim == pTimFreqFixa_dir){
		if(uiRightTimeBurst > 200){
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
