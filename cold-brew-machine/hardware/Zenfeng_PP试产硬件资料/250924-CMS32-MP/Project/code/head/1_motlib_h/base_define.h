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
/** \file 
*  File Name  : base_define.h
*  Author     : CMS  Appliction Team
*  Version    : V2.0
*  Date       : 01/03/2020
** Description: Basic Define
** 
******************************************************************************/

#ifndef  __BASE_DEFINE_H__
#define  __BASE_DEFINE_H__
/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include  "para_define.h"
#include  "user_control.h"


/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/


//---------------------------------------------------------------------------------//
typedef   signed          char 	s8;
typedef   signed short    int 	s16;
typedef   signed          int 	s32;

typedef unsigned          char 	u8;
typedef unsigned short    int 	u16;
typedef unsigned          int 	u32;

//---------------------------------------------------------------------------------//
#define   m_True   												(1)
#define   m_False  												(0)

//---------------------------------------------------------------------------------//

#define 	_Q10(A) 												(s32)((A)*1024)												// Q10 format
#define 	_Q12(A) 												(s32)((A)*4096)												// Q12 format
#define 	_Q15(A) 												(s32)((A)*32767)											// Q15 format
#define 	_2PI()													(3.1415926 * 2)	
#define 	_SQRT_3													(1.732051)
#define 	_SQRT_2													(1.414214)
#define 	_Q16_VAL												(65535)
#define 	_Q15_VAL												(32768)	
#define 	_Q12_VAL												(4096)	

//---------------------------------------------------------------------------------//

#define 	CH_IP_A													(0)	
#define 	CH_IP_B													(1)	
#define 	CH_IBUS													(2)	
#define 	CH_VBUS													(3)	
#define 	CH_VCTR													(4)	
#define 	CH_HALL													(5)	

//---------------------------------------------------------------------------------//

/* filter para */                                          

#define   LPF_SAMP_IBUS                		_Q15(0.1)
#define   LPF_SAMP_VBUS                		_Q15(0.1)
#define   LPF_SAMP_VCTRL               		_Q15(0.1)
#define   LPF_SAMP_TEMP               		_Q15(0.1)
#define 	LPF_SPEED_VAL        						_Q15(0.1)
#define		LPF_POWER_VAL										_Q15(0.1)

//---------------------------------------------------------------------------------//
#define 	WEAKENING_RANGE              		(0.90)												/* 弱磁程度 (%) */

#define 	BASE_FREQ												(MOTOR_SPEED_BASE*MOTOR_PAIRS/60)			/* 基准频率(HZ) */

#define 	TPWM_SAMP												(1.0/EPWM_FREQ)								/* 采样周期(S) */

#define 	BASE_KLPF 											(_2PI()*BASE_FREQ*TPWM_SAMP)	/* 2PI*Fbase*Tpwm */

#define		_LPF(A)													_Q15((BASE_KLPF*A)/(1+BASE_KLPF*A))

//---------------------------------------------------------------------------------//

/* Q15 calibration */
#define   SW_GAIN_IPHASE                 	((HW_ADC_REF*8)/(HW_ADC_REF-HW_AMP_REF_IP))
#define   SW_GAIN_VBUS                 		((HW_ADC_REF*8)/(HW_VBUS_VOLT*HW_VBUS_SDR*2))
#define   SW_GAIN_IBUS                 		((HW_ADC_REF*8)/((HW_IBUS_RES*MOTOR_IBUS_BASE*HW_AMP_GAIN_IBUS*1.2)+ HW_AMP_REF_IBUS))
#define   SW_GAIN_SPEED                 	((_Q15_VAL)/(MOTOR_SPEED_BASE*2))
#define   SW_GAIN_VCTRL                 	(_Q15_VAL/_Q12_VAL)


//---------------------------------------------------------------------------------//

#define		FRACMPY_Q15(x,y)								(((x)*(y))>>15)

#define		FRACMPY_Q10(x,y)								(((x)*(y))>>10)

#define 	DFILTER(Xn1,Xn0,Filnum)					(((Filnum*Xn1)>>15) + (((32767 - Filnum)*Xn0)>>15))

#define 	ABSFUN(Value)			 							((Value)>=0?(Value):(-(Value)))

#define 	MINFUN(varone,vartwo)    				((varone)<(vartwo)?(varone):(vartwo))

#define 	MAXFUN(varone,vartwo)    				((varone)>(vartwo)?(varone):(vartwo))

#define 	UPDNLMTFUN(Var,Max,Min)	 				{(Var)=((Var)>=(Max))?(Max):(Var);(Var)=((Var)<=(Min))?(Min):(Var);}


//---------------------------------------------------------------------------------//	

#define 	I_Value(Curr_Val)								_Q15(((Curr_Val*HW_PHASE_RES*HW_AMP_GAIN_IP)/(HW_ADC_REF-HW_AMP_REF_IP)))    //Unit in A

#define		Calc_Coeff_Vbus									_Q12((HW_VBUS_SDR*SW_GAIN_VBUS)/HW_ADC_REF)	//82.85  
#define		Calc_Coeff_Ibus									_Q12((HW_IBUS_RES*HW_AMP_GAIN_IBUS*SW_GAIN_IBUS)/HW_ADC_REF)	//45875.2
#define   Calc_Motor_Power(A)							(s32)(((s32)(A)*Calc_Coeff_Vbus*Calc_Coeff_Ibus)>>15) //116

//_Q12((HW_IBUS_RES*HW_AMP_GAIN_IBUS*((HW_ADC_REF*8)/((HW_IBUS_RES*MOTOR_IBUS_BASE*HW_AMP_GAIN_IBUS*1.2)+ HW_AMP_REF_IBUS)))/HW_ADC_REF)	//45875.2


#define   Calc_Vbus_Value(A)							_Q12((A*HW_VBUS_SDR*SW_GAIN_VBUS)/HW_ADC_REF)	


//---------------------------------------------------------------------------------//


#define  	Brige_Output_Down()   	{EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY; EPWM->MASK = 0x00003F2A;EPWM->LOCK = 0x0;}


/* 三相桥臂输出使能与关闭, 注意驱动是否需要输出使能 */
#if (Config_Debug_Mode == Debug_Disable)

#define  	Brige_Output_Off()  	{EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY; EPWM->MASK = 0x00003F00;EPWM->LOCK = 0x0;}
#define  	Brige_Output_On()   	{EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY; EPWM->MASK = 0x00000000;EPWM->LOCK = 0x0;}

#endif

/* 调试模式下，开启三相输出，用于硬件波形测试 */
#if (Config_Debug_Mode == Debug_Enable)

	#define  	Brige_Output_On()   		{EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY; EPWM->MASK = 0x00000000; EPWM->LOCK = 0x0;}

	#if (Config_Shunt_Mode == Double_Shunt)
	
		#define  	Brige_Output_Off()  	{EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY; EPWM->CMPDAT[0] = EPWM_HALFPERIOD;EPWM->CMPDAT[4] = EPWM_HALFPERIOD;EPWM->CMPDAT[2] = EPWM_HALFPERIOD; \
											 EPWM->CON3 |= 0x00001500;}
	
	#endif

	#if (Config_Shunt_Mode == Single_Shunt)	
		
		#define  	Brige_Output_Off()  	{EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY; EPWM->CMPDAT[0] = EPWM_HALFPERIOD|(EPWM_HALFPERIOD<<16);EPWM->CMPDAT[4] = EPWM_HALFPERIOD|(EPWM_HALFPERIOD<<16);\
											 EPWM->CMPDAT[2] = EPWM_HALFPERIOD|(EPWM_HALFPERIOD<<16); EPWM->CON3 |= 0x00001500;}
	
	#endif
		
#endif
	
//---------------------------------------------------------------------------------//


/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/


/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/


#endif /* __BASE_DEFINE_H__ */


