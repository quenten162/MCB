#include "uart.h"
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework

void UART_Config(uint32_t baudrate) 
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;
	NVIC_InitTypeDef nvic;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

	// C?u hình chân TX (PA9) và RX (PA10)
	gpio.GPIO_Pin = GPIO_Pin_9; // TX
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_10; // RX
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &gpio);

	usart.USART_BaudRate = baudrate;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &usart);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	nvic.NVIC_IRQChannel = USART1_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	USART_Cmd(USART1, ENABLE);
}

void UART_SendChar(char c) 
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, c);
}

void UART_SendString(char *str) 
{
	while (*str) {
		UART_SendChar(*str++);
  }
}

char UART_ReceiveChar(void) 
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return USART_ReceiveData(USART1);
}

void UART_ReceiveString(char *buffer, uint32_t bufferSize) 
{
	uint32_t index = 0;
	char receivedChar;

	while (index < bufferSize - 1) 
	{
		receivedChar = UART_ReceiveChar();
		if (receivedChar == '\r' || receivedChar == '\n') 
		{
			break;
		}
		buffer[index++] = receivedChar;
	}
	buffer[index] = '\0';
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}