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
/** \file demo_adcb.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "demo_adc.h"

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
float GET_TEMP_ADC(uint16_t adcx, uint16_t *NTC_List)
{
	uint8_t middle=0;
	uint8_t indexL=0;
	uint8_t indexR=145;
	
	if(adcx>=NTC_List[0])
    	return -20;
	if(adcx<=NTC_List[145])
   		return 125;

	   	
//	while((indexR-indexL) > 1)
//	{
//		middle=(indexL + indexR)>>1;
//		if(adcx==NTC_List[middle])
//			indexL =  middle;
//		else if(adcx > NTC_List[middle])
//			indexR = middle;
//		else if(adcx < NTC_List[middle])
//			indexL = middle;
//	}
	
	for(int i = 0; i <=145; i++)
	{
		if(adcx > NTC_List[i])
		{
			indexL = i - 1;
			break;
		}
	}
	
	float data=0;
	float t=0;
	float result=0;

	data = NTC_List[indexL] - NTC_List[indexL + 1];
	t = (float)(NTC_List[indexL] - adcx) / data;

	result = (float)indexL + t - 20.f;
	return result;
}
/*****************************************************************************
 ** \brief	ADCB_Software_Trigger_Mode
 **			设置ADC软件触发转换
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_Config(void)
{
	CGC_PER13PeriphClockCmd(CGC_PER13Periph_ADCEN,ENABLE);

	ADC_ConfigRunMode(ADC_MODE_HIGH,ADC_CONVERT_CONTINUOUS,ADC_CLK_DIV_64,27);	/*单次模式+高速模式+32分频+100.5 ADCClk采样保持时间*/
	
	ADC_ConfigChannelSwitchMode(ADC_SWITCH_HARDWARE);	/*硬件自动切换*/
	
	ADC_DisableChargeAndDischarge();					/*关闭充电\放电功能*/
	
	ADC_ConfigVREF(ADC_VREFP_VDD);		/*设置参考源*/
	
	ADC_EnableScanChannel(Temp1_ADC_CH_MSK);
	GPIO_Init(PORT2,PIN0,ANALOG_INPUT);
	
	ADC_EnableScanChannel(Temp2_ADC_CH_MSK);
	GPIO_Init(PORT2,PIN1,ANALOG_INPUT);
	
	ADC_EnableScanChannel(Pump_ADC_CH_MSK);
	GPIO_Init(PORT2,PIN7,ANALOG_INPUT);
	
	ADC_EnableScanChannel(TEC_ADC_CH_MSK);
	GPIO_Init(PORT0,PIN2,ANALOG_INPUT);
	
	ADC_EnableChannelInt(Temp1_ADC_CH_MSK);
	ADC_EnableChannelInt(Temp2_ADC_CH_MSK);
	ADC_EnableChannelInt(Pump_ADC_CH_MSK);
	ADC_EnableChannelInt(TEC_ADC_CH_MSK);

	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_SetPriority(ADC_IRQn,1);
	
	ADC_Start();
}

