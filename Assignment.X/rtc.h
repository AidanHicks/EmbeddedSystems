#ifndef RTC_H
#define RTC_H

typedef struct { 
    unsigned char second, minute, hour; 
} Time_t;

typedef struct {
    unsigned char day, month, year;
} Date_t;

void RTC_Init(void);
void RTC_Get_Time(Time_t *t);
void RTC_Set_Time(unsigned char h, unsigned char m, unsigned char s);

void RTC_Get_Date(Date_t* d);
void RTC_Set_Date(unsigned char d, unsigned char m, unsigned char y);

#endif