/*
 * File:   main.c
 * Author: 771028
 *
 * Created on October 24, 2025, 3:26 PM
 */

#include <xc.h>
#define _XTAL_FREQ 8000000  // Set to your actual oscillator frequency
#define SCAN_ON_US 900
#define BLANK_US 80

// task 1
#define BUZZER LATCbits.LATC2

void basicBuzzer() {
    
    ANSELC = 0x00;
    TRISC = 0x00;
    TRISCbits.TRISC2 = 0;
    BUZZER = 0;

    __delay_ms(20);
    
    while (1) {
        //LATCbits.LATC2 = 1;
        BUZZER = ~BUZZER;
        __delay_ms(10);
        //LATCbits.LATC2 = 0;
        BUZZER = ~BUZZER;
        __delay_ms(10);
    }
}


//task 2
void birthdayTune (){
    unsigned int j;
    
    PORTC = 0x00;
    TRISCbits.TRISC2 = 0;
    LATCbits.LATC2 = 0;
    
    __delay_ms(20);
    
    while (1) {
        //Note 1: G4
        for (j = 0; j < 98; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(1276);
            LATCbits.LATC2 = 0;
            __delay_us(1276);
        }
        __delay_ms(50);
        
        // Note 2: G4 (250 ms)
        for (j = 0; j < 98; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(1276);
            LATCbits.LATC2 = 0;
            __delay_us(1276);
        }
        __delay_ms(50);
        // Note 3: A4 (500 ms)
        for (j = 0; j < 220; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(1136);
            LATCbits.LATC2 = 0;
            __delay_us(1136);
        }
        __delay_ms(50);
        // Note 4: G4 (500 ms)
        for (j = 0; j < 196; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(1276);
            LATCbits.LATC2 = 0;
            __delay_us(1276);
        }
        __delay_ms(50);
        // Note 5: C5 (500 ms)
        for (j = 0; j < 262; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(956);
            LATCbits.LATC2 = 0;
            __delay_us(956);
        }
        __delay_ms(50);
        // Note 6: B4 (1000 ms)
        for (j = 0; j < 494; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(1012);
            LATCbits.LATC2 = 0;
            __delay_us(1012);
        }
        __delay_ms(50);
        // Note 7: G4 (250 ms)
        for (j = 0; j < 98; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(1276);
            LATCbits.LATC2 = 0;
            __delay_us(1276);
        }
        __delay_ms(50);
        // Note 8: G4 (250 ms)
        for (j = 0; j < 98; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(1276);
            LATCbits.LATC2 = 0;
            __delay_us(1276);
        }
        __delay_ms(50);
        // Note 9: A4 (500 ms)
        for (j = 0; j < 220; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(1136);
            LATCbits.LATC2 = 0;
            __delay_us(1136);
        }
        __delay_ms(50);
        // Note 10: G4 (500 ms)
        for (j = 0; j < 196; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(1276);
            LATCbits.LATC2 = 0;
            __delay_us(1276);
        }
        __delay_ms(50);
        // Note 11: D5 (500 ms)
        for (j = 0; j < 293; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(852);
            LATCbits.LATC2 = 0;
            __delay_us(852);
        }
        __delay_ms(50);
        // Note 12: C5 (1000 ms)
        for (j = 0; j < 524; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(956);
            LATCbits.LATC2 = 0;
            __delay_us(956);
        }
        __delay_ms(50);
        // Note 13: G4 (250 ms)
        for (j = 0; j < 98; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(1276);
            LATCbits.LATC2 = 0;
            __delay_us(1276);
        }
        __delay_ms(50);
        // Note 14: G4 (250 ms)
        for (j = 0; j < 98; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(1276);
            LATCbits.LATC2 = 0;
            __delay_us(1276);
        }
        __delay_ms(50);
        // Note 15: G5 (500 ms)
        for (j = 0; j < 392; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(638);
            LATCbits.LATC2 = 0;
            __delay_us(638);
        }
        __delay_ms(50);
        // Note 16: E5 (500 ms)
        for (j = 0; j < 329; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(759);
            LATCbits.LATC2 = 0;
            __delay_us(759);
        }
        __delay_ms(50);
        // Note 17: C5 (500 ms)
        for (j = 0; j < 262; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(956);
            LATCbits.LATC2 = 0;
            __delay_us(956);
        }
        __delay_us(50);
        // Note 18: B4 (500 ms)
        for (j = 0; j < 247; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(1012);
            LATCbits.LATC2 = 0;
            __delay_us(1012);
        }
        __delay_ms(50);
        // Note 19: A4 (1000 ms)
        for (j = 0; j < 441; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(1136);
            LATCbits.LATC2 = 0;
            __delay_us(1136);
        }
        __delay_ms(50);
        // Note 20: F5 (250 ms)
        for (j = 0; j < 188; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(664);
            LATCbits.LATC2 = 0;
            __delay_us(664);
        }
        __delay_ms(50);
        // Note 21: F5 (250 ms)
        for (j = 0; j < 188; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(664);
            LATCbits.LATC2 = 0;
            __delay_us(664);
        }
        __delay_ms(50);
        // Note 22: E5 (500 ms)
        for (j = 0; j < 329; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(759);
            LATCbits.LATC2 = 0;
            __delay_us(759);
        }
        __delay_ms(50);
        // Note 23: C5 (500 ms)
        for (j = 0; j < 262; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(956);
            LATCbits.LATC2 = 0;
            __delay_us(956);
        }
        __delay_ms(50);
        // Note 24: D5 (500 ms)
        for (j = 0; j < 293; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(852);
            LATCbits.LATC2 = 0;
            __delay_us(852);
        }
        __delay_ms(50);
        // Note 25: C5 (1000 ms)
        for (j = 0; j < 524; j++) {
            LATCbits.LATC2 = 1;
            __delay_us(956);
            LATCbits.LATC2 = 0;
            __delay_us(956);
        }
        // Pause before repeating the melody
        __delay_ms(2000);
    }
}

//task 3

void buttonBuzzer () {
    unsigned int i;
    
    TRISCbits.TRISC2 = 0; //RC2 as output 
    TRISBbits.TRISB0 = 1; //RB0 as input
    TRISBbits.TRISB1 = 1; // RB1 as input
    
    LATCbits.LATC2 = 0;
    ANSELB = 0;
    
    // Enable PORTB pull-ups
    
    while (1) {
        if (PORTBbits.RB0 == 0) {  //RB0 pressed
            for (i = 0; i < 200; i++) {
                LATCbits.LATC2 = 1;
                __delay_us(1276);
                LATCbits.LATC2 = 0;
                __delay_us(1276);
            }
            __delay_ms(100);
        }
        
        if (PORTBbits.RB1 == 0) { //RB1 pressed
            for (i = 0; i < 200; i++) {
                LATCbits.LATC2 = 1;
                __delay_us(956);
                LATCbits.LATC2 = 0;
                __delay_us(956);
            }
            __delay_ms(100);
        }
    }
}

//task 4

void dutyExperiment () {
    unsigned int duty;
    
    TRISCbits.TRISC2 = 0;
    
    //Set PWM frequency to 1.5 kHz using CCP1
    PR2 = 0xCF; // ~1.5 kHz at 8Mhz clock
    CCP1CON = 0b00001100; //PWM mode
    T2CON = 0b00000101; // Timer2 on, prescaler = 4
    
    while (1) {
        duty = (PR2 + 1) * 10 / 100 * 4; // 10%
        CCPR1L = duty >> 2;
        CCP1CON = (CCP1CON & 0xCF) | ((duty & 0x03) << 4);
        __delay_ms(1000);
        
        duty = (PR2 + 1) * 30 / 100 * 4; // 30%
        CCPR1L = duty >> 2;
        CCP1CON = (CCP1CON & 0xCF) | ((duty & 0x03) << 4);
        __delay_ms(1000);
        
        duty = (PR2 + 1) * 50 / 100 * 4; // 50%
        CCPR1L = duty >> 2;
        CCP1CON = (CCP1CON & 0xCF) | ((duty & 0x03) << 4);
        __delay_ms(1000);
        
        duty = (PR2 + 1) * 70 / 100 * 4; // 70%
        CCPR1L = duty >> 2;
        CCP1CON = (CCP1CON & 0xCF) | ((duty & 0x03) << 4);
        __delay_ms(1000);
        
        duty = (PR2 + 1) * 90 / 100 * 4; // 90%
        CCPR1L = duty >> 2;
        CCP1CON = (CCP1CON & 0xCF) | ((duty & 0x03) << 4);
        __delay_ms(2000);
    }
}



