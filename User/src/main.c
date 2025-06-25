#include "main.h"                

u8 name[15][15] = { "洪祖强","王戈林","王洪康","郭庆","陈辉","蔡健平",
				   "陈德璋","陈昊","林树波","陆墨莉","林楚明","杨琛",
				   "王斌斌","邓守正","王晶" };


int main(void)
{
	u8 send[20] = "hello,world!\r\n";
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
	//	AT24C02_Init();
	//	W25Q64_Init();
	printf("Reset!!!\r\n");

	//printf("开始擦除\r\n");
	//W25Q64_Sector_Erase(4098);
	//printf("擦除结束\r\n");
	//W25Q64_Write_Page(4098,sizeof(send),send);

	//W25Q64_Read_Bytes(4098, sizeof(send), recv);

	//printf("%s\r\n",recv);
	/*for (int i = 0; i < sizeof(send); i++)
	{
		printf("0x%x  ", recv[i]);
	}*/
	//GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	u32 i = 0;
	while (1)
	{
		key_num = Key_Scan();
		if (key_num == key3_click)
		{
			//num = Read_RNG(0, 14);
			//printf("%s\r\n", name[num]);
			DMA2_USART_TDATA((u32)&USART1->DR, (u32)send, sizeof(send));
			delay_ms(200);
			//printf("NDTR: %d\r\n", DMA2_Stream7->NDTR);
		}
		RECV_USART1();  //也可以接收数据
	}
	return 0;
}

void TIM7_IRQHandler(void)
{
	if (TIM7->SR & (1 << 0))
	{
		TIM7->SR &= ~(1 << 0);//清除标志位
		//按键非阻塞
		Key_Tick();
		//LED非阻塞
//		Led_Tick();
	}
}
