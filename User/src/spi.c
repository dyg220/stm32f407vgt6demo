#include "spi.h"

/**************************************************************
*�������ܣ�SPI ��ʼ������
*��    ����None
*�� �� ֵ��None
*��    ע��IO��ģ�����ã�	SCK----PA5----�������
							MISO---PA6----��������
							MOSI---PA7----�������
							CS-----PB14---�������

			���������ã�		SCK----PA5----��������
							MISO---PA6----����
							MOSI---PA7----��������
							CS-----PB14---�������
**************************************************************/
void SPI_GPIO_Init(void)
{
#if	SPI_SoftMode
	RCC->AHB1ENR |= (0x3 << 0);
	//SCK----PA5
	GPIOA->MODER &= ~(3 << 10);
	GPIOA->MODER |= (1 << 10);
	GPIOA->OTYPER &= ~(1 << 5);
	GPIOA->OSPEEDR |= (3 << 10);

	//MISO---PA6
	GPIOA->MODER &= ~(3 << 12);
	GPIOA->PUPDR &= ~(3 << 12);

	//MOSI---PA7
	GPIOA->MODER &= ~(3 << 14);
	GPIOA->MODER |= (1 << 14);
	GPIOA->OTYPER &= ~(1 << 7);
	GPIOA->OSPEEDR |= (3 << 14);

	SPI_SCK_H;
#else
	RCC->APB2ENR |= (1 << 12);

	//SCK----PA5
	GPIOA->MODER &= ~(3 << 10);
	GPIOA->MODER |= (2 << 10);
	GPIOA->OTYPER &= ~(1 << 5);
	GPIOA->OSPEEDR |= (3 << 10);
	GPIOA->AFR[0] &= ~(0xf << 20);
	GPIOA->AFR[0] |= (5 << 20);

	//MISO---PA6
	GPIOA->MODER &= ~(3 << 12);
	GPIOA->MODER |= (2 << 12);
	GPIOA->PUPDR &= ~(3 << 12);
	GPIOA->AFR[0] &= ~(0xf << 24);
	GPIOA->AFR[0] |= (5 << 24);
	//MOSI---PA7
	GPIOA->MODER &= ~(3 << 14);
	GPIOA->MODER |= (2 << 14);
	GPIOA->OTYPER &= ~(1 << 7);
	GPIOA->OSPEEDR |= (3 << 14);
	GPIOA->AFR[0] &= ~((u32)0xf << 28);

	GPIOA->AFR[0] |= (5 << 28);

	SPI1->CR1 &= ~(1 << 15);//˫�ߵ�������
	SPI1->CR1 &= ~(1 << 11);//8λ����֡
	SPI1->CR1 &= ~(1 << 10);//ȫ˫��
	SPI1->CR1 |= (1 << 9);//���ģʽ
	SPI1->CR1 |= (1 << 8);//����SPI����
	SPI1->CR1 &= ~(1 << 7);//MSB
	SPI1->CR1 &= ~(0x7 << 3);//2��Ƶ
	SPI1->CR1 |= (1 << 2);//������
	SPI1->CR1 |= (1 << 1);//ʱ�Ӽ���
	SPI1->CR1 |= (1 << 0);//ʱ����λ(mode3)

	SPI1->CR2 &= ~(1 << 4);//motorolaģʽ

	SPI1->I2SCFGR &= ~(1 << 11);//SPIģʽ
	SPI1->CR1 |= (1 << 6);//SPIʹ��
#endif
}


/**************************************************************
*�������ܣ�SPI���ݴ��亯�����շ�һ�壩
*��    ����None
*�� �� ֵ��None
*��    ע�� ������������Ӧ�ù�ע���������ùܷ���ֵ
			������������Ӧ�ù�ע����ֵ�����ùܲ���
**************************************************************/
u8 SPI_TransferByte(u8 TBuf)
{
	u8 RBuf = 0;
#if	SPI_SoftMode
	u8 i = 0;
	SPI_SCK_H;
	for (i = 0; i < 8; i++)
	{
		SPI_SCK_L;
		if (TBuf & (0x80 >> i))//��������һλ���ݣ��ӻ�Ҳ���ͣ�
		{
			SPI_MOSI_H;
		}
		else
		{
			SPI_MOSI_L;
		}
		RBuf = RBuf << 1;
		SPI_SCK_H;
		if (SPI_MISO_R)//�����������ݣ��ӻ�Ҳ����
		{
			RBuf |= 0x01;
		}
	}
#else
	while (!(SPI1->SR & (1 << 1)));//�ȴ�������ɣ����ͻ�����Ϊ�գ�
	SPI1->DR = TBuf;
	while (!(SPI1->SR & (1 << 0)));//�ȴ�������ɣ����ͻ�����Ϊ�գ�
	RBuf = SPI1->DR;
#endif
	return RBuf;
}






