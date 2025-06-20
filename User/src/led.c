#include "led.h"

u32 Led1_Count = 0;
u8 Led1_Mode = 0;

/*
*************************************************************
*�������ܣ�LED��ʼ��
*��    ����None
*�� �� ֵ��None
*��    ע��LED1---PC4  LED2---PC5  LED3---PC6  LED4---PC7
		   ���ǵ͵�ƽ����
*************************************************************
*/
void LED_Init(void)
{
	RCC->AHB1ENR |= (0x1 << 2);//��GPIOCʱ��
	//����ͨ�����ģʽ
	GPIOC->MODER &= ~(0xff << 8);
	GPIOC->MODER |= (0x55 << 8);
	//�����������
	GPIOC->OTYPER &= ~(0xf << 4);
	//��������ٶ�(����)
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


//LED��ʱ�ж�ִ�к���
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





