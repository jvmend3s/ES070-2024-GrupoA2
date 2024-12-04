/*
 * odometria.c
 *
 *  Created on: Nov 16, 2024
 *      Author: wyatt
 */

#include "odometria.h"
#include <math.h>

#define PI 3.141592654
#define BITOLA 0.13325 // [m]

odometry_state current_state;

void vOdometriaInit() {
	current_state.distance_traveled = 0;
	current_state.angle_orientation = 0;
	current_state.x = 0;
	current_state.y = 0;
}

void vCountOdometria(float d_left, float d_right) {
	float diff_distance;
	diff_distance = (d_left + d_right)/2;
	current_state.distance_traveled += diff_distance;

	float diff_angle;
	diff_angle = (d_right - d_left)/BITOLA; //rad

	current_state.x += diff_distance*cos(current_state.angle_orientation + diff_angle/2);
	current_state.y += diff_distance*sin(current_state.angle_orientation + diff_angle/2);
	current_state.angle_orientation += diff_angle;

	//correção do explode
	if (fabs(current_state.x) > 100 || fabs(current_state.y) > 100 || fabs(current_state.angle_orientation) > 200)
		vOdometriaInit();
}

float fDegreeToRad(float angle) {
	return (angle * PI)/180;
}

float fRadToDegree(float angle) {
	return (angle * 180)/PI;
}

odometry_state xGetCurrentState() {
	return current_state;
}
