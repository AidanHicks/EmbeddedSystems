#include "lib.h"

void set_led(unsigned short* latch, unsigned char pin, unsigned char state) {
    *(latch + LAT_TO_TRIS_OFFSET) = 0;
    
    if (latch == 0) return;
    auto mask = 1 << pin;
    if (state) {
        *latch |= mask;
    }
    else {
        *latch &= ~mask; 
    }
}

void clear_led(unsigned short* port) {
    *port = 0;
}