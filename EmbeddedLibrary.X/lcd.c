#include "lib.h"


void write_nibble(char nibble) {
    LATB0 = (nibble >> 0) & 0x01;
    LATB1 = (nibble >> 1) & 0x01;
    LATB2 = (nibble >> 2) & 0x01;
    LATB3 = (nibble >> 3) & 0x01;
    LATB5 = 1;
    __delay_ms(1);
    LATB5 = 0;
}

void write_cmd(char cmd) {
    LATB4 = 0; // Command mode
    write_nibble(cmd >> 4); // Send upper nibble
    write_nibble(cmd); // Send lower nibble
}

void lcd_char(char char_data) {
    LATB4 = 1; // Data mode
    write_nibble(char_data >> 4); // Send upper nibble
    write_nibble(char_data); // Send lower nibble
}

void lcd_string(char *text) {
    while (*text) {
        lcd_char(*text++);
    }
}

void lcd_init() {
    __delay_ms(20); // Wait for LCD to power up
    write_nibble(0x03); // Initialization sequence
    __delay_ms(5);
    write_nibble(0x03); // Repeat
    __delay_ms(5);
    write_nibble(0x03); // Repeat
    __delay_ms(5);
    write_nibble(0x02); // Set to 4-bit mode
    __delay_ms(5);
    write_cmd(0x28); // Function set: 2 lines, 4-bit mode, 5x8 dots
    write_cmd(0x0C); // Display ON, Cursor OFF
    write_cmd(0x06); // Entry mode set: auto-increment, no shift
    write_cmd(0x01); // Clear display
    __delay_ms(2); // Wait for clearing
}

void lcd_setpos(char row, char col) {
    char address;
    if (row == 1) {
        address = 0x80 + (col - 1); // Row 1 address
    } else {
        address = 0xC0 + (col - 1); // Row 2 address
    }
    write_cmd(address);
}
