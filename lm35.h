#ifndef LM35_H
#define LM35_H

#include <xc.h>

void LM35_Init(void);
unsigned int LM35_Read_Temp(void);

#endif