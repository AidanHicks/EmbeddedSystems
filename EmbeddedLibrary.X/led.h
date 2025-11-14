/* 
 * File:   led.h
 * Author: 767905
 *
 * Created on 14 November 2025, 12:38
 */

#ifndef LED_H
#define	LED_H


void led_set(unsigned char* latch, unsigned char pin, unsigned char state);
void led_on(unsigned char* latch, unsigned char pin);
void led_off(unsigned char* latch, unsigned char pin);
void led_clear_port(unsigned short* latch);


#endif	/* LED_H */

