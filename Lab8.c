#include <xc.h>
#define RTC16_ADDR 0x32
#define RTC16_REG_SEC 0x00
#define RTC16_REG_MIN 0x01
#define RTC16_REG_HOUR 0x02
#define RTC16_REG_CTRL1 0x0E
#define RTC16_REG_CTRL2 0x0F

#define RTC16_CT_EVERY_SECOND 0x04
#define RTC16_24H_MODE 0x20
#define RTC16_CLOCK_ENABLE 0x00

#ifndef FOSC_HZ
#define FOSC_HZ 800000UL
#endif

static void I2C1_WaitIdle(void){
    while ((SSP1CON2 & 0x1F) || (SSP1STAT & 0x04)) {}
}

void I2C1_Init_Master(unsigned long i2c_hz){
    unsigned long brg;
    ANSELC &= ~((1u<<3) | (1u<<4));
    TRISC |= (1u<<3) | (1u<<4);
    if (i2c_hz==0) i2c_hz=100000ul;
    brg = (FOSC_HZ/(4ul*i2c_hz)) - 1ul;
    if (brg>255ul) brg=255ul;
    SSP1ADD = (unsigned char)brg;
    SSP1STAT = 0x80;
    SSP1CON1 = 0x28;
    
}

void I2C1_Start(void){
    I2C1_WaitIdle();
    SSP1CON2 |= 0x01;
    while(SSP1CON2 & 0x01);

}
void I2C1_Restart(void){
    I2C1_WaitIdle();
    SSP1CON2 |= 0x02;
    while(SSP1CON2 |= 0x02);
}
void I2C1_Stop(void){
    I2C1_WaitIdle();
    SSP1CON2 |= 0x04;
    while(SSP1CON2 |= 0x04);
}
unsigned char I2C1_Write(unsigned char b){
    I2C1_WaitIdle();
    SSP1BUF=b;
    while(SSP1STAT & 0x01);
    I2C1_WaitIdle;
    return
    (SSP1CON2 & 0x40)?0u:1u;
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
unsigned char I2C1_Read_NACK(void){
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

typedef struct {unsigned char second, minute, hour; } rtc_time_t;

static unsigned char dec_to_bcd(unsigned char d){return ((d/10)<<4)|(d%10);}
static unsigned char bcd_to_dec(unsigned char b){return 10*(b>>4)+(b&0x0F);}

void rtc16_write_reg(unsigned char reg, unsigned char val){
    I2C1_Start();
    I2C1_Write((RTC16_ADDR<<1)|0);
    I2C1_Write(reg<<4);
    I2C1_Write(val);
    I2C1_Stop();
}

void rtc16_read_regs(unsigned char reg, unsigned char* buf, unsigned char len){
    unsigned char i;
    I2C1_Start();
    I2C1_Write((RTC16_ADDR<<1)|0);
    I2C1_Write(reg<<4);
    I2C1_Restart();
    I2C1_Write((RTC16_ADDR<<1)|1);
    for (i=0; i < len; i++){
        buf[i] = (i<len-1)?I2C1_Read_ACK():I2C1_Read_NACK();
        
    }
    I2C1_Stop();
            
}

void rtc16_default_cfg(void){
    rtc16_write_reg(RTC16_REG_CTRL1, RTC16_CT_EVERY_SECOND);
    rtc16_write_reg(RTC16_REG_CTRL2, RTC16_24H_MODE | RTC16_CLOCK_ENABLE);
    
    
}
void rtc16_set_time(rtc_time_t* t){
    unsigned char b[3];
    b[0]=dec_to_bcd(t->second);
    b[1]=dec_to_bcd(t->minute);
    b[2]=dec_to_bcd(t->hour);
    I2C1_Start();
    I2C1_Write((RTC16_ADDR<<1)|0);
    I2C1_Write(RTC16_REG_SEC<<4);
    I2C1_Write(b[0]); I2C1_Write(b[1]); I2C1_Write(b[2]);
    I2C1_Stop();        
}
void rtc16_get_time(rtc_time_t* t){
    unsigned char b[3];
    rtc16_read_regs(RTC16_REG_SEC,b,3);
    t->second=bcd_to_dec(b[0]);
    t->minute=bcd_to_dec(b[1]);
    t->hour=bcd_to_dec(b[2]);

}

void main(void){
    rtc_time_t now;
    unsigned long ms = 0;
    ANSELA=0; ANSELB=0; ANSELC=0; ANSELD=0; ANSELE=0;
    I2C1_Init_Master(100000ul);
    rtc16_default_cfg();
    
    now.hour=12; now.minute=34; now.second=0; rtc16_set_time(&now);
    
    rtc16_get_time(&now);
}