/**
 * @file main.c
 * @brief Main function for Lab3-Buttons application.
 */

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

void button_toggle(){
    unsigned char prev = 1;
    while (1) {
        unsigned char current = RB0_bit;
        
        if (prev == 1 && current == 0) {
            LATC0_bit = !LATC0_bit;
            Delay_ms(80);
        }
        prev = current;
    }
        

}

void button_blink() {
    while (1)
    {
        if (RB0_bit == 0) {
            LATC0_bit = 1;
            Delay_ms(1000);
            LATC0_bit = 0;
            Delay_ms(1000);
        } else {
            LATC0_bit = 0;
        }
    }

}

void main()
{
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    /* Replace with your application code */
    ANSELB = 0;
    ANSELC = 0;

    TRISB0_bit = 1;
    TRISC0_bit = 0;    
    // button_blink();
    button_toggle();
}
