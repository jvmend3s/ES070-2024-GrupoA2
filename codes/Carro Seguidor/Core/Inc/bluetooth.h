//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//----------------------------------------------File Name----------------------------------------------//
//	bluetooth.h																						   //
//----------------------------------------------Desciption---------------------------------------------//
//																									   //
//	This file implements the usage of bluetooth and its communication with smartphone				   //
//	The functions implemented on this file are:														   //
//	-	vBluetoothInit																				   //
//  -	HAL_UART_RxCpltCallback																		   //
//																									   //
//-----------------------------------------------Author------------------------------------------------//
//	João Vitor Mendes																				   //
//-----------------------------------------------Version-----------------------------------------------//
//	Created on :									07/09/24										   //
//	Revised on :									12/09/24										   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_

#include <main.h>
#include <usart.h>
#include <bleCmd.h>


//	Function name	: 	vBluetoothInit																   //
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	This funtion inits the bluetooth aplication, its save the pointer for the UART //
//						channel and inits the read of UART based on interruption					   //
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	UART_HandleTypeDef * huart													   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vBluetoothInit(UART_HandleTypeDef * huart);

//	Function name	: 	HAL_UART_RxCpltCallback 													   //
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	This function selects what move the robot will do based on the message that the//
//						bluetooth receive.															   //
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	UART_HandleTypeDef * huart													   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart);

#endif /* INC_BLUETOOTH_H_ */
