#include "keytime.h"

/**************************************************************
*�������ܣ�������������ʱ������
*��    ����None
*�� �� ֵ��None
*��    ע��PA0---TIM5_CH1
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
	TIM5->PSC = 84 - 1;//��һ����1us
	TIM5->ARR = 0xffff;
	
	//��������
	TIM5->CR1 |= (1<<7);//�����Զ�����Ԥװ�أ����壩
	TIM5->CR1 &= ~(0x3<<5);//���ض���ģʽ��������ݼ�
	TIM5->CR1 &= ~(1<<4);//���򣺵���
	TIM5->CR1 &= ~(1<<3);//������ģʽ�ر�
	TIM5->CR1 &= ~(1<<1);//ʹ��UEV
	TIM5->EGR |= (1<<0);//�ֶ�����һ�������¼�����ֵ���£�
	TIM5->SR &= ~(1<<0);//�����־λ
	
	//CCMR1
	TIM5->CCMR1 |= (0xf<<4);//�˲�
	TIM5->CCMR1 &= ~(0x3<<2);//�޷�Ƶ��һ�����ش���һ��
	TIM5->CCMR1 &= ~(0x3<<0);
	TIM5->CCMR1 |= (1<<0);//ͨ������IC1 ӳ�䵽 TI1 ��
	
	//CCER
	TIM5->CCER &= ~(1<<3);
	TIM5->CCER &= ~(1<<1);//�����ش���
	TIM5->CCER |= (1<<0);//ʹ�ܲ���
	
	//�����ж�   �����ж�
	TIM5->DIER |= (1<<0);//�����ж�
	TIM5->DIER |= (1<<1);//�����ж�
	
	//����NVIC
	NVIC_SetPriority(TIM5_IRQn,NVIC_EncodePriority(5,1,0));
	NVIC_EnableIRQ(TIM5_IRQn);
	
	TIM5->CR1 |= (1<<0);//ʹ�ܼ�����
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
	if(TIM5->SR&(1<<0))//�ж���û�����
	{
		TIM5->SR &= ~(1<<0);//�����־λ
		over_cnt++;//�������
	}
	if(TIM5->SR&(1<<1))//�ж���û�в���
	{
		TIM5->SR &= ~(1<<1);////�����־λ
		if(!(TIM5->CCER&(1<<1)))//�����ؽ���
		{
			TIM5->CCER |=(1<<1);//�ĳ��½���
			cnt1 = TIM5->CCR1;//������ֵ
			over_cnt = 0;
		}
		else if(TIM5->CCER&(1<<1))//�½��ؽ���
		{
			TIM5->CCER &= ~(1<<1);//�ĳ�������
			cnt2 = TIM5->CCR1;
			count = cnt2 + 65536*over_cnt - cnt1;// count us
			keytime = (float)count/1000.0f;//ms
			over_cnt = 0;
			keytime_flag = 1;
		}
	}
}








