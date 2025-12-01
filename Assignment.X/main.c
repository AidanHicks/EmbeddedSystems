#include "config_bits.h"
#include "lm35.h"
#include "sevenseg.h"
#include "lcd.h"
#include "i2c.h"
#include "rtc.h"
#include "eeprom_cfg.h"
#include "buttons.h"
#include "buzzer.h"
#include <stdio.h>

#define HEATER_LED LATEbits.LATE0

// Globals
SystemSettings settings;
Time_t current_time;
Date_t current_date;
unsigned int current_temp_raw;
unsigned int current_temp_c;
unsigned char heater_state = 0;
unsigned char alarm_active = 0;

// Menu States
typedef enum {
    VIEW_MAIN,
    MENU_SET_TEMP_LOW,
    MENU_SET_TEMP_HIGH,
    MENU_SET_HEAT_START,
    MENU_SET_HEAT_END,
    MENU_SET_ALARM,
    MENU_SET_TONE,
    MENU_SAVE
} MenuState;

MenuState current_state = VIEW_MAIN;
MenuState last_drawn_state = 255; 

void __interrupt() ISR(void) {
    // --- Timer 0 (System Ticks / Seven Seg) ---
    if(INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0; 
        SevenSeg_ISR_Handler();
        system_ticks++; 
    }
    
    // --- Timer 1 (Buzzer Duration ~1ms) ---
    if(PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;
        Buzzer_ISR_MelodyTick();
    }

    // --- Timer 2 (Buzzer Frequency) ---
    if(PIR1bits.TMR2IF) {
        PIR1bits.TMR2IF = 0;
        Buzzer_ISR_FrequencyTick();
    }
}

void Check_Heating_Logic() {
    unsigned int now_mins = current_time.hour * 60 + current_time.minute;
    unsigned int start_mins = settings.heat_start_hour * 60 + settings.heat_start_min;
    unsigned int end_mins = settings.heat_end_hour * 60 + settings.heat_end_min;
    
    unsigned char in_window = 1;
    if(start_mins < end_mins) {
        if(now_mins >= start_mins && now_mins < end_mins) in_window = 1;
    } else { // goes over the night
        if(now_mins >= start_mins || now_mins < end_mins) in_window = 1;
    }

    if(in_window) {
        if(current_temp_c/100 < settings.temp_low) heater_state = 1;
        else if(current_temp_c/100 >= settings.temp_high) heater_state = 0;
    } else {
        heater_state = 0;
    }
    
    HEATER_LED = heater_state;
}

void Handle_Alarm() {
    static unsigned char alarm_triggered_this_minute = 0;
    
    if(!settings.alarm_enabled) return;
    
    // Check start condition
    if (current_time.second < 2) {
        // Trigger alarm at exact second 00 if it matches setting
        if (current_time.hour == settings.alarm_hour && 
            current_time.minute == settings.alarm_min) {
            
            // Only trigger if we haven't already done so this minute
            if (!alarm_triggered_this_minute && !alarm_active) {
                alarm_active = 1;
                alarm_triggered_this_minute = 1; // Latch to prevent re-triggering immediately after cancel
            }
        }
    } else {
        // Reset the latch once we leave the 0th second
        alarm_triggered_this_minute = 0; 
    }
    
    // Loop logic: If active, ensure buzzer keeps playing
    if(alarm_active) {
        if(!Buzzer_IsPlaying()) {
             Buzzer_Play(settings.alarm_tone);
        }
    }
}

void Update_Display() {
    if (current_state != last_drawn_state) {
        LCD_Clear();
        last_drawn_state = current_state;
    }

    char buffer[17];
    
    switch(current_state) {
        case VIEW_MAIN:
            sprintf(buffer, "%02d:%02d %02d/%02d/%d", current_time.hour, current_time.minute, current_date.day, current_date.month, current_date.year);
            LCD_Set_Cursor(1,0); LCD_String(buffer);
            
            sprintf(buffer, "ALARM: %02d:%02d", settings.alarm_hour, settings.alarm_min);
            LCD_Set_Cursor(2,0); LCD_String(buffer);
            break;
            
        case MENU_SET_TEMP_LOW:
            LCD_Set_Cursor(1,0); LCD_String("Set Temp LOW:   ");
            sprintf(buffer, "%d C        ", settings.temp_low); 
            LCD_Set_Cursor(2,0); LCD_String(buffer);
            break;

        case MENU_SET_TEMP_HIGH:
            LCD_Set_Cursor(1,0); LCD_String("Set Temp HIGH:  ");
            sprintf(buffer, "%d C        ", settings.temp_high);
            LCD_Set_Cursor(2,0); LCD_String(buffer);
            break;
            
        case MENU_SET_HEAT_START:
            LCD_Set_Cursor(1,0); LCD_String("Heat Start Time:");
            sprintf(buffer, "%02d:%02d           ", settings.heat_start_hour, settings.heat_start_min);
            LCD_Set_Cursor(2,0); LCD_String(buffer);
            break;
            
        case MENU_SET_HEAT_END:
            LCD_Set_Cursor(1,0); LCD_String("Heat End Time:  ");
            sprintf(buffer, "%02d:%02d           ", settings.heat_end_hour, settings.heat_end_min);
            LCD_Set_Cursor(2,0); LCD_String(buffer);
            break;
            
        case MENU_SET_ALARM:
            LCD_Set_Cursor(1,0); LCD_String("Set Alarm Time: ");
            sprintf(buffer, "%02d:%02d           ", settings.alarm_hour, settings.alarm_min);
            LCD_Set_Cursor(2,0); LCD_String(buffer);
            break;

        case MENU_SET_TONE:
            LCD_Set_Cursor(1,0); LCD_String("Set Alarm Tone: ");
            if(!settings.alarm_enabled) {
                LCD_Set_Cursor(2,0); LCD_String("OFF             ");
            } else {
                if(settings.alarm_tone == 0) {
                     LCD_Set_Cursor(2,0); LCD_String("SIREN           ");
                } else {
                     LCD_Set_Cursor(2,0); LCD_String("BEEP            ");
                }
            }
            break;
            
        case MENU_SAVE:
            LCD_Set_Cursor(1,0); LCD_String("Save Settings?  ");
            LCD_Set_Cursor(2,0); LCD_String("UP=Yes DOWN=No  ");
            break;
    }
}

void Handle_Input() {
    unsigned char btn = Button_Read();
    if(btn == 0) return;
    
    // Silence alarm if any button is pressed
    if(alarm_active) {
        alarm_active = 0;
        Buzzer_Stop(); // Stop sound immediately
        return;
    }

    // State Machine Navigation
    switch(current_state) {
        case VIEW_MAIN:
            if(btn == 3) current_state = MENU_SET_TEMP_LOW;
            break;
            
        case MENU_SET_TEMP_LOW:
            if(btn == 1) settings.temp_low++;
            if(btn == 2) settings.temp_low--;
            if(btn == 3) current_state = MENU_SET_TEMP_HIGH;
            break;

        case MENU_SET_TEMP_HIGH:
            if(btn == 1) settings.temp_high++;
            if(btn == 2) settings.temp_high--;
            if(btn == 3) current_state = MENU_SET_HEAT_START;
            if(btn == 4) current_state = MENU_SET_TEMP_LOW;
            break;
            
        case MENU_SET_HEAT_START:
            if(btn == 2) settings.heat_start_hour++;
            if(btn == 1) settings.heat_start_min += 15;
            if(settings.heat_start_min >= 60) { settings.heat_start_min = 0; }
            if(settings.heat_start_hour >= 24) settings.heat_start_hour = 0;
            if(btn == 3) current_state = MENU_SET_HEAT_END;
            if(btn == 4) current_state = MENU_SET_TEMP_HIGH;
            break;
            
        case MENU_SET_HEAT_END:
            if(btn == 2) settings.heat_end_hour++;
            if(btn == 1) settings.heat_end_min += 15;
             if(settings.heat_end_min >= 60) { settings.heat_end_min = 0; }
            if(settings.heat_end_hour >= 24) settings.heat_end_hour = 0;
            if(btn == 3) current_state = MENU_SET_ALARM; 
            if(btn == 4) current_state = MENU_SET_HEAT_START;
            break;
            
        case MENU_SET_ALARM:
            if(btn == 2) settings.alarm_hour++;
            if(btn == 1) settings.alarm_min += 15;
            if(settings.alarm_min >= 60) { settings.alarm_min = 0; }
            if(settings.alarm_hour >= 24) settings.alarm_hour = 0;
            if(btn == 3) current_state = MENU_SET_TONE;
            if(btn == 4) current_state = MENU_SET_HEAT_END;
            break;

        case MENU_SET_TONE:
            // Cycle: OFF -> SIREN -> BEEP -> OFF
            if(btn == 1 || btn == 2) { 
                if(!settings.alarm_enabled) {
                    settings.alarm_enabled = 1;
                    settings.alarm_tone = 0;
                } else {
                    if(settings.alarm_tone == 0) {
                        settings.alarm_tone = 1;
                    } else {
                        settings.alarm_enabled = 0;
                    }
                }
            }
            if(btn == 3) current_state = MENU_SAVE;
            if(btn == 4) current_state = MENU_SET_ALARM;
            break;
            
        case MENU_SAVE:
            if(btn == 1) { // Yes
                EEPROM_Save_Settings(&settings);
                LCD_Clear();
                LCD_Set_Cursor(1,0); LCD_String("Saved!");
                __delay_ms(1000);
                current_state = VIEW_MAIN;
                last_drawn_state = 255; 
            }
            if(btn == 2) current_state = VIEW_MAIN; // No
            if(btn == 4) current_state = MENU_SET_TONE; 
            break;
    }
}

void main(void) {
    OSCCON = 0x66; 
    OSCCONbits.SCS = 0b00;

    // for heater led
    ANSELE = 0;
    TRISEbits.TRISE0 = 0;
    
    // init everything
    LM35_Init();
    SevenSeg_Init();
    LCD_Init();
    I2C1_Init_Master(100000ul);
    RTC_Init();
    Controls_Init();
    Buzzer_Init(); 
    EEPROM_Load_Settings(&settings);
    
    // Enable Interrupts
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    RTC_Set_Time(18,44,55);
    // RTC_Set_Date(1,12,25);
    
    RTC_Get_Time(&current_time);
    RTC_Get_Date(&current_date);
    
    while(1) {
        if(system_ticks % 100 == 0) {  // ~200ms
            current_temp_c = LM35_Read_Temp();
            SevenSeg_Update_Value(current_temp_c);
        }
        
        if(system_ticks % 100 == 0) { // ~200ms
            RTC_Get_Time(&current_time);
        }
        
        if (system_ticks % 300000 == 0) { // ~10 mins
            RTC_Get_Date(&current_date);
        }

        Check_Heating_Logic();
        Handle_Alarm();
        Handle_Input();
        
        if(system_ticks % 50 == 0) { // ~100ms
            Update_Display();
        }
    }
}