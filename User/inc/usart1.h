#ifndef _USART1_H_
#define _USART1_H_

#include "stm32f4xx.h"                  // Device header
#include "nvic.h"


void USART1_Init(u32 baud);
void USART1_SendByte(u8 Data);
//u8 USART1_RecvByte(void);
void USART1_SendStr(u8* str);
//void USART1_RecvStr(u8* str);
void USART1_IRQ_Config(void);
void RECV_USART1(void);

#endif
