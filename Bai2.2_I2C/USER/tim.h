#ifndef __TIM_H
#define __TIM_H

#include "stm32f10x.h"                  // Device header

void TIM_Config(void);
void Delay_ms(uint32_t ms);
void Delay_us(uint32_t us);

#endif