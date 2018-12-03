/*
 * Display.c
 *
 *  Created on: Nov 30, 2018
 *      Author: dillon
 */


#include "Display.h"
#include "msp.h"


char newline[] = {13,10};
char home[] = {0x1b, 0x5b, 0x48};

void initDisplay(DIO_PORT_Even_Interruptable_Type  *KPBus, uint8_t *pos1, uint8_t *pos2, uint8_t *pos3) {
    //uart pin config
    P1->SEL0 |= BIT2 | BIT3;          // set 2-UART pin as secondary function

    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            // Configure eUSCI clock source for SMCLK
            EUSCI_B_CTLW0_SSEL__SMCLK;
    // Baud Rate calculation
    // 24000000/(16*115200) = 26.041666
    // Fractional portion = 0.041666
    // User's Guide Table 21-4: UCBRSx = 0x01
    // UCBRFx = int ( (78.125-78)*16) = 0

    EUSCI_A0->BRW = 26;
    EUSCI_A0->MCTLW = (0 << EUSCI_A_MCTLW_BRF_OFS) |  EUSCI_A_MCTLW_OS16;

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag

    setup(KPBus, pos1,pos2,pos3);
}

void sendLetterToScreen(char letter) {
    int j;
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)); // Make sure buffer is empty
    for(j = 0; j< 1000; j++);

    EUSCI_A0->TXBUF = letter;
}

void sendWordToScreen(char * word, int size, int lineafter) {
    int i;
    for (i = 0; i < size; i++) {
        sendLetterToScreen(word[i]);
    }
    //add a new line after printing the word
    for (i = 0; i < lineafter; i++) {
        sendLetterToScreen(10);
        sendLetterToScreen(13);
    }
}

void printTwoCharNumber(int number) {   // if number = 3456
    //3456 / 1000 = 3
    sendLetterToScreen(48 + (number / 10) % 10);
    //3456 % 10 = 6
    sendLetterToScreen(48 + (number % 10));
}

void setup(DIO_PORT_Even_Interruptable_Type  *KPBus, uint8_t *pos1, uint8_t *pos2, uint8_t *pos3) {
    uint8_t firstNum, secondNum;
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;     // Enable USCI_A0 RX interrupt
    sendWordToScreen("Setup machine", 14, 2);
    sendWordToScreen("Enter rotor position 1: ", 24, 0);
    firstNum = KP_getKey(KPBus);
    sendLetterToScreen(48 + firstNum);
    secondNum = KP_getKey(KPBus);
    sendLetterToScreen(48 + secondNum);
    *pos1 = (firstNum * 10 + secondNum);

    sendWordToScreen(NULL, 0, 1);  // new line

    sendWordToScreen("Enter rotor position 2: ", 24, 0);
    firstNum = KP_getKey(KPBus);
    sendLetterToScreen(48 + firstNum);
    secondNum = KP_getKey(KPBus);
    sendLetterToScreen(48 + secondNum);
    *pos2 = (firstNum * 10 + secondNum);

    sendWordToScreen(NULL, 0, 1);  // new line

    sendWordToScreen("Enter rotor position 3: ", 24, 0);
    firstNum = KP_getKey(KPBus);
    sendLetterToScreen(48 + firstNum);
    secondNum = KP_getKey(KPBus);
    sendLetterToScreen(48 + secondNum);
    *pos3 = (firstNum * 10 + secondNum);

    sendWordToScreen(NULL, 0, 1);  // new line

    EUSCI_A0->IE &= ~EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt

    //Twice due to bug
    //sendWordToScreen(home, 3, 0);
    //sendWordToScreen(home, 3, 0);
}

void enterMessage(uint8_t *pos1, uint8_t *pos2, uint8_t *pos3) {

    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;     // Enable USCI_A0 RX interrupt
    sendWordToScreen("Enter message: ", 15, 0);

    sendWordToScreen(NULL, 0, 1);  // new line

    EUSCI_A0->IE &= ~EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt

    //Twice due to bug
    //sendWordToScreen(home, 3, 0);
    //sendWordToScreen(home, 3, 0);
}


void printFourCharNumber(int number) {   // if number = 3456
    //3456 / 1000 = 3
    sendLetterToScreen(48 + number / 1000);
    //(3456 / 100 = 34) % 10 = 4
    sendLetterToScreen(48 + (number / 100) % 10);
    //(3456 / 10 = 345) % 10 = 5
    sendLetterToScreen(48 + (number / 10) % 10);
    //3456 % 10 = 6
    sendLetterToScreen(48 + (number % 10));
}

