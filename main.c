#include <Rotor.h>
#include "msp.h"
#include "Display.h"
#include "Keypad.h"
#include "mspCommunication.h"

/**
 * main.c
 */

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;         // stop watchdog timer
    uint8_t user;                                       // determines send or receive first message
    DIO_PORT_Even_Interruptable_Type *KPBus=P4;

    setDCO(FREQ_48_MHZ);                                // MCLK fed from 48 MHZ DCO

    KP_init(KPBus);                                     // Initiates Keypad with Pin 4 connections
    mspCommunicationInit();

    Rotors allRotors = initDisplay(KPBus,&user);

    __enable_irq();

    while(1){
        message(&allRotors,&user);                      // send and recieve message loop
    }
}
