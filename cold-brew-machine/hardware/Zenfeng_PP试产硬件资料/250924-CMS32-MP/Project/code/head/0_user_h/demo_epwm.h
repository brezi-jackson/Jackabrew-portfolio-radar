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

/*****************************************************************************/
/** \file demo_epwm.h
**
** History:
** 
*****************************************************************************/
#ifndef __DEMO_EPWM_H_
#define __DEMO_EPWM_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "common.h"
#include "epwm.h"
#include "cgc.h"
#include "gpio.h"
#include "uart.h"
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/

							
/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/
typedef volatile struct
{
	float desired;     ///期望
	float offset;      //
	float prevError;    // 上次偏差
	float integ;        //误差积分累加值
	float kp;           //p参数
	float ki;           //i参数
	float kd;           //d参数
	float IntegLimitHigh;       //< integral limit
	float IntegLimitLow;
	float measured;     ////pid反馈量
	float out;
	float OutLimitHigh;
	float OutLimitLow;	
	float Control_OutPut;//控制器总输出
	float Last_Control_OutPut;//上次控制器总输出
	float Control_OutPut_Limit;//输出限幅
		/***************************************/
	float Last_FeedBack;//上次反馈值
	float Dis_Err;//微分量
	float Dis_Error_History[5];//历史微分量
	float Err_LPF;
	float Last_Err_LPF;
	float Dis_Err_LPF;

//	int8_t Err_Limit_Flag :1;//偏差限幅标志
//	int8_t Integrate_Limit_Flag :1;//积分限幅标志
//	int8_t Integrate_Separation_Flag :1;//积分分离标志		
}PidObject;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/


/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
extern PidObject Temp_pid;
extern float Temp_1;
extern float Temp_2;
/*****************************************************************************
 ** \brief	EPWM_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_Config(void);

void TEC_Set(uint32_t Temp_Desired, uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_EPWM_H_ */

