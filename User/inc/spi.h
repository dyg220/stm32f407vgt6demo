#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f4xx.h"                  // Device header

#define SPI_SCK_H	(GPIOA->ODR |= (1<<5))
#define SPI_SCK_L	(GPIOA->ODR &= ~(1<<5))

#define SPI_MOSI_H	(GPIOA->ODR |= (1<<7))
#define SPI_MOSI_L	(GPIOA->ODR &= ~(1<<7))

#define SPI_MISO_R	(GPIOA->IDR&(1<<6))

#define SPI_CS_H	(GPIOB->ODR |= (1<<14))
#define SPI_CS_L	(GPIOB->ODR &= ~(1<<14))

#define SPI_SoftMode	0  //1 IOÄ£ÄâÅäÖÃ   0 ¿ØÖÆÆ÷ÅäÖÃ


void SPI_GPIO_Init(void);
u8 SPI_TransferByte(u8 TBuf);

#endif


