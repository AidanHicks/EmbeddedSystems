#include "sevenseg.h"

// Segment patterns for 0-9 (Common Cathode)
const unsigned char SEG_MAP[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

// Digits buffer
volatile unsigned char digits[4] = {0, 0, 0, 0};
volatile unsigned char current_digit = 0;

void SevenSeg_Init(void) {
    TRISD = 0x00; // Segments Output
    TRISA &= 0xF0; // RA0-RA3 Digits Output
    
    LATA &= 0xF0; // Turn off digits initially
    LATD = 0x00;
    
    // Timer0 Setup for 2ms Interrupt (500Hz) -> 125Hz Refresh Rate
    // Fosc = 8MHz, Fcy = 2MHz (0.5us tick)
    // Prescaler 1:16 -> Timer Tick = 8us
    // Count needed for 2ms: 2000us / 8us = 250 counts
    // Preload = 65536 - 250 = 65286 = 0xFF06
    
    T0CONbits.T08BIT = 0; // 16-bit
    T0CONbits.T0CS = 0;   // Internal clock
    T0CONbits.PSA = 0;    // Prescaler assigned
    T0CONbits.T0PS = 3;   // 1:16 Prescaler
    
    TMR0H = 0xFF;         
    TMR0L = 0x06;
    
    INTCONbits.TMR0IE = 1;
    T0CONbits.TMR0ON = 1;
}

void SevenSeg_Update_Value(unsigned int number) {
    if(number > 9999) number = 9999;
    
    digits[0] = SEG_MAP[number / 1000];       // Thousands
    digits[1] = SEG_MAP[(number / 100) % 10]; // Hundreds
    digits[2] = SEG_MAP[(number / 10) % 10];  // Tens
    digits[3] = SEG_MAP[number % 10];         // Ones
}

void SevenSeg_ISR_Handler(void) {
    LATA &= 0xF0; // Turn off all digits (RA0-RA3)
    
    // FIX: Reverse mapping for EasyPIC v7
    LATD = digits[3 - current_digit]; 
    
    // Turn on specific digit
    switch(current_digit) {
        case 0: LATA |= 0x01; break; // RA0 (Rightmost Digit)
        case 1: LATA |= 0x02; break; // RA1
        case 2: LATA |= 0x04; break; // RA2
        case 3: LATA |= 0x08; break; // RA3 (Leftmost Digit)
    }
    
    current_digit++;
    if(current_digit > 3) current_digit = 0;
    
    // Reload Timer for 2ms
    TMR0H = 0xFF;
    TMR0L = 0x06;
    INTCONbits.TMR0IF = 0;
}