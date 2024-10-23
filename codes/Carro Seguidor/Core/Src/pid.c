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
pid_data_type xPidConfig;
// Counter used to control the integration error window
unsigned short usIntegratorCount = 0;
// Buffer used to store the errors to generate the integral error
float fIntegratorBuffer[INTEGRATOR_MAX_SIZE]={0};

float fError, fDifference, fOut;

/* ************************************************ */
/* Method name:        vPidInit                     */
/* Method description: Initialize the PID controller*/
/* Input params:       n/a                          */
/* Output params:      n/a                          */
/* ************************************************ */
void vPidInit(float fKp, float fKi, float fKd, unsigned short usIntSizeMs, float fOutputSaturation)
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
}

/* ************************************************** */
/* Method name:        vPidSetKp                      */
/* Method description: Set a new value for the PID    */
/*                     proportional constant          */
/* Input params:       fKp: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKp(float fKp)
{
	xPidConfig.fKp = fKp;
}


/* ************************************************** */
/* Method name:        fPidGetKp                      */
/* Method description: Get the value from the PID     */
/*                     proportional constant          */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKp(void)
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
void vPidSetKi(float fKi)
{
	xPidConfig.fKi = fKi;
}


/* ************************************************** */
/* Method name:        fPidGetKi                      */
/* Method description: Get the value from the PID     */
/*                     integrative constant           */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKi(void)
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
void vPidSetKd(float fKd)
{
	xPidConfig.fKd = fKd;
}


/* ************************************************** */
/* Method name:        fPidGetKd                      */
/* Method description: Get the value from the PID     */
/*                     derivative constant            */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKd(void)
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
void vPidSetIntegratorWindow (unsigned short usIntSizeMs)
{
	// Saturates Integrator size (10000 ms)
	if((usIntSizeMs/UPDATE_RATE_MS)> INTEGRATOR_MAX_SIZE)
	  usIntSizeMs = INTEGRATOR_MAX_SIZE * UPDATE_RATE_MS;

	xPidConfig.usIntegratorSize = usIntSizeMs/UPDATE_RATE_MS;
}

/* ************************************************** */
/* Method name:        usPidGetIntegratorWindow       */
/* Method description: Get the value from the         */
/*                     integrator window (in ms)      */
/* Input params:       n/a                            */
/* Output params:      usIntSizeMs: Value (in ms)     */
/* ************************************************** */
unsigned short usPidGetIntegratorWindow (void)
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
float fPidUpdateData(float fSensorValue, float fSetValue)
{
	float fError, fDifference, fOut;

	// Proportional error
	fError = fSetValue - fSensorValue;

	//Ingtegral error
	xPidConfig.fError_sum = xPidConfig.fError_sum - fIntegratorBuffer[usIntegratorCount] + fError;

	fIntegratorBuffer[usIntegratorCount] = fError;

	if(++usIntegratorCount >= xPidConfig.usIntegratorSize)
		usIntegratorCount = 0;

	// Differential error
	fDifference = (fError - xPidConfig.fError_previous);

	fOut = xPidConfig.fKp * fError
		 + xPidConfig.fKi * xPidConfig.fError_sum * UPDATE_RATE
		 + xPidConfig.fKd * fDifference / UPDATE_RATE;

	xPidConfig.fError_previous = fError;

    // Output Saturation
	if(isnan(fOut))
		fOut=0;
	if(fOut > xPidConfig.fOutputSaturation)
		fOut = xPidConfig.fOutputSaturation;
	else
		if (fOut < 0)
			fOut = 0;

	return fOut;
}
