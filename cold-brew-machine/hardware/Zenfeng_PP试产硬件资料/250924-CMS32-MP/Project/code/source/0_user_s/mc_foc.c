//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    mc_foc.c
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

//---------------------------------------------------------------------------/
//	Local pre-processor symbols/macros('#define')
//---------------------------------------------------------------------------/

//---------------------------------------------------------------------------/
//	Local variable  definitions
//---------------------------------------------------------------------------/
PID_Components        	PID_ID 								= {0};  /* D轴电流环 */

PID_Components        	PID_IQ 								= {0};	/* Q轴电流环*/

PID_Components        	PID_WL 								= {0};	/* 速度/功率外环*/

struct_SinCos	  				stru_SinCos 					= {0};	/* SinCos计算结构体 */

struct_Curr_abc		 			stru_Curr_abc 				= {0};	/* 马达abc三相电流 */

struct_Clark	  				stru_Curr_alphabeta 	= {0};	/* alpha、beta坐标系电流 */

struct_Park		  				stru_Curr_dq 					= {0};	/* d、q坐标系电流 */

struct_Park		  				stru_Curr_dq_ref 			= {0};	/* d、q坐标系参考电流 */

struct_RevPark	  			stru_Volt_alphabeta 	= {0};	/* alpha、beta坐标系电压 */

struct_Volt_dq	  			stru_Volt_dq 					= {0};	/* d、q坐标系电压 */

struct_Arctan     			stru_Arctan 					= {0};  /* 反正切计算 */ 

struct_SVPWM        		stru_SVM_Calc 				= {0}; 	/* 空间矢量结构体 */ 

//---------------------------------------------------------------------------/
//	Global variable definitions(declared in header file with 'extern')
//---------------------------------------------------------------------------/
volatile u16 						u16_Brake_Duty 				= 0;		/* 刹车占空比 */

struct_RevPark	  			stru_Volt_DeadComp;						/* 死区补偿电压 */

volatile s32						s32_FOC_IPD_Angle			= ERRORRETURN;
//---------------------------------------------------------------------------/
//	Local function prototypes('static')
//---------------------------------------------------------------------------/

//===========================================================================/
//***** definitions  end ****************************************************/
//===========================================================================/

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_ControlPara_Init
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_ControlPara_Init(void)
{
	//------------------------------------------------------------------------/	
	//变量初始化
	stru_Curr_alphabeta.Alpha = 0;
	stru_Curr_alphabeta.Beta 	= 0;
	stru_Curr_dq.Id 					= 0;
	stru_Curr_dq.Iq 					= 0;
	stru_Volt_dq.Ud 					= 0;
	stru_Volt_dq.Uq 					= 0;             
	stru_Curr_abc.Ia 					= 0;
	stru_Curr_abc.Ib 					= 0;
	stru_Curr_abc.Ic 					= 0;	
	stru_Volt_alphabeta.Alpha = 0;
	stru_Volt_alphabeta.Beta  = 0;
	stru_SinCos.Sin						= 0;
	stru_SinCos.Cos						= 0;		
	stru_Curr_dq_ref.Id 			= 0;
	stru_Curr_dq_ref.Iq 			= Para.Start.run_current_init;	
	
	//------------------------------------------------------------------------/	
	//变量初始化
	Fault.PhaseA_LossCnt 			= 0;
	Fault.PhaseB_LossCnt 			= 0;
	Fault.PhaseC_LossCnt 			= 0;
	
	//------------------------------------------------------------------------/	
	//变量初始化
	Flag.Charge 							= 0;
	Flag.IPD 									= 0;
	Flag.MotorBack						= 0;
	Flag.MotorFR 							= Para.Ctrl.MotorDir;		
	Flag.MotorFR_Pre 					= Flag.MotorFR;	
	
	//------------------------------------------------------------------------/	
	//变量初始化
	stru_FOC.Elec_Angle 			= 0;                                                                                                                                     
	stru_FOC.MotorRunTime  		= 0;  
	stru_FOC.Offset_Angle 		= 0;	
	stru_FOC.Elec_We 					= 0;
	stru_FOC.I_Cycle					= 0;	
		
	Para.Ctrl.Md_Svpwm 				= Segment_7;
	
	u16_Brake_Duty 						= 0;
	s32_FOC_IPD_Angle 				= ERRORRETURN;

	//------------------------------------------------------------------------/		
	//PID参数初始化
	FOC_PID_Parameter_Init();

	//位置观测器初始化
	FOC_RotorPosition_Init();
	
	//反电动势参数初始化
	FOC_BEMF_Init();
	
	//Hall控制参数初始化
	FOC_Hall_Init(&stru_Hall);

	//开环参数初始化
	FOC_OpenLoop_Init();
	//------------------------------------------------------------------------/	
	//刹车处理
	if(Fault.Brake)
	{
		Brige_Output_Down();	// 开下管
	}
	else
	{
		Brige_Output_Off();		//关闭桥臂输出	
	}
	
	//------------------------------------------------------------------------/		
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Read_Current
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void  FOC_Read_Current(void)
{
	//-----------------------------------------------------------------------/	
	//读取电流采样值 
	stru_Sample.ADVal[CH_IP_A] = (s32)ADC->DATA[ADC_DATA_CHA];
	stru_Sample.ADVal[CH_IP_B] = (s32)ADC->DATA[ADC_DATA_CHB];
	if (((INT8_VALUE(&startMode) == Start_Hall) || (Config_HallDebug_Mode == HALL_Debug_Enable)))
	{
    stru_Sample.ADVal[CH_HALL] = (s32)ADC->DATA[ADC_DATA_HALL];
  }
	
	//-----------------------------------------------------------------------/	
	//6525母线电压采样，特殊处理 	
	ADC_TGSAMP_VBUS_CONFIG();
	
	//-----------------------------------------------------------------------/	
	//单电阻电流计算 
	#if	(Config_Shunt_Mode == Single_Shunt)
	{		
		FOC_SingleCurrent_Calc();	
	}
	#else
	{		
		//---------------------------------------------------------------------/	
		//双电阻电流计算
		stru_Sample.ARR[CH_IP_A] = FRACMPY_Q10((stru_Sample.ADVal[CH_IP_A] - stru_Sample.REF[CH_IP_A]),stru_Sample.G_Ip);
		stru_Sample.ARR[CH_IP_B] = FRACMPY_Q10((stru_Sample.ADVal[CH_IP_B] - stru_Sample.REF[CH_IP_B]),stru_Sample.G_Ip);
	
		//---------------------------------------------------------------------/
		//电流相序匹配 
		if(FR_CW == Flag.MotorFR)
		{
			stru_Curr_abc.Ia = -stru_Sample.ARR[CH_IP_A];
			stru_Curr_abc.Ib = -stru_Sample.ARR[CH_IP_B];
			stru_Curr_abc.Ic = (stru_Sample.ARR[CH_IP_A]+stru_Sample.ARR[CH_IP_B]);	
		}			
		else
		{
			stru_Curr_abc.Ib = -stru_Sample.ARR[CH_IP_A];
			stru_Curr_abc.Ia = -stru_Sample.ARR[CH_IP_B];
			stru_Curr_abc.Ic = (stru_Sample.ARR[CH_IP_A]+stru_Sample.ARR[CH_IP_B]);	
		}		
		//---------------------------------------------------------------------/
	}	
	#endif
	
	//-----------------------------------------------------------------------/
	//HALL调试模式，用于HALL表格计算
	#if (Config_HallDebug_Mode == HALL_Debug_Enable)
	{
		FOC_Get_HallState(&stru_Hall);
	}
	#endif 

	//-----------------------------------------------------------------------/
	//HALL位置估算
	if (INT8_VALUE(&startMode) == Start_Hall)
	{
		if(MOTOR_STATE != MC_START)
		{	
			FOC_HallSpeed_Calc(&stru_Hall);
      
		}
	}	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Overcurrent_Check
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Overcurrent_Check(void)
{
	volatile int32_t 	v_ABS_Ia, v_ABS_Ib, v_ABS_Ic;

	//-----------------------------------------------------------------------/
	//电流的最大值
	v_ABS_Ia = ABSFUN(stru_Curr_abc.Ia);
	v_ABS_Ib = ABSFUN(stru_Curr_abc.Ib);
	v_ABS_Ic = ABSFUN(stru_Curr_abc.Ic);
	
	//-----------------------------------------------------------------------/
  #if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)	
	//过流检测--马达ABC相电流保护
	if(v_ABS_Ia > Fault.OverCurrent_Value)	{Fault_OverCurrent = 1;	SYSTEM_STATE = SYS_FAULT;}
	if(v_ABS_Ib > Fault.OverCurrent_Value) 	{Fault_OverCurrent = 1;	SYSTEM_STATE = SYS_FAULT;}
	if(v_ABS_Ic > Fault.OverCurrent_Value)	{Fault_OverCurrent = 1;	SYSTEM_STATE = SYS_FAULT;}		
  #endif
	//-----------------------------------------------------------------------/
	//缺相检测--电流最大值
	if(Fault.Ia_max < v_ABS_Ia)		{Fault.Ia_max = v_ABS_Ia;}
	if(Fault.Ib_max < v_ABS_Ib)		{Fault.Ib_max = v_ABS_Ib;}
	if(Fault.Ic_max < v_ABS_Ic)		{Fault.Ic_max = v_ABS_Ic;}
	//-----------------------------------------------------------------------/
	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Brake_Control
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Brake_Control(void)
{
	//-----------------------------------------------------------------------/	
	//刹车占空比
	if(u16_Brake_Duty <	32767)		u16_Brake_Duty	+=	100; 		
	if(u16_Brake_Duty >	32767)		u16_Brake_Duty	 =	32767;

	//-----------------------------------------------------------------------/	
	EPWM->LOCK 				= EPWM_LOCK_P1B_WRITE_KEY;	

	//计算duty
	EPWM->CMPDAT[0] 	= ((u16_Brake_Duty * stru_FOC.EPWM_Period)>>15);
	EPWM->CMPDAT[2] 	= ((u16_Brake_Duty * stru_FOC.EPWM_Period)>>15);
	EPWM->CMPDAT[4] 	= ((u16_Brake_Duty * stru_FOC.EPWM_Period)>>15); 
	
	//使能加载周期&&占空比
	EPWM->CON3 				|= 0x00001500;		
	EPWM->MASK 				 = 0x00001500;
	
	EPWM->LOCK = 0x0;
	//-----------------------------------------------------------------------/
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Start_Check
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Start_Check(void)
{
	//=======================================================================/
	// 启动状态检测	
	volatile static u16 v_Start_Count = 0;
	volatile static u32 v_StartFail_Count = 0;

	//-----------------------------------------------------------------------/
	// 转速大于设定值启动成功
	if(Basic.Mech_Speed > Basic.Speed_Close)
	{		
		v_Start_Count++;			
	}
	else
	{
		v_Start_Count = 0;
		v_StartFail_Count++;
	}	
	//-----------------------------------------------------------------------/
	// 超过设定时间转速未达到设定值认为启动失败
	if(v_Start_Count > 3200)
	{
		v_Start_Count 				= 0;
		v_StartFail_Count 		= 0;
		stru_FOC.Curr_Is_Ref 	= stru_Curr_dq_ref.Iq;
		MOTOR_STATE 					= MC_SW;

	}
	//-----------------------------------------------------------------------/
	if(v_StartFail_Count > Time.Motor_StartFail)
	{	
		v_Start_Count 				= 0;
		v_StartFail_Count 		= 0;	
		Fault_StartFail 			= 1;
		SYSTEM_STATE 					= SYS_RESTART; 
	}

	//-----------------------------------------------------------------------/
	// 停机或正反转切换
	if(Flag.MC_RunStop == 0)	
	{
		v_Start_Count 				= 0;
		v_StartFail_Count 		= 0;
		MOTOR_STATE 					= MC_INIT;
	}	
	//=======================================================================/
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Start_Check
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Hall_Start_Check(void)
{
	//=======================================================================/
	// 启动状态检测	
	volatile static u16 v_Start_Count 		= 0;
	volatile static u32 v_StartFail_Count = 0;

	//-----------------------------------------------------------------------/
	// 转速大于设定值启动成功
	if(Basic.Mech_Speed > Basic.Speed_Close)
	{		
    v_Start_Count++;			
	}
	else
	{
    v_Start_Count = 0;
    v_StartFail_Count++;
	}	
	//-----------------------------------------------------------------------/
	//超过设定时间转速未达到设定值认为启动失败
	//启动成功判定
	if(v_Start_Count > 3200)
	{
    v_Start_Count 				= 0;
    v_StartFail_Count 		= 0;
    stru_FOC.Curr_Is_Ref 	= stru_Curr_dq_ref.Iq;

    FOC_HallSwitchPara_Init();
  
    // 清故障信息
    MC_Clear_FaultMessage();
  
    MOTOR_STATE 					= MC_SW;

	}
	//-----------------------------------------------------------------------/
	//启动失败判定
	if(	Flag.MotorBack == 0)
	{
		//启动失败判定
    if(v_StartFail_Count > Time.Motor_StartFail)
    {	
      v_Start_Count 			= 0;
      v_StartFail_Count 	= 0;	
      Fault_StartFail 		= 1;
      SYSTEM_STATE 				= SYS_RESTART;
    }
	}
	else
	{
		//堵转失败判定
		if(v_StartFail_Count > Time.Motor_Block)	
		{	
			v_Start_Count 			= 0;
			v_StartFail_Count 	= 0;	
			Fault_Block					= 1;
			SYSTEM_STATE 				= SYS_FAULT;

		}
	}
	//-----------------------------------------------------------------------/
	//停机或正反转切换
	if((Flag.MC_RunStop == 0) || (Flag.MotorFR_Pre != Para.Ctrl.MotorDir))		
	{
    v_Start_Count 			= 0;
    v_StartFail_Count 	= 0;
    MOTOR_STATE 				= MC_INIT;
	}	

}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Stop_Check
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Stop_Check(void)
{	
	volatile static u32 v_Stop_Cnt = 0;
	
	//-------------------------------------------------------------------------/
	//正反转切换刹车( 2021-05-21)
	if(Flag.MotorFR_Pre != Para.Ctrl.MotorDir)
	{
		if(Basic.Mech_Speed < Basic.Speed_Brake)
		{	
			MOTOR_STATE = MC_BRAKE;	
		}
	}
	//-------------------------------------------------------------------------/
	else
	{
		//-----------------------------------------------------------------------/
		//Brake刹车功能( 2021-04-13)
		if(Fault.Brake)
		{
			Fault.Braking = 1;			
			if(Basic.Mech_Speed < Basic.Speed_Brake)
			{	
				MOTOR_STATE = MC_BRAKE;	
			}
		}
		//-----------------------------------------------------------------------/
		else 
		{	
			//---------------------------------------------------------------------/
			//进入刹车态
			if(Fault.Braking)
			{
				if(Basic.Mech_Speed < Basic.Speed_Brake)
				{	
					MOTOR_STATE = MC_BRAKE;	
				}
			}
			//---------------------------------------------------------------------/
			//正常停机
			else
			{
				//------------------------------------------------------------------/				
				//状态低于停机设定转速关闭EPWM输出 
				if(Basic.Mech_Speed <= Basic.Speed_Stop)
				{	
					if(++v_Stop_Cnt > Time.Motor_Stophold)
					{
						v_Stop_Cnt = 0;
						Basic.Mech_Speed = 0;		
						MOTOR_STATE = MC_INIT;	
					}
				}
				//------------------------------------------------------------------/				
				//停机过程中接收到重启命令
				else 
				{
					//----------------------------------------------------------------/					
					//允许电机恢复RUN态的条件
					if(stru_Volt_dq.Uq > FOC_STOP_UQ)
					{
						if((Flag.MC_RunStop==1) && (Flag.MotorFR_Pre == Para.Ctrl.MotorDir))
						{
							PID_WL.Out = 0;
							PID_WL.KiSum = 0;	
              // 速度环参数
              if (Speed_Loop == Config.Mode_Control)
              {
                stru_Speed_Ctrl.RampOut = Basic.Calb_Speed;
              }
              // 功率环参数
              else if (Power_Loop == Config.Mode_Control)
              {
                stru_Speed_Ctrl.RampOut = Basic.Power;
              }
              //VQ电压环参数
              else if (VQ_Loop == Config.Mode_Control)
              {
                stru_Speed_Ctrl.RampOut = stru_Volt_dq.Uq;
              }
              // 电流环参数
              else
              {
                stru_Speed_Ctrl.RampOut = stru_Curr_dq_ref.Iq;
              }
              
							MOTOR_STATE = MC_RUN;
						}
					}
					//----------------------------------------------------------------/
					//超时强制停机
					else
					{
						if(++v_Stop_Cnt > 48000)
						{
							v_Stop_Cnt = 0;
							Basic.Mech_Speed = 0;
							MOTOR_STATE = MC_INIT;	
						}
					}
				}
				//------------------------------------------------------------------/
			}		
		}
	}	
	//------------------------------------------------------------------------/

}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Control_Init
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Control_Init(void)
{
	//-----------------------------------------------------------------------//
	// 参数初始化 
	FOC_ControlPara_Init();
	
	//-----------------------------------------------------------------------//
	// 启动
	if(Flag.MC_RunStop)	
	{	
		EPWM->LOCK 			= EPWM_LOCK_P1B_WRITE_KEY;

		EPWM->CMPDAT[0] = stru_FOC.EPWM_Period - ((u16)((16384 * stru_FOC.EPWM_Period)>>15));
		EPWM->CMPDAT[2] = stru_FOC.EPWM_Period - ((u16)((16384 * stru_FOC.EPWM_Period)>>15));
		EPWM->CMPDAT[4] = stru_FOC.EPWM_Period - ((u16)((16384 * stru_FOC.EPWM_Period)>>15));	

		EPWM->CON3 		 |= 0x00001500;		/*使能加载周期&&占空比*/
		EPWM->LOCK 			= 0x0;		
	
		Brige_Output_On();
	
		MOTOR_STATE = MC_CHARGE;	

	}
	//-----------------------------------------------------------------------//
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Control_Charge
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Control_Charge(void)
{		
	//=======================================================================//
	//充电，双N控制预充电，PN控制无需充电
	#if (CHARGE_ENABLE == CHARGE_MODE)

	if(Flag.Charge == 0)
	{																					
		Flag.Charge = 1; 
		Time.Charge_Count = CHARGE_TIME;
	}
	//=======================================================================//	
	if(Time.Charge_Count == CHARGE_TIME)
	{
		EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
		
		if (Para.Ctrl.Md_Shunt == Double_Shunt)
		{
			EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD);	
		}
		else
		{
			EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD)<<16);
		}
							
		EPWM->CON3 |= (0x1UL <<(EPWM0 +EPWM_CON3_LOADEN0_Pos));	
		EPWM->MASK = 0x00003D00;						// U相输出 	
		EPWM->LOCK = 0x0;				
	}
	//=======================================================================//	
	else if(Time.Charge_Count == (CHARGE_TIME<<1)/3)
	{
		EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
		
		if (Para.Ctrl.Md_Shunt == Double_Shunt)
		{
			EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD);
			EPWM->CMPDAT[EPWM2] = (0xffff & EPWM_HALFPERIOD);
		}
		else
		{
			EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD)<<16);
			EPWM->CMPDAT[EPWM2] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD)<<16);
		}	
		EPWM->CON3 |= (0x1UL <<(EPWM0 +EPWM_CON3_LOADEN0_Pos));	
		EPWM->CON3 |= (0x1UL <<(EPWM2 +EPWM_CON3_LOADEN0_Pos));		
		EPWM->MASK = 0x00003500;						// U、V相输出 		
		EPWM->LOCK = 0x0;
	}
	//=======================================================================//	
	else if(Time.Charge_Count == CHARGE_TIME/3)
	{
		EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
		if (Para.Ctrl.Md_Shunt == Double_Shunt)
		{
			EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD);
			EPWM->CMPDAT[EPWM2] = (0xffff & EPWM_HALFPERIOD);
			EPWM->CMPDAT[EPWM4] = (0xffff & EPWM_HALFPERIOD);
		}
		else
		{
			EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD)<<16);
			EPWM->CMPDAT[EPWM2] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD)<<16);
			EPWM->CMPDAT[EPWM4] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD)<<16);
		}		
		EPWM->CON3 |= (0x1UL <<(EPWM0 +EPWM_CON3_LOADEN0_Pos));	
		EPWM->CON3 |= (0x1UL <<(EPWM2 +EPWM_CON3_LOADEN0_Pos));	
		EPWM->CON3 |= (0x1UL <<(EPWM4 +EPWM_CON3_LOADEN0_Pos));	
		EPWM->MASK = 0x00001500;						// U、V、W相输出 	
		EPWM->LOCK = 0x0;		
	}
	//=======================================================================//	
	if(Time.Charge_Count == 0)
	{		
		Brige_Output_On();
		
		#if (Config_Wind_Mode == Start_Wind)
		{
//			ADC1_EnableEPWMTiggerChannel(ADC_SCAN_UBEMF|ADC_SCAN_VBEMF|ADC_SCAN_WBEMF);
//			ADC1_EnableHardwareTigger(ADC1_TG_EPWM0_ZERO);	
			MOTOR_STATE = MC_WIND;	
		}
		#else
		{
			stru_Curr_dq_ref.Id 	= 0;
			stru_Curr_dq_ref.Iq 	= Para.Start.run_current_init;	
			stru_FOC.Curr_Is_Ref 	= Para.Start.run_current_final;
						
			MOTOR_STATE = MC_START;
		}
		#endif
	}
	
	//=======================================================================//		
	#else
		//---------------------------------------------------------------------/
  //使能桥臂输出
  Brige_Output_On();
  
  //解锁寄存器
  EPWM->LOCK 			= EPWM_LOCK_P1B_WRITE_KEY;

  //开通三相六管输出，先给零矢量
  EPWM->CMPDAT[0] = stru_FOC.EPWM_Period - ((u16)((16384 * stru_FOC.EPWM_Period)>>15));
  EPWM->CMPDAT[2] = stru_FOC.EPWM_Period - ((u16)((16384 * stru_FOC.EPWM_Period)>>15));
  EPWM->CMPDAT[4] = stru_FOC.EPWM_Period - ((u16)((16384 * stru_FOC.EPWM_Period)>>15));	

  //使能加载周期&&占空比
  EPWM->CON3 		 |= 0x00001500;	
  EPWM->LOCK 			= 0x0;		

  //---------------------------------------------------------------------/
    #if (Config_Wind_Mode == Start_Wind)
    {
      //开启反电动势检测相位
//      ADC_EnableEPWMTriggerChannel(ADC_SCAN_UBEMF|ADC_SCAN_VBEMF|ADC_SCAN_WBEMF);
//      ADC_EnableHardwareTrigger(ADC_TG_EPWM0_ZERO);	
      MOTOR_STATE = MC_WIND;	
    }
    #else
    {
      stru_Curr_dq_ref.Id 	= 0;
      stru_Curr_dq_ref.Iq 	= Para.Start.run_current_init;	
      stru_FOC.Curr_Is_Ref 	= Para.Start.run_current_final;
            
      MOTOR_STATE = MC_START;					
    }
    #endif
  //---------------------------------------------------------------------/
		
	#endif
	
	//=======================================================================//	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Control_Wind
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/

void FOC_Control_Wind(void)
{
		//-----------------------------------------------------------------------/

		//-----------------------------------------------------------------------/
		// MOS管全封 	
		Brige_Output_Off();
	
	
		//-----------------------------------------------------------------------/
		// 计算反电动势
		stru_bemf.AD_Value[0] = (int32_t)ADC->DATA[ADC_DATA_UBEMF];
		stru_bemf.AD_Value[1] = (int32_t)ADC->DATA[ADC_DATA_VBEMF];
		stru_bemf.AD_Value[2] = (int32_t)ADC->DATA[ADC_DATA_WBEMF];
	
	
		FOC_BEMF_Detection();
	
	
		//-----------------------------------------------------------------------/
		static uint16_t v_Status_Count = 0;
	
	
		// 判断电机反电动势转速 
		if( stru_bemf.Speed > BEMF_SPEED_MAX ) 
		{		
				stru_bemf.Status 	= 3;			//等待电机减速
				v_Status_Count 	= 0;
		}
		else if( (stru_bemf.Speed <= BEMF_SPEED_MAX) && (stru_bemf.Speed > BEMF_SPEED_MIN) ) 
		{
				if(++v_Status_Count > 50)
				stru_bemf.Status = 2;				//顺分启动 切入SW
		}
		else if( (stru_bemf.Speed <= BEMF_SPEED_MIN) && (stru_bemf.Speed > BEMF_SPEED_START) ) 
		{
				stru_bemf.Status 	= 1;			//等待电机减速
				v_Status_Count 	= 0;
		}	
		else
		{
				stru_bemf.Status 	= 0;			//静止启动
				v_Status_Count 	= 0;
		}	
		
		//---------------------------------------------------------------------------/	
		// 反电势启动检测持续时间 

		if( (++stru_bemf.Time > BEMF_CHECK_TIME) ) 
		{
				stru_bemf.Time = BEMF_CHECK_TIME;		
				
				//-----------------------------------------------------------------------/	
				switch(stru_bemf.Status)
				{
						//-------------------------------------------------------------------/
						// 静止启动 
						case 0:

							if(stru_bemf.Speed < 500)
							{
								
									// 启动前的参数初始化
									stru_Curr_dq_ref.Id = 0;
									stru_Curr_dq_ref.Iq = Para.Start.run_current_init;	
									stru_FOC.Curr_Is_Ref 	= Para.Start.run_current_init;//Para.Start.run_current_final;

									stru_bemf.Time = 0;	

									// 使能驱动
									Brige_Output_On();
									
									// 关闭采样
									ADC_DisableEPWMTriggerChannel(ADC_SCAN_UBEMF|ADC_SCAN_VBEMF|ADC_SCAN_WBEMF);	
								
									// 进入预定位状态
									MOTOR_STATE = MC_ALIGN;
							}					
						
							break;
							
						//-------------------------------------------------------------------/			
						// 等待电机减速 								
						case 1:	
					
							break;			
						
						//-------------------------------------------------------------------/	
						// 顺风启动 						
						case 2:

							if(stru_bemf.Angle < 1000)
							{
									//-------------------------------------------------------------/
									v_Status_Count 			= 0;
									stru_bemf.Time 				= 0;
									stru_bemf.Status				= 0;
									stru_bemf.Flag 				= 1;
								
									//-------------------------------------------------------------/									
									stru_rpd_est.Angle 		= stru_bemf.Angle;
								
									stru_rpd_est.PI_Ui			=	HWDIV_Div( (stru_bemf.DeltaTheta*32768),stru_rpd_est.Tc );
									stru_rpd_est.PI_Lower	= ((stru_rpd_est.PI_Ui * 26214)>>15);

									//-------------------------------------------------------------/									
									// Dq电流环PI
									PID_ID.Kp 					= 32768;
									PID_ID.Ki 					= 3276;
									PID_ID.Integral 		= stru_bemf.Ud>>1;
								
									PID_IQ.Kp 					= 32768;
									PID_IQ.Ki 					= 3276;
									PID_IQ.Integral 		= stru_bemf.Uq>>1;
							
									//-------------------------------------------------------------/	
									// 启动前的参数初始化
									stru_Curr_dq_ref.Id 	= 0;
									stru_Curr_dq_ref.Iq 	= Para.Start.run_current_init>>1;	
									stru_FOC.Curr_Is_Ref 		= Para.Start.run_current_final;
												
									Brige_Output_On();

									MOTOR_STATE = MC_SW;

									ADC_DisableEPWMTriggerChannel(ADC_SCAN_UBEMF|ADC_SCAN_VBEMF|ADC_SCAN_WBEMF);						
							}	
							
						//---------------------------------------------------------------/		
						// 等待降速
						case 3:

							break;	

						//---------------------------------------------------------------/						
						default:
							break;
						//---------------------------------------------------------------/				
				}	
		}
					
		
		//=======================================================================/
		// 顺风状态检测	
		if(Flag.MC_RunStop == 0)	
		{
				MOTOR_STATE = MC_INIT;
		}	
		//=======================================================================/
}


/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Wind_Check
* Description    : 顺逆风检测	
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Wind_Check(void)
{
	stru_rpd_est.PI_Kp = 10000;
	stru_rpd_est.PI_Ki = 0;

	//-----------------------------------------------------------------------/
	// 计算Alpha、Beta电流
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;	
	stru_Curr_alphabeta.Beta  = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib*2)>>14);   

	//计算d、q电流
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha)>>15)  + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta)>>15);  
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta )>>15)  - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha)>>15);

	//-----------------------------------------------------------------------/
	// 无感位置估算器
	FOC_RotorPosition_Detection();

	//-----------------------------------------------------------------------/	
	// 计算角度
	stru_FOC.Elec_Angle = stru_rpd_est.Angle;
	stru_SinCos = FOC_SinCos_Calc(stru_FOC.Elec_Angle);	

	//----------------------------------------------------------------------/	
	// Park逆变换	
	stru_Volt_dq.Ud = 0;	
	stru_Volt_dq.Uq = 0;
		
	//----------------------------------------------------------------------/	
	// Park逆变换
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud)>>15)  -  ((stru_SinCos.Sin * stru_Volt_dq.Uq)>>15);
	stru_Volt_alphabeta.Beta  = ((stru_SinCos.Sin * stru_Volt_dq.Ud)>>15)  +  ((stru_SinCos.Cos * stru_Volt_dq.Uq)>>15);

	
	//----------------------------------------------------------------------/	
	// 计算空间矢量
	if (Single_Shunt == Config.Mode_Shunt)	
	{		
		FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);			
	}	
	else
	{
		FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}
	
	//----------------------------------------------------------------------/
	if(Flag.MC_RunStop == 0)	{MOTOR_STATE = MC_INIT;}
	//----------------------------------------------------------------------/

}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Control_Wind
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Control_ObsWind(void)
{
		//-----------------------------------------------------------------------/
		// MOS管全封 	
//		Brige_Output_On();	
	
	
		//-----------------------------------------------------------------------/
		//顺逆风检测
		if( (++stru_bemf.Time < BEMF_CHECK_TIME) || (stru_bemf.Status != 0) )
		{
			FOC_Wind_Check();
		}
	
		//-----------------------------------------------------------------------/
		static uint16_t v_Status_Count = 0;
	
		stru_bemf.Speed = Basic.Mech_Speed;

		// 判断电机反电动势转速 
		if( stru_bemf.Speed > BEMF_SPEED_MAX ) 
		{		
				stru_bemf.Status 	= 3;			//等待电机减速
				v_Status_Count 	= 0;
		}
		else if( (stru_bemf.Speed <= BEMF_SPEED_MAX) && (stru_bemf.Speed > BEMF_SPEED_MIN) ) 
		{
				if(++v_Status_Count > 50)
				stru_bemf.Status = 2;				//顺分启动 切入SW
		}
		else if( (stru_bemf.Speed <= BEMF_SPEED_MIN) && (stru_bemf.Speed > BEMF_SPEED_START) ) 
		{
				stru_bemf.Status 	= 1;			//等待电机减速
				v_Status_Count 	= 0;
		}	
		else if(stru_bemf.Speed <= (-BEMF_SPEED_START)) 
		{
				stru_bemf.Status 	= 4;			//等待电机减速
				v_Status_Count 	= 0;

		}
		else
		{
				stru_bemf.Status 	= 0;			//静止启动
				v_Status_Count 	= 0;
		}	
		
		//---------------------------------------------------------------------------/	
		// 反电势启动检测持续时间 

		if( stru_bemf.Time > BEMF_CHECK_TIME ) 
		{
				stru_bemf.Time = BEMF_CHECK_TIME;		
				
				//-----------------------------------------------------------------------/	
				switch(stru_bemf.Status)
				{
						//-------------------------------------------------------------------/
						// 静止启动 
						case 0:

							//-------------------------------------------------------------------/
							//IPD初始位置检测
							#if (IPD_Enable == Config_IPD_Mode)

							if(Flag.IPD == 0)
							{
								s32_FOC_IPD_Angle = FOC_IPD_PulseSine(&stru_Pulse);
							}
							if(s32_FOC_IPD_Angle != ERRORRETURN)
							{
								if(Flag.IPD == 0)
								{
									Flag.IPD = 1;

									FOC_PID_Parameter_Init();
								}
								else
								{			
									stru_Curr_dq_ref.Id 	= 0;
									stru_Curr_dq_ref.Iq 	= Para.Start.run_current_init;
									stru_FOC.Curr_Is_Ref 	= Para.Start.run_current_final;
									stru_rpd_est.PI_Kp 		= Para.FOC.Ekp;	
									stru_rpd_est.PI_Ki 		= Para.FOC.Eki;
									
									stru_adap_est.Angle = s32_FOC_IPD_Angle + DEG_30;
									FOC_ThetaLimit(&stru_adap_est.Angle);
									
									MOTOR_STATE 					= MC_START;

								}
							}

							#else
							
								stru_Curr_dq_ref.Id 	= 0;
								stru_Curr_dq_ref.Iq 	= Para.Start.run_current_init;
								stru_FOC.Curr_Is_Ref 	= Para.Start.run_current_final;
								stru_rpd_est.PI_Kp 		= Para.FOC.Ekp;	
								stru_rpd_est.PI_Ki 		= Para.FOC.Eki;
								MOTOR_STATE 					= MC_START;	
							#endif 				
							break;
							
						//-------------------------------------------------------------------/			
						// 等待电机减速 								
						case 1:	
							if (IPD_Disable == Config_IPD_Mode)
							{
								stru_Curr_dq_ref.Id 	= 0;
								stru_Curr_dq_ref.Iq 	= Para.Start.run_current_init;
								stru_FOC.Curr_Is_Ref 	= Para.Start.run_current_final;
								stru_rpd_est.PI_Kp 		= Para.FOC.Ekp;	
								stru_rpd_est.PI_Ki 		= Para.FOC.Eki;
								MOTOR_STATE 					= MC_START;
							}
							break;			
						
						//-------------------------------------------------------------------/	
						// 顺风启动 						
						case 2:
						//	if(stru_rpd_est.Angle < 1000)
							{
								stru_Curr_dq_ref.Id 	= 0;
								stru_Curr_dq_ref.Iq 	= Para.Start.run_current_init;
								stru_FOC.Curr_Is_Ref 	= Para.Start.run_current_final;
								stru_rpd_est.PI_Kp 		= Para.FOC.Ekp;	
								stru_rpd_est.PI_Ki 		= Para.FOC.Eki;
								MOTOR_STATE 					= MC_SW;//MC_START;
								
								stru_bemf.Flag = 1;
							}

							break;
						//---------------------------------------------------------------/		
						// 等待降速
						case 3:

							break;	

						//---------------------------------------------------------------/			
						// 等待降速
						case 4:
								stru_adap_est.Angle = stru_FOC.Elec_Angle + DEG_60;
								FOC_ThetaLimit(&stru_adap_est.Angle);
							break;	

						//---------------------------------------------------------------/

						default:
							break;
						//---------------------------------------------------------------/				
				}	
		}

		//=======================================================================/
		// 顺风状态检测	
		if(Flag.MC_RunStop == 0)	
		{
				MOTOR_STATE = MC_INIT;
		}	
		//=======================================================================/
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Hall_Start
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Hall_Start(void)
{
	#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_ENABLE)
	volatile static uint8_t bSafeFlag     = 0;
		#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
		volatile static uint32_t wSafeCount = 0;
		#endif
	#endif
	
	//-----------------------------------------------------------------------/
	// 计算Alpha、Beta电流
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;	
	stru_Curr_alphabeta.Beta  = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib*2)>>14);   

	// 计算DQ电流
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha)>>15)  + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta)>>15);  
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta )>>15)  - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha)>>15);
	
	//-----------------------------------------------------------------------/
	// hall角度估算
	FOC_HallAngle_Calc(&stru_Hall);

	//-----------------------------------------------------------------------/
	// FOC角度
	if(stru_FOC.Elec_Angle > 65535) stru_FOC.Elec_Angle -= 65535;
	if(stru_FOC.Elec_Angle < 0)     stru_FOC.Elec_Angle += 65535; 
	
	stru_SinCos = FOC_SinCos_Calc(stru_FOC.Elec_Angle);	
	
	//--------------------------------------------------------------------------/	
	// 启动电流	
	if(++stru_FOC.I_Cycle > stru_FOC.IRAMP)	
	{	
		stru_FOC.I_Cycle = 0;		
		if (stru_Curr_dq_ref.Iq > Para.Start.run_current_final)	stru_Curr_dq_ref.Iq--;		
		if (stru_Curr_dq_ref.Iq < Para.Start.run_current_final)	stru_Curr_dq_ref.Iq++;
		if(stru_Curr_dq_ref.Iq > Para.Start.run_current_final) stru_Curr_dq_ref.Iq = Para.Start.run_current_final;
	}
	
	//--------------------------------------------------------------------------/	
	// Ud 
	stru_Volt_dq.Ud = PID_CALC2(&PID_ID, stru_Curr_dq_ref.Id, stru_Curr_dq.Id);

	// Vector 限制
	PID_IQ.Upper_Output = Vector_Vq_Limit(&stru_Volt_dq.Ud);
	
	#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
	// Uq
	stru_Volt_dq.Uq = PID_CALC2(&PID_IQ, stru_Curr_dq_ref.Iq, stru_Curr_dq.Iq);
	#else
	if(0 == bSafeFlag )
	{
		#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
			#if (ACMP_CH == ACMP_CH0)
			ACMP_Start(ACMP0);
			#endif
			#if (ACMP_CH == ACMP_CH1)
			ACMP_Start(ACMP1);
			#endif	
		#endif
		stru_Volt_dq.Uq = PID_CALC2(&PID_IQ, stru_Curr_dq_ref.Iq, stru_Curr_dq.Iq);
	
	}
	else
	{
		#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
			#if (ACMP_CH == ACMP_CH0)
			ACMP_Stop(ACMP0);
			#endif
			#if (ACMP_CH == ACMP_CH1)
			ACMP_Stop(ACMP1);
			#endif
		#endif
		
		// UQ电压值给定		
		#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_1)
			if(stru_Volt_dq.Uq < stru_FOC.VoltageDutySafe)
			{
				stru_Volt_dq.Uq++;
			}
			else if(stru_Volt_dq.Uq > stru_FOC.VoltageDutySafe)
			{
				stru_Volt_dq.Uq--;
			}
			else
			{
				stru_Volt_dq.Uq = stru_FOC.VoltageDutySafe;
			}
		#else
			if(wSafeCount < 10000)
			{
				stru_Volt_dq.Uq = stru_FOC.VoltageDutySafe;
			}
			else
			{
				stru_Volt_dq.Uq = 0;
			}
		#endif
		if(stru_Volt_dq.Uq > PID_IQ.Upper_Output)
		{
				stru_Volt_dq.Uq = PID_IQ.Upper_Output;
		}
	}
	#endif
	
	//--------------------------------------------------------------------------/	
	// Park 逆变换
	stru_Volt_alphabeta = RevPark(stru_SinCos, stru_Volt_dq);	

	//--------------------------------------------------------------------------/	
	// 计算电压占空比
	if (Single_Shunt == Config.Mode_Shunt)	
	{		
    FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);			
}	
	else
	{
    FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}

	//=======================================================================/
	// 启动状态检测	
	volatile static u16 v_Start_Count 		= 0;
	volatile static u32 v_StartFail_Count = 0;
	#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_ENABLE)
	volatile static u32 v_ClearFault_Count = 0;
	#endif

	//-----------------------------------------------------------------------/	
	//安规测试模式 2022-08-03
	#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_ENABLE)
	if(bSafeFlag)
	{
		if((Basic.Mech_Speed > (INT32_VALUE(&faultSpeedMin) + 50)) && (Basic.Mech_Speed < INT32_VALUE(&faultSpeedMax)))
		{
			if(++v_ClearFault_Count > 10000)
			{
				v_ClearFault_Count = 0;
				bSafeFlag          = 0;
				PID_IQ.Integral    = stru_Volt_dq.Uq;
				stru_FOC.Curr_Is_Ref = stru_Curr_dq.Iq;
			}
		}
		#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
		if(++wSafeCount > 20000)
		{
			bSafeFlag          = 0;
			wSafeCount         = 0;
			// 清故障信息
			MC_Clear_FaultMessage();	
			// 切换到初始状态
			MOTOR_STATE 			 = MC_INIT;		
		}		
		#endif
	}
	else
	{
		v_ClearFault_Count    = 0;		
	}
	
	#endif		
	
	//-----------------------------------------------------------------------/
	// 转速大于设定值启动成功
	if(Basic.Mech_Speed > Basic.Speed_Close)
	{		
			v_Start_Count++;			
	}
	else
	{
			v_Start_Count = 0;
			v_StartFail_Count++;
	}	
	//-----------------------------------------------------------------------/
	//超过设定时间转速未达到设定值认为启动失败
	//启动成功判定
	#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
	if(v_Start_Count > 3200)
	#else
	if((v_Start_Count > 3200) && (bSafeFlag == 0))	
	#endif
	{
    v_Start_Count 				= 0;
    v_StartFail_Count 		= 0;
    stru_FOC.Curr_Is_Ref 	= stru_Curr_dq_ref.Iq;

    FOC_HallSwitchPara_Init();
  
    // 清故障信息
    MC_Clear_FaultMessage();
  
    MOTOR_STATE 					= MC_SW;
	}
	//-----------------------------------------------------------------------/
	//启动失败判定
	if(	Flag.MotorBack == 0)
	{
		//启动失败判定
		#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
		if(v_StartFail_Count > Time.Motor_StartFail)
		#else
		if((v_StartFail_Count > Time.Motor_StartFail) && (bSafeFlag == 0))
		#endif
		{	
			v_Start_Count 			= 0;
			v_StartFail_Count 	= 0;	
			Fault_StartFail 		= 1;
			#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
      SYSTEM_STATE 				= SYS_RESTART;
			#else
				#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
				SYSTEM_STATE 			= SYS_RESTART;
				#endif
			bSafeFlag 				  = 1;
			#endif
		}
	}
	else
	{
		//堵转失败判定
		#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
		if(v_StartFail_Count > Time.Motor_Block)
		#else
		if((v_StartFail_Count > Time.Motor_Block) && (bSafeFlag == 0))
		#endif
		{	
			v_Start_Count 			= 0;
			v_StartFail_Count 	= 0;	
			Fault_Block					= 1;
			#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
			SYSTEM_STATE 				= SYS_FAULT;
			#else
				#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
				SYSTEM_STATE 			= SYS_FAULT;
				#endif
			bSafeFlag 				  = 1;
			#endif
		}
	}
	//-----------------------------------------------------------------------/
	//停机或正反转切换
	if((Flag.MC_RunStop == 0) || (Flag.MotorFR_Pre != Para.Ctrl.MotorDir))		
	{
		#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_ENABLE)
		Fault_StartFail 		  = 0;		
		Fault_Block           = 0;
		bSafeFlag 			   	  = 0;
			#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
			wSafeCount          = 0;
			#endif
		#endif
		v_Start_Count 			= 0;
		v_StartFail_Count 	= 0;
		MOTOR_STATE 				= MC_INIT;
	}	

	//========================================================================//	

}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Control_Start
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Control_Start(void)
{
	//-------------------------------------------------------------------------/
	//有感启动
	if (INT8_VALUE(&startMode) == Start_Hall)
    {
        FOC_Hall_Start();
	}
	//-------------------------------------------------------------------------/
	//无感启动
	else
    {
        FOC_Sensorless_Start();
    }

	//-------------------------------------------------------------------------/
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Sensorless_Start
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Sensorless_Start(void)
{
	#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_ENABLE)
	volatile static uint8_t bSafeFlag     = 0;
		#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
		volatile static uint32_t wSafeCount = 0;
		#endif
	#endif
	
	//-----------------------------------------------------------------------/
	// 过流检测
	FOC_Overcurrent_Check();

	//-----------------------------------------------------------------------/
	// 计算Alpha、Beta电流
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;	
	stru_Curr_alphabeta.Beta  = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib*2)>>14);   

	// 计算DQ电流
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha)>>15)  + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta)>>15);  
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta )>>15)  - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha)>>15);

	//-----------------------------------------------------------------------/
	// 无感位置估算器
	FOC_RotorPosition_Detection();
		
	//-----------------------------------------------------------------------/	
	// 计算角度正余弦
	stru_SinCos = FOC_SinCos_Calc(stru_FOC.Elec_Angle);	

	//-----------------------------------------------------------------------/
	// Q轴电流参考值	
	if(++stru_FOC.I_Cycle > stru_FOC.IRAMP)
	{
		stru_FOC.I_Cycle = 0;
		if(stru_Curr_dq_ref.Iq > stru_FOC.Curr_Is_Ref)		stru_Curr_dq_ref.Iq--;
		if(stru_Curr_dq_ref.Iq < stru_FOC.Curr_Is_Ref)		stru_Curr_dq_ref.Iq++;
		if(stru_Curr_dq_ref.Iq > Para.Start.run_current_final) stru_Curr_dq_ref.Iq = Para.Start.run_current_final;
	}

	//----------------------------------------------------------------------/	
	// DQ电流环计算
	#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
	FOC_DQCurrent_PIRegulator();
	#else
	if(0 == bSafeFlag )
	{
		#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
			#if (ACMP_CH == ACMP_CH0)
			ACMP_Start(ACMP0);
			#endif
			#if (ACMP_CH == ACMP_CH1)
			ACMP_Start(ACMP1);
			#endif	
		#endif
		FOC_DQCurrent_PIRegulator();
	
	}
	else
	{
		#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
			#if (ACMP_CH == ACMP_CH0)
			ACMP_Stop(ACMP0);
			#endif
			#if (ACMP_CH == ACMP_CH1)
			ACMP_Stop(ACMP1);
			#endif
		#endif
		FOC_FieldCurrent_PIRegulator();
		
		// UQ电压值给定		
		#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_1)
			if(stru_Volt_dq.Uq < stru_FOC.VoltageDutySafe)
			{
				stru_Volt_dq.Uq++;
			}
			else if(stru_Volt_dq.Uq > stru_FOC.VoltageDutySafe)
			{
				stru_Volt_dq.Uq--;
			}
			else
			{
				stru_Volt_dq.Uq = stru_FOC.VoltageDutySafe;
			}
			
			//堵转时自适应启动PI参数减小，防止电流频率过快导致马达失步或转速过高导致切入下一个状态出现重复启动现象
			if(stru_adap_est.PI_Kp < SAFE_MODE_KP_LIMIT*10) stru_adap_est.PI_Kp += 3;
			else if(stru_adap_est.PI_Kp > SAFE_MODE_KP_LIMIT*10) stru_adap_est.PI_Kp -= 3;
			else stru_adap_est.PI_Kp = SAFE_MODE_KP_LIMIT*10;			
		#else
			if(wSafeCount < 10000)
			{
				stru_Volt_dq.Uq = stru_FOC.VoltageDutySafe;
			}
			else
			{
				stru_Volt_dq.Uq = 0;
			}

			//堵转时自适应启动PI参数减小，防止电流频率过快
			stru_adap_est.PI_Kp = SAFE_MODE_KP_LIMIT*10;			
		#endif
		if(stru_Volt_dq.Uq > PID_IQ.Upper_Output)
		{
				stru_Volt_dq.Uq = PID_IQ.Upper_Output;
		}
						
	}
	#endif
	//----------------------------------------------------------------------/	
	// Park逆变换
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud)>>15)  -  ((stru_SinCos.Sin * stru_Volt_dq.Uq)>>15);
	stru_Volt_alphabeta.Beta  = ((stru_SinCos.Sin * stru_Volt_dq.Ud)>>15)  +  ((stru_SinCos.Cos * stru_Volt_dq.Uq)>>15);

	//----------------------------------------------------------------------/			
	// 死区补偿
	if (Config.Mode_DeadComp == Dead_Disable)
	{
		stru_Volt_DeadComp.Alpha = stru_Volt_alphabeta.Alpha;
		stru_Volt_DeadComp.Beta  = stru_Volt_alphabeta.Beta;	
	}
	else
	{
		//启动补固定值--按照死区0.5us补偿
		stru_DeadComp.s32_Comp_Coeff = 400;
		FOC_Calc_DeadComp();
		stru_Volt_DeadComp.Alpha = stru_Volt_alphabeta.Alpha 	+ stru_DeadComp.s32_Comp_Alpha;
		stru_Volt_DeadComp.Beta  = stru_Volt_alphabeta.Beta 	+ stru_DeadComp.s32_Comp_Beta;			
	}	

	//----------------------------------------------------------------------/	
	// 计算空间矢量
	if (Single_Shunt == Config.Mode_Shunt)	
	{		
		FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_DeadComp);			
	}	
	else
	{
		FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_DeadComp);
	}	
	
	//=======================================================================/
	// 启动状态检测	

	volatile static u16 v_Start_Count = 0;
	volatile static u32 v_StartFail_Count = 0;
 	volatile static u32 v_Block_Count = 0;   
	#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_ENABLE)
	volatile static u32 v_ClearFault_Count = 0;
	#endif
	
	//-----------------------------------------------------------------------/	
	//安规测试模式 2022-06-29
	#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_ENABLE)
	if(bSafeFlag)
	{
		if((Basic.Mech_Speed > (INT32_VALUE(&faultSpeedMin) + 200)) && (Basic.Mech_Speed < INT32_VALUE(&faultSpeedMax)))
		{
			if(++v_ClearFault_Count > 10000)
			{
				v_ClearFault_Count = 0;
				bSafeFlag          = 0;
				PID_IQ.Integral    = stru_Volt_dq.Uq;
				stru_FOC.Curr_Is_Ref = stru_Curr_dq.Iq;
			}
		}
		#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
		if(++wSafeCount > 20000)
		{
			bSafeFlag          = 0;
			wSafeCount         = 0;
			// 清故障信息
			MC_Clear_FaultMessage();	
			// 切换到初始状态
			MOTOR_STATE 			 = MC_INIT;		
		}		
		#endif
	}
	else
	{
		v_ClearFault_Count    = 0;		
	}
	
	#endif	
	
	//-----------------------------------------------------------------------/
	// 转速大于设定值启动成功
	if(Basic.Mech_Speed > Basic.Speed_Close)
	{		
		v_Start_Count++;			
	}
	else
	{
		v_Start_Count = 0;
		v_StartFail_Count++;
	}
  // 堵转保护判定
  if((Fault_Block||Flag.MotorBack) && ((Basic.Mech_Speed < INT32_VALUE(&faultSpeedMin)) || (Basic.Mech_Speed > INT32_VALUE(&faultSpeedMax))) )
  {
    v_Block_Count++;
  }
  else
  {
    if(v_Block_Count > 0)
       v_Block_Count--; 
  }
	//-----------------------------------------------------------------------/
	//超过设定时间转速未达到设定值认为启动失败
	//启动成功判定
	#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
	if(v_Start_Count > 3200)
	#else
	if((v_Start_Count > 3200) && (bSafeFlag == 0))	
	#endif
	{
		v_Start_Count 				= 0;
		v_StartFail_Count 		= 0;
		stru_FOC.Curr_Is_Ref 	= stru_Curr_dq_ref.Iq;
		stru_adap_est.PI_Kp   = Para.Start.Ekp;
			
		// 清故障信息
		MC_Clear_FaultMessage();
		
		// 切换下一状态
		MOTOR_STATE 					= MC_SW;

	}
	//-----------------------------------------------------------------------/
	//启动失败判定
	if(	Flag.MotorBack == 0)
	{
		//启动失败判定
		#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
		if(v_StartFail_Count > Time.Motor_StartFail)
		#else
		if((v_StartFail_Count > Time.Motor_StartFail) && (bSafeFlag == 0))
		#endif
		{	
			v_Start_Count 			= 0;
			v_StartFail_Count 	= 0;	
			Fault_StartFail 		= 1;
			#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
			SYSTEM_STATE 				= SYS_RESTART;
			#else
				#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
				SYSTEM_STATE 			= SYS_RESTART;
				#endif
			bSafeFlag 				  = 1;
			#endif
		}
	}
	else
	{
		//堵转失败判定
		#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
		if(v_Block_Count > Time.Motor_Block)
		#else
		if((v_StartFail_Count > Time.Motor_Block) && (bSafeFlag == 0))
		#endif		
		{	
			v_Start_Count 			= 0;
			v_StartFail_Count 	= 0;
            v_Block_Count       = 0;
			Fault_Block					= 1;
			#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
			SYSTEM_STATE 				= SYS_FAULT;
			#else
				#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
				SYSTEM_STATE 			= SYS_FAULT;
				#endif
			bSafeFlag 				  = 1;
			#endif
		}
	}

	//-----------------------------------------------------------------------/
	//停机或正反转切换
	if((Flag.MC_RunStop == 0) || (Flag.MotorFR_Pre != Para.Ctrl.MotorDir))	
	{
		#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_ENABLE)
		Fault_StartFail 		  = 0;		
		Fault_Block           = 0;
		bSafeFlag 			   	  = 0;
		stru_adap_est.PI_Kp   = Para.Start.Ekp;
			#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_2)
			wSafeCount          = 0;
			#endif
		#endif
		
		v_Start_Count 				= 0;
		v_StartFail_Count 		= 0;
        v_Block_Count       = 0;        
		MOTOR_STATE 					= MC_INIT;
	}	
	//=======================================================================/
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Control_Switch
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Control_Switch(void)
{
	//-----------------------------------------------------------------------/
	// 临时变量
	volatile static uint8_t		v_Angle_Ramp = 0;

	//-----------------------------------------------------------------------/
	// 过流检测
	FOC_Overcurrent_Check();		

	//-----------------------------------------------------------------------//
	// 计算Alpha,Beta电流
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;	
	stru_Curr_alphabeta.Beta  = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib*2)>>14); 
	
	// 计算DQ电流
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha)>>15)  + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta) >>15);  
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta )>>15)  - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha)>>15);

	//-----------------------------------------------------------------------/
	// 无感位置估算器
	FOC_RotorPosition_Detection();
	
	//-----------------------------------------------------------------------/
	// 计算角度正余弦
	stru_SinCos 	= FOC_SinCos_Calc(stru_FOC.Elec_Angle);	

	//-----------------------------------------------------------------------/
	// D轴电流给定	
	if(stru_Curr_dq_ref.Id > 0)		stru_Curr_dq_ref.Id--;
	if(stru_Curr_dq_ref.Id < 0)		stru_Curr_dq_ref.Id++;
	
	// Q轴电流给定	
	if(stru_Curr_dq_ref.Iq > stru_FOC.Curr_Is_Ref)		stru_Curr_dq_ref.Iq--;
	if(stru_Curr_dq_ref.Iq < stru_FOC.Curr_Is_Ref)		stru_Curr_dq_ref.Iq++;

	//-----------------------------------------------------------------------/
	// DQ电流环计算
	FOC_DQCurrent_PIRegulator();
	
	//-----------------------------------------------------------------------/
	// Park逆变换
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud)>>15)  -  ((stru_SinCos.Sin * stru_Volt_dq.Uq)>>15);
	stru_Volt_alphabeta.Beta  = ((stru_SinCos.Sin * stru_Volt_dq.Ud)>>15)  +  ((stru_SinCos.Cos * stru_Volt_dq.Uq)>>15);
	
	//----------------------------------------------------------------------/			
	// 死区补偿
	if (Config.Mode_DeadComp == Dead_Disable)
	{
		stru_Volt_DeadComp.Alpha = stru_Volt_alphabeta.Alpha;
		stru_Volt_DeadComp.Beta  = stru_Volt_alphabeta.Beta;	
	}
	else
	{
		//启动补固定值--按照死区0.5us，角度不补偿
		stru_DeadComp.s32_Comp_Coeff = 400;
		FOC_Calc_DeadComp();
		stru_Volt_DeadComp.Alpha = stru_Volt_alphabeta.Alpha 	+ stru_DeadComp.s32_Comp_Alpha;
		stru_Volt_DeadComp.Beta  = stru_Volt_alphabeta.Beta 	+ stru_DeadComp.s32_Comp_Beta;			
	}	

	//----------------------------------------------------------------------/	
	// 计算空间矢量
	if (Single_Shunt == Config.Mode_Shunt)	
	{		
		FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_DeadComp);			
	}	
	else
	{
		FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_DeadComp);
	}
	
	//----------------------------------------------------------------------/

	//=======================================================================/
	// 启动到运行态的过渡阶段 
	if((++stru_FOC.MotorRunTime  >= Para.Start.Hold_Time) ||(Basic.Mech_Speed > Basic.Speed_Switch))
	{
			
		// 切换态保持时间
		stru_FOC.MotorRunTime  	= Para.Start.Hold_Time;	
		
		// 补偿角
		stru_FOC.Offset_Angle 	= Para.FOC.OffsetTheta;

		// dq环参数切换
		PID_ID.Kp 							= Para.FOC.Dkp;
		PID_ID.Ki 							= Para.FOC.Dki;
		PID_IQ.Kp 							= Para.FOC.Qkp;	
		PID_IQ.Ki 							= Para.FOC.Qki;		

		// 速度环参数
		if (Speed_Loop == Config.Mode_Control)
		{
			PID_WL.Kp = Para.FOC.Wkp;
			PID_WL.Ki = Para.FOC.Wki;
			stru_Speed_Ctrl.RampOut = Basic.Calb_Speed;
		}
		// 功率环参数
		else if (Power_Loop == Config.Mode_Control)
		{
			stru_Speed_Ctrl.RampOut = Basic.Power;
		}
		//VQ电压环参数
		else if (VQ_Loop == Config.Mode_Control)
		{
			stru_Speed_Ctrl.RampOut = stru_Volt_dq.Uq;
			stru_FOC.Volt_Uq_Ref    = stru_Volt_dq.Uq;	
			PID_WL.Integral         = stru_Volt_dq.Uq;
		}
		// 电流环参数
		else
		{
			stru_Speed_Ctrl.RampOut = stru_Curr_dq_ref.Iq;
		}
		// 清故障信息
		MC_Clear_FaultMessage();

		// 切换参数初始化
		FOC_SwitchPara_Init();
		
		// 完成切换
		MOTOR_STATE = MC_RUN;
	}

	//=======================================================================/
	// 停机
	if((Flag.MC_RunStop == 0) || (Flag.MotorFR_Pre != Para.Ctrl.MotorDir))		
	{
		MOTOR_STATE = MC_STOP;
	}
	
	//=======================================================================/
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Control_Run
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Control_Run(void)
{
	//-----------------------------------------------------------------------/
	// 过流检测
	FOC_Overcurrent_Check();

	//-----------------------------------------------------------------------/
	// 计算Alpha、Beta电流
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;	
	stru_Curr_alphabeta.Beta  = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib*2)>>14);   

	// 计算D、Q电流
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha)>>15)  + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta)>>15);  
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta )>>15)  - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha)>>15);

	//-----------------------------------------------------------------------/
	// 无感位置估算器
	FOC_RotorPosition_Detection();
	
	//-----------------------------------------------------------------------/	
	// 计算角度正余弦
	stru_SinCos 	= FOC_SinCos_Calc(stru_FOC.Elec_Angle);	

	//-----------------------------------------------------------------------/
	// DQ电流给定
	if(Weaken_Disable == Config.Mode_Weeken)
	{
		stru_Curr_dq_ref.Id = 0;
		stru_Curr_dq_ref.Iq = stru_FOC.Curr_Is_Ref;
	}
	else
	{
		FOC_Control_Weakening();	
	}

	//-----------------------------------------------------------------------/
	// DQ电流环计算
	if (Config.Mode_Control != VQ_Loop) 
  {
    FOC_DQCurrent_PIRegulator();
  }
	else
  {
    FOC_FieldCurrent_PIRegulator();
    stru_Volt_dq.Uq    = stru_FOC.Volt_Uq_Ref;
    // VQ限幅处理
    if(stru_Volt_dq.Uq > PID_IQ.Upper_Output)
    {
      stru_Volt_dq.Uq  = PID_IQ.Upper_Output;
    }
  }

	//-----------------------------------------------------------------------/
	// Park逆变换
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud)>>15)  -  ((stru_SinCos.Sin * stru_Volt_dq.Uq)>>15);
	stru_Volt_alphabeta.Beta  = ((stru_SinCos.Sin * stru_Volt_dq.Ud)>>15)  +  ((stru_SinCos.Cos * stru_Volt_dq.Uq)>>15);
	
	//-----------------------------------------------------------------------/
	// 死区补偿
//	FOC_Control_Voice();	
	# if (Config_Voice_Down_Mode == Voice_Down_Enable)
	// 降速音处理
  FOC_Control_Voice();

	#else
	// 软件死区补偿
	if (Config.Mode_DeadComp == Dead_Disable)
	{
		stru_Volt_DeadComp.Alpha = stru_Volt_alphabeta.Alpha;
		stru_Volt_DeadComp.Beta  = stru_Volt_alphabeta.Beta;	
	}
	else
	{
		FOC_Calc_DeadComp();
		stru_Volt_DeadComp.Alpha = stru_Volt_alphabeta.Alpha 	+ stru_DeadComp.s32_Comp_Alpha;
		stru_Volt_DeadComp.Beta  = stru_Volt_alphabeta.Beta 	+ stru_DeadComp.s32_Comp_Beta;	
	}	
		
	#endif

	//-----------------------------------------------------------------------/
	// 计算空间矢量
	if (Single_Shunt == Config.Mode_Shunt)	
	{		
		FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_DeadComp);			
	}	
	else
	{
		FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_DeadComp);
	}

	//-----------------------------------------------------------------------/
	// 运行状态检测	
	if((Flag.MC_RunStop == 0) || (Flag.MotorFR_Pre != Para.Ctrl.MotorDir))		
	{
		// VQ模式处理
		if (Config.Mode_Control == VQ_Loop)
		{
				// IQ电流环赋积分初值
      PID_IQ.Integral     = stru_Volt_dq.Uq;		
      // IQ参考电流设置
      stru_Curr_dq_ref.Iq = stru_Curr_dq.Iq;
		}
		MOTOR_STATE             = MC_STOP;
	}
	
	//=======================================================================/		
	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_CTR_STOP
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Control_Stop(void)
{	
	//-----------------------------------------------------------------------/
	// 计算Alpha、Beta电流
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;	
	stru_Curr_alphabeta.Beta  = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib*2)>>14);   

	// 计算D、Q电流
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha)>>15)  + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta)>>15);  
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta )>>15)  - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha)>>15);

	//-----------------------------------------------------------------------/
	// 无感位置估算器
	FOC_RotorPosition_Detection();
	
	//----------------------------------------------------------------------/		
	// 计算角度正余弦
	stru_SinCos 	= FOC_SinCos_Calc(stru_FOC.Elec_Angle);	

	//----------------------------------------------------------------------/	
	// DQ电流为0	
	stru_FOC.Curr_Is_Ref = 0;

	if(stru_Curr_dq_ref.Id > 0)											stru_Curr_dq_ref.Id -=1;	
	if(stru_Curr_dq_ref.Id < 0)											stru_Curr_dq_ref.Id +=1;
	if(stru_Curr_dq_ref.Iq > stru_FOC.Curr_Is_Ref)	stru_Curr_dq_ref.Iq -=1;	
	if(stru_Curr_dq_ref.Iq < stru_FOC.Curr_Is_Ref)	stru_Curr_dq_ref.Iq +=1;

	//----------------------------------------------------------------------/	
	// DQ电压为0	
	if(stru_Volt_dq.Uq < FOC_STOP_UQ)
	{
		/*voltage Vds =0*/
		if			(stru_Volt_dq.Ud < 0)		stru_Volt_dq.Ud ++;	
		else if	(stru_Volt_dq.Ud > 0)		stru_Volt_dq.Ud --;
		else														stru_Volt_dq.Ud = 0;
		
		/*voltage Vqs =0*/
		if			(stru_Volt_dq.Uq < 0)		stru_Volt_dq.Uq ++;	
		else if	(stru_Volt_dq.Uq > 0)		stru_Volt_dq.Uq --;
		else														stru_Volt_dq.Uq = 0;
	}
	//----------------------------------------------------------------------/	
	else
	{
		// DQ电流环计算
		FOC_DQCurrent_PIRegulator();
	}
	
	//----------------------------------------------------------------------/	
	// Park逆变换
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud)>>15)  -  ((stru_SinCos.Sin * stru_Volt_dq.Uq)>>15);
	stru_Volt_alphabeta.Beta  = ((stru_SinCos.Sin * stru_Volt_dq.Ud)>>15)  +  ((stru_SinCos.Cos * stru_Volt_dq.Uq)>>15);
	
	//----------------------------------------------------------------------/	
	// 计算电压占空比
	if (Single_Shunt == Config.Mode_Shunt)	
	{		
    FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);			
	}	
	else
	{
    FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}

	//----------------------------------------------------------------------/
	// 停机状态检测
	FOC_Stop_Check();
	
	//----------------------------------------------------------------------/	
	//VQ模式处理
	if (Config.Mode_Control == VQ_Loop)  
  {
    stru_Speed_Ctrl.RampOut = stru_Volt_dq.Uq;
    stru_FOC.Volt_Uq_Ref    = stru_Volt_dq.Uq;
    
    PID_WL.KiSum            = stru_Volt_dq.Uq << 14;
    PID_WL.Integral         = stru_Volt_dq.Uq;
  }
	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Control_Charge
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Control_Brake(void)
{
	//-----------------------------------------------------------------------/	
	volatile static u32 v_Brake_Cnt = 0;

  //-----------------------------------------------------------------------/
	#if(Config_Shunt_Mode == Double_Shunt)
  {
    // 计算Alpha、Beta电流
    stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;	
    stru_Curr_alphabeta.Beta  = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib*2)>>14);   

    // 计算D、Q电流
    stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha)>>15)  + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta)>>15);  
    stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta )>>15)  - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha)>>15);

    //-----------------------------------------------------------------------/
    // 无感位置估算器
    FOC_RotorPosition_Detection();
    
    //-----------------------------------------------------------------------/
    // 计算角度正余弦
    stru_SinCos 	= FOC_SinCos_Calc(stru_FOC.Elec_Angle);	

    //-----------------------------------------------------------------------/
    // 输出电压
    stru_Volt_alphabeta.Alpha = 0;
    stru_Volt_alphabeta.Beta  = 0;
	}
  #endif
	//-----------------------------------------------------------------------/
	//刹车处理
	if(v_Brake_Cnt < 10)
	{
		Brige_Output_Off();
		stru_FOC.Curr_Is_Ref = 0;	
		stru_Curr_dq_ref.Id = 0;
		stru_Curr_dq_ref.Iq = 0;
		stru_Volt_dq.Ud = 0;
		stru_Volt_dq.Uq = 0;
		FOC_PID_Parameter_Init();
	}
	//抱死
	else
	{		
		Brige_Output_Down();					
	}

	//-----------------------------------------------------------------------/
	// 超时强制停机 
	if((++v_Brake_Cnt >= Time.Motor_Brake)||(Basic.Mech_Speed == 0))
	{
		v_Brake_Cnt = Time.Motor_Brake;

		v_Brake_Cnt 			= 0;
		Fault.Braking 		= 0;
		Basic.Mech_Speed 	= 0;
		MOTOR_STATE 			= MC_INIT;	
		
	}
	//-----------------------------------------------------------------------/	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Control_Fault
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Control_Fault(void)
{
	//-----------------------------------------------------------------------/
	// 计算Alpha、Beta电流
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;	
	stru_Curr_alphabeta.Beta  = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib*2)>>14);   

	// 计算D、Q电流
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha)>>15)  + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta)>>15);  
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta )>>15)  - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha)>>15);

	//-----------------------------------------------------------------------/
	// 无感位置估算器
	FOC_RotorPosition_Detection();

	//----------------------------------------------------------------------/		
	// 计算角度正余弦
	stru_SinCos 	= FOC_SinCos_Calc(stru_FOC.Elec_Angle);	

	//----------------------------------------------------------------------/	
	// DQ电流为0	
	stru_FOC.Curr_Is_Ref = 0;

	if(stru_Curr_dq_ref.Id > 0)											stru_Curr_dq_ref.Id -=1;	
	if(stru_Curr_dq_ref.Id < 0)											stru_Curr_dq_ref.Id +=1;
	if(stru_Curr_dq_ref.Iq > stru_FOC.Curr_Is_Ref)	stru_Curr_dq_ref.Iq -=10;	
	if(stru_Curr_dq_ref.Iq < stru_FOC.Curr_Is_Ref)	stru_Curr_dq_ref.Iq +=10;

	//----------------------------------------------------------------------/	
	//DQ电压为0	
	// if(stru_Volt_dq.Uq < FOC_STOP_UQ)
	{
		/*voltage Vds =0*/
		if			(stru_Volt_dq.Ud < -10)		stru_Volt_dq.Ud +=10;	
		else if	(stru_Volt_dq.Ud > 10)		stru_Volt_dq.Ud -=10;
		else															stru_Volt_dq.Ud = 0;
		
		/*voltage Vqs =0*/
		if			(stru_Volt_dq.Uq < -10)		stru_Volt_dq.Uq +=10;	
		else if	(stru_Volt_dq.Uq > 10)		stru_Volt_dq.Uq -=10;
		else															stru_Volt_dq.Uq = 0;
	}

	//----------------------------------------------------------------------/	
	//Park逆变换
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud)>>15)  -  ((stru_SinCos.Sin * stru_Volt_dq.Uq)>>15);
	stru_Volt_alphabeta.Beta  = ((stru_SinCos.Sin * stru_Volt_dq.Ud)>>15)  +  ((stru_SinCos.Cos * stru_Volt_dq.Uq)>>15);
	
	//----------------------------------------------------------------------/	
	//计算空间矢量
	if (Single_Shunt == Config.Mode_Shunt)	
	{		
		FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);			
	}	
	else
	{
		FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}

	//----------------------------------------------------------------------/
	//停机状态检测
	if((stru_Volt_dq.Ud == 0) && (stru_Volt_dq.Uq == 0))
	{
		Basic.Mech_Speed = 0;
		MOTOR_STATE = MC_INIT;
	}

}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Control_Process
* Description    : 
* Function Call  : EPWM_IRQHandler中断函数调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Control_Process(void)
{	
	//=======================================================================//
	// 电机控制状态机
	switch(MOTOR_STATE)
	{	
		//-------------------------------------------------------------------/	
		// 待机状态
		case MC_INIT:		
			
			FOC_Control_Init();			
    
			break;
		
		//-------------------------------------------------------------------/			
		// 充电状态
		case MC_CHARGE:	
			
			FOC_Control_Charge();	
	
			break;	
		
		//-------------------------------------------------------------------/	
		// 顺逆风状态		
		case MC_WIND:		

			FOC_Control_ObsWind();
		
			break;		
		
		//-------------------------------------------------------------------/	
		// 预定位状态		
		case MC_ALIGN:		
			
			FOC_Control_Align();
		
			break;
		
		//-------------------------------------------------------------------/	
		// 启动状态		
		case MC_START:				

			FOC_Control_Start();
		
			break;
		
		//-------------------------------------------------------------------/	
		// 切换状态		
		case MC_SW:			

			FOC_Control_Switch();
		
			break;
		
		//-------------------------------------------------------------------/	
		// 运行状态			
		case MC_RUN:

			FOC_Control_Run();
		
			break;
		
		//-------------------------------------------------------------------/	
		// 停机状态					
		case MC_STOP:

			FOC_Control_Stop();
		
			break;
		
		//-------------------------------------------------------------------/	
		// 刹车状态			
		case MC_BRAKE:
			
			FOC_Control_Brake();
		
			break;
		
		//-------------------------------------------------------------------/		
		// 故障状态
		case MC_FAULT:

			FOC_Control_Fault();
		
			break;

		//-------------------------------------------------------------------/	

		default:
			break;		
	}	
	//=======================================================================//
}

//===========================================================================//
//*********END   ************************************************************//
//===========================================================================//
