#include "config_bits.h"
#include "lm35.h"
#include "sevenseg.h"
#include "lcd.h"
#include "i2c.h"
#include "rtc.h"
#include "eeprom_cfg.h"
#include "buttons.h"
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
MenuState last_drawn_state = 255; 

void __interrupt() ISR(void) {
    if(INTCONbits.TMR0IF) {
        SevenSeg_ISR_Handler();
        system_ticks++; // tick every 2ms
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
    if(!settings.alarm_enabled) return;
    
    if(current_time.hour == settings.alarm_hour && 
       current_time.minute == settings.alarm_min && 
       current_time.second == 0) {
           alarm_active = 1; 
    }
    
    if(alarm_active) {
        if(system_ticks % 200 < 100) { 
            // Buzzer_Play(settings.alarm_tone); 
        }
        
        // Auto silence after 5 seconds or via button
        if(current_time.second > 5) alarm_active = 0; 
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
            sprintf(buffer, "%02d:%02d %02d/%02d/%d", current_time.hour, current_time.minute, current_date.day, current_date.month, current_date.year);
            LCD_Set_Cursor(1,0); LCD_String(buffer);
            
            sprintf(buffer, "T:%02d H:%s A:%s ", current_temp_c, 
                    heater_state ? "ON " : "OFF", 
                    settings.alarm_enabled ? "ON" : "--");
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
    EEPROM_Load_Settings(&settings);
    
    // Enable Interrupts
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    //// one time thing
    //RTC_Set_Time(15,39,00);
    //RTC_Set_Date(1,12,25);
    
    RTC_Get_Time(&current_time);
    RTC_Get_Date(&current_date);
    while(1) {
        if(system_ticks % 100 == 0) {  // ~200ms
            current_temp_c = LM35_Read_Temp();
            SevenSeg_Update_Value(current_temp_c);
        }
        
        if(system_ticks % 500 == 0) { // ~1s
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