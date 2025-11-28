#ifndef LCD_H
#define LCD_H

#include <xc.h>

// Pin Config
#define LCD_RS LATBbits.LATB4
#define LCD_EN LATBbits.LATB5
#define LCD_D4 LATBbits.LATB0
#define LCD_D5 LATBbits.LATB1
#define LCD_D6 LATBbits.LATB2
#define LCD_D7 LATBbits.LATB3

void LCD_Init(void);
void LCD_Cmd(char cmd);
void LCD_Char(char dat);
void LCD_String(const char* str);
void LCD_Set_Cursor(unsigned char row, unsigned char col);
void LCD_Clear(void);

#endif