#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdio.h>
#include "msp.h"
#include "Keypad.h"
#include "Rotor.h"

void initDisplay(DIO_PORT_Even_Interruptable_Type  *KPBus, uint8_t *pos1, uint8_t *pos2, uint8_t *pos3);
void sendLetterToScreen(char letter);
void sendWordToScreen(char * word, int size, int lineAfter);
void setup(DIO_PORT_Even_Interruptable_Type  *KPBus, uint8_t *pos1, uint8_t *pos2, uint8_t *pos3);
void printFourCharNumber(int number);
void message(Rotors allRotors);
void decryptMessageAndPrint(Rotors AllRotors, char * encryptedMessage);
void encryptAndSend(Rotors allRotors, int size, char * decryptedMessage);

#endif /* DISPLAY_H_ */
