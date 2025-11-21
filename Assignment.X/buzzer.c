/*
 * File:   buzzer.c
 * Author: 771028
 *
 * Created on November 21, 2025, 2:44 PM
 */

#include "buzzer.h"

static volatile uint16_t tone_timer = 0;
static volatile uint16_t melody_timer = 0;

static const Melody *currentMelody = 0;
static uint8_t noteIndex = 0;
static uint8_t buzzerActive = 0;

void Buzzer_Init(void)
{
    BUZZER_ANSEL = 0;
    BUZZER_TRIS = 0;
    BUZZER_LAT = 0;
    
    //Timer 2 for frequency generation
    T2CON = 0b00000111; //Prescaler 1:16; Timer off initially
    PR2 = 200;          //Placeholder
    
    //Timer 1 for melody timing
    T1CON = 0b00110001; //Prescaler 1:8; Timer1 ON
    TMR1 = 0;
    PIE1bits.TMR1IE - 1;
    
    buzzerActive = 0;
}

void Buzzer_StartTone(uint16_t freq, uint16_t duration)
{
    uint16_t period_us = 1000000UL / freq;
    
    //Set Timer2 to interrupt at half-period
    PR2 = (period_us / 2) - 1;
    
    tone_timer = duration;
    melody_timer = 0;
    currentMelody = 0;
    noteIndex = 0;
    buzzerActive = 1;
    
    TMR2 = 0;
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    T2CONbits.TMR2ON = 1;
}

void Buzzer_StartMelody(const Melody *mel)
{
   currentMelody = mel;
   noteIndex = 0;
   
   Buzzer_StartTone(1000000 / mel->notes[0], mel->durations[0]);
}

void Buzzer_Stop(void)
{
    T2CONbits.TMR2ON = 0;
    PIE1bits.TMR2IE = 0;
    
    BUZZER_LAT = 0;
    buzzerActive = 0;
}

/* Called inside Timer2 interrupt*/
void Buzzer_ISR_FrequencyTick(void)
{
    BUZZER_LAT ^= 1;
    
    if (tone_timer > 0)
        tone_timer--;
    else if (currentMelody)
        melody_timer = 1;    // Signal melody timer
    else
        Buzzer_Stop();
}

/* Called inside Timer1 interrupt*/
void Buzzer_ISR_MelodyTick(void)
{
    if (melody_timer) return;
    
    melody_timer = 0;
    noteIndex++;
    
    if (noteIndex < currentMelody->length)
    {
       uint16_t period = currentMelody->notes[noteIndex];
       uint16_t  duration = currentMelody->durations[noteIndex];
       Buzzer_StartTone(1000000 / period, duration);
    }
    else
    {
        Buzzer_Stop();
    }
}
