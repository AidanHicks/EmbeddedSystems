/*
 * File:   buzzer.c
 * Author: 771028
 *
 * Created on November 21, 2025, 2:44 PM
 */

#include "buzzer.h"

// Volatile counters for ISR
static volatile unsigned short tone_timer = 0;
static volatile unsigned char melody_timer_flag = 0;

static const Melody *currentMelody = 0;
static unsigned char noteIndex = 0;
static unsigned char buzzerActive = 0;

// --- Melody Definitions (using unsigned short) ---
const unsigned short alarm_notes[] = { 2000, 0, 2000, 0, 2000, 0 };
const unsigned short alarm_durs[]  = { 200, 100, 200, 100, 200, 1000 };
const Melody melody_alarm = { alarm_notes, alarm_durs, 6 };

const unsigned short beep_notes[] = { 3000 };
const unsigned short beep_durs[]  = { 100 };
const Melody melody_beep = { beep_notes, beep_durs, 1 };

void Buzzer_Init(void)
{
    // Configure RE1 as Digital Output
    BUZZER_ANSEL = 0;
    BUZZER_TRIS = 0;
    BUZZER_LAT = 0;
    
    // --- Timer 2 Setup (Frequency Generation) ---
    // Fosc = 8MHz -> Fcy = 2MHz (0.5us per tick)
    // Prescaler 1:16 -> TMR2 tick = 8us
    T2CON = 0b00000111; // Prescaler 1:16; Timer off initially (bit 2 is TMR2ON)
    T2CONbits.TMR2ON = 0;
    PR2 = 0;          
    
    // --- Timer 1 Setup (Duration Timing) ---
    // Prescaler 1:8 -> TMR1 tick = 4us
    // 1ms = 250 ticks
    T1CON = 0b00110001; // Prescaler 1:8; Timer1 ON; 16-bit
    TMR1 = 0;
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1; // Enable Timer 1 Interrupt
    
    buzzerActive = 0;
}

void Buzzer_StartTone(unsigned short freq, unsigned short duration)
{
    if (freq == 0) {
        // Rest (silence)
        T2CONbits.TMR2ON = 0;
        BUZZER_LAT = 0;
    } else {
        // Calculate PR2 for 8MHz Clock (2MHz Fcy) with 1:16 Prescaler
        // Tick = 8us.
        // We need to toggle every Half-Period.
        // Formula: PR2 = (1,000,000 / (freq * 2 * 8)) - 1 = (62500 / freq) - 1
        
        unsigned long pr2_calc = 62500 / freq;
        if(pr2_calc > 0) pr2_calc--;
        if(pr2_calc > 255) pr2_calc = 255; 
        
        PR2 = (unsigned char)pr2_calc;
        
        TMR2 = 0;
        PIR1bits.TMR2IF = 0;
        PIE1bits.TMR2IE = 1; // Enable TMR2 Interrupt
        T2CONbits.TMR2ON = 1; // Start TMR2
    }
    
    tone_timer = duration; 
    
    // Reset Timer 1 for 1ms
    // 65536 - 250 = 65286 (0xFF06)
    TMR1H = 0xFF;
    TMR1L = 0x06;
    
    buzzerActive = 1;
}

void Buzzer_StartMelody(const Melody *mel)
{
   currentMelody = mel;
   noteIndex = 0;
   Buzzer_StartTone(mel->notes[0], mel->durations[0]);
}

void Buzzer_Stop(void)
{
    T2CONbits.TMR2ON = 0;
    PIE1bits.TMR2IE = 0; // Disable TMR2 Interrupt
    BUZZER_LAT = 0;
    buzzerActive = 0;
    currentMelody = 0;
}

void Buzzer_Play(unsigned char tone_index) {
    // 0 = Alarm, 1 = Beep
    if(tone_index == 0) Buzzer_StartMelody(&melody_alarm);
    else Buzzer_StartMelody(&melody_beep);
}

unsigned char Buzzer_IsPlaying(void) {
    return buzzerActive;
}

// --- Interrupt Service Routines ---

/* Called inside Timer2 interrupt (Frequency Gen) - Toggles Pin */
void Buzzer_ISR_FrequencyTick(void)
{
    BUZZER_LAT ^= 1;
}

/* Called inside Timer1 interrupt (Duration Timing ~1ms) */
void Buzzer_ISR_MelodyTick(void)
{
    // Reload Timer 1 for 1ms (250 ticks at 4us/tick)
    TMR1H = 0xFF;
    TMR1L = 0x06;
    
    if (buzzerActive) {
        if (tone_timer > 0) {
            tone_timer--;
        } else {
            // Tone finished, check if melody continues
            if (currentMelody) {
                noteIndex++;
                if (noteIndex < currentMelody->length) {
                    Buzzer_StartTone(currentMelody->notes[noteIndex], currentMelody->durations[noteIndex]);
                } else {
                    Buzzer_Stop();
                }
            } else {
                Buzzer_Stop();
            }
        }
    }
}