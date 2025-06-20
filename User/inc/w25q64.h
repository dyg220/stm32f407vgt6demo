#ifndef _W25Q64_H_
#define _W25Q64_H_

#include "stm32f4xx.h"                  // Device header
#include "spi.h"
#include <stdio.h>

#define W25Q64_CS_H	 (GPIOB->ODR |= (1<<14))
#define W25Q64_CS_L	 (GPIOB->ODR &= ~(1<<14))


void W25Q64_Init(void);
void Write_Enable(void);
u8 Read_Status(void);
void Write_Status(u8 data);
void W25Q64_Read_Bytes(u32 addr, u32 len, u8* buf);
void W25Q64_Write_Page(u32 addr, u32 len, u8* buf);
void W25Q64_Sector_Erase(u32 addr);

#endif






