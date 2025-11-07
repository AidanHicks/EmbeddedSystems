
#include "lib.h"

void set_led(unsigned char* latch, unsigned char pin, unsigned char state) {
    *(latch + LAT_TO_TRIS_OFFSET) = 0;
    
    if (latch == 0) return;
    unsigned char mask = 1 << pin;
    if (state) {
        *latch |= mask;
    }
    else {
        *latch &= ~mask; 
    }
}

void led_on(unsigned char* latch, unsigned char pin) { set_led(latch,pin,1); }
void led_off(unsigned char* latch, unsigned char pin) { set_led(latch,pin,0); }

void clear_led(unsigned short* port) {
    *port = 0;
}