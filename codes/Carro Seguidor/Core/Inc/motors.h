/*
 * motors.h
 *
 *  Created on: Sep 4, 2024
 *      Author: ES670 B
 */

#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

enum Motor {
	left = 0,
	right
};

void vMotorsInit();
void vMotorsSetPWM(char, float, char);
void vMotorsSetVelocity(char, float, char);
void vMotorsSetPWMTimer(char, float, char, unsigned int );
void vMotorsSetOff(char);
void vUpdateTimerPWM();

#endif /* INC_MOTORS_H_ */
