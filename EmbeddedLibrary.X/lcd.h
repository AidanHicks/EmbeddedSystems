/* 
 * File:   lcd.h
 * Author: 767905
 *
 * Created on 14 November 2025, 12:39
 */

#ifndef LCD_H
#define	LCD_H

void lcd_init();
void lcd_char(char char_data);
void lcd_string(char *text);
void lcd_setpos(char row, char col) ;

#endif	/* LCD_H */

