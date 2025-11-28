#ifndef SEVENSEG_H
#define SEVENSEG_H

#include <xc.h>

void SevenSeg_Init(void);
void SevenSeg_Update_Value(unsigned int number);
void SevenSeg_ISR_Handler(void); // Call this in High Priority ISR

#endif