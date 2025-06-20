#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "stdio.h"

#define TRIG_H   GPIOB->ODR |= (1<<9)
#define TRIG_L   GPIOB->ODR &= ~(1<<9)

void Ultrasonic_Init(void);
void Read_Distance(void);

#endif

