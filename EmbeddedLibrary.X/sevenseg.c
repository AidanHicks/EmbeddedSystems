#include "lib.h"

#define DIG_ACTIVE_HIGH 1 // copy from 5C after testing
unsigned char DIG_MAP[4] = {0,1,2,3}; // copy from 5C after testing
#define SCAN_ON_US 900
#define BLANK_US 80
#define PRESS_TICKS 25
const unsigned char DIG_MASKS[4] = {0x01,0x02,0x04,0x08};
const unsigned char CC_DIGITS[10] =
{0x3F,0x06,0x5B,  0x4F   ,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

unsigned char seg_for(unsigned char d){ return CC_DIGITS[d%10u]; }
void all_off(void){ if (DIG_ACTIVE_HIGH) LATA=0x00; else LATA=0x0F; }
void enable_pos(unsigned char pos){
    unsigned char mask = DIG_MASKS[ DIG_MAP[pos] ];
    if (DIG_ACTIVE_HIGH) LATA = mask; else LATA = (unsigned char)(0x0F ^ mask);
}


unsigned char segments[4];

void set_seven_seg(unsigned short v) {
    ANSELA=0x00; ANSELD=0x00;
    TRISD=0x00;
    TRISA0 =0;
    TRISA1 =0;
    TRISA2 =0;
    TRISA3 =0;
    segments[0] = v % 10u;
    segments[1]  = (v/10u) % 10u;
    segments[2]  = (v/100u) % 10u;
    segments[3]  = (v/1000u) % 10u;
}

void seven_seg_scan(void){
        all_off(); LATD=0x00; __delay_us(BLANK_US); LATD=seg_for(segments[0]); enable_pos(0);
        __delay_us(SCAN_ON_US);
        all_off(); LATD=0x00; __delay_us(BLANK_US); LATD=seg_for(segments[1]); enable_pos(1);
        __delay_us(SCAN_ON_US);
        all_off(); LATD=0x00; __delay_us(BLANK_US); LATD=seg_for(segments[2]); enable_pos(2);
        __delay_us(SCAN_ON_US);
        all_off(); LATD=0x00; __delay_us(BLANK_US); LATD=seg_for(segments[3]); enable_pos(3);
        __delay_us(SCAN_ON_US);
}