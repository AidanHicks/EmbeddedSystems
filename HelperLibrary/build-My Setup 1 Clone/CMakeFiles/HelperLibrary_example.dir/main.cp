#line 1 "C:/Users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/main.c"
#line 10 "C:/Users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/main.c"
#line 1 "lib/lib.h"
#line 9 "lib/lib.h"
void set_led(unsigned short* latch, unsigned char pin, unsigned char state);

inline void led_on(unsigned short* latch, unsigned char pin) { set_led(latch,pin,1); }
inline void led_off(unsigned short* latch, unsigned char pin) { set_led(latch,pin,0); }


void clear_led_port(unsigned short* latch);

unsigned char button_pressed(unsigned short* port, unsigned short button);

void set_seven_seg(unsigned short value);
void seven_seg_scan();
void clear_seven_seg();
#line 11 "C:/Users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/main.c"

int main(void)
{
    set_seven_seg(9876);
    while (1) {
        seven_seg_scan();
    }
    return 0;
}
