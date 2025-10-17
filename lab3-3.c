/**
 * @file main.c
 * @brief Main function for Lab3-Task3 application.
 */

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

#define BUTTON_INC RB0_bit
#define BUTTON_DEC RB1_bit
#define LED_PORT   LATC

unsigned char inc_pressed(void) {
    static unsigned char prev = 1;
    unsigned char cur = BUTTON_INC;
    if (prev == 1 && cur == 0) {
        Delay_ms(30);
        if (BUTTON_INC == 0) {
             prev = 0; 
             return 1; 
        }
    }
    prev = cur;
    return 0;
}
unsigned char dec_pressed(void) {
    static unsigned char prev = 1;
    unsigned char cur = BUTTON_DEC;
    if (prev == 1 && cur == 0) {
        Delay_ms(30);
        if (BUTTON_DEC == 0) {
            prev = 0;
            return 1;
        }
    }
    prev = cur;
    return 0;
}



void main() {
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif


    ANSELB = 0;
    ANSELC = 0;

    TRISB0_bit = 1; // RB0 input
    TRISB1_bit = 1; // RB1 input
    TRISC = 0x00;

    unsigned char counter = 0;
    LED_PORT = counter;
    
    while (1) {
        if (inc_pressed()) { counter++; LED_PORT = counter; }
        if (dec_pressed()) { counter--; LED_PORT = counter; }
    }
}