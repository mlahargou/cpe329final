#include "delay.h"
#include "msp.h"
#include <stdio.h>


void optimize48Mhz(){
    /*Wait until not busy*/
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));

    /*Now not busy so Power source can be changed*/
    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;

    /*Wait until not busy*/
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));


    /* Configure Flash wait-state to 1 for both banks 0 & 1 */
    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL &
            ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) | FLCTL_BANK0_RDCTL_WAIT_1;
    FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL &
            ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) | FLCTL_BANK1_RDCTL_WAIT_1;
}

void setDCO(uint32_t frequency){
    CS->KEY = CS_KEY_VAL;         /*UNLOCK REGISTERS*/
    if (frequency == FREQ_48_MHZ)
        optimize48Mhz();

    CS->CTL0 = 0;                 /*CLEAR REGISTER CTL0   */
    CS->CTL0 = frequency;         /*Set DCO to specified frequency*/

    /*line 2619 */

    CS->CTL1 |= CS_CTL1_SELM_3;   /*((uint32_t)0x00000003) <- Selects MCLK source, SELM_3 = 011b = DCOCLK*/
    CS->KEY = 0;                  /*LOCK REGISTERS*/

}

void delay_us(uint64_t microSeconds){
    volatile uint32_t i,freq, end=0,sel;

    if (!microSeconds)
        return;

    if (microSeconds > 50000)
        microSeconds = 50000;

    sel= CS->CTL0;

    freq = 3*( 1 << ( (sel) >> 16))/2; /*freq = 1.5 * 2^n*/


    switch(freq){
        case 1:
            if(microSeconds<=119)/*8.4034khz*/
                return;
            end = (microSeconds-119)/7;
            break;

        case 3:
            if(microSeconds<=70)/*5.6466khz*/
                return;
            end = (microSeconds -64)/4;
            break;

        case 6:
            if(microSeconds<=55)/*12.931khz*/
                return;
            end = (microSeconds-55)/2 ;
            break;
        case 12:
            if(microSeconds<=11)/*30.56khz*/
                return;
            end = (microSeconds-6) ;
            break;


        case 24:
            if(microSeconds<=5)/*Change*/
                return;
            end = (microSeconds-3)*2 ;
        break;
        case 48:
            if(microSeconds<=5)/*Change*/
                return;
            end = (microSeconds-2)*4;
            break;
        default:
            break;
    }

   for (i = 0; i < end;i++);
}

void delay_ms(uint64_t milliseconds){
    int i;
    for(i=0; i< milliseconds;i++)
        delay_us(1000);

}

