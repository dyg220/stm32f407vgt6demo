#ifndef _NVIC_H_
#define _NVIC_H_

#include "stm32f4xx.h"                  // Device header
#include "stdio.h"
#include "led.h"
#include "delay.h"
#include "function.h"

#define Max_Size   256

typedef struct{
	u8 data[Max_Size];
	u16 index;
	u8 flag;
	
}USART1_INFO;


#endif

