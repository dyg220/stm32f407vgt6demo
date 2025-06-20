#include "nvic.h"

USART1_INFO USART1_Recv = { 0 };
/**************************************************************
*�������ܣ�USART1�жϷ�����
*��    ����None
*�� �� ֵ��None
*��    ע������������RXNEIE��IDLEIE�жϿ���λ�Ѿ�ʹ��
					 �ȴ�RXNE����IDLEΪ1
**************************************************************/

void USART1_IRQHandler(void)
{
	u8 clern = 0;
	// �ж����ĸ��¼�������ж�
	if (USART1->SR & (1 << 5)) // �����жϴ���
	{
		if (USART1_Recv.index < Max_Size - 1) // ��ֹ���
		{
			USART1_Recv.data[USART1_Recv.index] = USART1->DR; //;�����ݶ������������־λ
			USART1_Recv.index++;
		}
	}
	if (USART1->SR & (1 << 4)) // �����жϴ���
	{
		// ����жϱ�־λIDLE
		clern = USART1->SR;
		clern = USART1->DR;
		(void)clern;
		USART1_Recv.data[USART1_Recv.index] = '\0';
		USART1_Recv.flag = 1;
		USART1_Recv.index = 0;
	}
}

/**************************************************************
 *�������ܣ�EXTI0�жϷ�����
 *��    ����None
 *�� �� ֵ��None
 *��    ע��
 **************************************************************/
void EXTI0_IRQHandler(void)
{
	if (EXTI->PR & (1 << 0))
	{
		EXTI->PR |= (1 << 0); // �����־λ
	}
}

u8 EXTI6_flag = 0;
/**************************************************************
 *�������ܣ�EXTI6�жϷ�����
 *��    ����None
 *�� �� ֵ��None
 *��    ע��
 **************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if (EXTI->PR & (1 << 6))
	{
		EXTI->PR |= (1 << 6); // �����־λ
		EXTI6_flag = 1;
	}
}

void SysTick_Handler(void)
{
	if (SysTick->CTRL & (1 << 16))
	{
		// �����־λ
		SysTick->VAL = 0;
	}
}

/**************************************************************
 *�������ܣ����ڿ��Ź��жϷ�����
 *��    ����None
 *�� �� ֵ��None
 *��    ע��
 **************************************************************/
u8 wwdg_flag = 0;
void WWDG_IRQHandler(void)
{
	if (WWDG->SR & (1 << 0)) // �ж��жϱ�־λ
	{
		WWDG->CR |= (0x7F << 0); // �������ֵ
		WWDG->SR &= ~(1 << 0);   // �����־λ//����жϱ�־λ
		wwdg_flag = 1;
	}
}

ADC_t ADC_VAL = { 0 };
volatile u8 light_flag = 0;
volatile u8 mq2_flag = 0;
/**************************************************************
 *�������ܣ�ADC�жϷ�����
 *��    ����None
 *�� �� ֵ��None
 *��    ע��PC0-----ADC1_CH10   PC2----ADC1_CH12
 **************************************************************/
void ADC_IRQHandler(void)
{
	static u8 ADC_flag = 0;
	static u16 temp1 = 0, temp2 = 0;
	if (ADC1->SR & (1 << 1) && !(ADC_flag)) // ADC1��EOC��1
	{
		while (!(ADC1->SR & (1 << 1)))
			;
		temp1 = ADC1->DR;
		ADC_VAL.light_data = (float)(4095 - temp1) / 4095.0f * 100;
		ADC_flag = 1;
		light_flag = 1;
	}
	if (ADC1->SR & (1 << 1) && (ADC_flag)) // ADC1��EOC��1
	{
		while (!(ADC1->SR & (1 << 1)))
			;
		temp2 = ADC1->DR;
		ADC_VAL.mq2_data = (float)temp2 / 4095.0f * 100;
		ADC_flag = 0;
		mq2_flag = 1;
	}
}
