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

#ifndef INC_ULTRASSONICO_H_
#define INC_ULTRASSONICO_H_
#include "main.h"
typedef struct
{
  TIM_HandleTypeDef* pTimerEcoUltrassonicoFrontal;
  TIM_HandleTypeDef* TimerTrigger;
  float Distance;
}UltrassonicoData;



void vUltrassonicoInit(TIM_HandleTypeDef* pTimerEcoUltrassonicoFrontal ,TIM_HandleTypeDef* TimerTrigger);

float fUltrassonicoGetDistance(uint16_t uiAuxDistanceUltrassonicoFrontal1,uint16_t uiAuxDistanceUltrassonicoFrontal2);
#endif /* INC_ULTRASSONICO_H_ */
