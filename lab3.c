/**
 * @file main.c
 * @brief Main function for Lab3 application.
 */

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

int main(void)
{
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    ANSELB = 0;
    ANSELC = 0;

    TRISB0_bit = 1; //Set RB0 as input
    TRISC0_bit = 0; //Set RC0 as output
    LATC0_bit = 0; //start with LED off

    INTCON2.RBPU = 1; // disables all PORTB pull-ups

    while (1)
    {
        if (RB0_bit == 0) {
            LATC0_bit = 1;
        } else {
            LATC0_bit = 0;
        }
    }
}
