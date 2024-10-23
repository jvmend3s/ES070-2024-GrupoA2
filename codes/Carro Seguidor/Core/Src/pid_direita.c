/* ***************************************************************** */
/* File name:        pid.c                                           */
/* File description: This file has a couple of useful functions to   */
/*                   control the implemented PID controller          */
/* Author name:      julioalvesMS, IagoAF, rBacurau                  */
/* Creation date:    21jun2018                                       */
/* Revision date:    20mai2024                                       */
/* ***************************************************************** */

//CLASSE PID USADA PARA CONTROLE DA RODA DIREITA
#include <pid_direita.h>
#include "main.h"

// Struct used to store the PID configuration parameters
pid_data_type_direita xPidConfig_dir;
// Counter used to control the integration error window
unsigned short usIntegratorCount_dir = 0;
// Buffer used to store the errors to generate the integral error
float fIntegratorBuffer_dir[INTEGRATOR_MAX_SIZE]={0};

float fError_dir, fDifference_dir, fOut_dir;

/* ************************************************ */
/* Method name:        vPidInit                     */
/* Method description: Initialize the PID controller*/
/* Input params:       n/a                          */
/* Output params:      n/a                          */
/* ************************************************ */
void vPidInit_direita(float fKp, float fKi, float fKd, unsigned short usIntSizeMs, float fOutputSaturation)
{
	xPidConfig_dir.fKp = fKp;
	xPidConfig_dir.fKd = fKd;
	xPidConfig_dir.fKi = fKi;
	xPidConfig_dir.fError_previous = 0;
	xPidConfig_dir.fError_sum = 0.0;

	// Saturates Integrator size (up to 10 s)
	if((usIntSizeMs/UPDATE_RATE_MS)> INTEGRATOR_MAX_SIZE)
	  usIntSizeMs = INTEGRATOR_MAX_SIZE * UPDATE_RATE_MS;

	xPidConfig_dir.usIntegratorSize = usIntSizeMs/UPDATE_RATE_MS;

	xPidConfig_dir.fOutputSaturation = fOutputSaturation;
}

/* ************************************************** */
/* Method name:        vPidSetKp                      */
/* Method description: Set a new value for the PID    */
/*                     proportional constant          */
/* Input params:       fKp: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKp_direita(float fKp)
{
	xPidConfig_dir.fKp = fKp;
}


/* ************************************************** */
/* Method name:        fPidGetKp                      */
/* Method description: Get the value from the PID     */
/*                     proportional constant          */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKp_direita(void)
{
	return xPidConfig_dir.fKp;
}


/* ************************************************** */
/* Method name:        vPidSetKi                      */
/* Method description: Set a new value for the PID    */
/*                     integrative constant           */
/* Input params:       fKi: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKi_direita(float fKi)
{
	xPidConfig_dir.fKi = fKi;
}


/* ************************************************** */
/* Method name:        fPidGetKi                      */
/* Method description: Get the value from the PID     */
/*                     integrative constant           */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKi_direita(void)
{
	return xPidConfig_dir.fKi;
}


/* ************************************************** */
/* Method name:        vPidSetKd                      */
/* Method description: Set a new value for the PID    */
/*                     derivative constant            */
/* Input params:       fKd: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKd_direita(float fKd)
{
	xPidConfig_dir.fKd = fKd;
}


/* ************************************************** */
/* Method name:        fPidGetKd                      */
/* Method description: Get the value from the PID     */
/*                     derivative constant            */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKd_direita(void)
{
	return xPidConfig_dir.fKd;
}

/* ************************************************** */
/* Method name:        vPidSetIntegratorWindow        */
/* Method description: Set a new value for the        */
/*                     integrator window (in ms)      */
/* Input params:       usIntSizeMs: New value (in ms) */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetIntegratorWindow_direita(unsigned short usIntSizeMs)
{
	// Saturates Integrator size (10000 ms)
	if((usIntSizeMs/UPDATE_RATE_MS)> INTEGRATOR_MAX_SIZE)
	  usIntSizeMs = INTEGRATOR_MAX_SIZE * UPDATE_RATE_MS;

	xPidConfig_dir.usIntegratorSize = usIntSizeMs/UPDATE_RATE_MS;
}

/* ************************************************** */
/* Method name:        usPidGetIntegratorWindow       */
/* Method description: Get the value from the         */
/*                     integrator window (in ms)      */
/* Input params:       n/a                            */
/* Output params:      usIntSizeMs: Value (in ms)     */
/* ************************************************** */
unsigned short usPidGetIntegratorWindow_direita (void)
{
	return (xPidConfig_dir.usIntegratorSize*UPDATE_RATE_MS);
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
float fPidUpdateData_direita(float fSensorValue, float fSetValue)
{
	float fError_dir, fDifference_dir, fOut_dir;

	// Proportional error
	fError_dir = fSetValue - fSensorValue;

	//Ingtegral error
	xPidConfig_dir.fError_sum = xPidConfig_dir.fError_sum - fIntegratorBuffer_dir[usIntegratorCount_dir] + fError_dir;

	fIntegratorBuffer_dir[usIntegratorCount_dir] = fError_dir;

	if(++usIntegratorCount_dir >= xPidConfig_dir.usIntegratorSize)
		usIntegratorCount_dir = 0;

	// Differential error
	fDifference_dir = (fError_dir - xPidConfig_dir.fError_previous);

	fOut_dir = xPidConfig_dir.fKp * fError_dir
		 + xPidConfig_dir.fKi * xPidConfig_dir.fError_sum * UPDATE_RATE
		 + xPidConfig_dir.fKd * fDifference_dir / UPDATE_RATE;

	xPidConfig_dir.fError_previous = fError_dir;

    // Output Saturation
	if(fOut_dir > xPidConfig_dir.fOutputSaturation)
		fOut_dir = xPidConfig_dir.fOutputSaturation;
	else
		if (fOut_dir < 0)
			fOut_dir = 0;

	return fOut_dir;
}
