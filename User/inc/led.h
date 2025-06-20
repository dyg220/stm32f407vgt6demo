#ifndef _LED_H_
#define _LED_H_

#include "stm32f4xx.h"                  // Device header

#define LED1_ON   GPIOC->ODR &= ~(1<<4)
#define LED1_OFF  GPIOC->ODR |= (1<<4)
#define LED1_FZ   GPIOC->ODR ^= (1<<4)
#define LED2_ON   GPIOC->ODR &= ~(1<<5)
#define LED2_OFF  GPIOC->ODR |= (1<<5)
#define LED2_FZ   GPIOC->ODR ^= (1<<5)
#define LED3_ON   GPIOC->ODR &= ~(1<<6)
#define LED3_OFF  GPIOC->ODR |= (1<<6)
#define LED3_FZ   GPIOC->ODR ^= (1<<6)
#define LED4_ON   GPIOC->ODR &= ~(1<<7)
#define LED4_OFF  GPIOC->ODR |= (1<<7)
#define LED4_FZ   GPIOC->ODR ^= (1<<7)


void LED_Init(void);
void Led_Tick(void);
void Led_Tick(void);
void Led1_SetMode(u8 Mode);

#endif

