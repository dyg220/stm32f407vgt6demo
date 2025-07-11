#include "beep.h"

/*
*************************************************************
*函数功能：蜂鸣器初始化
*参    数：None
*返 回 值：None
*备    注：BEEP---PE0
*************************************************************
*/
void Beep_Init(void)
{
	//打开时钟
	RCC->AHB1ENR |= (0x1<<4);
	//配置通用输出模式
	GPIOE->MODER &= ~(0x3<<0);
	GPIOE->MODER |= (0x1<<0);
	//配置推挽输出
	GPIOE->OTYPER &= ~(0x1<<0);
	//配置输出速度(低速)
	GPIOE->OSPEEDR &= ~(0x3<<0);
}



