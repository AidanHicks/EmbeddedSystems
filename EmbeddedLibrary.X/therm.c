#include "lib.h"
#include "therm.h"

void therm_init() {
    ANSELE = 0x02;
    TRISE1 = 1;
    ADCON1 = 0x00;
    ADCON2 = 0xA9;
}

unsigned int VREF_mV = 5000u;
unsigned int adc_to_Celsius(unsigned int adc){
    unsigned long t = (unsigned long)adc; // widen to prevent overflow
    t = t * (unsigned long)VREF_mV; // adc * Vref_mV gives microvolts/step-scaled value
    t = (t + 511ul) / 1023ul; // divide by 1023 with rounding gives mV at the pin
    t = t / 10ul; // LM35 is 10 mV per degree => degrees C
    if(t>9999ul) t=9999ul; // keep it displayable
    return (unsigned int)t;
}

unsigned int therm_get_sample(unsigned char ch){
    ADCON0bits.CHS = ch; // select channel in CHS bits
    ADCON0bits.ADON = 1; // ADON = 1 turns the ADC on
    __delay_ms(2); // let the sample-and-hold capacitor charge
    ADCON0bits.GO_nDONE = 1; // GO/DONE = 1 starts conversion
    while(ADCON0bits.GO_nDONE){} // wait until GO/DONE returns to 0
    return adc_to_Celsius((unsigned int)((((unsigned int)ADRESH) << 8) | ADRESL)); // 10-bit value
}
