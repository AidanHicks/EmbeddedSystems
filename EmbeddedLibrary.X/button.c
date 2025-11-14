

#include "lib.h"
#include "button.h"
#define _XTAL_FREQ 8000000UL
#define DEBOUNCE_MS 30

unsigned char get_button_state(unsigned char* port, unsigned char button) {
    unsigned char button_state = *port;
    return (button_state & (1 << button)) >> button;
}
unsigned char prevs[36];
unsigned char button_pressed(unsigned char* port, unsigned char button) {
    // Set button tris to be an input
    unsigned char tris = *(port + PORT_TO_TRIS_OFFSET);
    tris = tris | (1 << button);
    unsigned char* ansel = (unsigned char*)(port - PORT_TO_ANSEL_OFFSET);
    *ansel = 0x0;
    

    // 0xF80 is the address of PORTA
    unsigned char index = ((unsigned short)port - 0xF80) * 8 + button;


    unsigned char now = get_button_state(port,button);
    if (prevs[index] == 1u && now == 0u) {
        __delay_ms(DEBOUNCE_MS);
        if (get_button_state(port,button) == 0u) { prevs[index] = 0u; return 1u; }
    }
    prevs[index] = now;
    return 0u;
}