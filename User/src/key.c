#include "key.h"

u8 Key_Num = 0;

/*
*************************************************************
*函数功能：按键初始化
*参    数：None
*返 回 值：None
*备    注：KEY1---PA0  KEY2---PE2  KEY3---PE3  KEY4---PE4
*************************************************************
*/
void Key_Init(void)
{
	//打开时钟
	RCC->AHB1ENR |= (0x1 << 0);//GPIOA
	RCC->AHB1ENR |= (0x1 << 4);//GPIOE
	//配置输入模式(浮空）PA0
	GPIOA->MODER &= ~(0x3 << 0);
	GPIOA->PUPDR &= ~(0x3 << 0);
	//配置输入模式(浮空）PE2 PE3 PE4
	GPIOE->MODER &= ~(0x3F << 4);
	GPIOE->PUPDR &= ~(0x3F << 4);
}


/*
*************************************************************
*函数功能：按键扫描
*参    数：None
*返 回 值：None
*备    注：KEY1---PA0  KEY2---PE2  KEY3---PE3  KEY4---PE4
*************************************************************
*/
u8 Key_Scan(void)
{

#if  Key_Mode
	if (Key_Num)
	{
		u8 Temp = 0;
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
	return 0;
#else
	u16 time_cnt = 0;
	u8 key_num = 0;
	if (Key1_Press)
	{
		delay_ms(20);
		if (Key1_Press)
		{
			while (Key1_Press)
			{
				time_cnt++;
				delay_ms(1);
			}
			if (time_cnt < 500)
				key_num = key1_click;
			else
				key_num = key1_long;
		}
	}
	if (Key2_Press)
	{
		delay_ms(20);
		if (Key2_Press)
		{
			while (Key2_Press)
			{
				delay_ms(1);
				time_cnt++;
			}
			if (time_cnt < 500)
				key_num = key2_click;
			else
				key_num = key2_long;
		}
	}
	if (Key3_Press)
	{
		delay_ms(20);
		if (Key3_Press)
		{
			while (Key3_Press)
			{
				delay_ms(1);
				time_cnt++;
			}
			if (time_cnt < 500)
				key_num = key3_click;
			else
				key_num = key3_long;
		}
	}
	if (Key4_Press)
	{
		delay_ms(20);
		if (Key4_Press)
		{
			while (Key4_Press)
			{
				delay_ms(1);
				time_cnt++;
			}
			if (time_cnt < 500)
				key_num = key4_click;
			else
				key_num = key4_long;
		}
	}
	return key_num;
#endif
}

u8 Key_GetState(void)
{
	if (GPIOA->IDR & (1 << 0))
	{
		return key1_click;
	}
	if (!(GPIOE->IDR & (1 << 2)))
	{
		return key2_click;
	}
	if (!(GPIOE->IDR & (1 << 3)))
	{
		return key3_click;
	}
	if (!(GPIOE->IDR & (1 << 4)))
	{
		return key4_click;
	}
	return 0;
}





void Key_Tick(void)
{
	static u32 Count = 0;
	static u8 CurrState = 0, PrevState = 0;

	Count++;
	if (Count >= 20)
	{
		Count = 0;
		PrevState = CurrState;
		CurrState = Key_GetState();
		if (CurrState == 0 && PrevState != 0)
		{
			Key_Num = PrevState;
		}
	}
}






