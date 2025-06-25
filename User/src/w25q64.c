#include "w25q64.h"

/**************************************************************
*�������ܣ�W25Q64��ʼ������
*��    ����None
*�� �� ֵ��None
*��    ע��CS---PB14
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

	SPI_GPIO_Init();//SPI��ʼ��

	W25Q64_CS_H;//����Ϊ�ߵ�ƽ

	Write_Status(0x00);//������п�д����
}


/**************************************************************
*�������ܣ�дʹ�ܺ���
*��    ����None
*�� �� ֵ��None
*��    ע��0x06  "ҳ���" "��������" "��������" "оƬ����" "д����״̬�Ĵ���"
**************************************************************/
void Write_Enable(void)
{
	W25Q64_CS_L;
	SPI_TransferByte(0x06);
	W25Q64_CS_H;
}

/**************************************************************
*�������ܣ���״̬�Ĵ�������
*��    ����None
*�� �� ֵ��״̬�Ĵ�����ֵ
*��    ע��0x05
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
*�������ܣ�д����״̬�Ĵ�������
*��    �����Ĵ���������Ϣ
*�� �� ֵ��None
*��    ע��0x01
**************************************************************/
void Write_Status(u8 data)
{
	//дʹ��
	Write_Enable();
	W25Q64_CS_L;
	SPI_TransferByte(0x01);
	SPI_TransferByte(data);
	W25Q64_CS_H;
	while (Read_Status() & (1 << 0));//�ȴ�BUSY��0

}

/**************************************************************
*�������ܣ�����������
*��    ����addr  ���Ķ�  u32 len ���೤  u8*buf ������������ݵĿռ�
*�� �� ֵ��None
*��    ע��0x03
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
*�������ܣ�ҳд����
*��    ����addr  ����д  u32 len   u8*buf ������������ݵĿռ�
*�� �� ֵ��None
*��    ע��0x02
**************************************************************/
void W25Q64_Write_Page(u32 addr, u32 len, u8* buf)
{
	if ((addr / 256) != ((addr + len - 1) / 256))
	{
		printf("���ȳ�����ǰҳʣ��\r\n");
		return;
	}

	//дʹ��
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
	while (Read_Status() & (1 << 0));//�ȴ�BUSY��0
}

/**************************************************************
*�������ܣ���ҳд����
*��    ����addr  ����д  u32 len   u8*buf ������������ݵĿռ�
*�� �� ֵ��None
*��    ע��0x02
**************************************************************/
void W25Q64_Write_Pages(u32 addr, u32 len, u8* buf)
{
	u32 remain = 0;
	remain = 256 - (addr % 256);//��ǰҳʣ��Ŀ�д�ֽ���
	if (len <= remain)
	{
		W25Q64_Write_Page(addr, len, buf);
		return;
	}
	else
	{
		//�ѵ�ǰҳд��
		W25Q64_Write_Page(addr, remain, buf);
		addr = addr + remain;//�洢�ռ�ĵ�ַƫ��
		buf = buf + remain;//���ݵ�ַ��ƫ��
		len = len - remain;//ʣ�����ݳ���

		//ʣ������ݳ��ȴ��ڵ���8
		while (len >= 256)
		{
			W25Q64_Write_Page(addr, 256, buf);
			addr = addr + 256;//�洢�ռ�ĵ�ַƫ��
			buf = buf + 256;//���ݵ�ַ��ƫ��
			len = len - 256;//ʣ�����ݳ���
		}
		//ʣ������һ��С��8������Ϊ0
		if (len != 0)
		{
			W25Q64_Write_Page(addr, len, buf);
		}
	}
}



/**************************************************************
*�������ܣ�������������
*��    ����addr ���Ŀ�ʼ����  (4096 Byte)
*�� �� ֵ��None
*��    ע��0x20
**************************************************************/
void W25Q64_Sector_Erase(u32 addr)
{
	//дʹ��
	Write_Enable();
	W25Q64_CS_L;
	SPI_TransferByte(0x20);
	SPI_TransferByte(addr >> 16);
	SPI_TransferByte(addr >> 8);
	SPI_TransferByte(addr);
	W25Q64_CS_H;
	while (Read_Status() & (1 << 0));//�ȴ�BUSY��0
}

/**************************************************************
*�������ܣ�������������
*��    ����addr ���Ŀ�ʼ����
*�� �� ֵ��None
*��    ע��0xd8
**************************************************************/
void W25Q64_block_Erase(u32 addr)
{
	//дʹ��
	Write_Enable();
	W25Q64_CS_L;
	SPI_TransferByte(0xd8);
	SPI_TransferByte(addr >> 16);
	SPI_TransferByte(addr >> 8);
	SPI_TransferByte(addr);
	W25Q64_CS_H;
	while (Read_Status() & (1 << 0));//�ȴ�BUSY��0
}

/**************************************************************
*�������ܣ�оƬ��������
*��    ����None
*�� �� ֵ��None
*��    ע��0xC7
**************************************************************/
void W25Q64_Chip_Erase(void)
{
	//дʹ��
	Write_Enable();
	W25Q64_CS_L;
	SPI_TransferByte(0xC7);
	W25Q64_CS_H;
	while (Read_Status() & (1 << 0));//�ȴ�BUSY��0
}









