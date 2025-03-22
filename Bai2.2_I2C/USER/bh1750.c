#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "bh1750.h"

void bh1750_Config(){
	GPIO_InitTypeDef gpio_structure;
	I2C_InitTypeDef i2c;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	gpio_structure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	gpio_structure.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio_structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_structure);
	
	i2c.I2C_Mode = I2C_Mode_I2C;
	i2c.I2C_Ack = I2C_Ack_Enable;
	i2c.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c.I2C_ClockSpeed = 100000;
	i2c.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C1,&i2c);
	I2C_Cmd(I2C1,ENABLE);
}

void i2c_Start(){
	I2C_GenerateSTART(I2C1,ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
}

void i2c_SendAddress(uint8_t addr, uint8_t direct){
	if(direct == 0){
		addr &= ~0x01; //and voi dao cua 0x01 de thiet lap bit thap nhat cua addr thanh 0
	}
	else{
		addr |= 0x01; //or voi 0x01 de thiet lap bit thap nhat cua addr thanh 1
	}
	I2C_Send7bitAddress(I2C1, addr, direct);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)&&!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
}

void i2c_SendByte(uint8_t data){
	I2C_SendData(I2C1,data);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
}

void i2c_Stop(){
	I2C_GenerateSTOP(I2C1,ENABLE);
}

uint8_t i2c_Receive(){
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	return I2C_ReceiveData(I2C1);
}

uint16_t I2C_ReadLight(){
	uint8_t data[2];
	i2c_Start();
	i2c_SendAddress(0x23<<1,0);
	i2c_SendByte(0x10); //gui du lieu de thiet lap che do doc lien tuc voi do phan giai cao
	i2c_Stop();
	i2c_Start();
	i2c_SendAddress(0x23<<1,1);
	data[0]= i2c_Receive();
	data[1]= i2c_Receive();
	i2c_Stop();
	return (data[0]<<8)|data[1];
}