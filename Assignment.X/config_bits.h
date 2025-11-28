#ifndef CONFIG_BITS_H
#define CONFIG_BITS_H

#include <xc.h>


#define _XTAL_FREQ 8000000      // 8MHz Clock

inline volatile unsigned long system_ticks = 0;

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