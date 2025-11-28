#ifndef CONFIG_BITS_H
#define CONFIG_BITS_H

#include <xc.h>

// CONFIG1H
#pragma config FOSC = INTIO67   // Internal oscillator block
#pragma config PLLCFG = OFF     // PLL disabled
#pragma config PRICLKEN = ON    // Primary clock enabled
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF       // Oscillator Switchover mode disabled

// CONFIG2L
#pragma config PWRTEN = ON      // Power-up Timer enabled
#pragma config BOREN = SBORDIS  // Brown-out Reset enabled in hardware
#pragma config BORV = 190       // Brown-out Reset Voltage

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer disabled

// CONFIG3H
#pragma config CCP2MX = PORTC1  // CCP2 input/output is multiplexed with RC1
#pragma config PBADEN = OFF     // PORTB<5:0> pins are configured as digital I/O
#pragma config CCP3MX = PORTB5  // P3A/CCP3 input/output is multiplexed with RB5
#pragma config HFOFST = ON      // HFINTOSC output and ready status are not delayed
#pragma config T3CMX = PORTC0   // T3CKI is on RC0
#pragma config P2BMX = PORTD2   // P2B is on RD2
#pragma config MCLRE = EXTMCLR  // MCLR pin enabled

#define _XTAL_FREQ 8000000      // 8MHz Clock

// Global Structure for Settings
typedef struct {
    unsigned char temp_low;
    unsigned char temp_high;
    unsigned char heat_start_hour;
    unsigned char heat_start_min;
    unsigned char heat_end_hour;
    unsigned char heat_end_min;
    unsigned char alarm_hour;
    unsigned char alarm_min;
    unsigned char alarm_enabled; // 1 = ON, 0 = OFF
    unsigned char alarm_tone;    // 0 = Tone A, 1 = Tone B
} SystemSettings;

#endif