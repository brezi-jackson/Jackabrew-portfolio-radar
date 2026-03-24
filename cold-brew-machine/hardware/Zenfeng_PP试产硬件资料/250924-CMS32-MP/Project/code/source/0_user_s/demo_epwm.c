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
* responsible for misuse or illegal use of this so9ftware for devices not
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
/** \file demo_epwm.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "demo_epwm.h"

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
 ** \brief	EPWM_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_Config(void)
{
	CGC_PER11PeriphClockCmd(CGC_PER11Periph_EPWM,ENABLE);	/*开启EPWM时钟*/
	/*
	(1)设置EPWM运行模式
	*/

	/*
	(2)设置EPWM时钟周期与死区
	*/		
	
	EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_16);			/*设置EPWM0时钟为APB时钟的1分频*/
	EPWM_ConfigChannelClk( EPWM1, EPWM_CLK_DIV_16);			/*设置EPWM1时钟为APB时钟的1分频*/
	EPWM_ConfigChannelClk( EPWM2, EPWM_CLK_DIV_16);			/*设置EPWM2时钟为APB时钟的1分频*/
	EPWM_ConfigChannelClk( EPWM3, EPWM_CLK_DIV_16);			/*设置EPWM3时钟为APB时钟的1分频*/
	EPWM_ConfigRunMode(  
//							EPWM_COUNT_UP_DOWN 		| 		/*上下计数模式(中心对齐)*/
//						 EPWM_OCU_SYMMETRIC 		|		/*对称模式*/
//						 EPWM_WFG_COMPLEMENTARYK   |		/*互补模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
	EPWM_ConfigChannelPeriod(EPWM0,  100);					/*EPWM0通道的周期 = 4800/Fepwm  = 300us*/
	EPWM_ConfigChannelPeriod(EPWM1,  100);					/*EPWM0通道的周期 = 4800/Fepwm  = 300us*/
	EPWM_ConfigChannelPeriod(EPWM2,  100);					/*EPWM2通道的周期 = 4800/Fepwm  = 300us*/
	EPWM_ConfigChannelPeriod(EPWM3,  100);					/*EPWM2通道的周期 = 4800/Fepwm  = 300us*/

	EPWM_ConfigChannelSymDuty(EPWM0, 0);
	EPWM_ConfigChannelSymDuty(EPWM1, 100);
	EPWM_ConfigChannelSymDuty(EPWM2, 0);
	EPWM_ConfigChannelSymDuty(EPWM3, 100);
	
	EPWM_DisableDeadZone(EPWM_CH_0_MSK | EPWM_CH_1_MSK |
						 EPWM_CH_2_MSK | EPWM_CH_3_MSK |
						 EPWM_CH_4_MSK | EPWM_CH_5_MSK);	/*关闭死区*/

	/*
	(3)设置EPWM反向输出
	*/
	EPWM_DisableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK |
							   EPWM_CH_2_MSK | EPWM_CH_3_MSK |
							   EPWM_CH_4_MSK | EPWM_CH_5_MSK );				/*关闭反相输出*/

	/*
	(4)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK);	/*设置为自动加载模式*/
	EPWM_ConfigLoadAndIntMode(EPWM0, EPWM_EACH_PERIOD_ZERO);
	EPWM_ConfigLoadAndIntMode(EPWM1, EPWM_EACH_PERIOD_ZERO);
	EPWM_ConfigLoadAndIntMode(EPWM2, EPWM_EACH_PERIOD_ZERO);
	EPWM_ConfigLoadAndIntMode(EPWM3, EPWM_EACH_PERIOD_ZERO);	/*加载点设置为每个周期点和零点*/
	/*
	(5)设置EPWM刹车
	*/
	EPWM_AllBrakeDisable();

	/*
	(6)设置中断
	*/
//	EPWM_EnableZeroInt(EPWM_CH_0_MSK);								/*开启零点中断*/
//	EPWM_EnableZeroInt(EPWM_CH_1_MSK);								/*开启零点中断*/
//	EPWM_EnableZeroInt(EPWM_CH_2_MSK);								/*开启零点中断*/
//	EPWM_EnableZeroInt(EPWM_CH_3_MSK);								/*开启零点中断*/
//	EPWM_EnableZeroInt(EPWM_CH_4_MSK);								/*开启零点中断*/		
//	
//	NVIC_EnableIRQ(EPWM_IRQn);
	/*
	(6)设置优先级
	*/	
//	NVIC_SetPriority(EPWM_IRQn,3);					/*优先级0~3， 0最高、3最低*/
	
	/*
	(7)设置IO口输出
	*/	
	GPIO_PinAFOutConfig(P10CFG, IO_OUTCFG_P10_EPWM0);				/*设置P10为EPWM0通道*/
	GPIO_PinAFOutConfig(P11CFG, IO_OUTCFG_P11_EPWM1);				/*设置P11为EPWM1通道*/
	GPIO_PinAFOutConfig(P12CFG, IO_OUTCFG_P12_EPWM2);				/*设置P12为EPWM2通道*/
	GPIO_PinAFOutConfig(P13CFG, IO_OUTCFG_P13_EPWM3);				/*设置P13为EPWM3通道*/

	GPIO_Init(PORT1,PIN0,OUTPUT);
	GPIO_Init(PORT1,PIN1,OUTPUT);
	GPIO_Init(PORT1,PIN2,OUTPUT);
	GPIO_Init(PORT1,PIN3,OUTPUT);
	
	EPWM_DisableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK |
					  EPWM_CH_2_MSK | EPWM_CH_3_MSK);

	/*
	(8)开启EPWM
	*/		
	EPWM_Start(EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK);	
}

extern uint8_t pump_set;
extern uint8_t pump_set_test;

float error;
float deriv;

float kp_test = 100;
float ki_test = 0.1;
int16_t OutLimitHigh = 550;
int16_t OutLimitLow = 10;
int16_t IntegLimitHigh = 5000;
int16_t IntegLimitLow = 5000;

void TEMP_PID_Controler(PidObject* pidproject, uint8_t mode)
{
	if(mode == 1)
	{
		if(pidproject->desired < 40)
		{
			error = -(pidproject->desired - 5 - pidproject->measured); 
			
			pidproject->OutLimitHigh = 550;
			pidproject->OutLimitLow  = 10;
		
			pidproject->kp = kp_test;
			pidproject->ki = ki_test;
			
			pidproject->IntegLimitHigh = IntegLimitHigh;
			pidproject->IntegLimitLow = -IntegLimitHigh;
		}
		else
		{
			error = 0;
		}
	}
	else if(mode == 2)
	{
		error = pidproject->desired - pidproject->measured; 
		
		if(pidproject->desired <= 60)	error += 2;

		pidproject->OutLimitHigh = OutLimitHigh;
		pidproject->OutLimitLow  = OutLimitLow;
	
		pidproject->kp = pidproject->desired - 15;
		pidproject->ki = 0.0003 * pidproject->desired;
			
		pidproject->IntegLimitHigh = 5000;
		pidproject->IntegLimitLow = -5000;
	}
	
	pidproject->integ += error;	 //误差积分累加值

	if(pidproject->integ >= pidproject->IntegLimitHigh)
		pidproject->integ = pidproject->IntegLimitHigh;
	else if(pidproject->integ <= pidproject->IntegLimitLow)
		pidproject->integ = pidproject->IntegLimitLow;

//	deriv = (error - Temp->prevError);  //前后两次误差做微分

	pidproject->out = pidproject->kp * error + pidproject->ki * pidproject->integ + pidproject->kd * deriv;//PID输出

	if(pidproject->out >= pidproject->OutLimitHigh)
		pidproject->out	= pidproject->OutLimitHigh;
	else if(pidproject->out <= pidproject->OutLimitLow)
		pidproject->out	= pidproject->OutLimitLow;
	
	pidproject->prevError = error;  //更新上次的误差
}

float C_kp_test = 0.2;
float C_ki_test = 0.05;
int16_t C_IntegLimitHigh = 2000;
int16_t C_IntegLimitLow = 1000;

void CURRENT_PID_Controler(PidObject* pidproject, uint8_t mode, uint32_t Temp_Desired)
{
	error = pidproject->desired - pidproject->measured; 
	
	if(mode == 1)
	{
		pidproject->OutLimitHigh = 100;
		pidproject->OutLimitLow  = 20;

		pidproject->kp = C_kp_test;
		pidproject->ki = C_ki_test;
		
		pidproject->IntegLimitHigh = C_IntegLimitHigh;
		pidproject->IntegLimitLow = -C_IntegLimitHigh;
	}
	else if(mode == 2)
	{
		pidproject->OutLimitHigh = 100;
		pidproject->OutLimitLow  = 5;
		
		pidproject->kp = 0.5;
		pidproject->ki = 0.001 * Temp_Desired - 0.01;
		
		pidproject->IntegLimitHigh = 2000;
		pidproject->IntegLimitLow = -pidproject->IntegLimitHigh;
	}

	pidproject->integ += error;	 //误差积分累加值

	if(pidproject->integ >= pidproject->IntegLimitHigh)
		pidproject->integ = pidproject->IntegLimitHigh;
	else if(pidproject->integ <= pidproject->IntegLimitLow)
		pidproject->integ = pidproject->IntegLimitLow;
	
//	deriv = (error - Temp->prevError);  //前后两次误差做微分

	pidproject->out = pidproject->kp * error + pidproject->ki * pidproject->integ + pidproject->kd * deriv;//PID输出

	if(pidproject->out >= pidproject->OutLimitHigh)
		pidproject->out	= pidproject->OutLimitHigh;
	else if(pidproject->out <= pidproject->OutLimitLow)
		pidproject->out	= pidproject->OutLimitLow;
	
	pidproject->prevError = error;  //更新上次的误差
}

void TEC_MOS_Set(uint32_t tec_pwm, uint8_t state)
{
	UART_Lock(UART0);
	
	switch(state)
	{
		case 0:
			EPWM_ConfigChannelSymDuty(EPWM0, 0);
			EPWM_ConfigChannelSymDuty(EPWM2, 0);
			/*P_MOS*/
			EPWM_DisableOutput(EPWM_CH_1_MSK);	//TEC-	EPWM_EnableOutput EPWM_DisableOutput
			EPWM_DisableOutput(EPWM_CH_3_MSK);	//TEC+
			
			/*N_MOS*/
			EPWM_DisableOutput(EPWM_CH_0_MSK);	//TEC+
			EPWM_DisableOutput(EPWM_CH_2_MSK);	//TEC-
			break;
		
		case 1://cooling
			EPWM_ConfigChannelSymDuty(EPWM0, 0);
			EPWM_ConfigChannelSymDuty(EPWM2, tec_pwm);
			/*P_MOS*/
			EPWM_EnableOutput(EPWM_CH_1_MSK);	//TEC-	EPWM_EnableOutput EPWM_DisableOutput
			EPWM_DisableOutput(EPWM_CH_3_MSK);	//TEC+
			
			/*N_MOS*/
			EPWM_DisableOutput(EPWM_CH_0_MSK);	//TEC+
			EPWM_EnableOutput(EPWM_CH_2_MSK);	//TEC-
				
			break;
		
		case 2://heating
			EPWM_ConfigChannelSymDuty(EPWM0, tec_pwm);
			EPWM_ConfigChannelSymDuty(EPWM2, 0);
			/*P_MOS*/
			EPWM_DisableOutput(EPWM_CH_1_MSK);	//TEC-	EPWM_EnableOutput EPWM_DisableOutput
			EPWM_EnableOutput(EPWM_CH_3_MSK);	//TEC+
			
			/*N_MOS*/
			EPWM_EnableOutput(EPWM_CH_0_MSK);	//TEC+
			EPWM_DisableOutput(EPWM_CH_2_MSK);	//TEC-
			break;
		
		default:
			break;
	}
}

uint32_t Temp_test;
uint8_t temp_state;

uint8_t	temp_state_last;
uint32_t tec_set;

PidObject Temp_pid, Current_pid;

extern int32_t TEC_Current;
int32_t tec_current_set;

extern uint8_t g_ticks;

uint8_t tec_pid_tag;

void TEC_Set(uint32_t Temp_Desired, uint8_t state)
{
	static uint8_t pid_tag;
		
	UART_Lock(UART0);
		
	if(temp_state_last != state)
	{	
		if(tec_set)
		{
			tec_set--;

			TEC_MOS_Set(Temp_test, temp_state_last);
		}
		else
		{
			TEC_MOS_Set(0, 0);
			temp_state_last = state;
		}
	}
	else
	{
		if(state)
		{		
			if(tec_pid_tag >= 10)
			{
				tec_pid_tag = 0;
				
				Temp_pid.measured = Temp_1;
				Temp_pid.desired = Temp_Desired;
					
				TEMP_PID_Controler(&Temp_pid, state);
				
				Current_pid.measured = TEC_Current;
				
				tec_current_set = Temp_pid.out;
			
				Current_pid.desired = tec_current_set;
				
				CURRENT_PID_Controler(&Current_pid, state, Temp_Desired);
					
				tec_set = Current_pid.out;
								
				TEC_MOS_Set(tec_set, state);

				temp_state_last = state;
			}
			else
				tec_pid_tag ++;
		}
	}
}

