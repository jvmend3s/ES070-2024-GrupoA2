//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//
//----------------------------------------------File Name----------------------------------------------//
//
//	communication.h
//
//----------------------------------------------Desciption---------------------------------------------//
//
//	This file shows the headers of all functions implemented on CommunicationStateMachine.c, each state
// shows up as the iddle state, while nothings happens, and the functional form, using some states to
// interact with any kinds of inputs that can be used in the heater controller system
//-----------------------------------------------Author------------------------------------------------//
//
//	João Vitor Mendes
//	Vitor Akio Isawa
//
//-----------------------------------------------Version-----------------------------------------------//
//
//	Created on :									29/04/24
//	Revised on :									03/05/24
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


//-----------------------------------------------Defines-----------------------------------------------//
#define _MAX_NUMBER_OF_CHARS 100
//-----------------------------------------------Includes----------------------------------------------//
#include "main.h"
#include <strings.h>
#include <stdio.h>


//----------------------------------------------Functions----------------------------------------------//
void vCommStateMachineInit(UART_HandleTypeDef *huartcontrol, UART_HandleTypeDef *huartV2V);
void vCommStateMachineSetParam(unsigned char ucParam, unsigned char * pValue);
void vCommStateMachineReturnParam(unsigned char ucParam);

//-----------------------------------------------------------------------------------------------------//
void vCommStateMachineFloatToString(float fInputNumber, int iDigits, unsigned char * ucOutputNumber);

float fCommStateMachineStringToFloat();
void vCommStateMachineSendMessage (UART_HandleTypeDef * huart, unsigned char* ucMessage);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
