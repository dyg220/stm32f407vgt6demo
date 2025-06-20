#include "nvic.h"

USART1_INFO USART1_Recv = { 0 };
/**************************************************************
*函数功能：USART1中断服务函数
*参    数：None
*返 回 值：None
*备    注：两个条件：RXNEIE和IDLEIE中断控制位已经使能
					 等待RXNE或者IDLE为1
**************************************************************/

void USART1_IRQHandler(void)
{
	u8 clern = 0;
	// 判断是哪个事件进入的中断
	if (USART1->SR & (1 << 5)) // 接收中断处理
	{
		if (USART1_Recv.index < Max_Size - 1) // 防止溢出
		{
			USART1_Recv.data[USART1_Recv.index] = USART1->DR; //;将数据读出还能清除标志位
			USART1_Recv.index++;
		}
	}
	if (USART1->SR & (1 << 4)) // 空闲中断处理
	{
		// 清除中断标志位IDLE
		clern = USART1->SR;
		clern = USART1->DR;
		(void)clern;
		USART1_Recv.data[USART1_Recv.index] = '\0';
		USART1_Recv.flag = 1;
		USART1_Recv.index = 0;
	}
}

/**************************************************************
 *函数功能：EXTI0中断服务函数
 *参    数：None
 *返 回 值：None
 *备    注：
 **************************************************************/
void EXTI0_IRQHandler(void)
{
	if (EXTI->PR & (1 << 0))
	{
		EXTI->PR |= (1 << 0); // 清除标志位
	}
}

u8 EXTI6_flag = 0;
/**************************************************************
 *函数功能：EXTI6中断服务函数
 *参    数：None
 *返 回 值：None
 *备    注：
 **************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if (EXTI->PR & (1 << 6))
	{
		EXTI->PR |= (1 << 6); // 清除标志位
		EXTI6_flag = 1;
	}
}

void SysTick_Handler(void)
{
	if (SysTick->CTRL & (1 << 16))
	{
		// 清除标志位
		SysTick->VAL = 0;
	}
}

/**************************************************************
 *函数功能：窗口看门狗中断服务函数
 *参    数：None
 *返 回 值：None
 *备    注：
 **************************************************************/
u8 wwdg_flag = 0;
void WWDG_IRQHandler(void)
{
	if (WWDG->SR & (1 << 0)) // 判断中断标志位
	{
		WWDG->CR |= (0x7F << 0); // 填入计数值
		WWDG->SR &= ~(1 << 0);   // 清除标志位//清除中断标志位
		wwdg_flag = 1;
	}
}

ADC_t ADC_VAL = { 0 };
volatile u8 light_flag = 0;
volatile u8 mq2_flag = 0;
/**************************************************************
 *函数功能：ADC中断服务函数
 *参    数：None
 *返 回 值：None
 *备    注：PC0-----ADC1_CH10   PC2----ADC1_CH12
 **************************************************************/
void ADC_IRQHandler(void)
{
	static u8 ADC_flag = 0;
	static u16 temp1 = 0, temp2 = 0;
	if (ADC1->SR & (1 << 1) && !(ADC_flag)) // ADC1的EOC置1
	{
		while (!(ADC1->SR & (1 << 1)))
			;
		temp1 = ADC1->DR;
		ADC_VAL.light_data = (float)(4095 - temp1) / 4095.0f * 100;
		ADC_flag = 1;
		light_flag = 1;
	}
	if (ADC1->SR & (1 << 1) && (ADC_flag)) // ADC1的EOC置1
	{
		while (!(ADC1->SR & (1 << 1)))
			;
		temp2 = ADC1->DR;
		ADC_VAL.mq2_data = (float)temp2 / 4095.0f * 100;
		ADC_flag = 0;
		mq2_flag = 1;
	}
}
