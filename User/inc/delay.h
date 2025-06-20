#ifndef _DELAY_H_
#define _DELAY_H_

#include "stm32f4xx.h"                  // Device header

void delay_us(uint32_t xus);
void delay_ms(uint32_t xms);
void SysTick_IRQ_Config(uint32_t xms);

#endif

