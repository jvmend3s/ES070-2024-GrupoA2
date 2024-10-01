// ********************************************** //
// File name: led.c
// File description:
//   Define as funções de configuração dos LEDs
// utilizando apenas o módulo GPIO da HAL
//
// Author name: Guilherme Polewacz
// Creation date: 16/09/24
// Revision date: 16/09/24
//
// ********************************************** //

#include "led.h"

// ********************************************** //
// Method name: vLedWriteLed
// Method description:
//
// Input params:

// Output params: n/a
// ********************************************** //
void vLedWriteLed(char cState){
	int aux=0;
	while(aux<=2){
		switch(aux){
		case 0:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11,(cState>> 0) & 1 );
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,(cState>> 1) & 1  );
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12,(cState>> 2) & 1 );
			break;
		}
		aux++;
	}

}
