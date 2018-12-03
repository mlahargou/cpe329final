/*
 * Keypad.c
 *
 *  Created on: Nov 30, 2018
 *      Author: dillon
 */


#include "Keypad.h"

void KP_init(DIO_PORT_Even_Interruptable_Type *KPBus) {
    KPBus->SEL0 = 0;
    KPBus->SEL1 = 0;
    KPBus->DIR &= ~(ROW1 | ROW2 | ROW3 | ROW4);  //inputs
    KPBus->DIR |= (COL1 | COL2 | COL3);  //outputs
    KPBus->REN |= (ROW1 | ROW2 | ROW3 | ROW4); //enable resistor
    KPBus->OUT &= ~(ROW1 | ROW2 | ROW3 | ROW4); //pull down
}

uint8_t KP_getKey(DIO_PORT_Even_Interruptable_Type *KPBus) {
    static uint8_t row, col, key;
    delay_ms(300);
    KPBus->OUT |= (COL1 | COL2 | COL3);
    delay_us(0);

    /* gives all the rows masked */
    while(1) {
        row = KPBus->IN & (ROW1 | ROW2 | ROW3 | ROW4);
        if (row != 0) {
            break;  //add a define
        }
    }

    /* Cycle through the columns */
    for (col = 0; col < 3; col++) {
        /*turn off all the columns */
        KPBus->OUT &= ~(COL1 | COL2 | COL3);
        KPBus->OUT |= (COL1 << col);
        delay_us(0);
        row = KPBus->IN & (ROW1 | ROW2 | ROW3 | ROW4);
        if (row != 0) {
            break;
        }
    }

    KPBus->OUT &= ~(COL1 | COL2 | COL3);  //turn off outputs
    if (col == 3) {
        return 0xFF;
    }
    if (row == 4) {
        row = 3;
    }
    else if (row == 8) {
        row = 4;
    }
    if (col == 0) {
        key = (row * 3) - 2;
    }
    else if (col == 1) {
        key = (row * 3) -1;
    }
    else if (col == 2) {
        key = row * 3;
    }
    if (key == 11) {
        key = 0;
    }
    return key;
}

uint8_t enterKey(DIO_PORT_Even_Interruptable_Type *KPBus){
    uint8_t key=0xFF;
    while(1){
        /*Gets inputted character */
        if((key=KP_getKey(KPBus)) != 0xFF) {
            return key;
        }
    }
}

uint8_t getNum(DIO_PORT_Even_Interruptable_Type *KPBus){
    uint8_t i=0,num,temp;

    for(i=0;i<2;i++){
        if((temp=enterKey(KPBus))==12 && i!=0)
            return num;

        if (!i && temp<10){
            num=temp;
        }

        else if(i && temp<10){
            num=(num*10)+temp;
        }
    }
    return num;
}






















