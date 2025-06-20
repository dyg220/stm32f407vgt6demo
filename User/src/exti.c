#include "exti.h"

/**************************************************************
*函数功能：EXTI0中断配置
*参    数：None
*返 回 值：None
*备    注：PA0映射到EXTI0上  按键1按下时上升沿  松手时下降沿
**************************************************************/
void EXTI0_IRQ_Config(void)
{
	RCC->APB2ENR |= (1<<14);//打开SYSCFG时钟
	SYSCFG->EXTICR[0] &= ~(0xf<<0);//将PA0映射到EXTI0上
	
	EXTI->RTSR &= ~(1<<0);//关闭上升沿触发
	EXTI->FTSR |= (1<<0);//打开下降沿触发
	
	EXTI->IMR |= (1<<0);//开放EXTI0线
	
	//配置NVIC
	NVIC_SetPriority(EXTI0_IRQn,NVIC_EncodePriority(5,2,2));//设置优先级
	NVIC_EnableIRQ(EXTI0_IRQn);//使能中断通道
}

/**************************************************************
*函数功能：软件中断配置
*参    数：None
*返 回 值：None
*备    注：需要开放中断请求
**************************************************************/
void EXTI6_IRQ_Config(void)
{
	EXTI->IMR |= (1<<6);//开放EXTI6线
	
	//配置NVIC
	NVIC_SetPriority(EXTI9_5_IRQn,NVIC_EncodePriority(5,2,2));//设置优先级
	NVIC_EnableIRQ(EXTI9_5_IRQn);//使能中断通道
}





