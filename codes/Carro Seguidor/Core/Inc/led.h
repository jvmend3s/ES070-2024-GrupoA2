// ********************************************** //
// File name: led.H
// File description:
//   Define as funções de configuração dos LEDs
// utilizando apenas o módulo GPIO da HAL
//
// Author name: Guilherme Polewacz
// Creation date: 16/09/24
// Revision date: 16/09/24
//
// ********************************************** //


#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"
enum leds
{
 red = 0,
 blue,
 green,

};

void vLedWriteLed(char);

#endif /* INC_LED_H_ */
