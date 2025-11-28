
#include "eeprom_cfg.h"

void Write_Byte(unsigned char addr, unsigned char data) {
    EEADR = addr;
    EEDATA = data;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0; // Disable Interrupts
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    while(EECON1bits.WR); // Wait
    INTCONbits.GIE = 1; // Re-enable
    EECON1bits.WREN = 0;
}

unsigned char Read_Byte(unsigned char addr) {
    EEADR = addr;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    return EEDATA;
}

void EEPROM_Load_Settings(SystemSettings *s) {
    unsigned char check = Read_Byte(0x00);
    if(check != 0xA5) { // First time run / Memory invalid
        // Defaults
        s->temp_low = 22;
        s->temp_high = 26;
        s->heat_start_hour = 6;
        s->heat_start_min = 30;
        s->heat_end_hour = 8;
        s->heat_end_min = 0;
        s->alarm_hour = 7;
        s->alarm_min = 0;
        s->alarm_enabled = 1;
        s->alarm_tone = 0;
        EEPROM_Save_Settings(s);
    } else {
        s->temp_low = Read_Byte(0x01);
        s->temp_high = Read_Byte(0x02);
        s->heat_start_hour = Read_Byte(0x03);
        s->heat_start_min = Read_Byte(0x04);
        s->heat_end_hour = Read_Byte(0x05);
        s->heat_end_min = Read_Byte(0x06);
        s->alarm_hour = Read_Byte(0x07);
        s->alarm_min = Read_Byte(0x08);
        s->alarm_enabled = Read_Byte(0x09);
        s->alarm_tone = Read_Byte(0x0A);
    }
}

void EEPROM_Save_Settings(SystemSettings *s) {
    Write_Byte(0x00, 0xA5); // Validity flag
    Write_Byte(0x01, s->temp_low);
    Write_Byte(0x02, s->temp_high);
    Write_Byte(0x03, s->heat_start_hour);
    Write_Byte(0x04, s->heat_start_min);
    Write_Byte(0x05, s->heat_end_hour);
    Write_Byte(0x06, s->heat_end_min);
    Write_Byte(0x07, s->alarm_hour);
    Write_Byte(0x08, s->alarm_min);
    Write_Byte(0x09, s->alarm_enabled);
    Write_Byte(0x0A, s->alarm_tone);
}