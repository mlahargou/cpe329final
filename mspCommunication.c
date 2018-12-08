/*
 * mspCommunication.c
 *
 *  Created on: Dec 3, 2018
 *      Author: michaellahargou
 */

#include "mspCommunication.h"
#include "msp.h"
#include "delay.h"

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
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;     // Enable USCI_A0 RX interrupt

    // Enable eUSCIA0 interrupt in NVIC module
    NVIC->ISER[0] |= 1 << ((EUSCIA2_IRQn) & 31);       //was [0], we are trying [1]?

    //initially set the message to incomplete
    messageComplete = 0;
}

// UART interrupt service routine
void EUSCIA2_IRQHandler(void)
{
    if (EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG)
    {
        EUSCI_A2->IFG &=~ EUSCI_A_IFG_RXIFG; //clear interupts
        recievedChar(EUSCI_A2->RXBUF);

    }
}

void recievedChar (char letter) {
    static int i = 0;

    //Add our letter to the message
    receivedMessage[i] = letter;
    i++;

    //If the letter is our stopping byte, we have completed the message
    if (letter == 0xff) {
        messageComplete = 1;
        i = 0;
    }
}

void sendMessege(char * message) {
    int i = 0;
    char letter = message[0];

    //Keep sending until our stop byte
    while (letter != 0xff) {
        // Check if the TX buffer is empty first
        while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A2->TXBUF = letter;
        i++;
        letter = message[i];
    }
   // delay_us(200);
    EUSCI_A2->TXBUF = 0xFF;
}
