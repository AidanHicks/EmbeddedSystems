#ifndef RTC_H
#define RTC_H

typedef struct {
    unsigned char sec;
    unsigned char min;
    unsigned char hour;
} Time;

void RTC_Init(void);
void RTC_Get_Time(Time *t);
void RTC_Set_Time(unsigned char h, unsigned char m);

#endif