#define DEBOUNCE_MS 30

unsigned char get_button_state(unsigned short* port, unsigned short button) {
    unsigned short button_state = *port;
    return (button_state & (1 << button)) >> button;
}

unsigned char prevs[36];
unsigned char button_pressed(unsigned short* port, unsigned short button) {
    // Set button tris to be an input
    unsigned short tris = *(port + PORT_TO_TRIS_OFFSET);
    tris = tris | (1 << button);

    // 0xF80 is the address of PORTA
    unsigned short index = ((unsigned short)port - 0xF80) * 8 + button;


    unsigned char now = get_button_state(port,button);
    if (prevs[index] == 1u && now == 0u) {
        Delay_ms(DEBOUNCE_MS);
        if (get_button_state(port,button) == 0u) { prevs[index] = 0u; return 1u; }
    }
    prevs[index] = now;
    return 0u;
}