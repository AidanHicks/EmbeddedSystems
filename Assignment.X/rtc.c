
#include "rtc.h"
#include "i2c.h"
#include <xc.h>


// 6 Step 4 ? RTC16 Driver Functions

#define RTC16_ADDR 0x32
#define RTC16_REG_SEC   0x00
#define RTC16_REG_MIN   0x01
#define RTC16_REG_HOUR  0x02
#define RTC16_REG_CTRL1 0x0E
#define RTC16_REG_CTRL2 0x0F

#define RTC16_REG_DAY 0x04
#define RTC16_REG_MONTH 0x05
#define RTC16_REG_YEAR 0x06

#define RTC16_CT_EVERY_SECOND 0x04
#define RTC16_24H_MODE       0x20
#define RTC16_CLOCK_ENABLE   0x00


static unsigned char dec_to_bcd(unsigned char d){ return ((d/10)<<4)|(d%10); }
static unsigned char bcd_to_dec(unsigned char b){ return 10*(b>>4)+(b&0x0F); }

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
  for(i=0;i<len;i++) buf[i]=(i<len-1)?I2C1_Read_ACK():I2C1_Read_NACK();
  I2C1_Stop();
}

void rtc16_default_cfg(void){
  rtc16_write_reg(RTC16_REG_CTRL1, RTC16_CT_EVERY_SECOND);
  rtc16_write_reg(RTC16_REG_CTRL2, RTC16_24H_MODE | RTC16_CLOCK_ENABLE);
}

// public functions

void RTC_Init() {
    
    rtc16_default_cfg();
}

void RTC_Set_Time(unsigned char h, unsigned char m, unsigned char s){
  I2C1_Start();
  I2C1_Write((RTC16_ADDR<<1)|0);
  I2C1_Write(RTC16_REG_SEC<<4);
  I2C1_Write(dec_to_bcd(s)); I2C1_Write(dec_to_bcd(m)); I2C1_Write(dec_to_bcd(h));
  I2C1_Stop();
}

void RTC_Get_Time(Time_t* t){
  unsigned char b[3];
  rtc16_read_regs(RTC16_REG_SEC,b,3);
  t->second=bcd_to_dec(b[0]);
  t->minute=bcd_to_dec(b[1]);
  t->hour =bcd_to_dec(b[2]);
}

void RTC_Get_Date(Date_t* t){
  unsigned char b[3];
  rtc16_read_regs(RTC16_REG_DAY,b,3);
  t->day=bcd_to_dec(b[0]);
  t->month=bcd_to_dec(b[1]);
  t->year =bcd_to_dec(b[2]);
}

void RTC_Set_Date(unsigned char d, unsigned char m, unsigned char y) {
    I2C1_Start();
    I2C1_Write((RTC16_ADDR<<1)|0);
    I2C1_Write(RTC16_REG_DAY<<4);
    I2C1_Write(dec_to_bcd(d)); I2C1_Write(dec_to_bcd(m)); I2C1_Write(dec_to_bcd(y));
    I2C1_Stop();
}
