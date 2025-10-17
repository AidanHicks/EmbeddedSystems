/**
 * @file main.c
 * @brief Main function for Lab4 application.
 */

/**
 * Any initialization code needed for MCU to function properly.
 * Do not remove this line or clock might not be set correctly.
 */

 /* TASk 2 START */
const unsigned char CC_DIGITS[10] =
{0x3F,0x06,0x5B,  0x4F   ,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
void numCycle() {
    unsigned char d;
    ANSELD = 0x00; ANSELA = 0x00; // digital
    TRISD = 0x00; // segments out
    TRISA0_bit = 0; // digit 0 enable out
    LATA = 0x00; // all digits off
    LATA.B0 = 1;

    while(1) {
        for (d = 0; d < 10; d++) {
            LATD = CC_DIGITS[d];
            Delay_ms(400);
        }
    }
}
/* TASL 2 END*/

/* TASk 1 START */
void fixedDigit() {
    unsigned char seg;
    ANSELD = 0x00; ANSELA = 0x00; // digital
    TRISD = 0x00; // segments out
    TRISA0_bit = 0; // digit 0 enable out
    LATA = 0x00; // all digits off
    seg = CC_DIGITS[3];
    LATD = seg; // show 3
    LATA.B0 = 1; // enable digit 0
    while(1) { }
}
/* TASk 1 end */


/*TASK 3*/
#define DEBOUNCE_MS 30
unsigned char seg_from_digit(unsigned char d) {
    return CC_DIGITS[d % 10u];
}
unsigned char pressed_falling_RB0(unsigned char *prev) {
    unsigned char now = RB0_bit;
    if (*prev == 1u && now == 0u) {
        Delay_ms(DEBOUNCE_MS);
        if (RB0_bit == 0u) { *prev = 0u; return 1u; }
    }
    *prev = now;
    return 0u;
}
unsigned char pressed_falling_RB1(unsigned char *prev) {
    unsigned char now = RB1_bit;
    if (*prev == 1u && now == 0u) {
        Delay_ms(DEBOUNCE_MS);
        if (RB1_bit == 0u) { *prev = 0u; return 1u; }
    }
    *prev = now;
    return 0u;
}

void show_digit(unsigned char d) {
    LATD = seg_from_digit(d);
}

void buttonPress() {
    unsigned char value = 0;
    unsigned char prev0 = 1, prev1 = 1;
    ANSELB = 0x00; ANSELD = 0x00; ANSELA = 0x00;
    TRISB0_bit = 1; TRISB1_bit = 1;
    TRISD = 0x00; TRISA0_bit = 0;
    LATA = 0x00; LATA.B0 = 1;
    show_digit(value);
    while(1) {
        if (pressed_falling_RB0(&prev0)) { value = (value + 1u) % 10u; show_digit(value); }
        if (pressed_falling_RB1(&prev1)) { value = (value + 9u) % 10u; show_digit(value); }
    }
}

/* TASK 3 END*/


/* TASK 4*/
#define DIG_ACTIVE_HIGH 1 // copy from 5C after testing
unsigned char DIG_MAP[4] = {0,1,2,3}; // copy from 5C after testing
#define SCAN_ON_US 900
#define BLANK_US 80
#define PRESS_TICKS 25
const unsigned char DIG_MASKS[4] = {0x01,0x02,0x04,0x08};
unsigned char seg_for(unsigned char d){ return CC_DIGITS[d%10u]; }
void all_off(void){ if (DIG_ACTIVE_HIGH) LATA=0x00; else LATA=0x0F; }
void enable_pos(unsigned char pos){
    unsigned char mask = DIG_MASKS[ DIG_MAP[pos] ];
    if (DIG_ACTIVE_HIGH) LATA = mask; else LATA = (unsigned char)(0x0F ^ mask);
}
void split4(unsigned int v, unsigned char *u, unsigned char *t,
    unsigned char *h, unsigned char *th){
    *u = v % 10u;
    *t = (v/10u) % 10u;
    *h = (v/100u) % 10u;
    *th = (v/1000u) % 10u;
}
void main(void){
    unsigned int value=1234;
    unsigned char u=0,t=0,h=0,th=0;
    unsigned char pos=0;
    unsigned char up_ticks=0, dn_ticks=0;
    unsigned char up_lat=0, dn_lat=0;
    ANSELA=0x00; ANSELB=0x00; ANSELD=0x00;
    TRISD=0x00;
    TRISA0_bit=0; TRISA1_bit=0; TRISA2_bit=0; TRISA3_bit=0;
    TRISB0_bit=1; TRISB1_bit=1;
    LATD=0x00; all_off();
    while(1){
        if(pos==0u) split4(value,&u,&t,&h,&th);
        all_off(); LATD=0x00; Delay_us(BLANK_US);
        if (pos==0u) LATD=seg_for(u);
        else if (pos==1u) LATD=seg_for(t);
        else if (pos==2u) LATD=seg_for(h);
        else LATD=seg_for(th);
        enable_pos(pos);
        Delay_us(SCAN_ON_US);
        all_off(); LATD=0x00; Delay_us(BLANK_US);
        pos++; if(pos>=4u) pos=0u;
        // non-blocking debounce for RB0 and RB1, active-low
        if (!up_lat){
            if (RB0_bit==0u){
                if (up_ticks<PRESS_TICKS) up_ticks++;
                if (up_ticks==PRESS_TICKS){ value=(value+1u)%10000u; up_lat=1u; }
            } else up_ticks=0u;
        } else {
            if (RB0_bit==1u){ up_lat=0u; up_ticks=0u; }
        }
        if (!dn_lat){
            if (RB1_bit==0u){
                if (dn_ticks<PRESS_TICKS) dn_ticks++;
                if (dn_ticks==PRESS_TICKS){ value=(value+9999u)%10000u; dn_lat=1u; }
            } else dn_ticks=0u;
        } else {
            if (RB1_bit==1u){ dn_lat=0u; dn_ticks=0u; }
        }
    }
}
/* TASK 4 END*/