#include "frequency.h"

/**************************************************************
*函数功能：捕获PWM波频率配置
*参    数：None
*返 回 值：None
*备    注：PA0---TIM5_CH1    使用PC6输出PWM波
**************************************************************/
void Frequency_Init(void)
{
	RCC->AHB1ENR |= (1<<0);
	RCC->APB1ENR |= (1<<3);
	
	GPIOA->MODER &= ~(0x3<<0);
	GPIOA->MODER |= (0x2<<0);
	GPIOA->AFR[0] &= ~(0xf<<0);
	GPIOA->AFR[0] |= (0x2<<0);
	
	//TIM5
	TIM5->PSC = 84 - 1;//计一个数1us
	TIM5->ARR = 0xffff;//65536,65536*1us 计数周期：65.536ms
	
	//基本配置
	TIM5->CR1 |= (1<<7);//配置自动重载预装载（缓冲）
	TIM5->CR1 &= ~(0x3<<5);//边沿对齐模式：递增或递减
	TIM5->CR1 &= ~(1<<4);//方向：递增
	TIM5->CR1 &= ~(1<<3);//单脉冲模式关闭
	TIM5->CR1 &= ~(1<<1);//使能UEV
	TIM5->EGR |= (1<<0);//手动产生一个更新事件（把值更新）
	TIM5->SR &= ~(1<<0);//清除标志位
	
	//SMCR
	TIM5->SMCR &= ~(0x7<<4);
	TIM5->SMCR |= (0x5<<4);//触发选择:滤波后的定时器输入1(TI1FP1)
	TIM5->SMCR &= ~(0x7<<0);
	TIM5->SMCR |= (0x4<<0);//从模式选择复位模式
	
	//CCMR1
	TIM5->CCMR1 |= (0xf<<4);//滤波
	TIM5->CCMR1 &= ~(0x3<<2);//无分频：一个边沿触发一次
	TIM5->CCMR1 &= ~(0x3<<0);
	TIM5->CCMR1 |= (1<<0);//通道方向IC1 映射到 TI1 上
	
	//CCER
	TIM5->CCER &= ~(1<<3);
	TIM5->CCER &= ~(1<<1);//上升沿触发
	TIM5->CCER |= (1<<0);//使能捕获
	
	TIM5->CR1 |= (1<<0);//使能计数器
}


void Read_Fre(void)
{
	static u32 cnt = 0;
	if(TIM5->SR&(1<<1)) //捕获发生
	{
		cnt = TIM5->CCR1;//读计数值，CNT:在跑（0）
		printf("%d\r\n",cnt);//计数值知道
		printf("Fre = %.1fHz\r\n",1000000.0/(cnt+1));
	}
}







