#include "rgb.h"

/*
*************************************************************
*函数功能：RGB彩灯初始化
*参    数：None
*返 回 值：None
*备    注：GPIOA——(AHB1 168MHz)   TIM5_CH123——(APB1 42MHz)
		   R——PA0——TIM5_CH1   G——PA1——TIM5_CH2   B——PA2——TIM5_CH3
		   共阳RGB模块，R、G、B低电平有效；共阴RGB模块，R、G、B高电平有效
*************************************************************
*/
void RGB_Init(void)
{
	RCC->AHB1ENR |=(1<<0);  //打开GPIOA时钟
	RCC->APB1ENR |=(1<<3);  //打开TIM5时钟
	
	GPIOA->MODER &=~(0xFF<< 0*2);
	GPIOA->MODER |=(0x2A<< 0*2);  //PA012设置为复用模式
	GPIOA->AFR[0] &=~(0xFFF<< 0*4);
	GPIOA->AFR[0] |=(0x222<< 0*4);  //PA012复用功能为TIM5
	
	TIM5->CR1 |=(1<<7);  //使能自动重载预装载寄存器（缓冲区）
	TIM5->CR1 &=~(3<<5);  //边沿对齐模式
	TIM5->CR1 |=(1<<4);  //递减计数（向下计数）
	TIM5->CR1 &=~(1<<3);  //不开启单脉冲模式
	TIM5->CR1 &=~(1<<1);  //使能更新事件生成
	//R——TIM5_CH1
	TIM5->CCMR1 &=~(0x7<<4);
	TIM5->CCMR1 |=(0x6<<4);  //配置PWM1模式：cnt<ccr时为有效电平
	TIM5->CCMR1 |=(1<<3);  //输出比较1预装载使能
	TIM5->CCMR1 &=~(3<<0);  //输出通道选择为CC1
	TIM5->CCER |=(1<<1);  //低电平有效
	TIM5->CCER |=(1<<0);  //使能通道CH1
	//G——TIM5_CH2
	TIM5->CCMR1 &=~(0x7<<12);
	TIM5->CCMR1 |=(0x6<<12);  //配置PWM1模式：cnt<ccr时为有效电平
	TIM5->CCMR1 |=(1<<11);  //输出比较2预装载使能
	TIM5->CCMR1 &=~(3<<8);  //输出通道选择为CC2
	TIM5->CCER |=(1<<5);  //低电平有效
	TIM5->CCER |=(1<<4);  //使能通道CH2
	
	//B——TIM5_CH3
	TIM5->CCMR2 &=~(0x7<<4);
	TIM5->CCMR2 |=(0x6<<4);  //配置PWM1模式：cnt<ccr时为有效电平
	TIM5->CCMR2 |=(1<<3);  //输出比较3预装载使能
	TIM5->CCMR2 &=~(3<<0);  //输出通道选择为CC3
	TIM5->CCER |=(1<<9);  //低电平有效
	TIM5->CCER |=(1<<8);  //使能通道CH3
	
	TIM5->PSC = 84-1;  //1us计一个数
	TIM5->ARR = 256-1;  //周期256us
	TIM5->CCR1 = 0;  //R占空比
	TIM5->CCR2 = 0;  //G占空比
	TIM5->CCR3 = 0;  //B占空比
	
	TIM5->EGR |=(1<<0);  //手动生成更新事件将psc和arr的值更新
	TIM5->SR &=~(1<<0);  //清除标志位
	
	TIM5->CR1 |=(1<<0);  //使能计数器
}

void RGB_Color(u8 r,u8 g,u8 b)
{
	TIM5->CCR1 = r;  //R占空比
	TIM5->CCR2 = g;  //G占空比
	TIM5->CCR3 = b;  //B占空比
}



