/*
 * Motors.c
 *
 *  Created on: Aug 28, 2024
 *      Author: ES670 B
 */

#include "motors.h"
#include "main.h"
#include "tim.h"

TIM_HandleTypeDef* pTimPWM;
unsigned int timer;
unsigned int timerMax;
unsigned int contar;
char motor;

void vMotorsInit() {
	pTimPWM = &htim1;
	HAL_TIM_PWM_Start(pTimPWM, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(pTimPWM, TIM_CHANNEL_2);
	//pTimPWM->Instance->CCR1 = 0;
}

//char: motor da direita ou esquerda
//float: PWM de 0 a 1
void vMotorsSetPWM(char motor, float PWM, char rotation) {
	if (motor == left) {
		pTimPWM->Instance->CCR1 = (PWM*1000) - 1;

		if (rotation) {
			HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 1);
			HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 0);
		} else if (!rotation) {
			HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 0);
			HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 1);
		}
	} else if (motor == right) {
		pTimPWM->Instance->CCR2 = (PWM*1000) - 1;

		if (rotation) {
			HAL_GPIO_WritePin(Motor_Dir_IN1_GPIO_Port,Motor_Dir_IN1_Pin, 1);
			HAL_GPIO_WritePin(Motor_Dir_IN2_GPIO_Port,Motor_Dir_IN2_Pin, 0);
		} else if (!rotation) {
			HAL_GPIO_WritePin(Motor_Dir_IN1_GPIO_Port,Motor_Dir_IN1_Pin, 0);
			HAL_GPIO_WritePin(Motor_Dir_IN2_GPIO_Port,Motor_Dir_IN2_Pin, 1);
		}
	}
}

void vMotorsSetVelocity(char motor, float linearVelocity, char rotation){

	float PWM = linearVelocity * 0.12; //exemplo, alterar para o valor medido

	if (motor == left) {
		pTimPWM->Instance->CCR1 = (PWM*1000) - 1;

		if (rotation) {
			HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 1);
			HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 0);
		} else if (!rotation) {
			HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 0);
			HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 1);
		}
	} else if (motor == right) {
		pTimPWM->Instance->CCR2 = (PWM*1000) - 1;

		if (rotation) {
			HAL_GPIO_WritePin(Motor_Dir_IN1_GPIO_Port,Motor_Dir_IN1_Pin, 1);
			HAL_GPIO_WritePin(Motor_Dir_IN2_GPIO_Port,Motor_Dir_IN2_Pin, 0);
		} else if (!rotation) {
			HAL_GPIO_WritePin(Motor_Dir_IN1_GPIO_Port,Motor_Dir_IN1_Pin, 0);
			HAL_GPIO_WritePin(Motor_Dir_IN2_GPIO_Port,Motor_Dir_IN2_Pin, 1);
		}
	}
}

void vMotorsSetPWMTimer(char motor, float PWM, char rotation, unsigned int duration) {
	timerMax = duration*2;
	if (motor == left) {
		pTimPWM->Instance->CCR1 = (PWM*1000) - 1;

		if (rotation) {
			HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 1);
			HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 0);
		} else if (!rotation) {
			HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 0);
			HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 1);
		}
	} else if (motor == right) {
		pTimPWM->Instance->CCR2 = (PWM*1000) - 1;

		if (rotation) {
			HAL_GPIO_WritePin(Motor_Dir_IN1_GPIO_Port,Motor_Dir_IN1_Pin, 1);
			HAL_GPIO_WritePin(Motor_Dir_IN2_GPIO_Port,Motor_Dir_IN2_Pin, 0);
		} else if (!rotation) {
			HAL_GPIO_WritePin(Motor_Dir_IN1_GPIO_Port,Motor_Dir_IN1_Pin, 0);
			HAL_GPIO_WritePin(Motor_Dir_IN2_GPIO_Port,Motor_Dir_IN2_Pin, 1);
		}
	}

	//inicia timer
	timer = 0;
	contar = 1;
	motor = motor;

	//finaliza timer
	//vMotorsSetOff(motor);
}

void vMotorsSetOff(char motor) {
	if (motor == left) {
		pTimPWM->Instance->CCR1 = 0;

		HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 0);
		HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 0);

	} else if (motor == right) {
		pTimPWM->Instance->CCR2 = 0;

		HAL_GPIO_WritePin(Motor_Dir_IN1_GPIO_Port,Motor_Dir_IN1_Pin, 0);
		HAL_GPIO_WritePin(Motor_Dir_IN2_GPIO_Port,Motor_Dir_IN2_Pin, 0);

	}
}


void vUpdateTimerPWM(){
	if (contar == 1)
	{
		timer += 1;
		if (timer == timerMax)
		{
			vMotorsSetOff(motor);
			contar = 0;
		}
	}
}

  //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  //(&htim1)->Instance->CCR1 = 1000-1;
  //HAL_GPIO_WritePin(Motor_Esq_PWM_GPIO_Port,Motor_Esq_PWM_Pin, 1);
  //HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 1);
  //HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 0);
