//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//----------------------------------------------File Name----------------------------------------------//
//	motors.h																						   //
//----------------------------------------------Desciption---------------------------------------------//
//																									   //
//	This file implements the motors using PWM														   //
//	The functions implemented on this file are:														   //
//	-	vMotorsInit																					   //
//  -	vMotorsSetPWM																				   //
//  -	vMotorsSetVelocity																			   //
//  -	vMotorsSetPWMTimer																			   //
//  -	vMotorsSetOff																		 		   //
//  -	vMotorsDurationCallback																		   //
//																									   //
//-----------------------------------------------Author------------------------------------------------//
//	Gustavo Wyatt Varga																				   //
//	Maria Clara 																					   //
//	João Vitor Mendes																				   //
//-----------------------------------------------Version-----------------------------------------------//
//	Created on :									07/09/24										   //
//	Revised on :									12/09/24										   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

void vMotorsInit();
void vMotorsSetPWM(char, float, char);
void vMotorsSetVelocity(char, float, char);

//	Function name	: 	vMotorsSetPWMTimer 															   //
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	This function set one motor to run based on PWM, and a rotation direction for  //
//						a pre-set time, it enable the interrupion that after some cycles defined by    //
//						duration input will stop the motor in vMotorsDurationCallback function         //
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	char motor, float PWM, char rotation, unsigned int duration					   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vMotorsSetPWMTimer(char, float, char, unsigned int );

void vMotorsSetOff(char);

//	Function name	: 	vMotorsDurationCallback 															   //
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	This function deals with Motors running duration callback, it will counts how  //
//						many interruptions for stop the motor based on usTimeOn (duration set) then	   //
//						stops the motors.															   //
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	n/a																			   //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vMotorsDurationCallback();

#endif /* INC_MOTORS_H_ */
