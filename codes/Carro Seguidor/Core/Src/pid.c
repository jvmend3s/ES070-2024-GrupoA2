/* ***************************************************************** */
/* File name:        pid.c                                           */
/* File description: This file has a couple of useful functions to   */
/*                   control the implemented PID controller          */
/* Author name:      julioalvesMS, IagoAF, rBacurau                  */
/* Creation date:    21jun2018                                       */
/* Revision date:    20mai2024                                       */
/* ***************************************************************** */

//CLASSE PID USADA PARA CONTROLE DA RODA ESQUERDA
#include <pid.h>
#include "main.h"

// Struct used to store the PID configuration parameters
extern pid_data_type xPidMotorRight;
extern pid_data_type xPidMotorLeft;
extern pid_data_type xPidDistance;
extern pid_data_type xPidAngle;



/* ************************************************ */
/* Method name:        vPidInit                     */
/* Method description: Initialize the PID controller*/
/* Input params:       n/a                          */
/* Output params:      n/a                          */
/* ************************************************ */
pid_data_type vPidInit(pid_data_type xPidConfig, float fKp, float fKi, float fKd, unsigned short usIntSizeMs, float fOutputSaturation)
{
	xPidConfig.fKp = fKp;
	xPidConfig.fKd = fKd;
	xPidConfig.fKi = fKi;
	xPidConfig.fError_previous = 0;
	xPidConfig.fError_sum = 0.0;

	// Saturates Integrator size (up to 10 s)
	if((usIntSizeMs/UPDATE_RATE_MS)> INTEGRATOR_MAX_SIZE)
	  usIntSizeMs = INTEGRATOR_MAX_SIZE * UPDATE_RATE_MS;

	xPidConfig.usIntegratorSize = usIntSizeMs/UPDATE_RATE_MS;

	xPidConfig.fOutputSaturation = fOutputSaturation;
	xPidConfig.usIntegratorCount = 0;
	for (int i =0; i<INTEGRATOR_MAX_SIZE; i++){
		xPidConfig.fIntegratorBuffer[i] = 0;
	}
	return xPidConfig;
}

/* ************************************************** */
/* Method name:        vPidSetKp                      */
/* Method description: Set a new value for the PID    */
/*                     proportional constant          */
/* Input params:       fKp: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
pid_data_type vPidSetKp(pid_data_type xPidConfig, float fKp)
{
	xPidConfig.fKp = fKp;
	return xPidConfig;
}


/* ************************************************** */
/* Method name:        fPidGetKp                      */
/* Method description: Get the value from the PID     */
/*                     proportional constant          */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKp(pid_data_type xPidConfig)
{
	return xPidConfig.fKp;
}


/* ************************************************** */
/* Method name:        vPidSetKi                      */
/* Method description: Set a new value for the PID    */
/*                     integrative constant           */
/* Input params:       fKi: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
pid_data_type vPidSetKi(pid_data_type xPidConfig, float fKi)
{
	xPidConfig.fKi = fKi;
	return xPidConfig;
}


/* ************************************************** */
/* Method name:        fPidGetKi                      */
/* Method description: Get the value from the PID     */
/*                     integrative constant           */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKi(pid_data_type xPidConfig)
{
	return xPidConfig.fKi;
}


/* ************************************************** */
/* Method name:        vPidSetKd                      */
/* Method description: Set a new value for the PID    */
/*                     derivative constant            */
/* Input params:       fKd: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
pid_data_type vPidSetKd(pid_data_type xPidConfig, float fKd)
{
	xPidConfig.fKd = fKd;
	return xPidConfig;
}


/* ************************************************** */
/* Method name:        fPidGetKd                      */
/* Method description: Get the value from the PID     */
/*                     derivative constant            */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKd(pid_data_type xPidConfig)
{
	return xPidConfig.fKd;
}

/* ************************************************** */
/* Method name:        vPidSetIntegratorWindow        */
/* Method description: Set a new value for the        */
/*                     integrator window (in ms)      */
/* Input params:       usIntSizeMs: New value (in ms) */
/* Output params:      n/a                            */
/* ************************************************** */
pid_data_type vPidSetIntegratorWindow (pid_data_type xPidConfig, unsigned short usIntSizeMs)
{
	// Saturates Integrator size (10000 ms)
	if((usIntSizeMs/UPDATE_RATE_MS)> INTEGRATOR_MAX_SIZE)
	  usIntSizeMs = INTEGRATOR_MAX_SIZE * UPDATE_RATE_MS;

	xPidConfig.usIntegratorSize = usIntSizeMs/UPDATE_RATE_MS;
	return xPidConfig;
}

/* ************************************************** */
/* Method name:        usPidGetIntegratorWindow       */
/* Method description: Get the value from the         */
/*                     integrator window (in ms)      */
/* Input params:       n/a                            */
/* Output params:      usIntSizeMs: Value (in ms)     */
/* ************************************************** */
unsigned short usPidGetIntegratorWindow (pid_data_type xPidConfig)
{
	return (xPidConfig.usIntegratorSize*UPDATE_RATE_MS);
}

/* ************************************************** */
/* Method name:        fPidUpdateData                 */
/* Method description: Update the control output      */
/*                     using the reference and sensor */
/*                     value                          */
/* Input params:       fSensorValue: Value read from  */
/*                     the sensor                     */
/*                     fReferenceValue: Value used as */
/*                     control reference              */
/* Output params:      float: New Control effort      */
/* ************************************************** */
float fPidUpdateData(pid_data_type xPidConfig, float fSensorValue, float fSetValue)
{
//	float fError, fDifference, fOut;

	// Proportional error
	xPidConfig.fError = fSetValue - fSensorValue;

	//Ingtegral error
	xPidConfig.fError_sum = xPidConfig.fError_sum - xPidConfig.fIntegratorBuffer[xPidConfig.usIntegratorCount] + xPidConfig.fError;

	xPidConfig.fIntegratorBuffer[xPidConfig.usIntegratorCount] = xPidConfig.fError;

	if(++xPidConfig.usIntegratorCount >= xPidConfig.usIntegratorSize)
		xPidConfig.usIntegratorCount = 0;

	// Differential error
	xPidConfig.fDifference = (xPidConfig.fError - xPidConfig.fError_previous);

	xPidConfig.fOut = xPidConfig.fKp * xPidConfig.fError
		 + xPidConfig.fKi * xPidConfig.fError_sum * UPDATE_RATE
		 + xPidConfig.fKd * xPidConfig.fDifference / UPDATE_RATE;

	xPidConfig.fError_previous = xPidConfig.fError;

    // Output Saturation
//	if(isnan(fOut))
//		fOut=0;
	if(xPidConfig.fOut > xPidConfig.fOutputSaturation)
		xPidConfig.fOut = xPidConfig.fOutputSaturation;
	else
		if (xPidConfig.fOut < 0)
			xPidConfig.fOut = 0;

	return xPidConfig.fOut;
}
