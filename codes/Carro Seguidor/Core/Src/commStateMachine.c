//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//----------------------------------------------File Name----------------------------------------------//
//	communicationStateMachine.c
//----------------------------------------------Desciption---------------------------------------------//
//
//	This file explains the funcion os the state machine used on the communicantion system,
//	using the RxCpltCallback function to interrupt the system and treat these interruptions.
//	the other functions are made to set and return parameters.
//
//	The functions implemented on this file are:
//	-	HAL_UART_RxCpltCallback
//	-	vCommunicationStateMachineReturnParam
//	-	vCommunicationStateMachineSetParam
//
//
//-----------------------------------------------Author------------------------------------------------//
//	João Vitor Mendes
//	Vitor Akio Isawa
//-----------------------------------------------Version-----------------------------------------------//
//	Created on :									29/04/24
//	Revised on :									03/05/24
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//-----------------------------------------------Defines-----------------------------------------------//
#define _IDDLE   '0'
#define _READY   '1'
#define _CAR	 '2'
#define _GET     '3'
#define _SET     '4'
#define _PARAM   '5'
#define _VALUE   '6'
#define _WAIT_VALUE '7'

#define MAX_VALUE_LENGTH 7

//-----------------------------------------------Includes----------------------------------------------//
#include "commStateMachine.h"
#include "bleCmd.h"
#include "communication.h"
#include "stm32g4xx_hal.h"

//----------------------------------------Variable and definitions-------------------------------------//
extern float fLeftSpeed; // l
extern float fRightSpeed; // r
extern float fLeftMotorKp; // a
extern float fLeftMotorKi; // b
extern float fRightMotorKp; // c
extern float fRightMotorKi; // d
extern float fSetPoint_left; //e
extern float fSetPoint_right; //f
float fOperation = 0 ;

static unsigned char ucValue[MAX_VALUE_LENGTH+1];

char cCar; // 0 = Carro Lider 1 = Carro Seguidor
char cWaitAnwser;
unsigned char ucState = _IDDLE;
unsigned char ucValueCount;
unsigned char ucValueCountv2v;
unsigned char ucDigit;
unsigned char ucDigitv2v;
unsigned char ucTransmitVector[_MAX_NUMBER_OF_CHARS];
UART_HandleTypeDef *pBluetoothControllerUART;
UART_HandleTypeDef *pV2VUART;

//-----------------------------------------------Functions---------------------------------------------//


void vCommStateMachineInit(UART_HandleTypeDef *huartcontrol, UART_HandleTypeDef *huartV2V){
	pBluetoothControllerUART = huartcontrol;
	pV2VUART =huartV2V;
	HAL_UART_Receive_IT(pBluetoothControllerUART, &ucDigit, 1);
	HAL_UART_Receive_IT(pV2VUART, &ucDigit, 1);
}


//	Function name	: 	HAL_UART_RxCpltCallback
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	Makes the treatment when recieve one byte from the uart, making a
//		                state machine to pass trought any input
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	ucByte
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    static unsigned char ucParam;

    if (huart == pBluetoothControllerUART || huart == pV2VUART){
        HAL_UART_Receive_IT(huart, &ucDigit, 1);

        if('#' == ucDigit){
            ucState = _CAR;
        }
        else{
			if (ucState != _IDDLE){
				switch (ucState){
				case _CAR:
					switch (ucDigit){
					case '1':
						cCar = 0; //lider
						ucState = _READY;
						break;

					case '2':
						cCar = 1; //seguidor
						ucState = _READY;
						break;
					default:
						ucState = _IDDLE;
					}
					break;
				case _READY:
					switch (ucDigit){
					case 'g':
						ucState = _GET;
						break;
					case 's':
						ucState = _SET;
						break;
					default:
						ucState = _IDDLE;
					}
					break;

				case _GET:
//					if ('t' == ucDigit || 'r' == ucDigit || 'h' == ucDigit || 'c' == ucDigit || 'p' == ucDigit || 'i' == ucDigit
//							|| 'd' == ucDigit || 's' == ucDigit || 'l' == ucDigit || 'o' == ucDigit){
					if ('v' == ucDigit || 'a' == ucDigit || 'b' == ucDigit || 'c' == ucDigit || 'd' == ucDigit || 'e' == ucDigit || 'f' == ucDigit|| 'l' == ucDigit || 'r' == ucDigit || 'o' == ucDigit){
					ucParam = ucDigit;
					ucState = _PARAM;
					}
					else
					ucState = _IDDLE;
					break;

				case _SET:
//					if ('r' == ucDigit || 'h' == ucDigit || 'c' == ucDigit || 'p' == ucDigit || 'i' == ucDigit || 'd' == ucDigit
//							|| 'l' == ucDigit || 'o' == ucDigit){
					if ('v' == ucDigit || 'a' == ucDigit || 'b' == ucDigit || 'c' == ucDigit || 'd' == ucDigit || 'e' == ucDigit || 'f' == ucDigit || 'l' == ucDigit || 'r' == ucDigit|| 'o' == ucDigit){
					ucParam = ucDigit;
					ucValueCount = 0;
					ucState = _VALUE;
					}
					else
					ucState = _IDDLE;
					break;

				case _PARAM:
					if (';' == ucDigit){
						vCommStateMachineReturnParam(ucParam);
					}
					if(cWaitAnwser){
						ucState = _WAIT_VALUE;
						ucValueCount = 0;
					}
					else
						ucState = _IDDLE;
					break;

				case _VALUE:
					if ((ucDigit >= '0' && ucDigit <= '9') || ',' == ucDigit){
						if (ucValueCount < MAX_VALUE_LENGTH){
							ucValue[ucValueCount++] = ucDigit;
						}
					}
					else{
						if (';' == ucDigit){
							ucValue[ucValueCount] = '\0';
							vCommStateMachineSetParam(ucParam, &ucValue);
						}
						ucState = _IDDLE;
					}
					break;
				case _WAIT_VALUE:
					if ((ucDigit >= '0' && ucDigit <= '9') || ',' == ucDigit){
						if (ucValueCount < MAX_VALUE_LENGTH){
							ucValue[ucValueCount++] = ucDigit;
						}
					}
					else{
						if (';' == ucDigit){
							ucValue[ucValueCount] = '\0';
							vCommStateMachineSendMessage(pBluetoothControllerUART, &ucValue);
						}
						ucState = _IDDLE;
					}
					break;
				}
			}
			else{
		        if(ucDigit == '1')
				    vBleCmdMotorForward();
				else if (ucDigit == '4')
					vBleCmdMotorLeftward();
				else if (ucDigit == '3')
					vBleCmdMotorRightward();
				else if (ucDigit == '2')
					vBleCmdMotorBackward();
			}
		}
    }
//    HAL_UART_Receive_IT(pBluetoothControllerUART, &ucDigit, 1);
}


//	Function name	: 	vCommStateMachineReturnParam
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	Returns the parameter indicated by the ucParam variable
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	ucParam
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vCommStateMachineReturnParam(unsigned char ucParam){
	unsigned char ucValue[MAX_VALUE_LENGTH];
	unsigned char ucValuev2v[MAX_VALUE_LENGTH];
	char * pMessage;
	//caso seja carro lider
	if(!cCar){
		switch (ucParam){
			case 'l': //left speed
				pMessage = pCommunicationFloatToString(fLeftSpeed, 2);
				vCommStateMachineSendMessage(pBluetoothControllerUART, pMessage);
				break;
			case 'r': //right speed
				pMessage = pCommunicationFloatToString(fRightSpeed, 2);
				vCommStateMachineSendMessage(pBluetoothControllerUART, pMessage);
				break;
			case 'a': //kp left
				pMessage = pCommunicationFloatToString(fLeftMotorKp, 4);
				vCommStateMachineSendMessage(pBluetoothControllerUART, pMessage);
				break;
			case 'b': //ki left
				pMessage = pCommunicationFloatToString(fLeftMotorKi, 4);
				vCommStateMachineSendMessage(pBluetoothControllerUART, pMessage);
				break;
			case 'c': //kp right
				pMessage = pCommunicationFloatToString(fRightMotorKp, 4);
				vCommStateMachineSendMessage(pBluetoothControllerUART, pMessage);
				break;
			case 'd': //ki left
				pMessage = pCommunicationFloatToString(fRightMotorKi, 4);
				vCommStateMachineSendMessage(pBluetoothControllerUART, pMessage);
				break;
			case 'e': //setpoint left
				pMessage = pCommunicationFloatToString(fSetPoint_left, 2);
				vCommStateMachineSendMessage(pBluetoothControllerUART, pMessage);
				break;
			case 'f': //setpoint right
				pMessage = pCommunicationFloatToString(fSetPoint_right, 2);
				vCommStateMachineSendMessage(pBluetoothControllerUART, pMessage);
				break;
			case 'o': //operation
				pMessage = pCommunicationFloatToString(fOperation, 0);
				vCommStateMachineSendMessage(pBluetoothControllerUART, pMessage);
				break;
    	}
	}
	else{
		switch (ucParam){
			case 'l': //left speed
				vCommStateMachineSendMessage(pV2VUART, "#gl;");
				break;
			case 'r': //right speed
				vCommStateMachineSendMessage(pV2VUART, "#gr;");
				break;
			case 'a': //kp left
				vCommStateMachineSendMessage(pV2VUART, "#ga;");
				break;
			case 'b': //ki left
				vCommStateMachineSendMessage(pV2VUART, "#gb;");
				break;
			case 'c': //kp right
				vCommStateMachineSendMessage(pV2VUART, "#gc;");
				break;
			case 'd': //ki left
				vCommStateMachineSendMessage(pV2VUART, "#gd;");
				break;
			case 'e': //setpoint left
				vCommStateMachineSendMessage(pV2VUART, "#ge;");
				break;
			case 'f': //setpoint right
				vCommStateMachineSendMessage(pV2VUART, "#gf;");
				break;
			case 'v': //vMax
				vCommStateMachineSendMessage(pV2VUART, "#gv;");
				break;
			case 'o':
				vCommStateMachineSendMessage(pV2VUART, "#go;");
				break;
		}
		cWaitAnwser = 1;
	}
}
//	Function name	: 	vCommunicationStateMachineSetParam
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	set the parameter indicated by the ucParam by ucValue
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	ucParam
//		                ucValue
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vCommStateMachineSetParam(unsigned char ucParam, unsigned char * pValue){
	float fValue;
	char cAux;
	if(cCar == 0){ // carro lider
		switch (ucParam){
			case 'a': //left kp
				fLeftMotorKp = fCommStateMachineStringToFloat(pValue);
				break;
			case 'b': //left ki
				fLeftMotorKi = fCommStateMachineStringToFloat(pValue);
				break;
			case 'c': //right kp
				fRightMotorKp = fCommStateMachineStringToFloat(pValue);
				break;
			case 'd': //left ki
				fRightMotorKi=fCommStateMachineStringToFloat(pValue);
				break;
			case 'e': //set left
				fValue = fCommStateMachineStringToFloat(pValue);
				if (fValue >= 0 && fValue<= 700){
					fSetPoint_left=fValue;
				}
				break;
			case 'f': //set right
				fValue = fCommStateMachineStringToFloat(pValue);
				if (fValue >= 0 && fValue<= 700){
					fSetPoint_right=fValue;
				}
				break;
			case 'o': //op
				fOperation=fCommStateMachineStringToFloat(pValue);
				break;
		}
	}
	else{
		switch (ucParam){

			case 'a': //left kp
				vCommStateMachineSendMessage(pV2VUART, "#sa");
				vCommStateMachineSendMessage(pV2VUART, pValue);
				vCommStateMachineSendMessage(pV2VUART, ";");
				break;
			case 'b': //left ki
				vCommStateMachineSendMessage(pV2VUART, "#sb");
				vCommStateMachineSendMessage(pV2VUART, pValue);
				vCommStateMachineSendMessage(pV2VUART, ";");
				break;
			case 'c': //right kp
				vCommStateMachineSendMessage(pV2VUART, "#sc");
				vCommStateMachineSendMessage(pV2VUART, pValue);
				vCommStateMachineSendMessage(pV2VUART, ";");
				break;
			case 'd': //left ki
				vCommStateMachineSendMessage(pV2VUART, "#sd");
				vCommStateMachineSendMessage(pV2VUART, pValue);
				vCommStateMachineSendMessage(pV2VUART, ";");
				break;
			case 'e': //set left
				vCommStateMachineSendMessage(pV2VUART, "#se");
				vCommStateMachineSendMessage(pV2VUART, pValue);
				vCommStateMachineSendMessage(pV2VUART, ";");
				break;
			case 'f': //set right
				vCommStateMachineSendMessage(pV2VUART, "#sf");
				vCommStateMachineSendMessage(pV2VUART, pValue);
				vCommStateMachineSendMessage(pV2VUART, ";");
				break;
			case 'v': //set right
				vCommStateMachineSendMessage(pV2VUART, "#sv");
				vCommStateMachineSendMessage(pV2VUART, pValue);
				vCommStateMachineSendMessage(pV2VUART, ";");
				break;
			case 'o': //set right
				vCommStateMachineSendMessage(pV2VUART, "#so");
				vCommStateMachineSendMessage(pV2VUART, pValue);
				vCommStateMachineSendMessage(pV2VUART, ";");
				fOperation = fCommStateMachineStringToFloat(pValue);
				break;

		}
	}
}

//	Function name	: 	fCommunicationStateMachineStringToFloat
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	Turns Strings to float using atof functon and set a "." instead a ","
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	unsigned char * ucInputNumber
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//float fCommunicationStateMachineStringToFloat(unsigned char * ucValue){
float fCommStateMachineStringToFloat(){
	float fInputNumber=0;
	int iPos=0;
	while (iPos >= 0){
		if ('\0'!=ucValue[iPos]){
			if (ucValue[iPos] == ','){
				ucValue[iPos] = '.';
			}
			iPos++;
		}
		else{
			iPos = -1;
		}
	}
	fInputNumber = atof (ucValue);
	return fInputNumber;
}

//	Function name	: 	vCommunicationStateMachineFloatToString
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	Turns Float to Strings using atof functon and set a "." instead a ","
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	float fInputNumber
//		                int iDigits
//		                unsigned char * ucOutputNumber
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vCommStateMachineFloatToString(float num, int nCasasDec, unsigned char * pOutputString){
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
	}

//	Function name	: 	vCommunicationStateMachineSendMessage
//-----------------------------------------------------------------------------------------------------//
//	 Description	:	Send message to putty from HAL_UART_Transmit
//-----------------------------------------------------------------------------------------------------//
//	      Inputs	:	UART_HandleTypeDef * huart
//		                unsigned char* ucMessage
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
void vCommStateMachineSendMessage (UART_HandleTypeDef * huart, unsigned char* ucMessage){
	int iPos = 0;
	unsigned char a;
	while(iPos >= 0 && iPos < _MAX_NUMBER_OF_CHARS){
		a = ucMessage[iPos];
		if(ucMessage[iPos] =='\000'){
			iPos = -1;
		}
		else if(ucMessage[iPos] =='\n'){
			HAL_UART_Transmit(huart, &ucMessage[iPos], 1, 100);
			iPos = -1;
		}
		else{
			HAL_UART_Transmit(huart, &ucMessage[iPos], 1, 100);
			iPos ++;
		}
	}
}

//---------------------------------------------Weak Functions------------------------------------------//
