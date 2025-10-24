#include "lib.h"

void CLEARLED() {
LATC = 0x00;
}

void SET_LED(unsigned char pin) {
LATC = (1 << pin); 
}

void Blink(){
    TRISC = 0x00;
    while (1){
        LATC = 0x01;
        Delay_ms(500);
        LATC = 0x00;
        Delay_ms(500);
    }
}

void BlinkALL(){
    const unsigned int ON_MS = 200; 
    const unsigned int OFF_MS = 800; 
    TRISC = 0x00;
    LATC = 0x00;
    LATC = 0xFF;
    Delay_ms(ON_MS);
    LATC = 0x00;
    Delay_ms(OFF_MS);
}

void Sweep(){
    signed char i;
    TRISC = 0x00;
    LATC = 0x00;
    while (1){
        //from RC0 to RC7
        Delay_ms(500); //Interval at end2
        for (i = 0; i <=7; i++){
            LATC = (1 << i);
            Delay_ms(120);
        }
        Delay_ms(500);//Interval at end1
        //backward from RC6 to RC0
        for(i = 6; i >= 0; i--) {
            LATC = (1 << i);
            Delay_ms(120);
        }
    }
}

void sweep_forward(){
    unsigned char i;
    for (i=0; i < 8; i++){
        SET_LED(i);
        Delay_ms(200);
    }
}

void sweep_backward(){
    signed char i;
    for (i = 7; i>=0; i--) {
        SET_LED(i);
        Delay_ms(200);
    }
}

void pause_at_end(){
    Delay_ms(500);
}

//Task 3
void SweepSpeedControl(){
    const int SPEED_MS = 20;
    unsigned char i;
    TRISC = 0x00;
    while (1){
        for (i = 0; i < 8; i++){
            LATC = (1 << i);
            Delay_ms(SPEED_MS);
        }
    }
}

void main (){
    TRISC = 0x00;
    CLEARLED();
    while (1){
        sweep_forward();
        pause_at_end();
        BlinkALL();
        sweep_backward();
        pause_at_end();
        BlinkALL();
    }
}
