
#include "lib.h"
#include "led.h"

void led_set(unsigned char* latch, unsigned char pin, unsigned char state) {
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

void led_on(unsigned char* latch, unsigned char pin) { led_set(latch,pin,1); }
void led_off(unsigned char* latch, unsigned char pin) { led_set(latch,pin,0); }

void led_clear_port(unsigned short* port) {
    *port = 0;
}