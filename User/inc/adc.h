#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f4xx.h"                  // Device header


void ADC1_CH10_Init(void);
void ADC2_CH12_Init(void);
void ADC1_CH10_CH12_Init(void);
void ADC1_CH10_CH12_Interrupt_Init(void);
void ADC1_CH10_CH12_T_Init(void);

#endif

