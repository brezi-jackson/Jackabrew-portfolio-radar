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
* for use in systems utilizing mixic components. CMS shall not be
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
/** \file isr.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "common.h"
#include "gpio.h"
#include "epwm.h"
#include "adc.h"
#include "uart.h"
#include "demo_adc.h"
#include "demo_uart.h"
#include "demo_ccp.h"
#include "flash.h"
#include "softinit.h"
/****************************************************************************/
/*	Local pre-processor symbols/macros('#define')
*****************************************************************************/

/****************************************************************************/
/*	Global variable definitions(declared in header file with 'extern')
*****************************************************************************/
extern void DelayTime_ms(unsigned int delay);

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
 ** \brief	void ADC_ClearAllInt_Flag(void)
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_ClearAll_Flag(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->ICLR |= 0xFFFFFFFF;
	ADC->LOCK = 0x00;
  
}

 /****************************************************************************
 ** \brief	ADCB_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
extern uint8_t Limit_Switch;

typedef struct {
    int32_t z1;
    float tc;
    int32_t in;
    int32_t out;
} lpf_1rd_t; 

lpf_1rd_t lpf1rd_0;
lpf_1rd_t lpf1rd_1;
lpf_1rd_t lpf1rd_temp_1;

float lpf_1rd_calc(lpf_1rd_t *lpf1rd, int32_t new_sample)
{
    lpf1rd->in =  new_sample;
	lpf1rd->out = lpf1rd->z1 + lpf1rd->tc * (lpf1rd->in - lpf1rd->z1);
    lpf1rd->z1 =  lpf1rd->out;

    return lpf1rd->out;
}

uint16_t Temp1_adc;
uint16_t Temp2_adc;
int32_t Pump_Current;
int32_t TEC_Current;
int32_t TEC_Current_adc;
int32_t TEC_Current_adc_base;

uint8_t temp_switch;
uint8_t temp_set;
uint8_t pump_state;
uint8_t pump_set;
uint8_t fan_set;

extern uint16_t Error_tx;
uint16_t io_change;

extern uint8_t pump_low_tag;

uint8_t pump_step = 32;

extern uint16_t pump_Control;
uint8_t adc_tag;
uint16_t pump_low_time, pump_low_time_set;

uint8_t TEC_tag;

void ADC_IRQHandler(void)
{
	lpf1rd_0.tc = 0.005;
	lpf1rd_1.tc = 0.1;
	lpf1rd_temp_1.tc = 0.1;
	
	UART_Lock(UART0);			//Lock when system used UART
	
	if(pump_low_time < 1000)
		pump_low_time++;
	else
		pump_low_time = 0;
	
	if(pump_low_tag && pump_state)
	{
		switch(pump_low_tag)
		{
			case 1:
				pump_low_time_set = 200;
				break;
			case 2:
				pump_low_time_set = 310;
				break;
			case 3:
				pump_low_time_set = 420;
				break;
			case 4:
				pump_low_time_set = 530;
				break;
			case 5:
				pump_low_time_set = 640;
				break;
			default:
				break;
		}
		
		if(pump_low_time < pump_low_time_set)
			Pump_Set(1, pump_step);
		else
			Pump_Set(0, pump_step);
	}
	
	switch(adc_tag)
	{
		case 0:
			if(ADC_GetChannelIntFlag(Temp1_ADC_DATA))
			{
				ADC_ClearIntFlag_CH();
				Temp1_adc = lpf_1rd_calc(&lpf1rd_temp_1, ADC_GetResult(Temp1_ADC_DATA));
				ADC_ClearChannelIntFlag(Temp1_ADC_DATA); 
				
				adc_tag++;
				
				Temp_1 = GET_TEMP_ADC(Temp1_adc, NTC_List_1);
			}
			else
				ADC_ClearAll_Flag();
			
			break;
		case 1:
			if(ADC_GetChannelIntFlag(Temp2_ADC_DATA))
				{
					ADC_ClearIntFlag_CH();
					Temp2_adc = ADC_GetResult(Temp2_ADC_DATA);
					ADC_ClearChannelIntFlag(Temp2_ADC_DATA);
					
					adc_tag++;
					
					Temp_2 = GET_TEMP_ADC(Temp2_adc, NTC_List_2);
				}
			else
				ADC_ClearAll_Flag();
			break;
		case 2:
			if(ADC_GetChannelIntFlag(Pump_ADC_DATA))
			{
				ADC_ClearIntFlag_CH();
				Pump_Current = lpf_1rd_calc(&lpf1rd_0, ADC_GetResult(Pump_ADC_DATA));
				ADC_ClearChannelIntFlag(Pump_ADC_DATA);
				
				adc_tag++;
			}
			else
				ADC_ClearAll_Flag();
			break;
		case 3:
			if(ADC_GetChannelIntFlag(TEC_ADC_DATA))
			{	
				ADC_ClearIntFlag_CH();
				
				TEC_Current_adc = lpf_1rd_calc(&lpf1rd_1, ADC_GetResult(TEC_ADC_DATA));
				
				ADC_ClearChannelIntFlag(TEC_ADC_DATA);
				
				adc_tag = 0;
				
				if(TEC_tag < 100)
				{
					TEC_tag++;
					TEC_Current_adc_base = TEC_Current_adc;
				}					
			}
			else
				ADC_ClearAll_Flag();
			
			break;
			
		default:
			ADC_ClearAll_Flag();
			break;
	}
}

/****************************************************************************
 ** \brief	NMI_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void NMI_Handler(void)
{
	
}
/****************************************************************************
 ** \brief	HardFault_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/

void HardFault_Handler(void)
{
//	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY; EPWM->MASK = 0x00003F00;EPWM->LOCK = 0x0;

//	DelayTime_ms(500);
	
}

/****************************************************************************
 ** \brief	SVC_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void SVC_Handler(void)
{
	
}

/****************************************************************************
 ** \brief	PendSV_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void PendSV_Handler(void)
{
	
}
                
/****************************************************************************
 ** \brief	INTP0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void INTP0_IRQHandler(void)
{
	
}

/****************************************************************************
 ** \brief	INTP1_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void INTP1_IRQHandler(void)
{
  
}

/****************************************************************************
 ** \brief	INTP2_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void INTP2_IRQHandler(void)
{
	
}

/****************************************************************************
 ** \brief	INTP3_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
//void INTP3_IRQHandler(void)
//{
	
//}

/****************************************************************************
 ** \brief	CCP_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
//void CCP_IRQHandler(void)
//{

//}

/****************************************************************************
 ** \brief	WWDT_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
//void WWDT_IRQHandler(void)
//{

//}

/****************************************************************************
 ** \brief	EPWM_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
//void EPWM_IRQHandler(void)
//{

//}

/****************************************************************************
 ** \brief	ADC_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
//void ADC_IRQHandler(void)
//{

//}

/****************************************************************************
 ** \brief	ACMP_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
//void ACMP_IRQHandler(void)
//{

//}

/****************************************************************************
 ** \brief	UART0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
#define RXBUFFERSIZE	9			/* 缓存大小 */
#define TXBUFFERSIZE	9			/* 缓存大小 */
#define VERSION_0		00
#define VERSION_1		01
#define VERSION_2		03

uint8_t UART_RX_Buffer[RXBUFFERSIZE];
uint8_t UART_TX_Buffer_use[TXBUFFERSIZE] = {0xEB, 0xB0};
uint8_t UART_TX_Buffer_Version[TXBUFFERSIZE] = {0xEC, 0xA0};

extern  uint8_t UART_TX_Buffer_cal[TXBUFFERSIZE];
uint8_t RX_Buffer[RXBUFFERSIZE];

uint8_t pump_var[4];

uint8_t UART_RX_STA;
uint16_t pump_cal_write_tag;

extern uint8_t pump_cal_tag;

float temp_1_tx_dowm;

void UART0_IRQHandler(void)
{
	static uint8_t Version_Send;
	static uint8_t temp_switch_last;
	static uint8_t temp_set_last;
	
	UART_RX_Buffer[UART_RX_STA] = Uart0_getchar();
	
	if(UART_RX_STA < RXBUFFERSIZE-1)
		UART_RX_STA ++;
	else
		UART_RX_STA = 0;
	
	if(UART_RX_Buffer[0] == 0xBE)
	{
		if(UART_RX_Buffer[1] == 0xA0)
		{
			if(UART_RX_Buffer[7] == 0xBB && UART_RX_Buffer[8] == 0xEE)
			{
				pump_cal_tag = 0;
				Version_Send = 0;
				
				temp_switch = UART_RX_Buffer[2];
				temp_set 	= UART_RX_Buffer[3];
				pump_state	= UART_RX_Buffer[4];
				pump_set	= UART_RX_Buffer[5];
				fan_set		= UART_RX_Buffer[6];
			}
		}
		else if(UART_RX_Buffer[1] == 0xA1)
		{
			if(UART_RX_Buffer[7] == 0xAA && UART_RX_Buffer[8] == 0xFF)
			{
				pump_cal_tag = 1;
				
				temp_switch = UART_RX_Buffer[2];
				temp_set 	= UART_RX_Buffer[3];
				pump_state	= UART_RX_Buffer[4];
				pump_set	= UART_RX_Buffer[5];
				fan_set		= UART_RX_Buffer[6];
			}
		}
		else
		{
			if(UART_RX_STA == 2)
			{
				UART_RX_STA = 0;
				UART_RX_Buffer[7] = 0x00;
				UART_RX_Buffer[8] = 0x00;
			}
		}
		uint16_t temp_1_tx;
		
		if(temp_switch == 2)
		{
			temp_1_tx = (uint16_t)(Temp_1 * 10.5f);
			
			if(temp_1_tx >= temp_set * 10)
				temp_1_tx = temp_set * 10;
			
			temp_1_tx_dowm = 0.5f;
			
			temp_switch_last = temp_switch;
			temp_set_last = temp_set;
		}
		else
		{
			if(temp_switch == 0)
			{
				temp_1_tx = (uint16_t)(Temp_1 * 10);
			}
			else
			{
				if(temp_switch_last == 2)
				{
					temp_1_tx = (uint16_t)(Temp_1 * 10.5f);
					
					if(temp_1_tx >= temp_set_last * 10)
						temp_1_tx = temp_set_last * 10;
					else
						temp_switch_last = temp_switch;
				}
				else
				{
					if(temp_1_tx_dowm > 0)
					{
						temp_1_tx_dowm -= 0.001f;
						temp_1_tx = (uint16_t)(Temp_1 * (10 + temp_1_tx_dowm));
					}	
					else
						temp_1_tx = (uint16_t)(Temp_1 * 10);
					
					if(temp_1_tx < temp_set * 10)	temp_1_tx = temp_set * 10;
				}
			}
		}
		
		uint16_t temp_2_tx = (uint16_t)(Temp_2 * 10);
			
		if(Version_Send < 10 && pump_cal_tag)
		{
			UART_TX_Buffer_Version[2] = VERSION_0;
			UART_TX_Buffer_Version[3] = VERSION_1;
			UART_TX_Buffer_Version[4] = VERSION_2;

			for(uint8_t UART_TX_STA = 0; UART_TX_STA < TXBUFFERSIZE; UART_TX_STA ++)
			{
				Uart0_putchar(UART_TX_Buffer_Version[UART_TX_STA]);
			}

			Version_Send ++;
		}
		else
		{
			UART_TX_Buffer_use[2] = temp_1_tx >> 8;
			UART_TX_Buffer_use[3] = temp_1_tx;
			UART_TX_Buffer_use[4] = temp_2_tx >> 8;
			UART_TX_Buffer_use[5] = temp_2_tx;
			UART_TX_Buffer_use[6] = Error_tx;
			UART_TX_Buffer_use[7] = TEC_Current >> 8;
			UART_TX_Buffer_use[8] = TEC_Current;

			for(uint8_t UART_TX_STA = 0; UART_TX_STA < TXBUFFERSIZE; UART_TX_STA ++)
			{
				Uart0_putchar(UART_TX_Buffer_use[UART_TX_STA]);
			}
		}
		pump_cal_write_tag = 0;
	}
	else if(UART_RX_Buffer[0] == 0xAE)
	{
		if(UART_RX_Buffer[1] == 0xB7)
		{	
			if(UART_RX_Buffer[7] == 0xCC && UART_RX_Buffer[8] == 0xDD)
			{
				pump_set = 80;

				temp_switch = 0;
				temp_set = 0;
				fan_set = 0;
				
				pump_var[0] = UART_RX_Buffer[2];
				pump_var[1] = UART_RX_Buffer[3];
				pump_var[2] = UART_RX_Buffer[4];
				pump_var[3] = UART_RX_Buffer[5];
				pump_state = UART_RX_Buffer[6];
				
				pump_cal_write_tag ++;
							
				for(uint8_t UART_TX_STA = 0; UART_TX_STA < TXBUFFERSIZE; UART_TX_STA ++)
				{
					Uart0_putchar(UART_TX_Buffer_cal[UART_TX_STA]);
				}
			}
		}
		else
		{
			if(UART_RX_STA == 2)
			{
				UART_RX_STA = 0;
				UART_RX_Buffer[7] = 0x00;
				UART_RX_Buffer[8] = 0x00;
			}
		}
	}
	else
	{
		UART_RX_STA = 0;
	}
}

/****************************************************************************
 ** \brief	TM01H_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void TM01H_IRQHandler(void)
{
	
}

/****************************************************************************
 ** \brief	TIMER0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
//void TIMER0_IRQHandler(void)
//{

//}

/****************************************************************************
 ** \brief	TIMER1_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
//void TIMER1_IRQHandler(void)
//{
	
//}

/****************************************************************************
 ** \brief	LSITIMER_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void LSITIMER_IRQHandler(void)
{
	
}

/****************************************************************************
 ** \brief	TM00_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void TM00_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	TM01_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void TM01_IRQHandler(void)
{
                       
}

/****************************************************************************
 ** \brief	TM02_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void TM02_IRQHandler(void)
{
                       
}

/****************************************************************************
 ** \brief	TM03_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void TM03_IRQHandler(void)
{
                       
}

/****************************************************************************
 ** \brief	WDT_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void WDT_IRQHandler(void)
{
	
}

/****************************************************************************
 ** \brief	I2C0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void I2C0_IRQHandler(void)
{
	
}

/****************************************************************************
 ** \brief	SSP0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void SSP0_IRQHandler(void)
{
	
}

/****************************************************************************
 ** \brief	LVI_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
//void LVI_IRQHandler(void)
//{
	
//}

/****************************************************************************
 ** \brief	FMC_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void FMC_IRQHandler(void)
{
	UART_Lock(UART0);			//Lock when system used UART
	//user code 	
}

