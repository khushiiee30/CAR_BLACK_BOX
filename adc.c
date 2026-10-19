#include <xc.h>
#define _XTAL_FREQ 20000000UL

#include "adc.h"

void init_adc(void)
{
    ADCON1 = 0x0A;  // Configure ADC pins

    TRISA5 = 1;  // Set RA5/AN4 as input

    VCFG0 = 0;  // Select VDD as positive voltage reference
    VCFG1 = 0;  // Select VSS as negative voltage reference

    ADCON2 = 0x92;  // Configure ADC conversion settings

    ADCON0 = 0x01;  // Select AN0 and enable ADC

    __delay_us(20);  // Wait for ADC to stabilize
}

unsigned short read_adc(unsigned char channel)
{
    unsigned short adc_value;

    ADCON0 = (unsigned char)((ADCON0 & 0xC3) | ((unsigned char)channel << 2));  // Select ADC channel

    __delay_us(10);  // Wait for channel selection

    GO = 1;  // Start ADC conversion

    while (GO)  // Wait until conversion completes
    {
        ;
    }

    adc_value = ((unsigned short)ADRESH << 8) | ADRESL;  // Combine ADC result registers

    return adc_value;  // Return ADC value
}

