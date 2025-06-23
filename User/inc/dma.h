#include "stm32f4xx.h"

#ifndef _DMA_H_	
#define _DMA_H_

#include "stm32f4xx.h"                  // Device header

/**************************************************************
 *函数功能：DMA初始化函数
 *参    数：None
 *返 回 值：None
 *备    注：DMA2通道2配置为内存到外设传输
 **************************************************************/

void DMA2_USART_TDATA(u32 p_addr, u32 ndtr, u32 m_addr);


#endif