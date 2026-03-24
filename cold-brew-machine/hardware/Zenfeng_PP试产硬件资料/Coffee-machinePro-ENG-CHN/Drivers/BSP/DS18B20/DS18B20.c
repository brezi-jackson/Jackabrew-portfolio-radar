/*
 * DS18B20.c
 *
 *  Created on: 2022年10月8日
 *      Author: LiangKeXue
 */
#include "DS18B20.h"
#include "delay.h"

GPIO_InitTypeDef GPIO_Init;
 
/*
 * 	DS18B20初始化函数
 * 	向DS18B20发送ROM命令和功能性命令
 */

void DS18B20_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();   //使能GPIOB时钟
    
    //PB6,7初始化设置
    GPIO_Initure.Pin=GPIO_PIN_1;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}
 
void DS_Init(void)
{
	uint8_t counte = 0;
	GPIO_SET_OUT();									//DO引脚设置成输出模式
	DSDO_RESET();									//主机发送低电平信号
	delay_us(750);
	DSDO_SET();										//主机释放总线，等待DS18B20回应
	GPIO_SET_IN();									//DO引脚设置成输入模式
	while(ReadPin() == GPIO_PIN_SET && counte<60)	//检测DS18B20是否回应低电平
	{
		counte++;
		delay_us(1);
	}
	counte = 0;
	while(ReadPin() == GPIO_PIN_RESET && counte<240)//DS18B20回应脉冲信号
	{
		counte++;
		delay_us(1);
	}
}
 
/*
 * 	向DS18B20写入数据
 * 	pData	要传入的8位数据
 */
 
void DS18B20_Write(uint8_t pData)
{
	GPIO_SET_OUT();							//DO引脚设置成输出模式
	uint8_t Bit = 0;
	for(Bit=0; Bit<8; Bit++)				//字节数据传输
	{
		if((pData & (0x01<<Bit)) != 0)		//按位写1
		{
			DSDO_RESET();					//DO引脚电平拉低
			delay_us(2);
			DSDO_SET();						//DO引脚电平拉高
			delay_us(60);
		}
		else								//按位写0
		{
			DSDO_RESET();					//DO引脚电平拉低
			delay_us(60);
			DSDO_SET();						//DO引脚电平拉高
			delay_us(2);
		}
	}
}
 
/*
 * 	读取DS18B20函数
 * 	pData	字节数据存放的位置
 */
uint16_t DS18B20_Read(void)
{
	uint16_t pData = 0;
	for(uint8_t Bit=0; Bit<16; Bit++)
	{
		GPIO_SET_OUT();											//DO引脚设置成输出模式
		DSDO_RESET();											//DO引脚电平拉低
		delay_us(2);
		DSDO_SET();												//DO引脚电平拉高
		GPIO_SET_IN();											//DO引脚设置成输入模式
		delay_us(12);
		if(ReadPin() == GPIO_PIN_SET)							//读DO引脚的电平
		{
			pData = pData | (0x0001<<Bit);						//按位置1
		}
		else
		{
			pData = pData & ((0xFFFE<<(16-Bit))|(0xFFFE>>Bit));	//按位清零
		}
		delay_us(50);
	}
	return pData;
}
 
float Data_TransFormation(void)
{
	float Tem;
	uint16_t pData = 0;
 
	DS_Init();							//DS18B20初始化
	DS18B20_Write(ROM_Skip);				//发送ROM命令（ROM跳过）
	DS18B20_Write(Temperature_Conversion);	//发送功能性指令（温度转换）
 
	HAL_Delay(750);							//温度转换时间
 
	DS_Init();
	DS18B20_Write(ROM_Skip);
	DS18B20_Write(Read_Scratch_Reg);		//读取暂存寄存器
 
	pData = DS18B20_Read();					//读取温度的16位值
	if((pData & 0xF000) == 0)				//温度为零上时
	{
		Tem = pData*0.0625;					//补码等于原码，计算温度，12位精度为0.0625
		pData = 0;
		return Tem;
	}
	else									//温度为零下时
	{
		Tem = (65536-pData)*0.0625;			//补码转换成原码，计算温度
		pData = 0;
		return -Tem;
	}
}
 
/*
 * 	DO（PA5）引脚设置成输入模式，接上拉电阻
 */
void GPIO_SET_IN(void)
{
	GPIO_Init.Pin = GPIO_PIN_DO;
	GPIO_Init.Mode = GPIO_MODE_INPUT;
	GPIO_Init.Pull = GPIO_PULLUP;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
 
	HAL_GPIO_Init(GPIO_DO,&GPIO_Init);
}
 
/*
 * 	DO（PA5）引脚设置成推挽输出模式，初始高电平
 */
void GPIO_SET_OUT(void)
{
	GPIO_Init.Pin = GPIO_PIN_DO;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_WritePin(GPIO_DO, GPIO_PIN_DO, GPIO_PIN_SET);
	HAL_GPIO_Init(GPIO_DO,&GPIO_Init);
}
