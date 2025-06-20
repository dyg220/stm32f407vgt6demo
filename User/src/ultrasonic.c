#include "ultrasonic.h"

/**************************************************************
*�������ܣ����������
*��    ����None
*�� �� ֵ��None
*��    ע��PB8---TIM4_CH3---ECHO    PB9---TRIG
**************************************************************/
void Ultrasonic_Init(void)
{
	RCC->AHB1ENR |= (1<<1);
	RCC->APB1ENR |= (1<<2);
	
	//PB9 ͨ�����(����)
	GPIOB->MODER &= ~(0x3<<18);
	GPIOB->MODER |= (1<<18);
	GPIOB->OTYPER &= ~(1<<9);
	
	//PB8 ����TIM4
	GPIOB->MODER &= ~(0x3<<16);
	GPIOB->MODER |= (0x2<<16);
	GPIOB->AFR[1] &= ~(0xf<<0);
	GPIOB->AFR[1] |= (0x2<<0);
	
	//TIM4
	TIM4->PSC = 84 - 1;//��һ����1us
	TIM4->ARR = 0xffff;
	
	//��������
	TIM4->CR1 |= (1<<7);//�����Զ�����Ԥװ�أ����壩
	TIM4->CR1 &= ~(0x3<<5);//���ض���ģʽ��������ݼ�
	TIM4->CR1 &= ~(1<<4);//���򣺵���
	TIM4->CR1 &= ~(1<<3);//������ģʽ�ر�
	TIM4->CR1 &= ~(1<<1);//ʹ��UEV
	TIM4->EGR |= (1<<0);//�ֶ�����һ�������¼�����ֵ���£�
	TIM4->SR &= ~(1<<0);//�����־λ
	
	//CCMR1
	TIM4->CCMR2 |= (0xf<<4);//�˲�
	TIM4->CCMR2 &= ~(0x3<<2);//�޷�Ƶ��һ�����ش���һ��
	TIM4->CCMR2 &= ~(0x3<<0);
	TIM4->CCMR2 |= (1<<0);//ͨ������IC1 ӳ�䵽 TI1 ��
	
	//CCER     CH3
	TIM4->CCER &= ~(1<<11);
	TIM4->CCER &= ~(1<<9);//�����ش���
	TIM4->CCER |= (1<<8);//ʹ�ܲ���
	
	//�����ж�   �����ж�
	TIM4->DIER |= (1<<0);//�����ж�
	TIM4->DIER |= (1<<3);//�����ж�
	
	//����NVIC
	NVIC_SetPriority(TIM4_IRQn,NVIC_EncodePriority(5,1,0));
	NVIC_EnableIRQ(TIM4_IRQn);
	
	TIM4->CR1 |= (1<<0);//ʹ�ܼ�����
	
}

volatile float distance = 0;
volatile u8 ultrasonic_flag = 0;

void TIM4_IRQHandler(void)
{
	static u32 over_cnt = 0;
	static u32 cnt1 = 0,cnt2 = 0,count = 0;
	
	if(TIM4->SR&(1<<0))//�ж���û�����
	{
		TIM4->SR &= ~(1<<0);//�����־λ
		over_cnt++;//�������
	}
	
	if(TIM4->SR&(1<<3))//�ж���û�в���
	{
		TIM4->SR &= ~(1<<3);////�����־λ
		if(!(TIM4->CCER&(1<<9)))//�����ؽ���
		{
			TIM4->CCER |=(1<<9);//�ĳ��½���
			cnt1 = TIM4->CCR3;//������ֵ
			over_cnt = 0;
		}
		else if(TIM4->CCER&(1<<9))//�½��ؽ���
		{
			TIM4->CCER &= ~(1<<9);//�ĳ�������
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
	while(!ultrasonic_flag);//�ȴ�ת������
	ultrasonic_flag = 0;
	printf("distance = %.1fcm\r\n",distance);
}




