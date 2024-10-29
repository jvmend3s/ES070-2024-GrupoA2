// ********************************************** //
// FILE NAME: buzzer.h
// FILE DESCRIPTION:
//  Codifica as funcoes de utilizacao do buzzer,
// utilizando PWM
//
// AUTHOR NAME: Guilherme Polewacz, Gustavo Wyatt
// CREATION DATE: 15/05/24
// REVISION DATE: x/05/24
//
// ********************************************** //

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

void vBuzzerConfig(unsigned short int, unsigned short int, TIM_HandleTypeDef*, TIM_HandleTypeDef*);
void vBuzzerPlay();
void vBuzzerStop();


#endif /* INC_BUZZER_H_ */
