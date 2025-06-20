#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include "stm32f4xx.h"                  // Device header
#include "adc.h"
#include "stdio.h"

typedef struct adc {
	float light_data;
	float mq2_data;
	float temp_data;


}ADC_t;



void Light_Init(void);
u16 Get_Light_VAL(void);
void MQ2_Init(void);
u16 Get_MQ2_VAL(void);
void Light_MQ2_Init(void);
ADC_t Get_Light_MQ2_VAL(void);
void Get_Light_MQ2_Intrerupt_VAL(void);
void Light_MQ2_T_Init(void);
ADC_t Get_Light_MQ2_T_VAL(void);

#endif

