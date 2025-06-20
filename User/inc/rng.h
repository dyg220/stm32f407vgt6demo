#ifndef _RNG_H_
#define _RNG_H_

#include "stm32f4xx.h"                  // Device header
#include "delay.h"

u8 RNG_Init(void);
int Read_RNG(int min, int max);

#endif
