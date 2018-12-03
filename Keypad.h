/*
*  Keypad.h
*
*  Author: dillon
*
*/

#ifndef KEYPAD_H
#define KEYPAD_H
#include "msp.h"
#include "delay.h"

#define ROW1 BIT0
#define ROW2 BIT1
#define ROW3 BIT2
#define ROW4 BIT3

#define COL1 BIT4
#define COL2 BIT5
#define COL3 BIT6

uint8_t enterKey(DIO_PORT_Even_Interruptable_Type *KPBus);
void KP_init(DIO_PORT_Even_Interruptable_Type *KPBus);
uint8_t KP_getKey(DIO_PORT_Even_Interruptable_Type *KPBus);
uint8_t getAndPrintTwoDigitNum(DIO_PORT_Even_Interruptable_Type *KPBus);


#endif /* KEYPAD_H_ */
