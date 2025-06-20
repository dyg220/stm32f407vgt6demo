#ifndef _BEEP_H_
#define _BEEP_H_

#include "stm32f4xx.h"                  // Device header

#define BEEP_OFF   GPIOE->ODR &= ~(1<<0)
#define BEEP_ON    GPIOE->ODR |= (1<<0)
#define BEEP_FZ    GPIOE->ODR ^= (1<<0)

void Beep_Init(void);

#endif

