/*
 * File:   buzzer.h
 * Author: 771028
 *
 * Created on November 21, 2025, 2:44 PM
 */

#ifndef BUZZER_H
#define BUZZER_H

#include <xc.h>

// Buzzer on RE1
#define BUZZER_LAT LATEbits.LATE1
#define BUZZER_TRIS TRISEbits.TRISE1
#define BUZZER_ANSEL ANSELEbits.ANSE1

typedef struct {
    const unsigned short *notes;    // Array of note frequencies (Hz)
    const unsigned short *durations; // Array of note durations (ms)
    unsigned char length;           // Number of notes
} Melody;

// Control Functions
void Buzzer_Init(void);
void Buzzer_StartTone(unsigned short freq, unsigned short duration_ms);
void Buzzer_StartMelody(const Melody *mel);
void Buzzer_Stop(void);
void Buzzer_Play(unsigned char tone_index); // 0=Alarm, 1=Beep

// New function to check status
unsigned char Buzzer_IsPlaying(void);

// Must be called from interrupts
void Buzzer_ISR_FrequencyTick(void); // Call in Timer 2
void Buzzer_ISR_MelodyTick(void);    // Call in Timer 1

#endif