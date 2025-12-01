#include "buttons.h"
#include "config_bits.h"

unsigned char last_btn_up = 0;
unsigned char last_btn_down = 0;
unsigned char last_btn_enter = 0;
unsigned char last_btn_back = 0;
unsigned long last_check = 0;


void Controls_Init(void) {
    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;
    TRISCbits.TRISC2 = 1;
    TRISCbits.TRISC5 = 1;
    ANSELC = 0;
    TRISCbits.TRISC6 = 0;
    BUZZER_PIN = 0;
}


unsigned char Get_Button(unsigned short button, unsigned char* last) {
    static unsigned char button_down = 0;
    if ((system_ticks - last_check) < 10) return 0;
    
    if (button) {
        if (*last == 0) {
            if (button) {
                *last = 1;
                last_check = system_ticks;
                return 1;
            }
        }
    } else {
        *last = 0;
    }
    return 0;
}

unsigned char Button_Read(void) {
    unsigned char result = 0;

    if (Get_Button(BTN_UP, &last_btn_up)) {
        return 1;
    } 

    if (Get_Button(BTN_DOWN, &last_btn_down)) {
        return 2;
    }

    if (Get_Button(BTN_ENTER, &last_btn_enter))
    {
        return 3;
    }
    
    if (Get_Button(BTN_BACK, &last_btn_back)) {
        return 4;
    }

    return result;
}
