#include "breathe.h"

/**************************************************************
*函数功能：TIM3定时中断配置
*参    数：None
*返 回 值：None
*备    注：APB1---42MHz TIM3---84MHz(LED3--PC6--TIM3_CH1)
**************************************************************/
void Breathe_Init(void)
{
	//打开时钟
	RCC->AHB1ENR |= (1<<2);
	RCC->APB1ENR |= (1<<1);
	//配置GPIO
	GPIOC->MODER &= ~(0x3<<12);
	GPIOC->MODER |= (0x2<<12);
	GPIOC->AFR[0] &= ~(0xf<<24);
	GPIOC->AFR[0] |= (0x2<<24);

	//TIM3
	TIM3->PSC = 84 - 1;//记一个数为1us
	TIM3->ARR = 1000 - 1;//一个计数周期为1ms
	//84M/84-1+1/(1000) = 1000Hz
	TIM3->CR1 |= (1<<7);//配置自动重载预装载（缓冲）
	TIM3->CR1 &= ~(0x3<<5);//边沿对齐模式
	TIM3->CR1 &= ~(1<<4);//方向：递增
	TIM3->CR1 &= ~(1<<3);//单脉冲模式关闭
//	TIM3->CR1 &= ~(1<<2);//更新请求源
	TIM3->CR1 &= ~(1<<1);//使能UEV
	TIM3->EGR |= (1<<0);//手动产生一个更新事件（把值更新）
	TIM3->SR &= ~(1<<0);//清除标志位
	
	//配置通道
	//CCMR1
	TIM3->CCMR1 &= ~(0x7<<4);
	TIM3->CCMR1 |= (0x6<<4);//配置PWM1模式
	TIM3->CCMR1 &= ~(1<<3);//输出比较 1 预装载使能
	TIM3->CCMR1 &= ~(0x3<<0);//CC1 通道配置为输出
	//CCER
	TIM3->CCER &= ~(1<<3);//输出CC1NP 必须保持清零
	TIM3->CCER |= (1<<1);//有效电平为低电平
	TIM3->CCER |= (1<<0);//通道使能
	
	//CCR1
	TIM3->CCR1 = 500;
	
	TIM3->CR1 |= (1<<0);//使能计数器
}


void Breathe_Show(void)
{
	int i;
	//从灭到最亮
	for(i = 0;i<1000;i++)
	{
		TIM3->CCR1 = i;
		delay_ms(2);
	}
	//从最亮到灭
	for(i = 999;i>0;i--)
	{
		TIM3->CCR1 = i;
		delay_ms(2);
	}
}



