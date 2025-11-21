#include "config_bits.h"
#include "lm35.h"
#include "sevenseg.h"
#include "lcd.h"
#include "rtc.h"
#include "eeprom_cfg.h"
#include "buttons.h"
#include <stdio.h>

// Pin Definitions for Heating
#define HEATER_LED LATEbits.LATE0

// System Globals
SystemSettings settings;
Time currentTime;
unsigned int current_temp_raw;
unsigned int current_temp_c;
unsigned char heater_state = 0;
unsigned char alarm_active = 0;
unsigned long last_tick = 0;

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
// FIX: Track the last state to detect changes
MenuState last_drawn_state = 255; 

// Global Tick Counter (incremented in ISR)
volatile unsigned long system_ticks = 0;

void __interrupt() ISR(void) {
    // Timer0 for 7-Seg and System Ticks
    if(INTCONbits.TMR0IF) {
        SevenSeg_ISR_Handler();
        system_ticks++; // increments every ~2ms
    }
}

void Check_Heating_Logic() {
    // Convert times to minutes for easier comparison
    unsigned int now_mins = currentTime.hour * 60 + currentTime.min;
    unsigned int start_mins = settings.heat_start_hour * 60 + settings.heat_start_min;
    unsigned int end_mins = settings.heat_end_hour * 60 + settings.heat_end_min;
    
    unsigned char in_window = 0;
    if(start_mins < end_mins) {
        if(now_mins >= start_mins && now_mins < end_mins) in_window = 1;
    } else {
        // Window crosses midnight
        if(now_mins >= start_mins || now_mins < end_mins) in_window = 1;
    }

    if(in_window) {
        if(current_temp_c < settings.temp_low) heater_state = 1;
        else if(current_temp_c >= settings.temp_high) heater_state = 0;
    } else {
        heater_state = 0;
    }
    
    HEATER_LED = heater_state;
}

void Handle_Alarm() {
    if(!settings.alarm_enabled) return;
    
    if(currentTime.hour == settings.alarm_hour && 
       currentTime.min == settings.alarm_min && 
       currentTime.sec == 0) {
           alarm_active = 1; 
    }
    
    // Play alarm for 2 seconds (approx 1000 ticks at 2ms)
    if(alarm_active) {
        // Non-blocking beep pattern
        if(system_ticks % 200 < 100) { 
             Buzzer_Play(settings.alarm_tone); 
        }
        
        // Auto silence after 5 seconds or via button
        if(currentTime.sec > 5) alarm_active = 0; 
    }
}

void Update_Display() {
    // FIX: Detect menu change and clear screen ONCE
    if (current_state != last_drawn_state) {
        LCD_Clear();
        last_drawn_state = current_state;
    }

    char buffer[17];
    
    switch(current_state) {
        case VIEW_MAIN:
            // Format with fixed width or padding
            sprintf(buffer, "Time: %02d:%02d:%02d  ", currentTime.hour, currentTime.min, currentTime.sec);
            LCD_Set_Cursor(1,0); LCD_String(buffer);
            
            sprintf(buffer, "T:%02d H:%s A:%s ", current_temp_c, 
                    heater_state ? "ON " : "OFF", 
                    settings.alarm_enabled ? "ON" : "--");
            LCD_Set_Cursor(2,0); LCD_String(buffer);
            break;
            
        case MENU_SET_TEMP_LOW:
            LCD_Set_Cursor(1,0); LCD_String("Set Temp LOW:   ");
            // Added spaces padding to clear previous digits if number shrinks
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
            
        case MENU_SAVE:
            LCD_Set_Cursor(1,0); LCD_String("Save Settings?  ");
            LCD_Set_Cursor(2,0); LCD_String("UP=Yes DOWN=No  ");
            break;
    }
}

void Handle_Input() {
    unsigned char btn = Button_Read();
    if(btn == 0) return;
    
    // Silence alarm if active
    if(alarm_active) {
        alarm_active = 0;
        return;
    }

    // State Machine Navigation
    switch(current_state) {
        case VIEW_MAIN:
            if(btn == 3) current_state = MENU_SET_TEMP_LOW; // Enter
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
            if(btn == 1) settings.heat_start_hour++;
            if(btn == 2) settings.heat_start_min += 15;
            if(settings.heat_start_min >= 60) { settings.heat_start_min = 0; }
            if(settings.heat_start_hour >= 24) settings.heat_start_hour = 0;
            if(btn == 3) current_state = MENU_SET_HEAT_END;
            if(btn == 4) current_state = MENU_SET_TEMP_HIGH;
            break;
            
        case MENU_SET_HEAT_END:
            if(btn == 1) settings.heat_end_hour++;
            if(btn == 2) settings.heat_end_min += 15;
             if(settings.heat_end_min >= 60) { settings.heat_end_min = 0; }
            if(settings.heat_end_hour >= 24) settings.heat_end_hour = 0;
            if(btn == 3) current_state = MENU_SAVE;
            if(btn == 4) current_state = MENU_SET_HEAT_START;
            break;
            
        case MENU_SAVE:
            if(btn == 1) { // Yes
                EEPROM_Save_Settings(&settings);
                LCD_Clear();
                LCD_Set_Cursor(1,0); LCD_String("Saved!");
                __delay_ms(1000);
                current_state = VIEW_MAIN;
                // Force clear on return to main
                last_drawn_state = 255; 
            }
            if(btn == 2) current_state = VIEW_MAIN; // No
            break;
    }
}

void main(void) {
    // FIX 1: Set Internal Oscillator to 8MHz explicitly
    OSCCON = 0x66; 

    // Initialization
    ANSELE = 0; // Default Digital
    TRISEbits.TRISE0 = 0; // Heater LED Out
    
    LM35_Init();
    SevenSeg_Init();
    LCD_Init();
    RTC_Init();
    Controls_Init();
    EEPROM_Load_Settings(&settings);
    
    // Enable Interrupts
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    while(1) {
        // 1. Read Hardware
        if(system_ticks % 100 == 0) {
            current_temp_c = LM35_Read_Temp();
            SevenSeg_Update_Value(current_temp_c);
        }
        
        if(system_ticks % 500 == 0) {
            RTC_Get_Time(&currentTime);
        }
        
        // 2. Logic
        Check_Heating_Logic();
        Handle_Alarm();
        
        // 3. User Interface
        Handle_Input();
        
        // Update LCD every ~100ms
        if(system_ticks % 50 == 0) {
            Update_Display();
        }
        
        __delay_ms(5);
    }
}