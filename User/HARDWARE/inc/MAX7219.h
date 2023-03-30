#ifndef __MAX7219_H
#define __MAX7219_H
	
	#include "stm32f10x.h"
	
	//MAX7219内部寄存器地址
	#define MAX7219_REG_NOOP		  0x00
	#define MAX7219_REG_D0			  0x01
	#define MAX7219_REG_D1			  0x02
	#define MAX7219_REG_D2		  	0x03
	#define MAX7219_REG_D3			  0x04
	#define MAX7219_REG_D4			  0x05
	#define MAX7219_REG_D5			  0x06
	#define MAX7219_REG_D6			  0x07
	#define MAX7219_REG_D7			  0x08
	#define MAX7219_REG_DECODE		0x09
	#define MAX7219_REG_INTENSITY	0x0A
	#define MAX7219_REG_SCANLIMIT	0x0B
	#define MAX7219_REG_SHUTDOWN	0x0C
	#define MAX7219_REG_TEST		  0x0F
	
	
	#define MAX7219_CS_ENABLE    GPIO_SetBits(GPIOA,GPIO_Pin_6)      //MAX7219的片选线CS使能
	#define MAX7219_CS_DISABLE   GPIO_ResetBits(GPIOA,GPIO_Pin_6)  //MAX7219的片选线CS失能
	
	
	void MAX7219_Init(void);
	void MAX7219_Data(u8 address,u8 data);
	void MAX7219_Display(u8 address,u8 data);

#endif
