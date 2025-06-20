#include "usart1.h"
#include <stdio.h>
#include <string.h>


/**************************************************************
*函数功能：USART1初始化
*参    数：None
*返 回 值：None
*备    注：PA9---USART1_TX   PA10---USART1_RX  波特率：115200
**************************************************************/

void USART1_Init(u32 baud)
{
	float Div = 0;
	u32 Mantissa = 0;
	u32 Fraction = 0;
	//打开时钟
	RCC->AHB1ENR |= (1 << 0);//GPIOA
	RCC->APB2ENR |= (1 << 4);//USART1
	//配置GPIO
	//复用模式
	GPIOA->MODER &= ~(0xf << 18);
	GPIOA->MODER |= (0xa << 18);
	//推挽输出
	GPIOA->OTYPER &= ~(0x3 << 9);
	//输出速度(通信协议尽量调高）
	GPIOA->OSPEEDR &= ~(0xf << 18);
	GPIOA->OSPEEDR |= (0xf << 18);
	//复用到USART1
	GPIOA->AFR[1] &= ~(0xff << 4);
	GPIOA->AFR[1] |= (0x77 << 4);

	Div = 84000000.0 / (16 * baud);//45.57
	Mantissa = (u32)Div;//45
	Fraction = (u32)((Div - Mantissa) * 16);//9

	//配置USART1
	USART1->CR1 &= ~(1 << 15);//过采样(16)
	USART1->BRR = (Mantissa << 4) | Fraction;//波特率
	USART1->CR1 &= ~(1 << 12);//8字长
	USART1->CR1 &= ~(1 << 10);//校验位(禁止)
	USART1->CR1 |= (1 << 3);//发送器使能
	USART1->CR1 |= (1 << 2);//接收器使能
	USART1->CR2 &= ~(0x3 << 12);//停止位

}


//发送一个字节数据
void USART1_SendByte(u8 Data)
{
	while ((USART1->SR & (1 << 7)) == 0);//判断发送数据寄存器是否空(为空TXE=1跳出)
	USART1->DR = Data;
}

//发送一个字符串
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
*函数功能：USART1中断配置
*参    数：None
*返 回 值：None
*备    注：开启接收中断+空闲中断    抢占：1       响应：2
**************************************************************/
void USART1_IRQ_Config(void)
{
	//使能接收+空闲中断控制位
	USART1->CR1 |= (1 << 5);//RXNEIE置1
	USART1->CR1 |= (1 << 4);//RXNEIE置1

	//配置NVIC
	NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(5, 1, 2));//设置优先级
	NVIC_EnableIRQ(USART1_IRQn);//使能中断通道
	USART1->CR1 |= (1 << 13);//使能USART1(注意放到最后开启)
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
//接收一个字节数据
u8 USART1_RecvByte(void)
{
	u8 data = 0;
	while((USART1->SR&(1<<5)) == 0);//判断发送数据寄存器是否空(不为为空RXNE=1跳出)
	data = USART1->DR;
	return data;
}
//接收一个字符串
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
