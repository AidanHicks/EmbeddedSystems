#include "buzzer.h"
#include <xc.h>

// =====================
// Internal Variables
// =====================

static volatile uint16_t tone_counter_ms = 0;
static volatile uint16_t half_period_ticks = 0; // Timer2 counts for half-period
static volatile uint8_t buzzer_active = 0;

static const Melody *current_melody = 0;
static volatile uint8_t note_index = 0;
static volatile uint16_t note_duration_ms = 0;

// =====================
// Initialization
// =====================

void Buzzer_Init(void)
{
    // Configure pins
    BUZZER_ANSEL = 0;
    BUZZER_TRIS = 0;
    BUZZER_LAT = 0;

    // Timer2 for frequency generation (tone)
    T2CON = 0b00000111; // Prescaler 1:16, timer off
    PR2 = 0;

    // Timer1 for melody timing (~1ms tick)
    T1CON = 0b00110001; // Prescaler 1:8, Timer1 off
    TMR1 = 0;
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 0;

    buzzer_active = 0;
}

// =====================
// Helper: Start Tone
// =====================

void Buzzer_StartTone(uint16_t freq_hz, uint16_t duration_ms)
{
    if(freq_hz == 0) return;

    // Calculate half-period in Timer2 ticks
    // Fosc = 8MHz, instruction cycle = Fosc/4 = 2MHz => 0.5us per tick
    // Timer2 prescaler = 16 => tick = 8us
    uint32_t half_period_us = 500000UL / freq_hz; // half period in us
    half_period_ticks = (uint16_t)(half_period_us / 8);
    if(half_period_ticks == 0) half_period_ticks = 1;

    PR2 = half_period_ticks - 1;
    TMR2 = 0;
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    T2CONbits.TMR2ON = 1;

    tone_counter_ms = duration_ms;
    buzzer_active = 1;
}

// =====================
// Helper: Start Melody
// =====================

void Buzzer_StartMelody(const Melody *mel)
{
    if(mel == 0 || mel->length == 0) return;

    current_melody = mel;
    note_index = 0;
    note_duration_ms = current_melody->durations[0];

    // Notes are stored as period in microseconds
    uint16_t freq = (current_melody->notes[0] > 0) ? 1000000UL / current_melody->notes[0] : 0;
    Buzzer_StartTone(freq, note_duration_ms);

    // Enable Timer1 for melody progression (~1ms)
    TMR1 = 0;
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
    T1CONbits.TMR1ON = 1;
}

// =====================
// Stop Buzzer
// =====================

void Buzzer_Stop(void)
{
    T2CONbits.TMR2ON = 0;
    PIE1bits.TMR2IE = 0;
    BUZZER_LAT = 0;
    buzzer_active = 0;

    // Stop melody
    current_melody = 0;
    note_index = 0;
    note_duration_ms = 0;

    // Disable Timer1
    T1CONbits.TMR1ON = 0;
    PIE1bits.TMR1IE = 0;
}

// =====================
// Timer2 ISR (Frequency Toggle)
// =====================

void Buzzer_ISR_FrequencyTick(void)
{
    if(!buzzer_active) return;

    // Toggle buzzer output
    BUZZER_LAT ^= 1;

    // Decrement tone duration (ms)
    if(tone_counter_ms > 0)
        tone_counter_ms--;

    // Stop tone if duration expired
    if(tone_counter_ms == 0 && current_melody == 0)
        Buzzer_Stop();
}

// =====================
// Timer1 ISR (Melody Timing)
// =====================

void Buzzer_ISR_MelodyTick(void)
{
    if(current_melody == 0) return;

    if(note_duration_ms > 0)
        note_duration_ms--;

    // Advance to next note when current note duration expires
    if(note_duration_ms == 0)
    {
        note_index++;
        if(note_index < current_melody->length)
        {
            note_duration_ms = current_melody->durations[note_index];
            uint16_t freq = (current_melody->notes[note_index] > 0) ? 1000000UL / current_melody->notes[note_index] : 0;
            Buzzer_StartTone(freq, note_duration_ms);
        }
        else
        {
            // End of melody
            Buzzer_Stop();
        }
    }
}

