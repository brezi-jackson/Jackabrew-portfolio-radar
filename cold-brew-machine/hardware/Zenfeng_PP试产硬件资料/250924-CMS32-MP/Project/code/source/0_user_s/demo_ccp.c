/*******************************************************************************
* Copyright (C) 2019 China Micro Semiconductor Limited Company. All Rights Reserved.
*
* This software is owned and published by:
* CMS LLC, No 2609-10, Taurus Plaza, TaoyuanRoad, NanshanDistrict, Shenzhen, China.
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with CMS
* components. This software is licensed by CMS to be adapted only
* for use in systems utilizing CMS components. CMS shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. CMS is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/

/****************************************************************************/
/** \file demo_ccp.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "demo_ccp.h"

/****************************************************************************/
/*	Local pre-processor symbols/macros('#define')
*****************************************************************************/

/****************************************************************************/
/*	Global variable definitions(declared in header file with 'extern')
*****************************************************************************/

/****************************************************************************/
/*	Local type definitions('typedef')
*****************************************************************************/

/****************************************************************************/
/*	Local variable  definitions('static')
*****************************************************************************/


/****************************************************************************/
/*	Local function prototypes('static')
*****************************************************************************/

/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/

/*****************************************************************************
 ** \brief	CCP_PWM_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PUMP_PWM_Config(void)
{
	
	/*
	(1)设置CCP的时钟
	*/
	CGC_PER11PeriphClockCmd(CGC_PER11Periph_CCP,ENABLE);				/*开启CCP模块时钟*/
	CCP_ConfigCLK(CCP0,CCP_CLK_DIV_16,CCP_RELOAD_CCPLOAD,100);		/*Fahb=48M, Pccp0 = 100us*/
	/*
	(2)设置CCP运行模式
	*/
	CCP_EnablePWMMode(CCP0);		/*设置CCP0为PWM模式*/
	/*
	(3)设置50%占空比输出
	*/	
	CCP_ConfigCompare(CCP0, CCPxA, 0); 	/*20us*/
	CCP_ConfigCompare(CCP0, CCPxB, 0); 	/*40us*/	

	/*
	(4)设置CCP 反相输出
	*/		
	CCP_DisableReverseOutput(CCP0,CCPxA);	/*关闭反相输出*/
	CCP_DisableReverseOutput(CCP0,CCPxB);  

	/*
//	(5)设置CCP 中断`
//	*/	
//	CCP_EnableOverflowInt(CCP0);			/*开启CCP0溢出中断*/
////	CCP_EnableOverflowInt(CCP1);	00000		/*开启CCP1溢出中断*/	
//	NVIC_EnableIRQ(CCP_IRQn);
//	/*
//	(6)设置CCP 中断优先级
//	*/	
//	NVIC_SetPriority(CCP_IRQn,3);					/*优先级0~3， 0最高、3最低*/	

	/*
	(7)设置CCP IO口复用
	*/	
	GPIO_PinAFOutConfig(P26CFG,IO_OUTCFG_P26_CCP0A_O);
	GPIO_PinAFOutConfig(P24CFG,IO_OUTCFG_P24_CCP0B_O);

	GPIO_Init(PORT2,PIN6,OUTPUT);
	GPIO_Init(PORT2,PIN4,OUTPUT);

	/*
	(8)开启CCP模块
	*/	
	CCP_EnableRun(CCP0);			
	CCP_Start(CCP0);			/*CCP0模块使能*/
}

/*****************************************************************************
 ** \brief	CCP_PWM_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void FAN_PWM_Config(void)
{
	/*
	(1)设置CCP的时钟
	*/
	CGC_PER11PeriphClockCmd(CGC_PER11Periph_CCP,ENABLE);				/*开启CCP模块时钟*/
	CCP_ConfigCLK(CCP1,CCP_CLK_DIV_16,CCP_RELOAD_CCPLOAD,100);		/*Fahb=48M, Pccp0 = 100us*/
	/*
	(2)设置CCP运行模式
	*/
	CCP_EnablePWMMode(CCP1);		/*设置CCP0为PWM模式*/
	/*
	(3)设置占空比输出
	*/
	CCP_ConfigCompare(CCP1, CCPxB, 50); 	/*20us*/
//	CCP_ConfigCompare(CCP1, CCPxA, 0); 	/*20us*/

	/*
	(4)设置CCP 反相输出
	*/		
	CCP_DisableReverseOutput(CCP1,CCPxB);	/*关闭反相输出*/
//	CCP_DisableReverseOutput(CCP1,CCPxA);	/*关闭反相输出*/


//	RESTPinGpio_Set(1);
	
	GPIO_PinAFOutConfig(P07CFG,IO_OUTCFG_P07_CCP1B_O);
//	GPIO_PinAFOutConfig(P06CFG,IO_OUTCFG_P06_CCP1A_O);

	GPIO_Init(PORT0,PIN7,OUTPUT);
//	GPIO_Init(PORT0,PIN6,OUTPUT);

	/*
	(8)开启CCP模块
	*/
	CCP_EnableRun(CCP1);			
	CCP_Start(CCP1);			/*CCP0模块使能*/
}

void Fan_Set(uint8_t Fan_Ctrl)
{
	UART_Lock(UART0);
	
	if(Fan_Ctrl)
//		CCP_ConfigCompare(CCP1, CCPxA, Fan_Ctrl);
		CCP_ConfigCompare(CCP1, CCPxB, Fan_Ctrl);
	else
//		CCP_ConfigCompare(CCP1, CCPxA, 0);
		CCP_ConfigCompare(CCP1, CCPxB, 0);
}

void Pump_Drive(uint8_t state, uint8_t Pump_Ctrl)
{
	UART_Lock(UART0);
	switch(state)
	{
		case 0:
			CCP_ConfigCompare(CCP0, CCPxA, 0);
			CCP_ConfigCompare(CCP0, CCPxB, 0);
			break;
		case 1:
			CCP_ConfigCompare(CCP0, CCPxA, 100);
			CCP_ConfigCompare(CCP0, CCPxB, 100 - Pump_Ctrl);
			break;
		case 2:
			CCP_ConfigCompare(CCP0, CCPxA, 100 - Pump_Ctrl);
			CCP_ConfigCompare(CCP0, CCPxB, 100);
			break;
		
		default:
			break;	
	}
}

extern uint8_t pump_state;

uint8_t pump_low_tag;
uint16_t pump_Control;
extern uint8_t pump_error;

extern uint8_t pump_cal_tag;
extern float pump_var_a, pump_var_b;
void Pump_Set(uint8_t state, uint8_t Pump_Ctrl) //KHPP50
{
	if(pump_error)
	{
		pump_Control = 0;
		state = 0;
		Pump_Drive(state, pump_Control);
	}
	else
	{
		if(Pump_Ctrl >= 30 && Pump_Ctrl <= 80)
		{
			pump_low_tag = 0;

			if(pump_cal_tag)
				pump_Control = Pump_Ctrl;
			else
				pump_Control = Pump_Ctrl * pump_var_a + pump_var_b;

			Pump_Drive(state, pump_Control);
		}
		else if(Pump_Ctrl < 30)
		{
			if(state)
			{
				switch(Pump_Ctrl)
				{
					case 0:
						pump_low_tag = 0;
						Pump_Drive(0, 0);
						break;
					
					case 5:
						pump_low_tag = 1;
						break;
					
					case 10:
						pump_low_tag = 2;
						break;
					
					case 15:
						pump_low_tag = 3;
						break;
					
					case 20:
						pump_low_tag = 4;
						break;
					
					case 25:
						pump_low_tag = 5;
						break;
					
					default:
						break;
				}
			}
			else
			{
				pump_low_tag = 0;
				Pump_Drive(0, 0);
			}
		}
	}
}
