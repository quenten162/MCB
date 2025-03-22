#include "tim.h"
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework

volatile uint32_t Delay;

void TIM_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef timer;
	timer.TIM_Period = 1000 - 1;
	timer.TIM_Prescaler = 72 - 1;
	timer.TIM_ClockDivision = 0;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &timer);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	NVIC_InitTypeDef NVIC_Structure;
	NVIC_Structure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_Structure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Structure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Structure);

	TIM_Cmd(TIM2, ENABLE);
}

void Delay_us(uint32_t us) 
{
  TIM_SetCounter(TIM2, 0);
  while(TIM_GetCounter(TIM2) < us);
}


void Delay_ms(uint32_t ms) 
{
  while(ms--)
	{
		Delay_us(1000);
  }
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if (Delay != 0x00)
		{
			Delay--;
		}
	}
}
