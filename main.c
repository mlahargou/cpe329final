#include "msp.h"
#include "rotor.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	Rotors allRotors = initRotors(0,0,0);

    char outLetter = getCharThroughAllRotors(allRotors, 'z');

    while(1);
}
