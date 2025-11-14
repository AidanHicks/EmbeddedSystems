/*
 * File:   lab9.c
 * Author: 771028
 *
 * Created on November 14, 2025, 2:14 PM
 */


#include <xc.h>

// Step 1 - Project scaffolding and prototypes
#ifndef FOSC_HZ
#define FOSC_HZ 8000000UL
#endif

void I2C1_Init_Master(unsigned long i2c_hz);
void I2C1_Start(void);
void I2C1_Restart(void);
void I2C1_Stop(void);
unsigned char I2C1_Write(unsigned char b);
unsigned char I2C1_Read_ACK(void);
unsigned char I2C1_Read_NACK(void);

void eeprom_write_byte(unsigned int addr10, unsigned char datas);
unsigned char eeprom_read_byte(unsigned int addr10);

unsigned char press_RA4(void);
unsigned char press_RA5(void);

void seg_init(void);
void seg_show_value_frame(unsigned int value);

//Step 2 - Configure I2C1 (MSSP1) at 100kHz
static void I2C1_WaitIdle(void) {
    while ((SSP1CON2 & 0x1F) || (SSP1STAT & 0x04))
    {
    }
}

void I2C1_Init_Master(unsigned long i2c_hz){
    unsigned long brg;
    ANSELC &= ~(1u << 3);
    ANSELC &= ~(1u << 4);
    TRISC |= (1u << 3) | (1u << 4);
    
    if (i2c_hz == 0u1)
        i2c_hz = 100000ul;
    brg = (FOSC_HZ / (4ul * i2c_hz));
    if (brg)
        brg -= 1ul;
    if (brg > 255ul)
        brg = 255ul;
    SSP1ADD = (unsigned char)brg;
    
    SSP1CON1 = 0x28; // Master mode, SSPEN=1
    SSP1STAT = 0x80; // SMP=1
}

void I2C1_Start(void)
{
    I2C1_WaitIdle();
    SSP1CON2 |= 0x01;
    while (SSP1CON2 & 0x01){
    }
}
void I2C1_Restart(void)
{
    I2C1_WaitIdle();
    SSP1CON2 |= 0x02;
    while (SSP1CON2 & 0x02){
    }
}
void I2C1_Stop(void){
    I2C1_WaitIdle();
    SSP1CON2 |= 0x04;
    while (SSP1CON2 & 0x04){
    }
}
unsigned char I2C1_Write(unsigned char b)
{
    I2C1_WaitIdle();
    SSP1BUF = b;
    while (SSP1STAT & 0x01)
    {
    }
    I2C1_WaitIdle();
    return (SSP1CON2 & 0x40) ? 0u : 1u;
}
unsigned char I2C1_Read_ACK(void)
{
    unsigned char v;
    I2C1_WaitIdle();
    SSP1CON2 |= 0x08;
    while (!(SSP1STAT & 0x01))
    {
    }
    v = SSP1BUF;
    I2C1_WaitIdle();
    SSP1CON2 &= ~(1u << 5);
    SSP1CON2 |= (1u << 4);
    while (SSP1CON2 & (1u << 4))
    {
    }
    return v;
}
unsigned char I2C1_Read_NACK(void)
{
    unsigned char v;
    I2C1_WaitIdle();
    SSP1CON2 |= 0x08;
    while (!(SSP1STAT & 0x01))
    {
    }
    I2C1_WaitIdle();
    SSP1CON2 |= (1u << 5);
    SSP1CON2 |= (1u << 4);
    while (SSP1CON2 & (1u << 4))
    {
    }
    return v;
}

// Step 3 - External EEPROM byte driver with ACK polling
#define M24C08_E2 0
#define EEPROM_WRITE_TIMEOUT_MS 15u

static unsigned char m24c08_devsel_write(unsigned int addr10){
    unsigned char a9a8 = (addr10 >> 8) & 0x03;
    return (unsigned char)((0b1010 << 4) | (M24C08_E2 << 3) | (a9a8 << 1));
}
static unsigned char m24c08_devsel_read(unsigned int addr10){
    unsigned char a9a8 = (addr10 >> 8) & 0x03;
    return (unsigned char)((0b1010 << 4) | (M24C08_E2 << 3) | (a9a8 << 1) | 1);
}

void eeprom_write_byte(unsigned int addr10, unsigned char datas)
{
    unsigned char devW = m24c08_devsel_write(addr10);
    unsigned char a7_0 = (unsigned char)(addr10 & 0xFF);
    unsigned int tries = 0;
    
    I2C1_Start();
    I2C1_Write(devW);
    I2C1_Write(a7_0);
    I2C1_Write(datas);
    I2C1_Stop();
    
    while (1)
    {
        I2C1_Start();
        if (I2C1_Write(devW))
        {
            I2C1_Stop();
            break;
        }
        I2C1_Stop();
        __delay_ms(1);
        if (++tries >= EEPROM_WRITE_TIMEOUT_MS)
            break;
    }
}

unsigned char eeprom_read_byte(unsigned int addr10)
{
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

// Step 4 - Buttons on RA4 and RA5 with debounce
unsigned char press_RA4(void)
{
    static unsigned char last = 0, stable = 0, cnt = 0;
    unsigned char raw = PORTAbits.RA4 ? 1u : 0u;
    if (raw == stable)
    {
        if (cnt < 5u)
            cnt++;
    }
    else
    {
        cnt = 0;
        stable = raw;
    }
    if (cnt >= 5u)
    {
        if (stable && !last)
        {
            last = 1;
            return 1;
        }
        if (!stable)
            last = 0;
    }
    return 0;
}

unsigned char press_RA5(void)
{
    static unsigned char last = 0, stable = 0, cnt = 0;
    unsigned char raw = PORTAbits.RA5 ? 1u : 0u;
    if (raw == stable)
    {
        if (cnt < 5u)
            cnt++;
    }
    else
    {
        cnt = 0;
        stable = raw;
    }
    if (cnt >= 5u)
    {
        if (stable && !last)
        {
            last = 1;
            return 1;
        }
        if (!stable)
            last = 0;
    }
    return 0;
}

// Step 5 - Seven-segment display driver

#define DIG_ACTIVE_HIGH 1
static const unsigned char DIG_MAP[4] = {3, 2,  1, 0};
static const unsigned char DIG_MASKS[4] = {0x01, 0x02, 0x04, 0x08};
static const unsigned char CC_DIGITS[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
#define SCAN_ON_US 900
#define BLANK_US 80

static void all_off(void)
{
    if (DIG_ACTIVE_HIGH)
        LATA = 0x00;
    else
        LATA = 0x0F;
}
static void enable_pos(unsigned char pos)
{
    unsigned char mask = DIG_MASKS[DIG_MAP[pos & 3u]];
    if (DIG_ACTIVE_HIGH)
        LATA = mask;
    else
        LATA = (unsigned char)(0x0F ^ mask);
}
static unsigned char seg_for(unsigned char d) { return CC_DIGITS[d % 10u]; }

void seg_init(void)
{
    ANSELA = 0x00;
    ANSELD = 0x00;
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    TRISD = 0x00;
    LATA = 0x00;
    LATD = 0x00;
}

void seg_show_value_frame(unsigned int value)
{
    unsigned char d0 = (value / 1000) % 10, d1 = (value / 100) % 10, 
                  d2 = (value / 10) % 10, d3 = value % 10;
    unsigned char show0 = (value > 999), show1 = (value > 99), show2 = (value > 9);
    
    all_off();
    LATD = 0;
    __delay_us(BLANK_US);
    LATD = show0 ? seg_for(d0) : 0;
    enable_pos(0);
    __delay_us(SCAN_ON_US);
    
    all_off();
    LATD = 0;
    __delay_us(BLANK_US);
    LATD = show1 ? seg_for(d1) : 0;
    enable_pos(1);
    __delay_us(SCAN_ON_US);
    
    all_off();
    LATD = 0;
    __delay_us(BLANK_US);
    LATD = show2 ? seg_for(d2) : 0;
    enable_pos(2);
    __delay_us(SCAN_ON_US);
    
    all_off();
    LATD = 0;
    __delay_us(BLANK_US);
    LATD = seg_for(d3);
    enable_pos(3);
    __delay_us(SCAN_ON_US);
    
    all_off();
}

//Step 6 - Main program and auto-save logic

void main(void)
{
    unsigned int addr = 0x000;
    unsigned char value, last_saved = 0xFF, i;
    unsigned int idle_ms = 0;
    
    ANSELA = 0x00;
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA5 = 1;
    
    I2C1_Init_Master(100000ul);
    seg_init();
    
    value = eeprom_read_byte(addr);
    if (value > 99)
        value = 0;
    
    while (1)
    {
        for (i = 0; i < 5; i++)
            seg_show_value_frame(value);
        
        if (press_RA4())
        {
            if (value < 99)
                value++;
            idle_ms = 0;
        }
        if (press_RA5())
        {
            if (value > 0)
                value--;
            idle_ms = 0;
        }
        
        __delay_ms(4);
        if (idle_ms < 2000)
            idle_ms += 24;
        
        if (idle_ms >= 2000 && value != last_saved)
        {
            unsigned char verify;
            eeprom_write_byte(addr, value);
            verify = eeprom_read_byte(addr);
            if (verify == value)
                last_saved = value;
            else
                for (i = 0; i < 5; i++)
                {
                    seg_show_value_frame(8888);
                    __delay_ms(100);
                }
            idle_ms = 2000;
        }
    }
    
}