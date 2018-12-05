/*
 * Display.c
 *
 *  Created on: Nov 30, 2018
 *      Author: dillon
 */


#include "Display.h"
#include "msp.h"
#include "Rotor.h"
#include "mspCommunication.h"


char str[1024];
uint32_t br=0,index=0;

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

    NVIC->ISER[0] |= 1 << ((EUSCIA0_IRQn) & 31);

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
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;     // Enable USCI_A0 RX interrupt
    sendWordToScreen("Setup machine", 14, 2);

    sendWordToScreen("Enter rotor position 1: ", 24, 0);
    *pos1 = getAndPrintTwoDigitNum(KPBus);

    sendWordToScreen(NULL, 0, 1);  // new line

    sendWordToScreen("Enter rotor position 2: ", 24, 0);
    *pos2 = getAndPrintTwoDigitNum(KPBus);

    sendWordToScreen(NULL, 0, 1);  // new line

    sendWordToScreen("Enter rotor position 3: ", 24, 0);
    *pos3 = getAndPrintTwoDigitNum(KPBus);

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

void inputWord(uint32_t *size){
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;     // Enable USCI_A0 RX interrupt

    while(1){
        if(br || index==1023){
            str[index]=(char)255;
            index++;
            break;
        }
    }
    *size=index;
    index=0;
    br=0;
    EUSCI_A0->IE &= ~EUSCI_A_IE_RXIE;     // Enable USCI_A0 RX interrupt
}


void message(Rotors allRotors){
    uint32_t size=0;
    sendWordToScreen("Enter message: ", 15, 0);
    inputWord(&size);

    /*Send word here
     *
    for(i=0;i<1024;i++)
        str[i]=0;
     */

    sendWordToScreen(NULL, 0, 2);
    encryptAndSend(allRotors,size,str);



    /*
     *
     * Left off here, remove function calls below and replace with working algorithm
     *
     *
     * */

    //wait until we have a message to receive

    sendWordToScreen("Waiting for message...", 22, 1);
    while(!messageComplete);
    //reset our received messageComplete
    messageComplete = 0;
    delay_ms(1);

    sendWordToScreen("Message from user: MSG should appear after: ", 44, 2);
    decryptMessageAndPrint(allRotors, receivedMessage);

}


void decryptMessageAndPrint(Rotors allRotors, char * encryptedMessage) {
    char letter = encryptedMessage[0];

    while (letter != 0xff) {
        sendLetterToScreen(getCharThroughAllRotors(allRotors, letter));
        index++;
        letter = encryptedMessage[index];
    }
}

void encryptAndSend(Rotors allRotors, int size, char * decryptedMessage) {
    char * encryptedMessage;
    char letter = decryptedMessage[0];
    int index = 0;

    while(index < size) {
        encryptedMessage[index] = getCharThroughAllRotors(allRotors, letter);
        index++;
        letter = decryptedMessage[index];
    }

    encryptedMessage[size] = 0xff;

    sendMessege(decryptedMessage);
}

void EUSCIA0_IRQHandler(void)
{
    uint8_t input;
    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    {
        // Check if the TX buffer is empty first
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

        input = EUSCI_A0->RXBUF;

        if(input==13)
            br=1;

        else{
            str[index]=input;
            index++;
            sendLetterToScreen(input);
        }
    }
}
