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
/** \file user_hostinithex.h
**
** 
**
** History:
** 
*****************************************************************************/
#ifndef  __USER_HOSTINITHEX_H__
#define  __USER_HOSTINITHEX_H__

#ifdef __cplusplus
extern "C"
{
#endif
	
/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include <stdint.h>

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
#define INT8_VALUE(addr)      (*((int8_t*)addr))
#define INT32_VALUE(addr)     (*((int32_t*)addr))
#define FLOAT_VALUE(addr)     (*((float*)addr))
	
	
/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/
typedef struct 
{		
		float    swGainVbus;
		float    swGainIbus;
		int32_t  calcCoeffVbus;
		int32_t  calcCoeffIbus;
				
		float    iValue;
		
		int32_t  vspStopQ15;
		int32_t  vspStartQ15;
		int32_t  vspMaxQ15;
		int32_t  vspSpeedK;
		int32_t  vspSpeedB;

		int32_t  pwmSpeedK;
		int32_t  pwmSpeedB;
		
    int32_t  clkSpeedStart;	//PWM启动速度
    int32_t  clkSpeedMax;		//PWM最大速度		
		int32_t  clkSpeedK;
		int32_t  clkSpeedB;
    
    float    gainSpeedBase;
    uint16_t runSpeedStart;	//速度环/VQ环控制运行启动值
		uint16_t runSpeedMax;		//速度环/VQ环控制运行最大值
		
    uint16_t vqRunMin;		//VQ环运行最小值
		uint16_t vqRunMax;		//VQ环运行最大值
		
		
}struct_Value_t;

typedef struct 
{
    struct_Value_t value;
}struct_Config_t;

extern volatile struct_Config_t userHostSet;
/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/

//功能模式参数
extern const int8_t   safeTestMode;
extern const float    safeTestCurrent;
extern const int8_t   pcbHallMode;
extern const int8_t   controlMode;
extern const int8_t   startMode;
extern const int8_t   motorFR;
extern const int8_t   sleepMode;
extern const int8_t   reverseMode;
extern const int8_t   faultRestartMode;
extern const int8_t   deadCompMode;
extern const int8_t   breakMode;
extern const int32_t  breakSpeed;
extern const int32_t  breakTime;
//电机及硬件参数}
extern const int8_t   motorPairs;
extern const float    motorRS;
extern const float    motorLS;
extern const float    busVoltage;
extern const float    phaseResistance;
extern const float    ibusResistance;
//启动参数
extern const float    startCurrentInit;
extern const float    startCurrentFinal;
extern const int32_t  startIRamp;
extern const int32_t  satrtEKP;
extern const int32_t  satrtEKI;
extern const int32_t  satrtEOutMin;
//运行参数
extern const int32_t  speedRampInc;
extern const int32_t  speedRampDec;
extern const float    focWKP;
extern const float    focWKI;
extern const float    focWKD;
extern const float    focRunCurrentMin;
extern const float    focRunCurrentMax;
extern const int32_t  focRunSpeedMin;
extern const int32_t  focRunSpeedMax;
extern const float    vectorVoltageMax;
//故障保护参数
extern const int32_t  faultBlockTime;
extern const int32_t  faultStartFailTime;
extern const int32_t  faultRestartDelayTime;
extern const int32_t  faultSpeedMax;
extern const int32_t  faultSpeedMin;
extern const float    faultOverVoltage;
extern const float    faultUnderVoltage;
extern const float    faultOverVoltageRecover;
extern const float    faultUnderVoltageRecover;
extern const float    faultOverCurrent;
//调速控制参数
extern const int8_t   speedControlMode;
extern const float    vspStop;
extern const float    vspStart;
extern const float    vspMax;
extern const int32_t  vspSpeedStart;
extern const int32_t  vspSpeedMax;
extern const int8_t   vspLockSpeedMode;
extern const int32_t  pwmStop;
extern const int32_t  pwmStart;
extern const int32_t  pwmHold;
extern const int32_t  pwmMax;
extern const int32_t  pwmSpeedStart;
extern const int32_t  pwmSpeedMax;
extern const int8_t   pwmLockSpeedMode;
extern const int32_t  clkStop;
extern const int32_t  clkStart;
extern const int32_t  clkMax;
extern const int8_t   clkLockSpeedMode;
extern const int8_t   lockSpeedMode;
extern const int32_t  speedStart;
extern const int32_t  speedMax;
extern const int32_t  speedBaseValue;
extern const float    faultHardOverCurrent;
extern const int32_t  focOffsetAngle;
extern const float    vqRunVoltageMin;
extern const float    vqRunVoltageMax;
extern const int32_t  vqLoopLimitSpeedValue;
extern const float    vqLoopLimitCurrentValue;

/*****************************************************************************/
 /* Global function prototypes ('extern', definition in C source) */
 /*****************************************************************************/
 
void  MC_HEX_Host_Init(void);
 
#ifdef __cplusplus

#endif
 
#endif /* __USER_HOSTINITHEX_H__ */
