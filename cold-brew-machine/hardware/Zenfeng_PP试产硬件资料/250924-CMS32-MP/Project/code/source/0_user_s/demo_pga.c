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
/** \file demo_pga.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "demo_pga.h"
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
 ** \brief	PGA_PGA0_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PGA_PGA0_Config(void)
{
  /*
	(1)设置PGA0时钟
	*/
	CGC_PER13PeriphClockCmd(CGC_PER13Periph_PGA0EN,ENABLE);
	 
  /*
	(2)设置PGA0正端、负端输入 IO口
	*/
	GPIO_Init(PORT0,PIN0,ANALOG_INPUT);		//PGA0 +
	GPIO_Init(PORT0,PIN1,ANALOG_INPUT);		//PGA0 -
	  
  /*
	(3)设置PGA0增益
	*/
	PGA_ConfigGain(PGA0x,PGA_GAIN_10);		
  /*
	(4)设置PGA0参考电压
	*/
	PGA_VrefCtrl(PGA0x,VrefHalf);
	
  /*
	(5)设置PGA0模式
	*/
	PGA_ModeSet(PGA0x,PgaDiffer);
	
	 /*
	(6)设置PGA0输出
	*/
	#if 1
	//PGA0输出到PAD串联电阻选择  0: 内部不串电阻  1: 内部串10K电阻
	PGA0_ConfigResistorPAD(0);
	
	//PPGA0输出到PAD通道使能  0: 禁止  1: 使能
	PGA_EnableOutput(PGA0x);	
  
	GPIO_Init(PORT0,PIN2,ANALOG_INPUT);  //PGA_OUT

	#endif
		
  /*
	(7)使能PGA0
	*/
	PGA_Start(PGA0x);
}

/*****************************************************************************
 ** \brief	PGA_PGA1_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PGA_PGA1_Config(void)
{
	/*
	(1)设置PGA 时钟
	*/
	CGC_PER13PeriphClockCmd(CGC_PER13Periph_PGA1EN,ENABLE);	
	/*
	(2)设置PGA 参考电压
	*/	
	PGA_VrefCtrl(PGA1x,VrefHalf);
	/*
	(3)设置PGA 输出
	*/		
	PGA_EnableOutput(PGA1x);	//P04:OUT,  P24:A1P
	
	/*
	(4)设置PGA 增益
	*/	
	PGA_ConfigGain(PGA1x,PGA_GAIN_2d5);
	/*
	(5)设置PGA模式
	*/	
	PGA_ModeSet(PGA1x,PgaSingle);    //使能单端模式

	/*
	(6)开启PGA
	*/
	PGA_Start(PGA1x);
}

/*****************************************************************************
 ** \brief	PGA_PGA2_Config
 **					
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PGA_PGA2_Config(void)
{
	/*
	(1)设置PGA 时钟
	*/
	CGC_PER13PeriphClockCmd(CGC_PER13Periph_PGA2EN,ENABLE);		
	/*
	(2)设置PGA 参考电压
	*/	
	PGA_VrefCtrl(PGA2x,VrefHalf);
	/*
	(3)设置PGA 输出
	*/		
	PGA_EnableOutput(PGA2x);	   //P04:OUT,  P26:A2P
	
	/*
	(4)设置PGA 增益
	*/	
	PGA_ConfigGain(PGA2x,PGA_GAIN_5);
	/*
	(5)设置PGA模式
	*/	
	PGA_ModeSet(PGA2x,PgaDiffer);    //使能差分模式

	/*
	(6)开启PGA
	*/
	PGA_Start(PGA2x);
}

