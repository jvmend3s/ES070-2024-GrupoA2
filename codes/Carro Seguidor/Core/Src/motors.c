//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//----------------------------------------------File Name----------------------------------------------//
//	motors.c																						   //
//----------------------------------------------Desciption---------------------------------------------//
//																									   //
//	This file implements the motors using PWM														   //
//	The functions implemented on this file are:														   //
//	-	vMotorsInit																					   //
//  -	vMotorsSetPWM																				   //
//  -	vMotorsSetVelocity																			   //
//  -	vMotorsSetPWMTimer																			   //
//  -	vMotorsSetOff																		 		   //
//  -	vMotorsDurationCallback																		   //
//																									   //
//-----------------------------------------------Author------------------------------------------------//
//	Gustavo Wyatt Varga																				   //
//	Maria Clara 																					   //
//	João Vitor Mendes																				   //
//-----------------------------------------------Version-----------------------------------------------//
//	Created on :									07/09/24										   //
//	Revised on :									12/09/24										   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
#include "motors.h"
#include "main.h"
#include "tim.h"

TIM_HandleTypeDef* pTimPWM;
TIM_HandleTypeDef* pTimDuration;
unsigned char ucDurationCounter;
unsigned short int usTimeOn;
extern float fSetPoint_left; //e
extern float fSetPoint_right; //f

void vMotorsInit(TIM_HandleTypeDef* htimPWM, TIM_HandleTypeDef* htimTimer) {
	pTimPWM = htimPWM;
	pTimDuration =htimPWM;
	HAL_TIM_PWM_Start(pTimPWM, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(pTimPWM, TIM_CHANNEL_2);
	//pTimPWM->Instance->CCR1 = 0;
	HAL_NVIC_DisableIRQ(TIM5_IRQn);

	ucDurationCounter = 0;
	usTimeOn =0;

}

//char: motor da direita ou esquerda
//float: PWM de 0 a 1
void vMotorsSetPWM(char motor, float PWM, char rotation) {
	if (motor == left) {
		pTimPWM->Instance->CCR2 = (PWM*1000) - 1;

		if (rotation) {
			HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 1);
			HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 0);
		} else if (!rotation) {
			HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 0);
			HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 1);
		}
	} else if (motor == right) {
		pTimPWM->Instance->CCR1 = (PWM*1000) - 1;

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
		pTimPWM->Instance->CCR2 = (PWM*1000) - 1;

		if (rotation) {
			HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 1);
			HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 0);
		} else if (!rotation) {
			HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 0);
			HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 1);
		}
	} else if (motor == right) {
		pTimPWM->Instance->CCR1 = (PWM*1000) - 1;

		if (rotation) {
			HAL_GPIO_WritePin(Motor_Dir_IN1_GPIO_Port,Motor_Dir_IN1_Pin, 1);
			HAL_GPIO_WritePin(Motor_Dir_IN2_GPIO_Port,Motor_Dir_IN2_Pin, 0);
		} else if (!rotation) {
			HAL_GPIO_WritePin(Motor_Dir_IN1_GPIO_Port,Motor_Dir_IN1_Pin, 0);
			HAL_GPIO_WritePin(Motor_Dir_IN2_GPIO_Port,Motor_Dir_IN2_Pin, 1);
		}
	}
}
//	Function name	: 	vMotorsSetPWMTimer 															   //
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	This function set one motor to run based on PWM, and a rotation direction for  //
//						a pre-set time, it enable the interrupion that after some cycles defined by    //
//						duration input will stop the motor in vMotorsDurationCallback function         //
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	char motor, float PWM, char rotation, unsigned int duration					   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vMotorsSetPWMTimer(char motor, float PWM, char rotation, unsigned int duration) {
	if (motor == left) {
//		pTimPWM->Instance->CCR2 = (PWM*1000) - 1;

		if (rotation) {
			HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 1);
			HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 0);
		} else if (!rotation) {
			HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 0);
			HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 1);
		}

	} else if (motor == right) {
//		pTimPWM->Instance->CCR1 = (PWM*1000) - 1;

		if (rotation) {
			HAL_GPIO_WritePin(Motor_Dir_IN1_GPIO_Port,Motor_Dir_IN1_Pin, 1);
			HAL_GPIO_WritePin(Motor_Dir_IN2_GPIO_Port,Motor_Dir_IN2_Pin, 0);
		} else if (!rotation) {
			HAL_GPIO_WritePin(Motor_Dir_IN1_GPIO_Port,Motor_Dir_IN1_Pin, 0);
			HAL_GPIO_WritePin(Motor_Dir_IN2_GPIO_Port,Motor_Dir_IN2_Pin, 1);
		}
	}
	usTimeOn = duration;
	ucDurationCounter = 0;
	HAL_NVIC_EnableIRQ(TIM5_IRQn);

//	vMotorsSetOff(motor);
}

void vMotorsSetOff(char motor) {
	if (motor == left) {
		pTimPWM->Instance->CCR2 = 0;

		HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 0);
		HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 0);

	} else if (motor == right) {
		pTimPWM->Instance->CCR1 = 0;

		HAL_GPIO_WritePin(Motor_Dir_IN1_GPIO_Port,Motor_Dir_IN1_Pin, 0);
		HAL_GPIO_WritePin(Motor_Dir_IN2_GPIO_Port,Motor_Dir_IN2_Pin, 0);

	}
}

//	Function name	: 	vMotorsDurationCallback 															   //
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	This function deals with Motors running duration callback, it will counts how  //
//						many interruptions for stop the motor based on usTimeOn (duration set) then	   //
//						stops the motors.															   //
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	n/a																			   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vMotorsDurationCallback(){
	if((ucDurationCounter*10)==usTimeOn){
		fSetPoint_left = 0;
		fSetPoint_right = 0;
		vMotorsSetOff(left);
		vMotorsSetOff(right);
		usTimeOn = 0;
		ucDurationCounter = 0;
		HAL_NVIC_DisableIRQ(TIM5_IRQn);
	}
	else
	{
		ucDurationCounter++;
	}
}



  //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  //(&htim1)->Instance->CCR1 = 1000-1;
  //HAL_GPIO_WritePin(Motor_Esq_PWM_GPIO_Port,Motor_Esq_PWM_Pin, 1);
  //HAL_GPIO_WritePin(Motor_Esq_IN3_GPIO_Port,Motor_Esq_IN3_Pin, 1);
  //HAL_GPIO_WritePin(Motor_Esq_IN4_GPIO_Port,Motor_Esq_IN4_Pin, 0);
