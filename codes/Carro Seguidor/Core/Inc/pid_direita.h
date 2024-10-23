/*
 * pid_direita.h
 *
 *  Created on: Oct 20, 2024
 *      Author: wyatt
 */

#ifndef INC_PID_DIREITA_H_
#define INC_PID_DIREITA_H_

#define UPDATE_RATE_MS      100
#define UPDATE_RATE         (UPDATE_RATE_MS/1000.0)
#define INTEGRATOR_MAX_SIZE 100

typedef struct pid_data_type_direita {
	float fKp, fKi, fKd;         		// PID gains
	float fError_previous;       		// used in the derivative
	float fError_sum;            		// integrator cumulative error
	unsigned short usIntegratorSize; 	//integrator window size
	float fOutputSaturation;            // output saturation
} pid_data_type_direita;


/* ************************************************ */
/* Method name:        vPidInit                     */
/* Method description: Initialize the PID controller*/
/* Input params:       n/a                          */
/* Output params:      n/a                          */
/* ************************************************ */
void vPidInit_direita(float fKp, float fKi, float fKd, unsigned short usIntSizeMs, float fOutputSaturation);


/* ************************************************** */
/* Method name:        vPidSetKp                      */
/* Method description: Set a new value for the PID    */
/*                     proportional constant          */
/* Input params:       fKp: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKp_direita(float fKp);


/* ************************************************** */
/* Method name:        fPidGetKp                      */
/* Method description: Get the value from the PID     */
/*                     proportional constant          */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKp_direita(void);


/* ************************************************** */
/* Method name:        vPidSetKi                      */
/* Method description: Set a new value for the PID    */
/*                     integrative constant           */
/* Input params:       fKi: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKi_direita(float fKi);


/* ************************************************** */
/* Method name:        fPidGetKi                      */
/* Method description: Get the value from the PID     */
/*                     integrative constant           */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKi_direita(void);


/* ************************************************** */
/* Method name:        vPidSetKd                      */
/* Method description: Set a new value for the PID    */
/*                     derivative constant            */
/* Input params:       fKd: New value                 */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetKd_direita(float fKd);


/* ************************************************** */
/* Method name:        fPidGetKd                      */
/* Method description: Get the value from the PID     */
/*                     derivative constant            */
/* Input params:       n/a                            */
/* Output params:      float: Value                   */
/* ************************************************** */
float fPidGetKd_direita(void);


/* ************************************************** */
/* Method name:        vPidSetIntegratorWindow        */
/* Method description: Set a new value for the        */
/*                     integrator window (in ms)      */
/* Input params:       usIntSizeMs: New value (in ms) */
/* Output params:      n/a                            */
/* ************************************************** */
void vPidSetIntegratorWindow_direita(unsigned short usIntSizeMs);


/* ************************************************** */
/* Method name:        usPidGetIntegratorWindow       */
/* Method description: Get the value from the         */
/*                     integrator window (in ms)      */
/* Input params:       n/a                            */
/* Output params:      usIntSizeMs: Value (in ms)     */
/* ************************************************** */
unsigned short usPidGetIntegratorWindow_direita(void);


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
float fPidUpdateData_direita(float fSensorValue, float fSetValue);

#endif /* INC_PID_DIREITA_H_ */
