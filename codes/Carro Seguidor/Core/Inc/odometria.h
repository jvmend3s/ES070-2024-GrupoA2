/*
 * odometria.h
 *
 *  Created on: Nov 16, 2024
 *      Author: wyatt
 */

#ifndef INC_ODOMETRIA_H_
#define INC_ODOMETRIA_H_

typedef struct odometry_state {
	float distance_traveled, angle_orientation; // m, rad
	float x, y; //m, m
} odometry_state;

void vOdometriaInit();
void vCountOdometria(float, float);
float fDegreeToRad(float);
float fRadToDegree(float);
odometry_state xGetCurrentState();

#endif /* INC_ODOMETRIA_H_ */
