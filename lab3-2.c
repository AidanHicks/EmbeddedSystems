/**
 * @file main.c
 * @brief Main function for Lab3-Task2 application.
 */

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#ifdef PREINIT_SUPPORTED
#include "preinit.h"
#endif

void main()
{
    /* Do not remove this line or clock might not be set correctly. */
    #ifdef PREINIT_SUPPORTED
    preinit();
    #endif

    /* Replace with your application code */
    ANSELB = 0;
    ANSELC = 0;

    TRISB0_bit = 1;  //RB0 to incrment
    TRISB1_bit = 1;  //RB1 to decrement
    TRISC = 0x00;

    unsigned char counter = 0;
    LATC = counter; //Draw current value

    unsigned char prev_inc = 1;
    unsigned char prev_dec = 1;

    while (1)
    {
        unsigned char cur_inc = RB0_bit; //live RB0
        unsigned char cur_dec = RB1_bit; //live RB1

        if (prev_inc == 1 && cur_inc == 0) {
            counter++;
            LATC = counter;
            Delay_ms(80);
        }

        if (prev_dec == 1 && cur_dec == 0) {
            counter--;
            LATC = counter;
            Delay_ms(80);
        }

        prev_inc = cur_inc;
        prev_dec = cur_dec;
    }

}
