// ********************************************** //
// FILE NAME: communication.h
// FILE DESCRIPTION:
//  Implementacao da comunicacao UART com o
// computador.
// COM3 - bauld rate = 115200
//
// AUTHOR NAME: Guilherme Polewacz, Gustavo Wyatt
// CREATION DATE: 23/04/24
// REVISION DATE: /04/24
//
// ********************************************** //
#ifndef INC_COMMUNICATION_H_
#define INC_COMMUNICATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "usart.h"
#include "string.h"

extern unsigned char ucInputChar;


void vCommunicationInit(UART_HandleTypeDef * huartController);
void vCommunicationClearGlobalVariables();
float vCommunicationStringToFloat(char*);
char* pCommunicationFloatToString(float, int);
void vCommunicationSendString(char*);

#endif /* INC_COMMUNICATION_H_ */
