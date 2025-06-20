#include "ultrasonic.h"

/**************************************************************
*函数功能：超声波测距
*参    数：None
*返 回 值：None
*备    注：PB8---TIM4_CH3---ECHO    PB9---TRIG
**************************************************************/
void Ultrasonic_Init(void)
{
	RCC->AHB1ENR |= (1<<1);
	RCC->APB1ENR |= (1<<2);
	
	//PB9 通用输出(推挽)
	GPIOB->MODER &= ~(0x3<<18);
	GPIOB->MODER |= (1<<18);
	GPIOB->OTYPER &= ~(1<<9);
	
	//PB8 复用TIM4
	GPIOB->MODER &= ~(0x3<<16);
	GPIOB->MODER |= (0x2<<16);
	GPIOB->AFR[1] &= ~(0xf<<0);
	GPIOB->AFR[1] |= (0x2<<0);
	
	//TIM4
	TIM4->PSC = 84 - 1;//计一个数1us
	TIM4->ARR = 0xffff;
	
	//基本配置
	TIM4->CR1 |= (1<<7);//配置自动重载预装载（缓冲）
	TIM4->CR1 &= ~(0x3<<5);//边沿对齐模式：递增或递减
	TIM4->CR1 &= ~(1<<4);//方向：递增
	TIM4->CR1 &= ~(1<<3);//单脉冲模式关闭
	TIM4->CR1 &= ~(1<<1);//使能UEV
	TIM4->EGR |= (1<<0);//手动产生一个更新事件（把值更新）
	TIM4->SR &= ~(1<<0);//清除标志位
	
	//CCMR1
	TIM4->CCMR2 |= (0xf<<4);//滤波
	TIM4->CCMR2 &= ~(0x3<<2);//无分频：一个边沿触发一次
	TIM4->CCMR2 &= ~(0x3<<0);
	TIM4->CCMR2 |= (1<<0);//通道方向IC1 映射到 TI1 上
	
	//CCER     CH3
	TIM4->CCER &= ~(1<<11);
	TIM4->CCER &= ~(1<<9);//上升沿触发
	TIM4->CCER |= (1<<8);//使能捕获
	
	//捕获中断   更新中断
	TIM4->DIER |= (1<<0);//更新中断
	TIM4->DIER |= (1<<3);//捕获中断
	
	//配置NVIC
	NVIC_SetPriority(TIM4_IRQn,NVIC_EncodePriority(5,1,0));
	NVIC_EnableIRQ(TIM4_IRQn);
	
	TIM4->CR1 |= (1<<0);//使能计数器
	
}

volatile float distance = 0;
volatile u8 ultrasonic_flag = 0;

void TIM4_IRQHandler(void)
{
	static u32 over_cnt = 0;
	static u32 cnt1 = 0,cnt2 = 0,count = 0;
	
	if(TIM4->SR&(1<<0))//判断有没有溢出
	{
		TIM4->SR &= ~(1<<0);//清除标志位
		over_cnt++;//溢出次数
	}
	
	if(TIM4->SR&(1<<3))//判断有没有捕获
	{
		TIM4->SR &= ~(1<<3);////清除标志位
		if(!(TIM4->CCER&(1<<9)))//上升沿进来
		{
			TIM4->CCER |=(1<<9);//改成下降沿
			cnt1 = TIM4->CCR3;//读计数值
			over_cnt = 0;
		}
		else if(TIM4->CCER&(1<<9))//下降沿进来
		{
			TIM4->CCER &= ~(1<<9);//改成上升沿
			cnt2 = TIM4->CCR3;
			count = cnt2 + 65536*over_cnt - cnt1;// count us
			distance = (float)count/58.0f;
			ultrasonic_flag = 1;
			over_cnt = 0;
		}
	}
}

void Start_Ultrasonic(void)
{
	TRIG_H;
	delay_us(15);
	TRIG_L;
}

void Read_Distance(void)
{
	Start_Ultrasonic();
	while(!ultrasonic_flag);//等待转换结束
	ultrasonic_flag = 0;
	printf("distance = %.1fcm\r\n",distance);
}




