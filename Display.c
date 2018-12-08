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

Rotors initDisplay(DIO_PORT_Even_Interruptable_Type  *KPBus,uint8_t *user) {
    uint8_t pos1, pos2, pos3;
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

    setup(KPBus, &pos1,&pos2,&pos3,user);
    return initRotors(pos1,pos2,pos3);
}

void sendLetterToScreen(char letter) {
    int j;
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)); // Make sure buffer is empty
    for(j = 0; j< 1000; j++);

    //Send the transmit buffer the letter
    EUSCI_A0->TXBUF = letter;
}

void sendWordToScreen(char * word, int size, int lineafter) {
    int i;
    //Send each letter to the screen
    for (i = 0; i < size; i++) {
        sendLetterToScreen(word[i]);
    }
    //Add a new line after printing the word
    for (i = 0; i < lineafter; i++) {
        sendLetterToScreen(10);
        sendLetterToScreen(13);
    }
}

/*
 * This handles printing a two char number to the screen.
 * We turn it from its int value to ascii value by adding 48.
 */
void printTwoCharNumber(int number) {   // if number = 36
    //36 / 10  % 10 = 3
    //We do the modulus in case the number is bigger than 9.
    sendLetterToScreen(48 + (number / 10) % 10);
    //36 % 10 = 6
    sendLetterToScreen(48 + (number % 10));
}

void setup(DIO_PORT_Even_Interruptable_Type  *KPBus, uint8_t *pos1, uint8_t *pos2, uint8_t *pos3, uint8_t *user) {
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;     // Enable USCI_A0 RX interrupt
    sendWordToScreen("Setup machine", 14, 2);

    sendWordToScreen("Enter rotor position 1: ", 24, 0);
    *pos1 = getAndPrintTwoDigitNum(KPBus);      // determines rotor 1 position

    sendWordToScreen(NULL, 0, 1);  // new line

    sendWordToScreen("Enter rotor position 2: ", 24, 0);
    *pos2 = getAndPrintTwoDigitNum(KPBus);      // determines rotor 2 position

    sendWordToScreen(NULL, 0, 1);  // new line

    sendWordToScreen("Enter rotor position 3: ", 24, 0);
    *pos3 = getAndPrintTwoDigitNum(KPBus);      // determines rotor 3 position

    sendWordToScreen(NULL, 0, 2);  // new line

    sendWordToScreen("Enter 01 to send first, Enter 00 to receive first: ", 51, 0);

    *user = getAndPrintTwoDigitNum(KPBus);      // determines message order

    sendWordToScreen(NULL, 0, 2);  // new line

    EUSCI_A0->IE &= ~EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt
}

void enterMessage(uint8_t *pos1, uint8_t *pos2, uint8_t *pos3) {
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;     // Enable USCI_A0 RX interrupt
    sendWordToScreen("Enter message: ", 15, 0);     //prompts user to enter a message

    sendWordToScreen(NULL, 0, 1);  // new line

    EUSCI_A0->IE &= ~EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt
}

void inputWord(uint32_t *size){
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;     // Enable USCI_A0 RX interrupt
    index=0;
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

void message(Rotors *allRotors, uint8_t* user){
    if (*user == 0){                      //if user = 0, we will wait for a message before sending
        sendWordToScreen("Waiting for message...", 22, 1);
        while(!messageComplete);          //wait for message to be completely delivered
        messageComplete = 0;              //reset our received messageComplete flag

        sendWordToScreen("Message from user: ", 19, 0);
        decryptMessageAndPrint(allRotors, receivedMessage);     //runs encrypted result through enigma decryption
        sendWordToScreen(NULL, 0, 2);

        *user = 1;
    }

    uint32_t size=0;
    sendWordToScreen("Enter message: ", 15, 0);
    inputWord(&size);

    sendWordToScreen(NULL, 0, 2);
    encryptAndSend(allRotors,size,str);         //encrypts input array and sends it over UART


    sendWordToScreen("Waiting for message...", 22, 1);
    while(!messageComplete);                //wait for message to be completely delivered
    messageComplete = 0;                    //reset our received messageComplete flag

    sendWordToScreen("Message from user: ", 19, 0);
    decryptMessageAndPrint(allRotors, receivedMessage);       //runs encrypted result through enigma decryption
    sendWordToScreen(NULL, 0, 2);
}

void decryptMessageAndPrint(Rotors *allRotors, char * encryptedMessage) {
    char letter = encryptedMessage[0];
    index=0;

    //Once we hit our stop char of 0xff, we have completed the decryption.
    while (letter != 0xff) {       //send each letter in array until reaching stop bit
        sendLetterToScreen(getCharThroughAllRotors(allRotors, letter));
        index++;
        letter = encryptedMessage[index];
    }
}

void encryptAndSend(Rotors *allRotors, int size, char * decryptedMessage) {
    char encryptedMessage[1024];
    char letter = decryptedMessage[0];
    int i = 0;
    char val;

    /*
     * Because we know the size of the word, encrypt the message and send each
     * char one at a time.  Once we get to the end of size, add a 0xff so
     * the receiving enigma machine knows when the message is complete.
     */
    while(i < size-1) {
         val = getCharThroughAllRotors(allRotors, letter);
         encryptedMessage[i] = val;
         val = encryptedMessage[i];
        i++;
        letter = decryptedMessage[i];
    }

    //Add the stop char.
    encryptedMessage[size-1] = 0xff;

    sendMessege(encryptedMessage);
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
