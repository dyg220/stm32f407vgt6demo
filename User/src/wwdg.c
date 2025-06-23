#include "wwdg.h"


/**************************************************************
*函数功能：窗口看门狗配置
*参    数：None
*返 回 值：None
*备    注：
**************************************************************/
void WWDG_Init(void)
{
	RCC->APB1ENR |= (1 << 11);//打开时钟
	WWDG->CFR |= (0x3 << 7);//8分频
	WWDG->CFR |= (0x70 << 0);//设置窗口值
	WWDG->CFR |= (1 << 9);//使能提前唤醒中断
	WWDG->SR &= ~(1 << 0);//清除标志位
	WWDG->CR |= (0x7F << 0);//填入计数值

	//配置NVIC
	NVIC_SetPriority(WWDG_IRQn, NVIC_EncodePriority(5, 1, 0));
	NVIC_EnableIRQ(WWDG_IRQn);
	//打开看门狗
	WWDG->CR |= (1 << 7);
}


