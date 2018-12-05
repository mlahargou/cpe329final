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
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    DIO_PORT_Even_Interruptable_Type *KPBus=P4;
    uint8_t *pos1=NULL, *pos2=NULL, *pos3=NULL;

    Rotors allRotors = initRotors(0,0,0);

    setDCO(FREQ_48_MHZ);

    KP_init(KPBus);
    mspCommunicationInit();
    initDisplay(KPBus,pos1,pos2,pos3);

    __enable_irq();

    while(1){
        message(allRotors);
    }
}
