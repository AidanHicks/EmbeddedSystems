
#include "lm35.h"
#include "config_bits.h"

void LM35_Init(void) {
    // Configure AN6 (RE1) as Analog, others Digital
    ANSELEbits.ANSE2 = 0; // RE2 Digital
    ANSELEbits.ANSE1 = 1; // RE1 Analog (AN6)
    ANSELEbits.ANSE0 = 0; // RE0 Digital

    // ADC Configuration
    ADCON0bits.CHS = 6;   // Select Channel 6 (AN6)
    ADCON1bits.PVCFG = 0; // Vref+ = VDD (5V)
    ADCON1bits.NVCFG = 0; // Vref- = VSS (GND)
    ADCON2bits.ACQT = 4;  // Acquisition time
    ADCON2bits.ADCS = 2;  // Clock conversion Fosc/32
    ADCON0bits.ADON = 1;  // Enable ADC
}

unsigned int LM35_Read_Temp(void) {
    ADCON0bits.GO = 1;          // Start conversion
    while (ADCON0bits.GO);      // Wait for completion
    
    unsigned int adc_val = (ADRESH << 8) + ADRESL;
    
    // Convert to Voltage (5000mV / 1024 steps)
    // Then to Degrees (10mV = 1C)
    // Temp = (ADC * 5000 / 1024) / 10
    // Simplified: Temp = (ADC * 500) / 1024
    unsigned long temp = (unsigned long)adc_val * 500;
    return (unsigned int)(temp / 1024);
}