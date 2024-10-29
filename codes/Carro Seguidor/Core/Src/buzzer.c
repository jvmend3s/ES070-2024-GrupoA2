// ********************************************** //
// FILE NAME: buzzer.h
// FILE DESCRIPTION:
//  Codifica as funcoes de utilizacao do buzzer,
// utilizando PWM
//
// AUTHOR NAME: Guilherme Polewacz, Gustavo Wyatt
// CREATION DATE: 15/05/24
// REVISION DATE: 16/05/24
//
// ********************************************** //

#include "main.h"
#include "buzzer.h"

unsigned short int usBuzzerFreq, usBuzzerPeriod;
TIM_HandleTypeDef* pTimPWMBuzzer;
TIM_HandleTypeDef* pTimBuzzer;


// ********************************************** //
// METHOD NAME: vBuzzerConfig
// METHOD DESCRIPTION:
//  Inicia as variaveis da classe
//
// INPUT PARAMS: usFrequency - Frequencia de operacao
//				 usPeriod - Periodo de operacao
//				 htim - ponteiro para o timer
// referente ao PWM do buzzer
//
// OUTPUT PARAMS: n/a
// ********************************************** //
void vBuzzerConfig(unsigned short int usFrequency, unsigned short int usPeriod, TIM_HandleTypeDef* htimPWM, TIM_HandleTypeDef* htimer) {

  usBuzzerFreq = 0;
  usBuzzerPeriod = 0;

  if (usFrequency >= 100 & usFrequency <= 10000)
	  usBuzzerFreq = usFrequency;

  if (usPeriod >= 1 & usPeriod <= 10000)
	  usBuzzerPeriod = usPeriod;

  pTimPWMBuzzer = htimPWM;
  pTimPWMBuzzer->Instance->ARR = 1000000/usBuzzerFreq;
  pTimPWMBuzzer->Instance->CCR1 = 500000/usBuzzerFreq;

  pTimBuzzer = htimer;
  pTimBuzzer->Instance->ARR = usBuzzerPeriod*100;
}


// ********************************************** //
// METHOD NAME: vBuzzerPlay
// METHOD DESCRIPTION: Liga o buzzer na frequencia
// e duracao definidas no config
//
// INPUT PARAMS: n/a
//
// OUTPUT PARAMS: n/a
// ********************************************** //
void vBuzzerPlay() {
	HAL_TIM_PWM_Start(pTimPWMBuzzer, TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(pTimBuzzer);
	pTimBuzzer->Instance->CNT = 0;
}

void vBuzzerStop() {
	HAL_TIM_PWM_Stop(pTimPWMBuzzer, TIM_CHANNEL_1);
	HAL_TIM_Base_Stop_IT(pTimBuzzer);
}
