#include "stm32f4xx.h"

#ifndef _DMA_H_	
#define _DMA_H_

#include "stm32f4xx.h"                  // Device header

/**************************************************************
 *�������ܣ�DMA��ʼ������
 *��    ����None
 *�� �� ֵ��None
 *��    ע��DMA2ͨ��2����Ϊ�ڴ浽���贫��
 **************************************************************/

void DMA2_USART_TDATA(u32 p_addr, u32 ndtr, u32 m_addr);


#endif