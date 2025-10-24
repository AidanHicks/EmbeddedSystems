#line 1 "C:/Users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/lib/button.c"


unsigned char get_button_state(unsigned short* port, unsigned short button) {
    unsigned short button_state = *port;
    return (button_state & (1 << button)) >> button;
}

unsigned char prevs[36];
unsigned char button_pressed(unsigned short* port, unsigned short button) {

    unsigned short tris = *(port + PORT_TO_TRIS_OFFSET);
    tris = tris | (1 << button);


    unsigned short index = ((unsigned short)port - 0xF80) * 8 + button;


    unsigned char now = get_button_state(port,button);
    if (prevs[index] == 1u && now == 0u) {
        Delay_ms( 30 );
        if (get_button_state(port,button) == 0u) { prevs[index] = 0u; return 1u; }
    }
    prevs[index] = now;
    return 0u;
}
