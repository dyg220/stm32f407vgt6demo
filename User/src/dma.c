#include "dma.h"
#include <stdio.h>

void DMA2_USART_TDATA(u32 p_addr, u32 m_addr, u32 ndtr) {

	RCC->AHB1ENR |= (1 << 22);  //ʹ��DMA2ʱ��

	DMA2_Stream7->CR &= ~(1 << 0); //���ENλ
	while (DMA2_Stream7->CR & (1 << 0)); //�ȴ�DMA2_Stream7_ENλ������

	DMA2_Stream7->CR &= ~(7 << 25); //���DMA2_Stream7_CHSELλ
	DMA2_Stream7->CR |= (4 << 25);	// ѡ��DMA2_Stream7_CHSELΪ4��USART2��
	DMA2_Stream7->CR &= ~(3 << 16); //���ȼ�Ϊ��
	DMA2_Stream7->CR &= ~(1 << 9);	//�����ַ�̶�
	DMA2_Stream7->CR &= ~(3 << 11);  //������������1byte
	DMA2_Stream7->CR |= (1 << 10);	//�洢����ַ����
	DMA2_Stream7->CR &= ~(3 << 13); //�洢����������1Byte
	DMA2_Stream7->CR &= ~(1 << 8);  //��ֹѭ��ģʽ
	DMA2_Stream7->CR &= ~(3 << 6);   //��0
	DMA2_Stream7->CR |= (1 << 6);	//�洢��������
	DMA2_Stream7->CR &= ~(1 << 5);  //DMA����������

	DMA2_Stream7->NDTR = ndtr; //�������
	DMA2_Stream7->PAR = p_addr;  //�����ַ
	DMA2_Stream7->M0AR = m_addr; //�洢����ַ
	//printf("NDTR: %d \r\n", DMA2_Stream7->NDTR);

	DMA2_Stream7->FCR &= ~(1 << 2); //FIFOģʽ�ر�,ʹ��ֱ��ģʽ
	USART1->CR3 |= (1 << 7); //ʹ��DMA����

	DMA2->HIFCR |= (1 << 27); //������ɱ�־λ����

	DMA2_Stream7->CR |= (1 << 0); //ʹ��������

}
