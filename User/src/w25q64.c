#include "w25q64.h"

/**************************************************************
*函数功能：W25Q64初始化函数
*参    数：None
*返 回 值：None
*备    注：CS---PB14
**************************************************************/
void W25Q64_Init(void)
{
	RCC->AHB1ENR |= (1 << 1);
	//CS-----PB14
	GPIOB->MODER &= ~(3 << 28);
	GPIOB->MODER |= (1 << 28);
	GPIOB->OTYPER &= ~(1 << 14);
	GPIOB->PUPDR &= ~(3 << 28);
	GPIOB->OSPEEDR |= (3 << 28);

	SPI_GPIO_Init();//SPI初始化

	W25Q64_CS_H;//空闲为高电平

	Write_Status(0x00);//解除所有块写保护
}


/**************************************************************
*函数功能：写使能函数
*参    数：None
*返 回 值：None
*备    注：0x06  "页编程" "扇区擦除" "块区擦除" "芯片擦除" "写控制状态寄存器"
**************************************************************/
void Write_Enable(void)
{
	W25Q64_CS_L;
	SPI_TransferByte(0x06);
	W25Q64_CS_H;
}

/**************************************************************
*函数功能：读状态寄存器函数
*参    数：None
*返 回 值：状态寄存器的值
*备    注：0x05
**************************************************************/
u8 Read_Status(void)
{
	u8 sta = 0;
	W25Q64_CS_L;
	SPI_TransferByte(0x05);
	sta = SPI_TransferByte(0xff);
	W25Q64_CS_H;
	return sta;
}


/**************************************************************
*函数功能：写控制状态寄存器函数
*参    数：寄存器配置信息
*返 回 值：None
*备    注：0x01
**************************************************************/
void Write_Status(u8 data)
{
	//写使能
	Write_Enable();
	W25Q64_CS_L;
	SPI_TransferByte(0x01);
	SPI_TransferByte(data);
	W25Q64_CS_H;
	while (Read_Status() & (1 << 0));//等待BUSY变0

}

/**************************************************************
*函数功能：连续读函数
*参    数：addr  从哪读  u32 len 读多长  u8*buf 保存读出的数据的空间
*返 回 值：None
*备    注：0x03
**************************************************************/
void W25Q64_Read_Bytes(u32 addr, u32 len, u8* buf)
{
	W25Q64_CS_L;
	SPI_TransferByte(0x03);
	SPI_TransferByte(addr >> 16);
	SPI_TransferByte(addr >> 8);
	SPI_TransferByte(addr);
	while (len--)
	{
		*buf = SPI_TransferByte(0xff);
		buf++;
	}
	W25Q64_CS_H;
}

/**************************************************************
*函数功能：页写函数
*参    数：addr  往哪写  u32 len   u8*buf 保存读出的数据的空间
*返 回 值：None
*备    注：0x02
**************************************************************/
void W25Q64_Write_Page(u32 addr, u32 len, u8* buf)
{
	if ((addr / 256) != ((addr + len - 1) / 256))
	{
		printf("长度超出当前页剩余\r\n");
		return;
	}

	//写使能
	Write_Enable();
	W25Q64_CS_L;
	SPI_TransferByte(0x02);
	SPI_TransferByte(addr >> 16);
	SPI_TransferByte(addr >> 8);
	SPI_TransferByte(addr);
	while (len--)
	{
		SPI_TransferByte(*buf);
		buf++;
	}
	W25Q64_CS_H;
	while (Read_Status() & (1 << 0));//等待BUSY变0
}

/**************************************************************
*函数功能：跨页写函数
*参    数：addr  往哪写  u32 len   u8*buf 保存读出的数据的空间
*返 回 值：None
*备    注：0x02
**************************************************************/
void W25Q64_Write_Pages(u32 addr, u32 len, u8* buf)
{
	u32 remain = 0;
	remain = 256 - (addr % 256);//当前页剩余的可写字节数
	if (len <= remain)
	{
		W25Q64_Write_Page(addr, len, buf);
		return;
	}
	else
	{
		//把当前页写满
		W25Q64_Write_Page(addr, remain, buf);
		addr = addr + remain;//存储空间的地址偏移
		buf = buf + remain;//数据地址的偏移
		len = len - remain;//剩余数据长度

		//剩余的数据长度大于等于8
		while (len >= 256)
		{
			W25Q64_Write_Page(addr, 256, buf);
			addr = addr + 256;//存储空间的地址偏移
			buf = buf + 256;//数据地址的偏移
			len = len - 256;//剩余数据长度
		}
		//剩余数据一定小于8，可能为0
		if (len != 0)
		{
			W25Q64_Write_Page(addr, len, buf);
		}
	}
}



/**************************************************************
*函数功能：扇区擦除函数
*参    数：addr 从哪开始擦除  (4096 Byte)
*返 回 值：None
*备    注：0x20
**************************************************************/
void W25Q64_Sector_Erase(u32 addr)
{
	//写使能
	Write_Enable();
	W25Q64_CS_L;
	SPI_TransferByte(0x20);
	SPI_TransferByte(addr >> 16);
	SPI_TransferByte(addr >> 8);
	SPI_TransferByte(addr);
	W25Q64_CS_H;
	while (Read_Status() & (1 << 0));//等待BUSY变0
}

/**************************************************************
*函数功能：块区擦除函数
*参    数：addr 从哪开始擦除
*返 回 值：None
*备    注：0xd8
**************************************************************/
void W25Q64_block_Erase(u32 addr)
{
	//写使能
	Write_Enable();
	W25Q64_CS_L;
	SPI_TransferByte(0xd8);
	SPI_TransferByte(addr >> 16);
	SPI_TransferByte(addr >> 8);
	SPI_TransferByte(addr);
	W25Q64_CS_H;
	while (Read_Status() & (1 << 0));//等待BUSY变0
}

/**************************************************************
*函数功能：芯片擦除函数
*参    数：None
*返 回 值：None
*备    注：0xC7
**************************************************************/
void W25Q64_Chip_Erase(void)
{
	//写使能
	Write_Enable();
	W25Q64_CS_L;
	SPI_TransferByte(0xC7);
	W25Q64_CS_H;
	while (Read_Status() & (1 << 0));//等待BUSY变0
}









