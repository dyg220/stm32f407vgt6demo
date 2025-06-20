#include "exti.h"

/**************************************************************
*�������ܣ�EXTI0�ж�����
*��    ����None
*�� �� ֵ��None
*��    ע��PA0ӳ�䵽EXTI0��  ����1����ʱ������  ����ʱ�½���
**************************************************************/
void EXTI0_IRQ_Config(void)
{
	RCC->APB2ENR |= (1<<14);//��SYSCFGʱ��
	SYSCFG->EXTICR[0] &= ~(0xf<<0);//��PA0ӳ�䵽EXTI0��
	
	EXTI->RTSR &= ~(1<<0);//�ر������ش���
	EXTI->FTSR |= (1<<0);//���½��ش���
	
	EXTI->IMR |= (1<<0);//����EXTI0��
	
	//����NVIC
	NVIC_SetPriority(EXTI0_IRQn,NVIC_EncodePriority(5,2,2));//�������ȼ�
	NVIC_EnableIRQ(EXTI0_IRQn);//ʹ���ж�ͨ��
}

/**************************************************************
*�������ܣ�����ж�����
*��    ����None
*�� �� ֵ��None
*��    ע����Ҫ�����ж�����
**************************************************************/
void EXTI6_IRQ_Config(void)
{
	EXTI->IMR |= (1<<6);//����EXTI6��
	
	//����NVIC
	NVIC_SetPriority(EXTI9_5_IRQn,NVIC_EncodePriority(5,2,2));//�������ȼ�
	NVIC_EnableIRQ(EXTI9_5_IRQn);//ʹ���ж�ͨ��
}





