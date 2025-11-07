#include <xc.h>
#define _XTAL_FREQ 16000000UL

#define LCD_RS LATBbits.LATB0
#define LCD_EN LATBbits.LATB1
#define LCD_DATA LATD

void lcd_pulse(void) {
    LCD_EN = 1;
    __delay_us(40);
    LCD_EN = 0;
    __delay_us(40);
}

void lcd_cmd(unsigned char cmd) {
    LCD_RS = 0;
    LATD = (LATD & 0x0F) | (cmd & 0xF0);
    lcd_pulse();
    LATD = (LATD & 0x0F) | ((cmd << 4) & 0xF0);
    lcd_pulse();
    __delay_ms(2);
}

void lcd_data(unsigned char data) {
    LCD_RS = 1;
    LATD = (LATD & 0x0F) | (data & 0xF0);
    lcd_pulse();
    LATD = (LATD & 0x0F) | ((data << 4) & 0xF0);
    lcd_pulse();
    __delay_ms(2);
}

void lcd_init(void) {
    TRISD = 0x00;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    LCD_RS = 0;
    LCD_EN = 0;
    __delay_ms(20);
    lcd_cmd(0x02);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
    __delay_ms(2);
}

void lcd_goto_line2(void) {
    lcd_cmd(0xC0);
}

void lcd_print(const char *str) {
    while(*str) {
        lcd_data(*str++);
    }
}

void main(void) {
    lcd_init();
    lcd_print("Robotic Systems");
    lcd_goto_line2();
    lcd_print("and Simulation");
    while(1);
}
