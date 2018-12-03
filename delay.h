#ifndef DELAY_H
#define DELAY_H
#include <stdint.h>
#define FREQ_1_5_MHZ CS_CTL0_DCORSEL_0
#define FREQ_3_MHZ   CS_CTL0_DCORSEL_1
#define FREQ_6_MHZ   CS_CTL0_DCORSEL_2
#define FREQ_12_MHZ  CS_CTL0_DCORSEL_3
#define FREQ_24_MHZ  CS_CTL0_DCORSEL_4
#define FREQ_48_MHZ  CS_CTL0_DCORSEL_5

    void delay_ms(uint64_t milliSeconds);
    void delay_us(uint64_t microSeconds);
    void setDCO(uint32_t frequency);


#endif
