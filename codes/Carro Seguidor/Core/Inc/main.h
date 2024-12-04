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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <bluetooth.h>
#include <bleCmd.h>
#include <led.h>
#include <buzzer.h>
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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define Motor_Esq_PWM_Pin GPIO_PIN_0
#define Motor_Esq_PWM_GPIO_Port GPIOC
#define Motor_Dir_PWM_Pin GPIO_PIN_1
#define Motor_Dir_PWM_GPIO_Port GPIOC
#define LPUART1_TX_Pin GPIO_PIN_2
#define LPUART1_TX_GPIO_Port GPIOA
#define LPUART1_RX_Pin GPIO_PIN_3
#define LPUART1_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define Motor_Esq_IN3_Pin GPIO_PIN_12
#define Motor_Esq_IN3_GPIO_Port GPIOB
#define BT_Baixo_Pin GPIO_PIN_7
#define BT_Baixo_GPIO_Port GPIOC
#define Motor_Dir_IN2_Pin GPIO_PIN_10
#define Motor_Dir_IN2_GPIO_Port GPIOA
#define RED_LED_Pin GPIO_PIN_11
#define RED_LED_GPIO_Port GPIOA
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define T_SWO_Pin GPIO_PIN_3
#define T_SWO_GPIO_Port GPIOB
#define Motor_Dir_IN1_Pin GPIO_PIN_7
#define Motor_Dir_IN1_GPIO_Port GPIOB
#define Motor_Esq_IN4_Pin GPIO_PIN_9
#define Motor_Esq_IN4_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
enum Motor {
	left = 0,
	right
};

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
