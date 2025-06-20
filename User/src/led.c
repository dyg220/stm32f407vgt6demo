#include "led.h"

u32 Led1_Count = 0;
u8 Led1_Mode = 0;

/*
*************************************************************
*函数功能：LED初始化
*参    数：None
*返 回 值：None
*备    注：LED1---PC4  LED2---PC5  LED3---PC6  LED4---PC7
		   都是低电平点亮
*************************************************************
*/
void LED_Init(void)
{
	RCC->AHB1ENR |= (0x1 << 2);//打开GPIOC时钟
	//配置通用输出模式
	GPIOC->MODER &= ~(0xff << 8);
	GPIOC->MODER |= (0x55 << 8);
	//配置推挽输出
	GPIOC->OTYPER &= ~(0xf << 4);
	//配置输出速度(低速)
	GPIOC->OSPEEDR &= ~(0xff << 8);

	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}

void Led1_SetMode(u8 Mode)
{
	Led1_Mode = Mode;
}


//LED定时中断执行函数
void Led_Tick(void)
{
	if (Led1_Mode == 0)
	{
		LED1_OFF;
	}
	if (Led1_Mode == 1)
	{
		Led1_Count++;
		Led1_Count %= 1000;
		if (Led1_Count < 500)
		{
			LED1_ON;
		}
		else
		{
			LED1_OFF;
		}
	}
}





