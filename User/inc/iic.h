#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f4xx.h"                  // Device header
#include "delay.h"

#define IIC_SDA_H 	(GPIOB->ODR |= (1<<7))
#define IIC_SDA_L 	(GPIOB->ODR &= ~(1<<7))

#define IIC_SCL_H 	(GPIOB->ODR |= (1<<6))
#define IIC_SCL_L 	(GPIOB->ODR &= ~(1<<6))

#define IIC_SDA_R	(GPIOB->IDR&(1<<7))

#define ACK 	 0
#define NO_ACK   1

void IIC_GPIO_Init(void);
void IIC_Start(void);
u8 IIC_SendByte(u8 Byte);
u8 IIC_RecvByte(u8 ack);
u8 IIC_RecvAck(void);
void IIC_SendAck(u8 ack);
void IIC_Stop(void);



#endif

