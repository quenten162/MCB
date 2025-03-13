#include "test.h"
#include "stdio.h"
#include "string.h"

void Delay_us(uint32_t us) {
  TIM_SetCounter(TIM2, 0);
  while(TIM_GetCounter(TIM2) < us);
}

void Delay_ms(uint32_t ms) {
  while(ms--) {
		Delay_us(1000);
  }
}

void Timer_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_TimeBaseInitTypeDef timer;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Period = 0xFFFF;
  timer.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInit(TIM2, &timer);
	TIM_Cmd(TIM2, ENABLE);
}
void GPIO_Config(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef gpio;
	
  gpio.GPIO_Pin = GPIO_Pin_12;
  gpio.GPIO_Mode = GPIO_Mode_Out_OD;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &gpio);
	
	gpio.GPIO_Pin = GPIO_Pin_13;
  gpio.GPIO_Mode = GPIO_Mode_Out_PP;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &gpio);
	
}


void USART_Config(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    
  GPIO_InitTypeDef gpio;
	
	//PA9 - Tx
  gpio.GPIO_Pin = GPIO_Pin_9;
  gpio.GPIO_Mode = GPIO_Mode_AF_PP;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpio);
	
	//PA10 - Rx
  gpio.GPIO_Pin = GPIO_Pin_10;
  gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &gpio);

  USART_InitTypeDef uart;
	
  uart.USART_BaudRate = 9600;
  uart.USART_WordLength = USART_WordLength_8b;
  uart.USART_StopBits = USART_StopBits_1;
  uart.USART_Parity = USART_Parity_No;
  uart.USART_Mode = USART_Mode_Tx;
  uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init(USART1, &uart);

  USART_Cmd(USART1, ENABLE);
}

void USART_SendString(char *str){
	while(*str){
		USART_SendData(USART1, *str);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		str++;
	}
}

void USART_SendNumber(uint8_t num){
	char buffer[5]; 
  sprintf(buffer, "%d", num); 
  USART_SendString(buffer);  
}


uint16_t u16Tim;
uint8_t u8Buff[5];
uint8_t u8CheckSum;
unsigned int i, j;


int main()
{	
	Timer_Config();
	GPIO_Config();
	USART_Config();
	while(1)
	{
		test();
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		Delay_ms(20);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		
		TIM_SetCounter(TIM2, 0);
		while(TIM_GetCounter(TIM2) < 10){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
			}
		}
		
		u16Tim = TIM_GetCounter(TIM2);
		if(u16Tim >= 10){
			while(1){
			}

		}
		
		TIM_SetCounter(TIM2, 0);
		while(TIM_GetCounter(TIM2) < 45){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
			}
		}
		
		u16Tim = TIM_GetCounter(TIM2);
		if((u16Tim >= 45) || (u16Tim <= 5)){
			while(1){
			}

		}
		
		TIM_SetCounter(TIM2, 0);
		while(TIM_GetCounter(TIM2) < 90){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
			}
		}
		
		u16Tim = TIM_GetCounter(TIM2);
		if((u16Tim >= 90) || (u16Tim <= 70)){
			while(1){
			}

		}
		
		TIM_SetCounter(TIM2,0);
		while(TIM_GetCounter(TIM2) < 95){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
			}
		}
		
		u16Tim = TIM_GetCounter(TIM2);
		if((u16Tim >= 95) || (u16Tim <= 75)){
			while(1){
			}

		}
		
		for(i = 0; i < 5; ++i){
			for(j = 0; j < 8 ; j++){
				TIM_SetCounter(TIM2,0);
				while(TIM_GetCounter(TIM2) < 65){
					if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
						break;
					}
				}
				
				u16Tim = TIM_GetCounter(TIM2);
				if((u16Tim >= 65) || (u16Tim <= 45)){
					while(1){
					}

				}
				
				TIM_SetCounter(TIM2, 0);
				while(TIM_GetCounter(TIM2) < 80){
					if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
						break;
					}
				}
				
				u16Tim = TIM_GetCounter(TIM2);
				if((u16Tim >= 80) || (u16Tim <= 10)){
					while(1){
					}

				}
				
				u8Buff[i] <<= 1;
				if(u16Tim > 45){
					u8Buff[i] |= 1;
				} else{
					u8Buff[i] &= ~1;
				}
			} 
		}
		
		u8CheckSum = u8Buff[0] + u8Buff[1] + u8Buff[2] + u8Buff[3];
		if (u8CheckSum != u8Buff[4]) {
			while(1){
			
			}
		} 
		
			USART_SendString("Temp: ");
			USART_SendNumber(u8Buff[2]);
			USART_SendString(" *C\n");

			USART_SendString("Humi: ");
			USART_SendNumber(u8Buff[0]);
			USART_SendString(" %\n");
	}
		
	
}
	

