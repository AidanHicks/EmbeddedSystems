/**
 * @file lib.h
 * @brief HelperLibrary library.
 */

/* User code here */
#define LAT_TO_TRIS_OFFSET 0x9

void set_led(unsigned short* latch, unsigned char pin, unsigned char state);

inline void led_on(unsigned short* latch, unsigned char pin) { set_led(latch,pin,1); }
inline void led_off(unsigned short* latch, unsigned char pin) { set_led(latch,pin,0); }


void clear_led_port(unsigned short* latch);

unsigned char button_pressed(unsigned short* port, unsigned short button);

void set_seven_seg(unsigned short value);
void seven_seg_scan();
void clear_seven_seg();