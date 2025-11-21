/*
 * File:   buzzer.h
 * Author: 771028
 *
 * Created on November 21, 2025, 2:44 PM
 */

#ifndef BUZZER_H
#define BUZZER_H

#include <xc.h>
#include <stdint.h>

#define BUZZER_LAT LATCbits.LATC2
#define BUZZER_TRIS TRISCbits.TRISC2
#define BUZZER_ANSEL ANSELCbits.ANSC2

typedef struct {
    const uint16_t *notes;    //Array of note periods (us)
    const uint16_t *durations; //Array of note durations (ms)
    uint8_t length;           //Number of notes
} Melody;

//Control Functions
void Buzzer_Init(void);
void Buzzer_StartTone(uint16_t frequency, uint16_t duration_ms);
void Buzzer_StartMelody(const Melody *mel);
void Buzzer_Stop(void);

//Must be called from interrupts
void Buzzer_ISR_FrequencyTick(void);
void Buzzer_ISR_MelodyTick(void);

#endif