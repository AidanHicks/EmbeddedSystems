
#include "lm35.h"
#include "config_bits.h"

void LM35_Init(void) {
    ANSELEbits.ANSE2 = 0; // RE2 Digital
    ANSELEbits.ANSE1 = 1; // RE1 Analog (AN6)
    ANSELEbits.ANSE0 = 0; // RE0 Digital

    
    ADCON0bits.CHS = 6;
    ADCON1bits.PVCFG = 0;
    ADCON1bits.NVCFG = 0;
    ADCON2bits.ACQT = 4;
    ADCON2bits.ADCS = 2;
    ADCON0bits.ADON = 1; 
}

unsigned int LM35_Read_Temp(void) {
    ADCON0bits.GO = 1; 
    __delay_ms(1);
    while (ADCON0bits.GO); 
    
    unsigned int adc_val = (ADRESH << 8) + ADRESL;

    unsigned long temp = (unsigned long)adc_val * 500;
    return (unsigned int)(temp / 1024);
}