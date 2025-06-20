#include "spi.h"

/**************************************************************
*函数功能：SPI 初始化函数
*参    数：None
*返 回 值：None
*备    注：IO口模拟配置：	SCK----PA5----推挽输出
							MISO---PA6----浮空输入
							MOSI---PA7----推挽输出
							CS-----PB14---推挽输出

			控制器配置：		SCK----PA5----复用推挽
							MISO---PA6----复用
							MOSI---PA7----复用推挽
							CS-----PB14---推挽输出
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

	SPI1->CR1 &= ~(1 << 15);//双线单向配置
	SPI1->CR1 &= ~(1 << 11);//8位数据帧
	SPI1->CR1 &= ~(1 << 10);//全双工
	SPI1->CR1 |= (1 << 9);//软件模式
	SPI1->CR1 |= (1 << 8);//允许SPI传输
	SPI1->CR1 &= ~(1 << 7);//MSB
	SPI1->CR1 &= ~(0x7 << 3);//2分频
	SPI1->CR1 |= (1 << 2);//主配置
	SPI1->CR1 |= (1 << 1);//时钟极性
	SPI1->CR1 |= (1 << 0);//时钟相位(mode3)

	SPI1->CR2 &= ~(1 << 4);//motorola模式

	SPI1->I2SCFGR &= ~(1 << 11);//SPI模式
	SPI1->CR1 |= (1 << 6);//SPI使能
#endif
}


/**************************************************************
*函数功能：SPI数据传输函数（收发一体）
*参    数：None
*返 回 值：None
*备    注： 主机发送数据应该关注参数，不用管返回值
			主机接受数据应该关注返回值，不用管参数
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
		if (TBuf & (0x80 >> i))//主机发送一位数据（从机也发送）
		{
			SPI_MOSI_H;
		}
		else
		{
			SPI_MOSI_L;
		}
		RBuf = RBuf << 1;
		SPI_SCK_H;
		if (SPI_MISO_R)//主机接受数据，从机也接受
		{
			RBuf |= 0x01;
		}
	}
#else
	while (!(SPI1->SR & (1 << 1)));//等待发送完成（发送缓冲区为空）
	SPI1->DR = TBuf;
	while (!(SPI1->SR & (1 << 0)));//等待发送完成（发送缓冲区为空）
	RBuf = SPI1->DR;
#endif
	return RBuf;
}






