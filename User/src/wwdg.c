#include "wwdg.h"


/**************************************************************
*�������ܣ����ڿ��Ź�����
*��    ����None
*�� �� ֵ��None
*��    ע��
**************************************************************/
void WWDG_Init(void)
{
	RCC->APB1ENR |= (1 << 11);//��ʱ��
	WWDG->CFR |= (0x3 << 7);//8��Ƶ
	WWDG->CFR |= (0x70 << 0);//���ô���ֵ
	WWDG->CFR |= (1 << 9);//ʹ����ǰ�����ж�
	WWDG->SR &= ~(1 << 0);//�����־λ
	WWDG->CR |= (0x7F << 0);//�������ֵ

	//����NVIC
	NVIC_SetPriority(WWDG_IRQn, NVIC_EncodePriority(5, 1, 0));
	NVIC_EnableIRQ(WWDG_IRQn);
	//�򿪿��Ź�
	WWDG->CR |= (1 << 7);
}


