//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//----------------------------------------------File Name----------------------------------------------//
//	bleCmd.c  																					   //
//----------------------------------------------Desciption---------------------------------------------//
//																									   //
//	This file implements the commands sent by bluetooth based on with message it receives, the idea is //
//	The bluetooth communicates only with this library and this library uses all the others libs to     //
//	implements everything 																			   //
//	The functions implemented on this file are:														   //
//	-	vBleCmdMotorForward																			   //
//  -	vBleCmdMotorLeftward																		   //
//  -	vBleCmdMotorRightward																		   //
//  -	vBleCmdMotorBackward																		   //
//  -	vBleCmdMotorStop																		       //
//																									   //
//-----------------------------------------------Author------------------------------------------------//
//	João Vitor Mendes																				   //
//-----------------------------------------------Version-----------------------------------------------//
//	Created on :									07/09/24										   //
//	Revised on :									12/09/24										   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
#include "bleCmd.h"
#include "main.h"
extern float fSetPoint_left; //e
extern float fSetPoint_right; //f

//	Function name	: 	vBleCmdMotorForward															   //
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	This function command the robot to go forward								   //
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	n/a																			   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vBleCmdMotorForward(){
	fSetPoint_left = 185;
	fSetPoint_right = 200;
	vMotorsSetPWMTimer(left, 0.8, 1, 1000);
	vMotorsSetPWMTimer(right, 0.8, 1, 1000);
}

//	Function name	: 	vBleCmdMotorLeftward														   //
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	This function command the robot to go leftward								   //
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	n/a																			   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vBleCmdMotorLeftward(){
	fSetPoint_left = 140;
	fSetPoint_right = 260;
	vMotorsSetPWMTimer(left, 0.5, 1, 1000);
	vMotorsSetPWMTimer(right, 0.8, 1, 1000);
}

//	Function name	: 	vBleCmdMotorRightward														   //
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	This function command the robot to go rightward								   //
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	n/a																			   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vBleCmdMotorRightward(){
	fSetPoint_left = 260;
	fSetPoint_right = 140;
	vMotorsSetPWMTimer(left, 0.8, 1, 1000);
	vMotorsSetPWMTimer(right, 0.5, 1, 1000);
}

//	Function name	: 	vBleCmdMotorBackward														   //
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	This function command the robot to go backward								   //
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	n/a																			   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vBleCmdMotorBackward(){
	fSetPoint_left = -200;
	fSetPoint_right = -200;
	vMotorsSetPWMTimer(left, 0.8, 0, 1000);
	vMotorsSetPWMTimer(right, 0.8, 0, 1000);
}


//	Function name	: 	vBleCmdMotorStop														       //
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	This function command the robot to stop the motors							   //
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	n/a																			   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vBleCmdMotorStop(){
	vMotorsSetOff(left);
	vMotorsSetOff(right);
}

