#include "usart1.h"
#include <stdio.h>
#include <string.h>


/**************************************************************
*�������ܣ�USART1��ʼ��
*��    ����None
*�� �� ֵ��None
*��    ע��PA9---USART1_TX   PA10---USART1_RX  �����ʣ�115200
**************************************************************/

void USART1_Init(u32 baud)
{
	float Div = 0;
	u32 Mantissa = 0;
	u32 Fraction = 0;
	//��ʱ��
	RCC->AHB1ENR |= (1 << 0);//GPIOA
	RCC->APB2ENR |= (1 << 4);//USART1
	//����GPIO
	//����ģʽ
	GPIOA->MODER &= ~(0xf << 18);
	GPIOA->MODER |= (0xa << 18);
	//�������
	GPIOA->OTYPER &= ~(0x3 << 9);
	//����ٶ�(ͨ��Э�龡�����ߣ�
	GPIOA->OSPEEDR &= ~(0xf << 18);
	GPIOA->OSPEEDR |= (0xf << 18);
	//���õ�USART1
	GPIOA->AFR[1] &= ~(0xff << 4);
	GPIOA->AFR[1] |= (0x77 << 4);

	Div = 84000000.0 / (16 * baud);//45.57
	Mantissa = (u32)Div;//45
	Fraction = (u32)((Div - Mantissa) * 16);//9

	//����USART1
	USART1->CR1 &= ~(1 << 15);//������(16)
	USART1->BRR = (Mantissa << 4) | Fraction;//������
	USART1->CR1 &= ~(1 << 12);//8�ֳ�
	USART1->CR1 &= ~(1 << 10);//У��λ(��ֹ)
	USART1->CR1 |= (1 << 3);//������ʹ��
	USART1->CR1 |= (1 << 2);//������ʹ��
	USART1->CR2 &= ~(0x3 << 12);//ֹͣλ

}


//����һ���ֽ�����
void USART1_SendByte(u8 Data)
{
	while ((USART1->SR & (1 << 7)) == 0);//�жϷ������ݼĴ����Ƿ��(Ϊ��TXE=1����)
	USART1->DR = Data;
}

//����һ���ַ���
void USART1_SendStr(u8* str)
{
	while (*str != '\0')
	{
		USART1_SendByte(*str);
		str++;
	}
	USART1_SendByte('\0');
}


int fputc(int ch, FILE* p)
{
	USART1_SendByte(ch);
	return ch;
}


/**************************************************************
*�������ܣ�USART1�ж�����
*��    ����None
*�� �� ֵ��None
*��    ע�����������ж�+�����ж�    ��ռ��1       ��Ӧ��2
**************************************************************/
void USART1_IRQ_Config(void)
{
	//ʹ�ܽ���+�����жϿ���λ
	USART1->CR1 |= (1 << 5);//RXNEIE��1
	USART1->CR1 |= (1 << 4);//RXNEIE��1

	//����NVIC
	NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(5, 1, 2));//�������ȼ�
	NVIC_EnableIRQ(USART1_IRQn);//ʹ���ж�ͨ��
	USART1->CR1 |= (1 << 13);//ʹ��USART1(ע��ŵ������)
}

extern USART1_INFO USART1_Recv;

void RECV_USART1(void)
{
	if (USART1_Recv.flag == 1)
	{
		printf("%s\r\n", USART1_Recv.data);
		USART1_Recv.flag = 0;
		memset(USART1_Recv.data, 0, sizeof(USART1_Recv.data));
	}
}


/*
//����һ���ֽ�����
u8 USART1_RecvByte(void)
{
	u8 data = 0;
	while((USART1->SR&(1<<5)) == 0);//�жϷ������ݼĴ����Ƿ��(��ΪΪ��RXNE=1����)
	data = USART1->DR;
	return data;
}
//����һ���ַ���
void USART1_RecvStr(u8* str)
{
	u8 data = 0;
	do
	{
		data = USART1_RecvByte(); //abc\r\n
		*str= data;  //[0]a
		str++;
	}while(data != '\n');
	*str = '\0';
}

*/
