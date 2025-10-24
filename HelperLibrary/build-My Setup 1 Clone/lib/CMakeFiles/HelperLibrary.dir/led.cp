#line 1 "C:/Users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/lib/led.c"
#line 1 "lib.h"
#line 9 "lib.h"
void set_led(unsigned short* latch, unsigned char pin, unsigned char state);

inline void led_on(unsigned short* latch, unsigned char pin) { set_led(latch,pin,1); }
inline void led_off(unsigned short* latch, unsigned char pin) { set_led(latch,pin,0); }


void clear_led_port(unsigned short* latch);

unsigned char button_pressed(unsigned short* port, unsigned short button);

void set_seven_seg(unsigned short value);
void seven_seg_scan();
void clear_seven_seg();
#line 2 "C:/Users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/lib/led.c"

void set_led(unsigned short* latch, unsigned char pin, unsigned char state) {
    *(latch +  0x9 ) = 0;

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
