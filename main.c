#include "msp.h"
#include "rotor.h"
#include "delay.h"
#include "Keypad.h"
#include "Display.h"


/**
 * main.c
 */
void main(void)
{
    /*
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	Rotors allRotors = initRotors(0,0,0);

    char outLetter = getCharThroughAllRotors(allRotors, 'z');

    while(1);
    */
    DIO_PORT_Even_Interruptable_Type  *KPBus=P4;

    uint8_t *pos1=NULL, *pos2=NULL, *pos3=NULL;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    setDCO(FREQ_48_MHZ);
    __enable_irq();
    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);
    KP_init(KPBus);
    initDisplay(KPBus,pos1,pos2,pos3);


    while(1);
}
