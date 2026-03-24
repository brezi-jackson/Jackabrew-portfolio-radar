/*
 * DS18B20.h
 *
 *  Created on: 2022年10月8日
 *      Author: 79482
 */
 
#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_
 
#include "sys.h"
 
#define GPIO_PIN_DO GPIO_PIN_1		//设置DO引脚
#define GPIO_DO	 GPIOA
 
#define DSDO_SET()		HAL_GPIO_WritePin(GPIO_DO, GPIO_PIN_DO, GPIO_PIN_SET)		//DO引脚置1
#define DSDO_RESET()	HAL_GPIO_WritePin(GPIO_DO, GPIO_PIN_DO, GPIO_PIN_RESET)		//DO引脚清零
#define ReadPin()		HAL_GPIO_ReadPin(GPIO_DO, GPIO_PIN_DO)						//读取DO引脚
 
/*
 * 	ROM命令
 */
#define ROM_Search 		0xF0    //ROM搜索
#define ROM_Read 		0x33	//ROM读取
#define ROM_Matching 	0x55	//ROM匹配
#define ROM_Skip 		0xCC	//ROM跳过
#define Alert_Search	0xEC	//警报搜索
 
/*
 * 	DS18B20功能命令
 */
#define Temperature_Conversion	0x44	//温度转换
#define Write_Scratch_Reg		0x4E	//写入暂存寄存器
#define Read_Scratch_Reg		0xBE	//读取暂存寄存器
#define Copy_Scrath_Reg			0x48	//拷贝暂存寄存器
#define Recall_EEPROM			0xB8	//召回EEPROM
#define Read_Power_Mode			0xB4	//读取供电模式
 
void DelayUs (uint16_t number);
 
void GPIO_SET_IN(void);
void GPIO_SET_OUT(void);
void DS18B20_Init(void);
void DS18B20_Write(uint8_t pData);
uint16_t DS18B20_Read(void);
float Data_TransFormation(void);
 
#endif /* INC_DS18B20_H_ */
