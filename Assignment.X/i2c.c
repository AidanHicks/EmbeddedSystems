#include <xc.h>
#include "i2c.h"
#define _XTAL_FREQ 8000000UL             // Define your crystal frequency

#pragma config FOSC = HSMP               // Use 4?16 MHz external crystal (RA6/RA7 = oscillator)
#pragma config PLLCFG = OFF              // Keep PLL off (no ×4 multiplier)
#pragma config PRICLKEN = ON             // Primary oscillator always on for stability
#pragma config FCMEN = OFF               // Disable Fail-Safe Clock Monitor
#pragma config IESO = OFF                // Disable automatic clock switching

#pragma config PWRTEN = OFF              // No Power-up Timer delay
#pragma config BOREN  = OFF              // Brown-Out Reset disabled
#pragma config WDTEN = OFF               // Watchdog Timer disabled
#pragma config PBADEN = OFF              // PORTB pins digital on reset
#pragma config MCLRE  = INTMCLR          // RE3 is digital input (not reset). Use EXTMCLR to enable reset.
#pragma config LVP = OFF                 // Low-voltage programming disabled



#define FOSC_HZ 8000000UL

static void I2C1_WaitIdle(void){
  while ((SSP1CON2 & 0x1F) || (SSP1STAT & 0x04)) { }
}

void I2C1_Init_Master(unsigned long i2c_hz){
  unsigned long brg;
  ANSELC &= ~((1u<<3)|(1u<<4)); // digital
  TRISC |= (1u<<3)|(1u<<4);    // inputs
  if(i2c_hz==0) i2c_hz=100000ul;
  brg = (FOSC_HZ/(4ul*i2c_hz)) - 1ul;
  if(brg>255ul) brg=255ul;
  SSP1ADD = (unsigned char)brg;
  SSP1STAT = 0x80;   // SMP = 1
  SSP1CON1 = 0x28;  // master, enable
}


void I2C1_Start(void)
{ 
  I2C1_WaitIdle(); 
  SSP1CON2 |= 0x01; 
  while(SSP1CON2 & 0x01){}; 
}

void I2C1_Restart(void)
{ 
  I2C1_WaitIdle(); 
  SSP1CON2 |= 0x02; 
  while(SSP1CON2 & 0x02){}; 
  
  }

void I2C1_Stop(void)
{ 
  I2C1_WaitIdle(); 
  SSP1CON2 |= 0x04; 
  while(SSP1CON2 & 0x04){};   
}

unsigned char I2C1_Write(unsigned char b)
{
  I2C1_WaitIdle(); 
  SSP1BUF=b; 
  while(SSP1STAT & 0x01); 
  I2C1_WaitIdle();

  return (SSP1CON2 & 0x40)?0u:1u; // 1=ACK received
}

unsigned char I2C1_Read_ACK(void)
{
  unsigned char v; 
  I2C1_WaitIdle(); 
  SSP1CON2 |= 0x08;
  while(!(SSP1STAT & 0x01)); 
  v=SSP1BUF; I2C1_WaitIdle();
  SSP1CON2 &= ~(1u<<5); SSP1CON2 |= (1u<<4); 
  while(SSP1CON2 & (1u<<4));
  return v;
}

unsigned char I2C1_Read_NACK(void)
{
  unsigned char v; 
  I2C1_WaitIdle(); 
  SSP1CON2 |= 0x08;
  while(!(SSP1STAT & 0x01)); 
  v=SSP1BUF; I2C1_WaitIdle();
  SSP1CON2 |= (1u<<5); 
  SSP1CON2 |= (1u<<4); 
  while(SSP1CON2 & (1u<<4));
  return v;
}