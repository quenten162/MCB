#ifndef __BH1750_H
#define __BH1750_H

#include "stm32f10x.h"                  // Device header

void i2c_Start(void);
void i2c_SendAddress(uint8_t addr, uint8_t direct);
void i2c_SendByte(uint8_t data);
void i2c_Stop(void);
uint8_t i2c_Receive(void);
uint16_t I2C_ReadLight(void);

#endif