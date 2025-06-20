#include "rng.h"

u8 RNG_Init(void)
{
	u8 time_out = 0;
	RCC->AHB2ENR |= (1 << 6);
	RNG->CR |= (1 << 2);//使能发生器
	while (!(RNG->SR & (1 << 0)))
	{
		time_out++;
		delay_us(100);
		if (time_out > 100) return 1;
	}
	return 0;
}

int Read_RNG(int min, int max)
{
	u32 rng_num = 0;
	u8 time_out = 0;
	while (!(RNG->SR & (1 << 0)))
	{
		time_out++;
		delay_us(100);
		if (time_out > 100) return 1;
	}
	rng_num = RNG->DR;
	rng_num = rng_num % ((max - min) + 1) + min;
	return rng_num;
}
