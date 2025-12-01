
#include "eeprom_cfg.h"
#include "i2c.h"

#define M24C08_E2 0
#define EEPROM_WRITE_TIMEOUT_MS 15u

static unsigned char m24c08_devsel_write(unsigned int addr10){
    unsigned char a9a8 = (addr10 >> 8) & 0x03;
    return (unsigned char)((0b1010<<4) | (M24C08_E2<<3) | (a9a8<<1));
}
static unsigned char m24c08_devsel_read(unsigned int addr10){
    unsigned char a9a8 = (addr10 >> 8) & 0x03;
    return (unsigned char)((0b1010<<4) | (M24C08_E2<<3) | (a9a8<<1) | 1);
}

void Write_Byte(unsigned int addr10, unsigned char data){
    unsigned char devW = m24c08_devsel_write(addr10);
    unsigned char a7_0 = (unsigned char)(addr10 & 0xFF);
    unsigned int tries = 0;

    I2C1_Start();
    I2C1_Write(devW);
    I2C1_Write(a7_0);
    I2C1_Write(data);
    I2C1_Stop();

    while(1){
        I2C1_Start();
        if(I2C1_Write(devW)){ I2C1_Stop(); break; }
        I2C1_Stop();
        __delay_ms(1);
        if(++tries >= EEPROM_WRITE_TIMEOUT_MS) break;
    }
}

unsigned char Read_Byte(unsigned int addr10){
    unsigned char devW = m24c08_devsel_write(addr10);
    unsigned char devR = m24c08_devsel_read(addr10);
    unsigned char a7_0 = (unsigned char)(addr10 & 0xFF);
    unsigned char v;

    I2C1_Start();
    I2C1_Write(devW);
    I2C1_Write(a7_0);
    I2C1_Restart();
    I2C1_Write(devR);
    v = I2C1_Read_NACK();
    I2C1_Stop();
    return v;
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