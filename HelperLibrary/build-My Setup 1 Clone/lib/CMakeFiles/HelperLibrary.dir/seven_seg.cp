#line 1 "C:/Users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/lib/seven_seg.c"
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
#line 2 "C:/Users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/lib/seven_seg.c"


unsigned char DIG_MAP[4] = {3,2,1,0};



const unsigned char DIG_MASKS[4] = {0x01,0x02,0x04,0x08};
const unsigned char CC_DIGITS[10] =
{0x3F,0x06,0x5B,  0x4F   ,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

unsigned char seg_for(unsigned char d){ return CC_DIGITS[d%10u]; }
void all_off(void){ if ( 1 ) LATA=0x00; else LATA=0x0F; }
void enable_pos(unsigned char pos){
    unsigned char mask = DIG_MASKS[ DIG_MAP[pos] ];
    if ( 1 ) LATA = mask; else LATA = (unsigned char)(0x0F ^ mask);
}


unsigned char segments[4];

void set_seven_seg(unsigned short v) {
    ANSELA=0x00; ANSELD=0x00;
    TRISD=0x00;
    TRISA0_bit=0; TRISA1_bit=0; TRISA2_bit=0; TRISA3_bit=0;
    segments[0] = v % 10u;
    segments[1]  = (v/10u) % 10u;
    segments[2]  = (v/100u) % 10u;
    segments[3]  = (v/1000u) % 10u;
}

void seven_seg_scan(void){
        all_off(); LATD=0x00; Delay_us( 80 ); LATD=seg_for(segments[0]); enable_pos(0);
        Delay_us( 900 );
        all_off(); LATD=0x00; Delay_us( 80 ); LATD=seg_for(segments[1]); enable_pos(1);
        Delay_us( 900 );
        all_off(); LATD=0x00; Delay_us( 80 ); LATD=seg_for(segments[2]); enable_pos(2);
        Delay_us( 900 );
        all_off(); LATD=0x00; Delay_us( 80 ); LATD=seg_for(segments[3]); enable_pos(3);
        Delay_us( 900 );
}
