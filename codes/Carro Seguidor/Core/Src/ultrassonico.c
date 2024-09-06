// ********************************************** //
// FILE NAME: ultrassonico.c
// FILE DESCRIPTION:
// Testes do ultrassonico
//
// AUTHOR NAME: Guilherme Polewacz
// CREATION DATE: 27/08/24
// REVISION DATE: x/08/24
//
// ********************************************** //

#include "ultrassonico.h"
UltrassonicoData xUltrassonicoData;

// ********************************************** //
// METHOD NAME: vUltrassonicoInit
// METHOD DESCRIPTION: Inicialização dos timer de triger e de leitura input capture dos sensores.
//
// INPUT PARAMS: TIM_HandleTypeDef* pTimerEcoUltrassonicoFrontal ,TIM_HandleTypeDef* TimerTrigger
//
// OUTPUT PARAMS: n/a
// ********************************************** //
void vUltrassonicoInit(TIM_HandleTypeDef* pTimerEcoUltrassonicoFrontal ,TIM_HandleTypeDef* TimerTrigger) {

  xUltrassonicoData.pTimerEcoUltrassonicoFrontal=pTimerEcoUltrassonicoFrontal;
  xUltrassonicoData.TimerTrigger= TimerTrigger;
  xUltrassonicoData.Distance=0.0;

  HAL_TIM_PWM_Start(xUltrassonicoData.TimerTrigger, TIM_CHANNEL_1);
  //Define um pulso de 10us a cada 100ms no trigger, utilizando logica pwm
  xUltrassonicoData.TimerTrigger->Instance->CCR1 = 8;

  HAL_TIM_IC_Start_IT(xUltrassonicoData.pTimerEcoUltrassonicoFrontal, TIM_CHANNEL_1);

}

// ********************************************** //
// METHOD NAME: vUltrassonicoGetDistance
// METHOD DESCRIPTION:
//                   Rotina de calculo de distancia detectada pelo sensor ultrassonico,
//                   por meio dos valores de contagem dos timers no modo input capture
//                   e sua equivalencia com o tempo.
//
// INPUT PARAMS: uint16_t uiAuxDistanceUltrassonicoFrontal1,uint16_t uiAuxDistanceUltrassonicoFrontal2
//
// OUTPUT PARAMS: Distance
// ********************************************** //

float fUltrassonicoGetDistance(uint16_t uiAuxDistanceUltrassonicoFrontal1,uint16_t uiAuxDistanceUltrassonicoFrontal2){
	/*
	 Para calcular a distancia
	 1-Realiza-se a subtracao dos dois valores de contagem do eco
	 2-Multiplica-se esse valor por 1.5, que é o tempo em microsegundos de cada contagem.
	 3-Divide-se esse valor por 58.772 us, que é o tempo que leva para 1 cm refletir a onda no sensor.
	 4-Obtem-se o valor da distancia em cm.
	 */
	xUltrassonicoData.Distance=(uiAuxDistanceUltrassonicoFrontal2-uiAuxDistanceUltrassonicoFrontal1)*1.5/(58.772);
	return xUltrassonicoData.Distance;
}





