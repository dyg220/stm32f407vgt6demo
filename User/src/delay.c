#include "delay.h"


/*
*************************************************************
*函数功能：延时函数(us)
*参    数：xus
*返 回 值：None
*备    注：处理器时钟：168M          最大计时时间：99.86ms
           外部参考时钟：21M         最大计时时间：798.9ms
*************************************************************
*/
void delay_us(uint32_t xus)
{
	SysTick->CTRL &= ~(1<<2);//外部参考时钟
	SysTick->LOAD = 21*xus - 1;//重装载值
	SysTick->VAL = 0;//清除计数值
	SysTick->CTRL &= ~(1<<1);//不使能中断
	SysTick->CTRL |= (1<<0);//启动计数器
	while(!(SysTick->CTRL&(1<<16)));//等待计数完成
	SysTick->CTRL &= ~(1<<0);//关闭计数器
}


//毫秒级延时
void delay_ms(uint32_t xms)
{
	while(xms--)
	{
		delay_us(1000);
	}
}


/*
*************************************************************
*函数功能：定时中断
*参    数：xms
*返 回 值：None
*备    注：处理器时钟：168M          最大计时时间：99.86ms
           外部参考时钟：21M         最大计时时间：798.9ms
*************************************************************
*/
void SysTick_IRQ_Config(uint32_t xms)
{
	SysTick->CTRL &= ~(1<<2);//外部参考时钟
	SysTick->LOAD = 21000*xms - 1;//重装载值
	SysTick->VAL = 0;//清除计数值
	SysTick->CTRL |= (1<<1);//使能中断
	SysTick->CTRL |= (1<<0);//启动计数器
	NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(5,1,1));
}


///*
//*************************************************************
//*函数功能：延时函数(us)
//*参    数：xus
//*返 回 值：None
//*备    注：主频168Mhz   时钟周期1/168 us
//           执行__NOP();空语句为一个时钟周期
//		   粗略延时
//*************************************************************
//*/
//void delay_us(uint32_t xus)
//{
//	while(xus--)
//	{
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//	}
//}


///*
//*************************************************************
//*函数功能：延时函数(ms)
//*参    数：mus
//*返 回 值：None
//*备    注：主频168Mhz   时钟周期1/168 us
//           执行__NOP();空语句为一个时钟周期
//		   粗略延时
//*************************************************************
//*/
//void delay_ms(uint32_t xms)
//{
//	while(xms--)
//	{
//		delay_us(1000);
//	}
//}

