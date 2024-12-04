// ********************************************** //
// FILE NAME: communication.c
// FILE DESCRIPTION:
//  Implementacao da comunicacao UART com o
// computador.
// COM3 - bauld rate = 115200
//
// AUTHOR NAME: Guilherme Polewacz, Gustavo Wyatt
// CREATION DATE: 23/04/24
// REVISION DATE: 03/04/24
//
// ********************************************** //

#include "communication.h"

unsigned char ucInputChar;
char cNumeroString[10]; // tamanho maximo: "-1000.000\n"
char pOutputString[100];

// ********************************************** //
// METHOD NAME: vCommunicationInit
// METHOD DESCRIPTION:
//  Inicializa variaveis do communication.
//
// INPUT PARAMS: n/a
//
// OUTPUT PARAMS: n/a
// ********************************************** //
void vCommunicationInit() {

	for (int i = 0; i < 10; i++)
		cNumeroString[i] = 0;

	ucInputChar = 0;
}


// ********************************************** //
// METHOD NAME: vCommunicationClearGlobalVariables
// METHOD DESCRIPTION:
//  Funcao auxiliar da callback, zera as variaveis
// globais.
//
// INPUT PARAMS: n/a
//
// OUTPUT PARAMS: n/a
// ********************************************** //
void vCommunicationClearGlobalVariables() {
	for (int i = 0; i < 10; i++)
	  cNumeroString[i] = 0;

	ucInputChar = 0;

	for (int i = 0; i < 100; i++)
	  pOutputString[i] = 0;
}

// ********************************************** //
// METHOD NAME: vCommunicationStringToFloat
// METHOD DESCRIPTION:
//  Transforma string em float.
//
// INPUT PARAMS: char* - string contendo o numero
//
// OUTPUT PARAMS: float - numero gerado
// ********************************************** //
float vCommunicationStringToFloat(char* str) {
	// Troca ',' por '.'
	for (int i = 0; i < 10; i++)
		if (',' == str[i])
			str[i] = '.';

	return atof(str);
}

// ********************************************** //
// METHOD NAME: pCommunicationFloatToString
// METHOD DESCRIPTION:
//  Transforma float em string.
//
// INPUT PARAMS: num - numero a ser convertido
//               nCasasDec - numero de casas decimais a serem consideradas
//
// OUTPUT PARAMS: char* - string contendo o num
// ********************************************** //
char* pCommunicationFloatToString(float num, int nCasasDec) { // n casas decimais // para numeros < 1
	float fAux = num;
	// Utilizado na quantidade de casas que a string vai ter
	int iN = nCasasDec;
	int iPos = 0;
	// "mapping" para facilitar na adicao de char na string
	char pNumberToChar[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	// Limpa a string global
	for (int i = 0; i < 100; i++)
		pOutputString[i] = 0;

	// Caso float negativo, adiciona '-' na string
	if (fAux < 0) {
		fAux = fAux*-1;
		pOutputString[iPos] = '-';
		iPos++;
	}

	// Caso menor que 1, adiciona 0 na string
	if (fAux < 1) {
		pOutputString[iPos] = '0';
		iPos++;
	}

	// Move a virgula do numero ate que fique menor que 1
	while (fAux >= 1) {
		fAux = fAux/10;
		iN++;
	}

	// Enquanto ainda nao tiver preenchido o numero de casas desejadas
	while (iN > 0) {
		fAux = fAux*10;
		// Seleciona o numero da enesima posicao
		int iNumber = floor(fmod(fAux, 10));
		// Adiciona virgula na posicao correta
		if (iN == nCasasDec) {
			pOutputString[iPos] = ',';
			iPos++;
		}

		// Adiciona char igual ao numero, na string de retorno
		pOutputString[iPos] = pNumberToChar[iNumber];
		iPos++;
		iN--;
	}

	return &pOutputString;
}

// ********************************************** //
// METHOD NAME: vCommunicationSendString
// METHOD DESCRIPTION:
//  Envia a string por UART, char por char.
//
// INPUT PARAMS: char* - string a ser enviada
//
// OUTPUT PARAMS: n/a
// ********************************************** //
void vCommunicationSendString(char* str) {
	for (int i = 0; i < strlen(str); i++) {
	  HAL_UART_Transmit(&hlpuart1, &str[i], 1, 1000);
	}
}

