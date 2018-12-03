/*
 * mspCommunication.c
 *
 *  Created on: Dec 3, 2018
 *      Author: michaellahargou
 */

#include "mspCommunication.h"


//run this at 12MHz

void mspCommunicationInit() {
    // Configure UART pins
    P3->SEL0 |= BIT2 | BIT3;                // set 2-UART pin as second function

    // Configure UART
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A2->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_A_CTLW0_SSEL__SMCLK;      // Select clock source from SMCLK

    // Baud Rate calculation
    // 24000000/(16*115200) = 26.041666
    // Fractional portion = 0.041666
    // User's Guide Table 21-4: UCBRSx = 0x01
    // UCBRFx = int ( (78.125-78)*16) = 0

    EUSCI_A2->BRW = 26;
    EUSCI_A2->MCTLW = (0 << EUSCI_A_MCTLW_BRF_OFS) |  EUSCI_A_MCTLW_OS16;

    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI

    EUSCI_A2->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
}

// UART interrupt service routine
void EUSCIA2_IRQHandler(void)
{
    if (EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG)
    {
        // Check if the TX buffer is empty first
        //while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

        recievedChar(EUSCI_A2->RXBUF);
    }
}

void recievedChar (char letter) {
    static int index = 0;
    static char message[1024];

    message[index] = letter;

    if (letter == 0xff) {
        decryptAndPrint(message);
        index = 0;
    }

    index++;
}

void sendMessege(char * message) {
    int i = 0;
    char letter = message[0];

    while (letter != 0xff) {
        // Check if the TX buffer is empty first
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

        EUSCI_A2->TXBUF = letter;
        i++;
        letter = message[i];
    }
}
