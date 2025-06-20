#include "keytime.h"

/**************************************************************
*函数功能：测量按键按下时间配置
*参    数：None
*返 回 值：None
*备    注：PA0---TIM5_CH1
**************************************************************/
void KeyTime_Init(void)
{
	RCC->AHB1ENR |= (1<<0);
	RCC->APB1ENR |= (1<<3);
	
	GPIOA->MODER &= ~(0x3<<0);
	GPIOA->MODER |= (0x2<<0);
	GPIOA->AFR[0] &= ~(0xf<<0);
	GPIOA->AFR[0] |= (0x2<<0);
	
	//TIM5
	TIM5->PSC = 84 - 1;//计一个数1us
	TIM5->ARR = 0xffff;
	
	//基本配置
	TIM5->CR1 |= (1<<7);//配置自动重载预装载（缓冲）
	TIM5->CR1 &= ~(0x3<<5);//边沿对齐模式：递增或递减
	TIM5->CR1 &= ~(1<<4);//方向：递增
	TIM5->CR1 &= ~(1<<3);//单脉冲模式关闭
	TIM5->CR1 &= ~(1<<1);//使能UEV
	TIM5->EGR |= (1<<0);//手动产生一个更新事件（把值更新）
	TIM5->SR &= ~(1<<0);//清除标志位
	
	//CCMR1
	TIM5->CCMR1 |= (0xf<<4);//滤波
	TIM5->CCMR1 &= ~(0x3<<2);//无分频：一个边沿触发一次
	TIM5->CCMR1 &= ~(0x3<<0);
	TIM5->CCMR1 |= (1<<0);//通道方向IC1 映射到 TI1 上
	
	//CCER
	TIM5->CCER &= ~(1<<3);
	TIM5->CCER &= ~(1<<1);//上升沿触发
	TIM5->CCER |= (1<<0);//使能捕获
	
	//捕获中断   更新中断
	TIM5->DIER |= (1<<0);//更新中断
	TIM5->DIER |= (1<<1);//捕获中断
	
	//配置NVIC
	NVIC_SetPriority(TIM5_IRQn,NVIC_EncodePriority(5,1,0));
	NVIC_EnableIRQ(TIM5_IRQn);
	
	TIM5->CR1 |= (1<<0);//使能计数器
}

volatile u8 keytime_flag = 0;
volatile float keytime = 0;

void Read_KeyTime(void)
{
	if(keytime_flag == 1)
	{
		keytime_flag = 0;
		printf("keytime = %.1fms\r\n",keytime);
	}
	
}

void TIM5_IRQHandler(void)
{
	static u32 over_cnt = 0;
	static u32 cnt1 = 0,cnt2 = 0,count = 0;
	if(TIM5->SR&(1<<0))//判断有没有溢出
	{
		TIM5->SR &= ~(1<<0);//清除标志位
		over_cnt++;//溢出次数
	}
	if(TIM5->SR&(1<<1))//判断有没有捕获
	{
		TIM5->SR &= ~(1<<1);////清除标志位
		if(!(TIM5->CCER&(1<<1)))//上升沿进来
		{
			TIM5->CCER |=(1<<1);//改成下降沿
			cnt1 = TIM5->CCR1;//读计数值
			over_cnt = 0;
		}
		else if(TIM5->CCER&(1<<1))//下降沿进来
		{
			TIM5->CCER &= ~(1<<1);//改成上升沿
			cnt2 = TIM5->CCR1;
			count = cnt2 + 65536*over_cnt - cnt1;// count us
			keytime = (float)count/1000.0f;//ms
			over_cnt = 0;
			keytime_flag = 1;
		}
	}
}








