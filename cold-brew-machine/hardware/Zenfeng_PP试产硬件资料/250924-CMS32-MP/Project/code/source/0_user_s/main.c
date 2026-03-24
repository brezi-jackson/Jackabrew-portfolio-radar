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
* for use in systems tilizing mixic components. CMS shall not be
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

//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    main.c
* @author  CMS Motor Control Team
* @version 第二代风机平台
* @date    2022年5月
* @brief   
*---------------------------------------------------------------------------//
*****************************************************************************/
//==========================================================================//

//---------------------------------------------------------------------------/
//	include files
//---------------------------------------------------------------------------/
#include "mcu_driver.h"
#include "motor_driver.h"
#include "user.h"
#include "demo_ccp.h"
#include "demo_epwm.h"
#include "demo_adc.h"
#include "demo_uart.h"
#include "flash.h"
#include "demo_pga.h"
//---------------------------------------------------------------------------/
//	Local pre-processor symbols/macros('#define')
//---------------------------------------------------------------------------/

//---------------------------------------------------------------------------/
//	Local variable  definitions
//---------------------------------------------------------------------------/

//---------------------------------------------------------------------------/
//	Global variable definitions(declared in header file with 'extern')
//---------------------------------------------------------------------------/

//---------------------------------------------------------------------------/
//	Local function prototypes('static')
//---------------------------------------------------------------------------/

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : main
* Description    : 主函数
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
uint8_t g_ticks;

#define RXBUFFERSIZE	9			/* 缓存大小 */
#define TXBUFFERSIZE	9			/* 缓存大小 */

uint8_t PUMP_A;
uint8_t PUMP_B;

extern uint8_t temp_switch;
extern uint8_t temp_set;
extern uint8_t pump_state;
extern uint8_t pump_set;
extern uint8_t fan_set;
extern int32_t Pump_Current;

uint8_t Limit_Switch;

uint16_t Error_tx;

extern uint16_t Temp1_adc;
extern uint16_t Temp2_adc;

float Temp_1;
float Temp_2;

uint8_t temp_test,temp_state_test;

uint8_t pump_set_test, pump_state_test;
uint16_t pump_error;

uint16_t pump_cur_ref;
uint32_t pump_cur_ref_sum;
uint16_t pump_cur_tag;
uint16_t Pump_Cur_Avg[100];
uint16_t pump_error_time;
uint8_t pump_set_last = 0, pump_check;
extern uint16_t pump_Control;

uint8_t pump_cal_tag;
float pump_var_a, pump_var_b;

extern uint16_t pump_cal_write_tag;
extern uint8_t pump_var[4];
uint8_t UART_TX_Buffer_cal[TXBUFFERSIZE] = {0xEA, 0x7B};
uint8_t pump_state_last;

extern	int32_t TEC_Current;
extern int32_t TEC_Current_adc;
extern int32_t TEC_Current_adc_base;

uint16_t pump_delay = 3500;

int main(void)
{	
	uint32_t msCnt; 	// count value of 1ms
	SystemCoreClockUpdate();
//	msCnt = SystemCoreClock / 1000;
//	SysTick_Config(msCnt); 

	PGA_PGA0_Config();
	
	ADC_Config();
	
	DelayTime_ms(3000);//保证在烧录io初始化前有足够时间烧录代码

	EPWM_Config();
  
	PUMP_PWM_Config();
	
	FAN_PWM_Config();	//烧录口改为风扇驱动 绝对不能放到延迟之前
	
	GPIO_Init(PORT2,PIN2,INPUT);
	
	UART0_Config();
	
	uint8_t *pump_var_addr;
	pump_var_addr	= (uint8_t *)0x500200;

	if(*pump_var_addr == 0xFF)
	{
		pump_cal_tag = 1;
	}
	else
	{
		pump_cal_tag = 0;
		
		pump_var_a = (float)(*pump_var_addr     << 8 | *(pump_var_addr+1)) / 10000;
		pump_var_b = (float)(*(pump_var_addr+2) << 8 | *(pump_var_addr+3)) / 100;
	}
		
	while(1)
	{
		UART_Lock(UART0);
		
		Limit_Switch = PORT_GetBit(PORT2,PIN2);
		
		if(pump_cal_write_tag >= 700)
		{
			flash_write(0x500200, sizeof(pump_var), (uint8_t *)pump_var);
								
			if((pump_var[0] == *pump_var_addr) && (pump_var[1] == *(pump_var_addr+1)) && (pump_var[2] == *(pump_var_addr+2)) && (pump_var[3] == *(pump_var_addr+3)))
			{
				pump_cal_tag = 0;
				
				UART_TX_Buffer_cal[2] = 1;
				pump_cal_write_tag = 0;
				
				pump_var_a = (float)(*pump_var_addr     << 8 | *(pump_var_addr+1)) / 10000;
				pump_var_b = (float)(*(pump_var_addr+2) << 8 | *(pump_var_addr+3)) / 100;
			}
			else
				UART_TX_Buffer_cal[2] = 0;
		}

		TEC_Current = TEC_Current_adc - TEC_Current_adc_base;
		
//		if(pump_state)
//		{	
//			if(pump_set !=  pump_set_last || pump_state_last != pump_state)
//			{
//				pump_check = 1;
//				pump_cur_tag = 0;
//				pump_cur_ref_sum = 0;
//				pump_cur_ref = 0;
//			}
//			
//			pump_set_last = pump_set;
//			
//			if(pump_check)
//			{
//				if(pump_cur_tag < pump_delay)
//				{
//					pump_cur_tag++;
//				}
//				else if(pump_cur_tag >= pump_delay && pump_cur_tag <(pump_delay + 100))
//				{
//					Pump_Cur_Avg[pump_cur_tag - pump_delay] = Pump_Current;
//					pump_cur_tag++;
//				}
//				else if(pump_cur_tag >= (pump_delay + 100))
//				{
//					for(uint8_t i = 0; i<100; i++)
//					{
//						pump_cur_ref_sum += Pump_Cur_Avg[i];
//					}
//					pump_cur_ref = pump_cur_ref_sum/100;
//					pump_check = 0;
//				}
//			}
//			else
//			{
//				
//				if((pump_set >= 30) &&(Pump_Current - pump_cur_ref) > 100)
//					pump_error_time++;
//				else if((pump_set < 30) &&(Pump_Current - pump_cur_ref) > 80)
//				{
//					pump_error_time++;
//				}
//				else
//					pump_error_time = 0;
//			}
//			
//			if(pump_error_time > 500)
//			{
//				pump_error_time = 0;
//				pump_error = 1;
//			}
//		}
//		else
//		{
//			pump_set_last = pump_set;
//			
//			pump_cur_tag = 0;
//			pump_error_time = 0;
//			pump_check = 0;
//			pump_cur_ref = 0;
//		}
//		
//		pump_state_last = pump_state;
		
		if(Temp1_adc > 0xFE0 || Temp1_adc < 0x80)
			Error_tx |= 0x04;
		else
			Error_tx &= 0xFB;
		
		if(Temp2_adc > 0xFE0 || Temp1_adc < 0x80)
			Error_tx |= 0x08;
		else
			Error_tx &= 0xF7;
		
		if(Limit_Switch)
			Error_tx |= 0x02;
		else
			Error_tx &= 0xFD;
		
//		if(pump_error)
//		{
//			Error_tx |= 0x01;
//			Pump_Set(0, 0);
//		}
//		else
//		{
//			Error_tx &= 0xFE;
//			Pump_Set(pump_state, pump_set);
//		}

		Pump_Set(pump_state, pump_set);  
		TEC_Set(temp_set, temp_switch);
		
		Fan_Set(fan_set);
/**************** test *****************/
//		TEC_Set(temp_test, temp_state_test);
//		Pump_Set(1, pump_set_test);
//		Pump_Drive(pump_state_test, 45);
/***************************************/	
		
		if(!ADC_IS_BUSY())
		{	
			UART_Lock(UART0);
			ADC_Go();
			while(ADC_IS_BUSY());
		}
	}
}


///***********************************************************************************************************************
//* Function Name: SysTick Handler
//* Description  : Decreament the g_ticks value
//* Arguments    : None
//* Return Value : None
//***********************************************************************************************************************/
//void SysTick_Handler(void)
//{
//	UART_Lock(UART0);			//Lock when system used UART
//	g_ticks = 1;
//}
/******************************** END OF FILE *******************************/
