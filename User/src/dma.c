#include "dma.h"
#include <stdio.h>

void DMA2_USART_TDATA(u32 p_addr, u32 m_addr, u32 ndtr) {

	RCC->AHB1ENR |= (1 << 22);  //使能DMA2时钟

	DMA2_Stream7->CR &= ~(1 << 0); //清除EN位
	while (DMA2_Stream7->CR & (1 << 0)); //等待DMA2_Stream7_EN位清除完成

	DMA2_Stream7->CR &= ~(7 << 25); //清除DMA2_Stream7_CHSEL位
	DMA2_Stream7->CR |= (4 << 25);	// 选择DMA2_Stream7_CHSEL为4（USART2）
	DMA2_Stream7->CR &= ~(3 << 16); //优先级为低
	DMA2_Stream7->CR &= ~(1 << 9);	//外设地址固定
	DMA2_Stream7->CR &= ~(3 << 11);  //外设数据增量1byte
	DMA2_Stream7->CR |= (1 << 10);	//存储器地址递增
	DMA2_Stream7->CR &= ~(3 << 13); //存储器数据增量1Byte
	DMA2_Stream7->CR &= ~(1 << 8);  //禁止循环模式
	DMA2_Stream7->CR &= ~(3 << 6);   //置0
	DMA2_Stream7->CR |= (1 << 6);	//存储器到外设
	DMA2_Stream7->CR &= ~(1 << 5);  //DMA是流控制器

	DMA2_Stream7->NDTR = ndtr; //传输次数
	DMA2_Stream7->PAR = p_addr;  //外设地址
	DMA2_Stream7->M0AR = m_addr; //存储器地址
	//printf("NDTR: %d \r\n", DMA2_Stream7->NDTR);

	DMA2_Stream7->FCR &= ~(1 << 2); //FIFO模式关闭,使能直接模式
	USART1->CR3 |= (1 << 7); //使能DMA接收

	DMA2->HIFCR |= (1 << 27); //传输完成标志位清零

	DMA2_Stream7->CR |= (1 << 0); //使能数据流

}
