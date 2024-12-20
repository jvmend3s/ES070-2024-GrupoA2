//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//----------------------------------------------File Name----------------------------------------------//
//	bluetooth.c  																					   //
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
#include <bluetooth.h>


unsigned char ucInput;
//UART_HandleTypeDef * pBluetoothControllerUART;

//	Function name	: 	vBluetoothInit																   //														   //
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	This funtion inits the bluetooth aplication, its save the pointer for the UART //
//						channel and inits the read of UART based on interruption					   //
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	UART_HandleTypeDef * huart													   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vBluetoothInit(UART_HandleTypeDef * huart){
//    pBluetoothControllerUART = huart;
//    HAL_UART_Receive_IT(pBluetoothControllerUART, &ucInput, 1);
}


//	Function name	: 	HAL_UART_RxCpltCallback 													   //
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	This function selects what move the robot will do based on the message that the//
//						bluetooth receive.															   //
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	UART_HandleTypeDef * huart													   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart){
//    if(huart == pBluetoothControllerUART){
//        if(ucInput == '1'){
//            vBleCmdMotorForward();
//        }
//        else if (ucInput == '4')
//        {
//        	vBleCmdMotorLeftward();
//        }
//        else if (ucInput == '3')
//        {
//        	vBleCmdMotorRightward();
//        }
//        else if (ucInput == '2')
//        {
//        	vBleCmdMotorBackward();
//        }
//        HAL_UART_Receive_IT(pBluetoothControllerUART, &ucInput, 1);
//    }
//}
