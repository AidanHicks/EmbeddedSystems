#ifndef BUTTONS_H
#define BUTTONS_H

#define BTN_UP PORTCbits.RC0
#define BTN_DOWN PORTCbits.RC1
#define BTN_ENTER PORTCbits.RC2
#define BTN_BACK PORTCbits.RC5

#define BUZZER_PIN LATCbits.LATC6

void Controls_Init(void);
unsigned char Button_Read(void); // Returns 0 (none), 1(Up), 2(Down), 3(Enter), 4(Back)
void Buzzer_Play(unsigned char tone_type);

#endif