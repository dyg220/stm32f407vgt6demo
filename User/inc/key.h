#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f4xx.h"                  // Device header
#include "delay.h"

enum key_value
{
	key1_click = 1,
	key2_click,
	key3_click,
	key4_click,
	key1_long,
	key2_long,
	key3_long,
	key4_long
};

#define Key_Mode       1  //1代表非阻塞   0代表有阻塞

#define Key1_Press 		GPIOA->IDR&(1<<0)
#define Key2_Press 		!(GPIOE->IDR&(1<<2))
#define Key3_Press 		!(GPIOE->IDR&(1<<3))
#define Key4_Press 		!(GPIOE->IDR&(1<<4))

void Key_Init(void);
u8 Key_Scan(void);
u8 Key_GetState(void);
void Key_Tick(void);

#endif

