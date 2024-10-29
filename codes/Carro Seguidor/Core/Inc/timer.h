/*
 * timer.h
 *
 *  Created on: Oct 26, 2024
 *      Author: wyatt
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

typedef struct single_timer {
	int iCounter, iTotalMs;
} single_timer;

void vTimerInit();
int iSetTimer(int);
char cCountTime(int, int);

#endif /* INC_TIMER_H_ */
