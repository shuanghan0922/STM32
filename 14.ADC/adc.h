#ifndef _ADC_H
#define _ADC_H
#include "sys.h"


void ADC1_Init(void);
u16 ADC_GetValue(u8 ch);
u16 ADC_GetAverage(u8 ch,u8 times);



#endif

