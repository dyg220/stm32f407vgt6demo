#include "main.h"                

u8 name[15][15] = { "����ǿ","������","���鿵","����","�»�","�̽�ƽ",
				   "�µ��","���","������","½ī��","�ֳ���","���",
				   "�����","������","����" };



int main(void)
{
	u8 send[20] = "hello,world!";
	u8 recv[20] = { 0 };
	u8 num = 0, key_num = 0;
	NVIC_SetPriorityGrouping(7 - 2);
	LED_Init();
	Beep_Init();
	Fire_Init();
	Key_Init();
	USART1_Init(115200);
	USART1_IRQ_Config();
	Tim7_IRQ_Config(1);
	//	Breathe_Init();
	//	RGB_Init();
	//	SG90_Init();
	RNG_Init();
	AT24C02_Init();
	W25Q64_Init();
	printf("Reset!!!\r\n");

	printf("��ʼ����\r\n");
	W25Q64_Sector_Erase(4098);
	printf("��������\r\n");
	//W25Q64_Write_Page(4098,sizeof(send),send);

	W25Q64_Read_Bytes(4098, sizeof(send), recv);

	//printf("%s\r\n",recv);
	for (int i = 0; i < sizeof(send); i++)
	{
		printf("0x%x  ", recv[i]);
	}

	while (1)
	{

		key_num = Key_Scan();
		if (key_num == key3_click)
		{
			num = Read_RNG(0, 14);
			printf("%s\r\n", name[num]);
		}
		RECV_USART1();
	}
	return 0;
}

void TIM7_IRQHandler(void)
{
	if (TIM7->SR & (1 << 0))
	{
		TIM7->SR &= ~(1 << 0);//�����־λ
		//����������
		Key_Tick();
		//LED������
//		Led_Tick();
	}
}
