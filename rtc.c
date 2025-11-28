
#include "rtc.h"
#include <xc.h>
#include "config_bits.h"

// Simple I2C Bit-banging on RC3(SCL) and RC4(SDA)
#define SCL LATCbits.LATC3
#define SDA LATCbits.LATC4
#define SCL_DIR TRISCbits.TRISC3
#define SDA_DIR TRISCbits.TRISC4

void I2C_Init(void) {
    SCL_DIR = 1; SDA_DIR = 1; // Float high
    ANSELCbits.ANSC3 = 0; // Disable analog
    ANSELCbits.ANSC4 = 0;
}

void I2C_Start(void) {
    SDA_DIR = 0; SDA = 0; __delay_us(5);
    SCL_DIR = 0; SCL = 0; __delay_us(5);
}

void I2C_Stop(void) {
    SCL_DIR = 0; SCL = 0; SDA_DIR = 0; SDA = 0; __delay_us(5);
    SCL_DIR = 1; __delay_us(5);
    SDA_DIR = 1; __delay_us(5);
}

void I2C_Write(unsigned char data) {
    for(int i=0; i<8; i++) {
        if(data & 0x80) SDA_DIR = 1; else { SDA_DIR = 0; SDA = 0; }
        __delay_us(2); SCL_DIR = 1; __delay_us(2); SCL_DIR = 0;
        data <<= 1;
    }
    SDA_DIR = 1; // ACK Pulse
    __delay_us(2); SCL_DIR = 1; __delay_us(2); SCL_DIR = 0;
}

unsigned char I2C_Read(unsigned char ack) {
    unsigned char data = 0;
    SDA_DIR = 1;
    for(int i=0; i<8; i++) {
        SCL_DIR = 1; __delay_us(2);
        if(PORTCbits.RC4) data |= (1 << (7-i));
        SCL_DIR = 0; __delay_us(2);
    }
    if(ack) { SDA_DIR = 0; SDA = 0; } else SDA_DIR = 1;
    SCL_DIR = 1; __delay_us(2); SCL_DIR = 0;
    return data;
}

unsigned char BCD2Dec(unsigned char bcd) { return ((bcd >> 4) * 10) + (bcd & 0x0F); }
unsigned char Dec2BCD(unsigned char dec) { return ((dec / 10) << 4) + (dec % 10); }

void RTC_Init(void) { I2C_Init(); }

void RTC_Get_Time(Time *t) {
    I2C_Start();
    I2C_Write(0xD0); // Address Write
    I2C_Write(0x00); // Start from seconds
    I2C_Stop();
    
    I2C_Start();
    I2C_Write(0xD1); // Address Read
    t->sec = BCD2Dec(I2C_Read(1));
    t->min = BCD2Dec(I2C_Read(1));
    t->hour = BCD2Dec(I2C_Read(0));
    I2C_Stop();
}

void RTC_Set_Time(unsigned char h, unsigned char m) {
    I2C_Start();
    I2C_Write(0xD0);
    I2C_Write(0x00); // Reg 0
    I2C_Write(0x00); // Seconds reset
    I2C_Write(Dec2BCD(m));
    I2C_Write(Dec2BCD(h));
    I2C_Stop();
}