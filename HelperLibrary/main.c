/**
 * @file main.c
 * @brief Main function for HelperLibrary application.
 */

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */
#include "lib/lib.h"

int main(void)
{
    set_seven_seg(9876);
    while (1) {
        seven_seg_scan();
    }
    return 0;
}
