#include "stm32f10x.h"                  // Device header
#include "tim.h"
#include "uart.h"
#include "bh1750.h"
#include "stdio.h"

int main(void){
	TIM_Config();
	UART_Config(9600);
	bh1750_Config();
	
	while(1){
		UART_SendString("Hello\r\n");
//		
//		uint16_t light = I2C_ReadLight();
//		char buffer[50];
//		sprintf(buffer, "Light Sensor: %d lux\n", light);
//    UART_SendString(buffer);
		Delay_ms(1000);
	}
} 