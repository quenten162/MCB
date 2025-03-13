#include "stm32f10x.h"
#include <stdio.h>

void GPIO_Config(void);
//void EXTI_Config(void);
//void NVIC_Config(void);
//void TIM2_Config(void);
void UART_Config(void);
//void UART_SendString(char *str);
//void UART_SendDataValue(uint8_t value);


void Delay_ms(uint16_t _time){
	uint16_t i,j;
	for(i = 0; i < _time; i++){
		for(j = 0; j < 0x2AFF; j++);
	}
}

int main(void) {
    GPIO_Config();
//    EXTI_Config();
//    NVIC_Config();
//    TIM2_Config();
    UART_Config();
    
    while (1) {
			uint8_t sensor = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
			printf("%d\n",sensor);
			Delay_ms(100);
			
//        if (sensor_flag) {
//            uint8_t sensor_value = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
//            UART_SendDataValue(sensor_value);
//            sensor_flag = 0; // Reset flag
//        }
    }
}

void GPIO_Config(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // Cam bien LM393 noi vao PA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // Input pull-up
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void UART_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	NVIC_InitTypeDef nvic;
	
	nvic.NVIC_IRQChannel = USART1_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	GPIO_InitTypeDef gpio;
	
	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);
	
	USART_InitTypeDef usart;
	
	usart.USART_BaudRate = 9600;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1 ,&usart);
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);
	USART_Cmd(USART1, ENABLE);
}

void uart_SendChar(char _chr)
{
	USART_SendData(USART1, _chr);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
}

struct __FILE {
 int dummy;
};
FILE __stdout;
int fputc(int ch, FILE *f) {
	uart_SendChar(ch);
 return ch;
}

//void TIM2_IRQHandler(void) {
//    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
//        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0) {
//            sensor_flag = 1;
//        }
//        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//    }
//}

//void EXTI0_IRQHandler(void) {
//    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
//        sensor_flag = 1;
//        EXTI_ClearITPendingBit(EXTI_Line0);
//    }
//}
