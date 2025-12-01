#include "lcd.h"
#include "config_bits.h"

void LCD_Pulse(void) {
    LCD_EN = 1;
    __delay_us(50);
    LCD_EN = 0;
    __delay_us(50);
}

void LCD_Nibble(char nibble) {
    LCD_D4 = (nibble >> 0) & 1;
    LCD_D5 = (nibble >> 1) & 1;
    LCD_D6 = (nibble >> 2) & 1;
    LCD_D7 = (nibble >> 3) & 1;
    LCD_Pulse();
}

void LCD_Cmd(char cmd) {
    LCD_RS = 0;
    LCD_Nibble(cmd >> 4);
    LCD_Nibble(cmd);
    __delay_ms(2);
}

void LCD_Char(char dat) {
    LCD_RS = 1;
    LCD_Nibble(dat >> 4);
    LCD_Nibble(dat);
    __delay_us(50);
}

void LCD_Init(void) {
    TRISB = 0x00; // Output
    LCD_RS = 0; LCD_EN = 0;
    __delay_ms(20);
    
    // Reset sequence
    LCD_Nibble(0x03); __delay_ms(5);
    LCD_Nibble(0x03); __delay_us(150);
    LCD_Nibble(0x03);
    LCD_Nibble(0x02); // 4-bit mode
    
    LCD_Cmd(0x28); // 4-bit, 2 line, 5x7
    LCD_Cmd(0x0C); // Display ON, Cursor OFF
    LCD_Cmd(0x06); // Increment cursor
    LCD_Clear();
}

void LCD_String(const char* str) {
    while(*str) LCD_Char(*str++);
}

void LCD_Set_Cursor(unsigned char row, unsigned char col) {
    unsigned char address;
    if(row == 1) address = 0x80 + col;
    else address = 0xC0 + col;
    LCD_Cmd(address);
}

void LCD_Clear(void) {
    LCD_Cmd(0x01);
    __delay_ms(2);
}