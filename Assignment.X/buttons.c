#include "buttons.h"
#include "config_bits.h"

// State tracking to ensure one press = one action
// 0 = Released, 1 = Pressed
unsigned char last_btn_up = 0;
unsigned char last_btn_down = 0;
unsigned char last_btn_enter = 0;
unsigned char last_btn_back = 0;

void Controls_Init(void) {
    TRISCbits.TRISC0 = 1; // Inputs
    TRISCbits.TRISC1 = 1;
    TRISCbits.TRISC2 = 1;
    TRISCbits.TRISC5 = 1;
    ANSELC = 0;
    TRISCbits.TRISC6 = 0; // Buzzer Output
    BUZZER_PIN = 0;
}

unsigned char Button_Read(void) {
    unsigned char result = 0;

    // --- UP BUTTON (RC0) ---
    if (BTN_UP) { // If pin is High (Pressed)
        if (last_btn_up == 0) { // And was previously Released
            __delay_ms(20); // Debounce delay
            if (BTN_UP) {   // Check again
                result = 1;
                last_btn_up = 1; // Mark as 'Held'
            }
        }
    } else {
        last_btn_up = 0; // Reset state when released
    }

    // --- DOWN BUTTON (RC1) ---
    if (BTN_DOWN) {
        if (last_btn_down == 0) {
            __delay_ms(20);
            if (BTN_DOWN) {
                result = 2;
                last_btn_down = 1;
            }
        }
    } else {
        last_btn_down = 0;
    }

    // --- ENTER BUTTON (RC2) ---
    if (BTN_ENTER) {
        if (last_btn_enter == 0) {
            __delay_ms(20);
            if (BTN_ENTER) {
                result = 3;
                last_btn_enter = 1;
            }
        }
    } else {
        last_btn_enter = 0;
    }
    
    // --- BACK BUTTON (RC5) ---
    if (BTN_BACK) {
        if (last_btn_back == 0) {
            __delay_ms(20);
            if (BTN_BACK) {
                result = 4;
                last_btn_back = 1;
            }
        }
    } else {
        last_btn_back = 0;
    }

    return result;
}

void Buzzer_Play(unsigned char tone_type) {
    unsigned int i;
    if(tone_type == 0) { // Tone A: High Pitch
        for(i=0; i<100; i++) { // Reduced length for faster UI response
            BUZZER_PIN = 1; __delay_us(200);
            BUZZER_PIN = 0; __delay_us(200);
        }
    } else { // Tone B: Warble
        for(i=0; i<50; i++) {
            BUZZER_PIN = 1; __delay_us(500);
            BUZZER_PIN = 0; __delay_us(500);
        }
         for(i=0; i<50; i++) {
            BUZZER_PIN = 1; __delay_us(300);
            BUZZER_PIN = 0; __delay_us(300);
        }
    }
}