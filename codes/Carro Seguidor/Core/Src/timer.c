/*
 * timer.c
 *
 *  Created on: Oct 26, 2024
 *      Author: wyatt
 */

#include "timer.h"

int iIndex;
single_timer iActiveTimers[20]; //Consegue guardar 20 contadores diferentes

void vTimerInit() {
	iIndex = 0;
	for (int i = 0; i < 20; i++) {
		iActiveTimers[i].iCounter = 0;
		iActiveTimers[i].iTotalMs = 0;
	}
}

//set o timer para comecar a contar, conta até total_ms, no timerCode
//Retorna o "codigo' do timer
int iSetTimer(int total_ms) {
	iActiveTimers[iIndex].iTotalMs = total_ms;
	return ++iIndex;
}

// conta (ms) ms, funcao chamada a cada 10 ms, pela main (pTimGeral)
char cCountTime(int ms, int timerCode) {
	iActiveTimers[timerCode].iCounter += ms;
	if (iActiveTimers[timerCode].iCounter >= iActiveTimers[timerCode].iTotalMs) {
		iActiveTimers[timerCode].iCounter = 0;
		return 1;
	}

	return 0;
}
