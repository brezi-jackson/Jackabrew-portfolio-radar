//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    motro_control.c
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
volatile SystStates 		SYSTEM_STATE 			= SYS_INIT;		/* 系统流程状态机 */

volatile MotorStates 		MOTOR_STATE 			= MC_INIT;		/* 电机控制流程状态机 */

volatile Union_Fault		u_Fault_Flag 			= {0};				/* 电机故障指示标志 */

struct_Basic_Para		  	Basic 				  	= {0};				/* 基本参数结构体 */

struct_Falg 				    Flag 			    		= {0};				/* 标志位结构体 */

struct_Time    				  Time 			    		= {0}; 				/* 时间计数结构体 */

struct_Count    		  	Count 					  = {0}; 	 			/* 数学计数结构体 */

struct_Fault 				    Fault 				  	= {0};				/* 故障参数结构体 */

struct_MotCtrl_Para			Para 			    		= {0};				/* 电机控制参数结构体 */

struct_Config			    	Config				  	= {0};				/* 系统配置选项结构体 */

struct_Base_Value			  stru_base_value		= {0};				/* 电机基值参数结构体 */	

struct_ADC_Sample 		 	stru_Sample				= {0};				/* 采样参数结构体 */

struct_FOC_Ctrl			  	stru_FOC			  	= {0};				/* FOC控制参数结构体 */

struct_SpeedRamp		  	stru_Speed_Ctrl		= {0};				/* 速度控制结构体 */

struct_Hall				    	stru_Hall 				= {0};				/* HALL控制结构体 */

//---------------------------------------------------------------------------/
//	Global variable definitions(declared in header file with 'extern')
//---------------------------------------------------------------------------/
u8 u8_ZF_Cnt1 = 0;
u8 u8_ZF_Cnt2 = 0;

//---------------------------------------------------------------------------/
//	Local function prototypes('static')
//---------------------------------------------------------------------------/

//===========================================================================/
//***** definitions  end ****************************************************/
//===========================================================================/

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : User_Control
* Description    : 
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/

//===========================================================================/
// 电机运行使能 
void User_Motor_On(void)
{
	Flag.MC_RunStop = 1;
  
}

//===========================================================================/
// 电机停止 
void User_Motor_Off(void)
{
	Flag.MC_RunStop = 0;

}

//===========================================================================/
// 电机方向控制 
void User_Motor_FRControl(u8 Input_Val)
{
	if(Input_Val == FR_CW)
		Para.Ctrl.MotorDir = FR_CW;
	else
		Para.Ctrl.MotorDir = FR_CCW;
	
}

//===========================================================================/
// 调速控制 
void User_MotorSpeed_Set(s32 Input_Val)
{
	//-----------------------------------------------------------------------/		
	//电流环控制，给定值/32767 = 给定电流/最大电流--相电流峰值
	if (Config.Mode_Control == Current_Loop)
	{
		if(Input_Val > stru_FOC.Curr_Iq_Max)
			Input_Val = stru_FOC.Curr_Iq_Max;
		stru_FOC.Speed_Ref = Input_Val;
  }
	
	//-----------------------------------------------------------------------/	
	//速度环控制
	else if (Config.Mode_Control == Speed_Loop)
	{
		#if (LIMIT_SPEED_ENABLE)
		if(Input_Val > LIMIT_SPEED_VALUE)
			Input_Val = (s32)LIMIT_SPEED_VALUE;
		#endif
		
		stru_FOC.Speed_Ref = FRACMPY_Q10(Input_Val,stru_Sample.G_Sp);
	
  }

	//-----------------------------------------------------------------------/	
	//功率环控制 
	else if (Config.Mode_Control == Power_Loop)
	{
		//直接给定功率值 
		#if 0
		if(Input_Val > LIMIT_POWER_VALUE)
			Input_Val = (s32)LIMIT_POWER_VALUE;
		stru_FOC.Speed_Ref = Input_Val * stru_Sample.G_Pw;
		
		//按照调速信号比例给定
		#else
		
		stru_FOC.Speed_Ref = Input_Val;
		
		#endif
    
  }
	
	//-----------------------------------------------------------------------/
	//VQ电压环控制
	else if (Config.Mode_Control == VQ_Loop)
  {
    if(Input_Val > 32767)
    {
      Input_Val = 32767;
    }
    stru_FOC.Speed_Ref = Input_Val;
  }
	//-----------------------------------------------------------------------/
}

//===========================================================================/
// 关闭所有功能模块 
void System_Modules_off(void)
{
	Brige_Output_Off();	
	User_Motor_Off();
  
}

//===========================================================================/
// EPWM故障刹车保护重启 
void EPWM_ResetFaultBrake(void)
{
	EPWM_ClearBrakeIntFlag(); 		/*清除刹车中断标志位*/				
	EPWM_Start(EPWM_CH_0_MSK | EPWM_CH_1_MSK|EPWM_CH_2_MSK | EPWM_CH_3_MSK| EPWM_CH_4_MSK | EPWM_CH_5_MSK);
	EPWM_ClearBrake();
  
}

//===========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : System_Control
* Description    : 系统状态机流程控制
* Function Call  : 主循环调用
* Input Paragram : 
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void System_Control(void)
{
	switch(SYSTEM_STATE)
	{
		//===================================================================//
		// 系统初始化 
		case SYS_INIT:
		{
      //禁用SWD
      #if(Motor_Debug_Mode == Motor_Debug_Offline)
      DBG->DBGSTOPCR |= (1<<24);
      #endif
      
			//FOC控制参数初始化
			FOC_ControlPara_Init();
			//基值参数初始化
			FOC_BaseParameter_Init();
			//故障清零，防止上电误动作
			EPWM_ResetFaultBrake();
			//---------------------------------------------------------------/	
			//Brake 刹车功能 
			if (INT8_VALUE(&breakMode) == Brake_Enable)
      {
        if(PORT_GetBitValue(PORT0 ,PIN0) == 1)
        {
          Fault.Brake = 1;					
        }
        else
        {
          Fault.Brake = 0;			
        }
      }
			//---------------------------------------------------------------/	
			//正反转功能 
			if (INT8_VALUE(&reverseMode) == Motor_ZF_Enable)	
      {
        if(PORT_GetBitValue(PORT0 ,PIN0) == 1)
        {
          if(++u8_ZF_Cnt1 > 100)
          {
            u8_ZF_Cnt1 = 100;
            User_Motor_FRControl(1-(u8)INT8_VALUE(&motorFR));
            SYSTEM_STATE = SYS_RUN; 
          }
          u8_ZF_Cnt2 = 0;		
        }
        else
        {
          if(++u8_ZF_Cnt2 > 100)
          {
            u8_ZF_Cnt2 = 100;
            User_Motor_FRControl((u8)INT8_VALUE(&motorFR));	
            SYSTEM_STATE = SYS_RUN; 
          }
          u8_ZF_Cnt1 = 0;
		
        }
      }            
			//---------------------------------------------------------------/	
			if(INT8_VALUE(&reverseMode) == Motor_ZF_Disable)
      {
        SYSTEM_STATE = SYS_RUN; 
      }		
			//---------------------------------------------------------------/	

			break;
		}		
		//===================================================================//
		// 系统正常工作状态 
		case SYS_RUN:
		{	
			//马达调速控制
			if(Time.Motor_Restart == 0)
			{				
				//电压保护--过欠压状态时马达关机，电压恢复正常后马达重启
				if(Fault_OverVoltage || Fault_UnderVoltage || Fault.Brake)	
				{					
					User_Motor_Off();
				}	
				//进入调速控制
				else
				{
					User_Speed_Control();
				}
			}				
			break;
		}
		//===================================================================//		
		// 马达重启状态
		case SYS_RESTART:
		{	
			//马达关机
			User_Motor_Off();	

			//故障保护后允许马达重启
			#if (Motor_Restart_Mode == Motor_Restart_Enable)
			{
				//马达重启时间
				Time.Motor_Restart = INT32_VALUE(&faultRestartDelayTime)*1000;
				//清除故障信息
				EPWM_ResetFaultBrake();				
				MC_Clear_FaultMessage();
				Count.Motor_Restart = 0;
				//系统恢复正常状态
				SYSTEM_STATE = SYS_RUN; 	
			}
			#else	
			{			
				//重启间隔时间--1000ms
				Time.Motor_Restart = 1000;
				
				//启动失败允许重启3次
				if(++Count.Motor_Restart >= 3)
				{
					Count.Motor_Restart = 3;
					SYSTEM_STATE = SYS_WAIT; 
				}	
				else
				{
					SYSTEM_STATE = SYS_RUN; 	
				}
			}
			#endif
			
			//马达设置为待机状态
			MOTOR_STATE = MC_INIT;

			break;
		}	
		//===================================================================//
		// 故障停机态 
		case SYS_FAULT:
		{
			//马达关机
			User_Motor_Off();
			
			//故障保护后允许马达重启
			#if (Motor_Restart_Mode == Motor_Restart_Enable)
			{
				//马达重启时间
				Time.Motor_Restart = INT32_VALUE(&faultRestartDelayTime)*1000;
				//清除故障信息
				EPWM_ResetFaultBrake();				
				MC_Clear_FaultMessage();
				Count.Motor_Restart = 0;
				//系统恢复正常状态
				SYSTEM_STATE = SYS_RUN; 	
			}
			//故障保护后系统需要重新上电恢复
			#else
			{
				Time.Motor_Restart = 0;
				SYSTEM_STATE = SYS_WAIT; 
			}
			#endif
			//马达设置为故障状态
			MOTOR_STATE = MC_INIT;
			
			break;
		}
		//===================================================================//
		// 等待故障保护恢复
		case SYS_WAIT:
		{
			//系统关机
			System_Modules_off();	
			
			//允许调速信号复位重启
			#if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)			
			if((Capture.Frequency == 0) && (Capture.DutyCycle==0) && (Basic.VCtr <= userHostSet.value.vspStopQ15))
			{
				//清除故障信息				
				EPWM_ResetFaultBrake();				
				MC_Clear_FaultMessage();
				Count.Motor_Restart = 0;
				//系统恢复正常状态
				SYSTEM_STATE = SYS_RUN; 
			}
			#else	
				#if (CONFIG_SAFE_WORK_MODE == SAFE_TEST_MODE_1)
					EPWM_ResetFaultBrake();
					MC_Clear_FaultMessage();
					Count.Motor_Restart = 0;
					//系统恢复正常状态
					SYSTEM_STATE = SYS_RUN;
				#else
				if(Fault_ShortCircuit)
				{
					if((Capture.Frequency == 0) && (Capture.DutyCycle==0) && (Basic.VCtr <= VSP_REF_L))
					{
						//清除故障信息				
						EPWM_ResetFaultBrake();	
						MC_Clear_FaultMessage();
						Count.Motor_Restart = 0;
						//系统恢复正常状态
						SYSTEM_STATE = SYS_RUN; 
					}
				}
				else
				{
					MC_Clear_FaultMessage();
					Count.Motor_Restart = 0;
					//系统恢复正常状态
					SYSTEM_STATE = SYS_RUN;
				} 	
				#endif
			#endif
			break;
		}	
		//===================================================================//
		default:
			break;		
	}
	//=======================================================================//
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : MC_PhaseLoss_Check
* Description    : 缺相保护功能
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void MC_PhaseLoss_Check(void)
{
	static u8 v_PhaseLoss_Count = 0;	
	
	//-------------------------------------------------------------------------------------/
	if((MOTOR_STATE == MC_START) || (MOTOR_STATE == MC_SW) || (MOTOR_STATE == MC_RUN))
	{
		//减速时不进行缺相检测
		if((Flag.Speed_Ramp == 0) || (stru_FOC.Curr_Is_Ref > 0))
		{
			if(++v_PhaseLoss_Count > 100)
			{
				v_PhaseLoss_Count = 80;
				//-------------------------------------------------------------------------------------/
				// A相电流判断
				if(((Fault.Ia_max > Fault.Ib_max*2) || (Fault.Ia_max > Fault.Ic_max*2)) && (Fault.Ia_max > Fault.Phaseloss_Value))
				{
					Fault.PhaseA_LossCnt++;
				}
				else
				{
					Fault.PhaseA_LossCnt = 0;
				}
				//-------------------------------------------------------------------------------------/				
				// B相电流判断		
				if(((Fault.Ib_max > Fault.Ia_max*2) || (Fault.Ib_max > Fault.Ic_max*2)) && (Fault.Ib_max > Fault.Phaseloss_Value))
				{
					Fault.PhaseB_LossCnt++;
				}
				else
				{
					Fault.PhaseB_LossCnt = 0;
				}
				//-------------------------------------------------------------------------------------/
				// C相电流判断			
				if(((Fault.Ic_max > Fault.Ia_max*2) || (Fault.Ic_max > Fault.Ib_max*2)) && (Fault.Ic_max > Fault.Phaseloss_Value))
				{
					Fault.PhaseC_LossCnt++;
				}
				else
				{
					Fault.PhaseC_LossCnt = 0;
				}
				//-------------------------------------------------------------------------------------/
				Fault.Ia_max = 0;
				Fault.Ib_max = 0;
				Fault.Ic_max = 0;
				//-------------------------------------------------------------------------------------/		
				if((Fault.PhaseA_LossCnt > 100) || (Fault.PhaseB_LossCnt > 100) || (Fault.PhaseC_LossCnt > 100))
				{		
					v_PhaseLoss_Count 	 = 0;				
					Fault.PhaseA_LossCnt = 0;
					Fault.PhaseB_LossCnt = 0;
					Fault.PhaseC_LossCnt = 0;	
					Fault_PhaseLoss 		 = 1;
					SYSTEM_STATE 				 = SYS_FAULT; 
				}
				//-------------------------------------------------------------------------------------/	
			}
		}
	}	
	else
	{
		v_PhaseLoss_Count = 0;
	}
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : MC_MotorBlock_Check
* Description    : 堵转保护功能
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void MC_MotorBlock_Check(void)
{
	static u16  v_block_count = 0;
	
	//-------------------------------------------------------------------------/
	//无感堵转保护
	//转速超过设定的转速范围认为堵转保护
	if((MOTOR_STATE == MC_SW)||(MOTOR_STATE == MC_RUN))
	{	
		if( (Basic.Mech_Speed < INT32_VALUE(&faultSpeedMin)) || (Basic.Mech_Speed > INT32_VALUE(&faultSpeedMax)) )
		{
			v_block_count++;
		}
		else
		{
			v_block_count = 0;
		}		
		if(v_block_count > 10)
		{
			v_block_count = 0;
			Fault_Block = 1;
		}	
	}
	else
	{
		v_block_count = 0;
	}
	
	//-------------------------------------------------------------------------/
	//有感堵转判断
	if (INT8_VALUE(&startMode) == Start_Hall)
	{
    static u16  v_block_hall = 0;
        
    if((MOTOR_STATE == MC_SW)||(MOTOR_STATE == MC_RUN))
    {	
      if((stru_Hall.Speed < INT32_VALUE(&faultSpeedMin)) || (stru_Hall.Speed > INT32_VALUE(&faultSpeedMax)))
      {
        v_block_hall++;
      }
      else
      {
        v_block_hall = 0;
      }		
      if(v_block_hall > 10)//FAULT_BLOCK_TIME
      {
        v_block_hall = 0;
        Fault_Block = 1;
      }	
    }
    else
    {
      v_block_hall = 0;
    }
  }	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : MC_Voltage_Check
* Description    : 电压保护功能
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void MC_Voltage_Check(void)
{	
	static u16 Fault_Over_Count = 0,Fault_Lower_Count = 0,Fault_Recover_Count = 0;

	//过压与欠压保护 
	if(Basic.Vbus > Fault.OverVoltage_Value)
	{
		Fault_Over_Count++;
		if(Fault_Over_Count > 100)
		{
			Fault_OverVoltage = 1;
		}
		Fault_Lower_Count = 0;
		Fault_Recover_Count = 0;
	}
	else if(Basic.Vbus < Fault.LowerVoltage_Value)
	{
		Fault_Lower_Count++;
		if(Fault_Lower_Count > 100)
		{
			Fault_UnderVoltage = 1;	
		} 
		Fault_Over_Count = 0;
		Fault_Recover_Count = 0;        
	}
	//电压保护恢复
	else if((Basic.Vbus < Fault.OV_Recover_Value) && (Basic.Vbus > Fault.UV_Recover_Value)) 
	{			
		Fault_Recover_Count++;
		if(Fault_Recover_Count > 100)
		{
			Fault_OverVoltage = 0;
			Fault_UnderVoltage = 0;	
		}
		Fault_Over_Count  = 0;
		Fault_Lower_Count = 0;
	}
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : MC_MotorFault_Check
* Description    : 电机故障保护检测
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void MC_MotorFault_Check(void)
{	
	//------------------------------------------------------------------------/
  #if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
	// 缺相检测
    #if(FAULT_PHASELOSS_ENABLE)
    MC_PhaseLoss_Check();
    #endif
  #endif
	//------------------------------------------------------------------------/
	// 堵转检测
	#if(FAULT_BLOCK_ENABLE)	
  MC_MotorBlock_Check();
	#endif

	//------------------------------------------------------------------------/
  #if (CONFIG_SAFE_TEST_MODE == SAFE_TEST_DISABLE)
	// 电压检测
    #if(FAULT_VOLTAGE_ENABLE)	
    MC_Voltage_Check();
    #endif
  #endif
	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : MC_MotorSpeed_Calc
* Description    : 电机转速计算
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void  MC_MotorSpeed_Calc(void)
{
  static s32 speedbuf;
	//计算电机转速 
	Basic.Mech_Speed  = (stru_FOC.Elec_We*Basic.Speed_Base)/Basic.We;
  
  speedbuf = Basic.Mech_Speed + SPEED_OFFSET;
  Basic.FG_Speed   = DFILTER(speedbuf, Basic.FG_Speed ,1638);//Para.Lpf.Samp_Ibus
	//FG输出转速
//	Basic.FG_Speed 		= Basic.Mech_Speed + SPEED_OFFSET;

  
	//转速标定值计算，用于速度环
	Basic.Calb_Speed 	= FRACMPY_Q10(Basic.Mech_Speed,stru_Sample.G_Sp);

	//计算反电动势速度 
	//stru_bemf.Speed 	= (Basic.Freq_EPWM *60 )/(stru_bemf.We*MOTOR_PAIRS);	
	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : MC_Clear_FaultMessage
* Description    : 清除故障信息
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void MC_Clear_FaultMessage(void)
{
	Fault_Flag 					= 0;
	Fault_Block 				= 0;			
	Fault_PhaseLoss			= 0;		
	Fault_OverCurrent 	= 0;	
	Fault_StartFail 		= 0;			
	Fault_TorgueLoss 		= 0;	
	Fault.Block_Cnt 		= 0;
	Fault.Brake_Cnt		 	= 0;
	Fault.StartFail_Cnt = 0;
	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : ADC0_CALB
* Description    : ADC0通道采样
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void ADC0_Sample(void)
{		
     //母线电压采样
		stru_Sample.ADVal[CH_VBUS] = ADC->DATA[ADC_DATA_VBUS];
		
    //输入电流采样
		stru_Sample.ADVal[CH_IBUS] = ADC->DATA[ADC_DATA_IBUS];
		
		//调速电压采样
		if (INT8_VALUE(&speedControlMode) == VSP_Control)
			stru_Sample.ADVal[CH_VCTR] = ADC->DATA[ADC_DATA_CTRL];
		else
			stru_Sample.ADVal[CH_VCTR] = 0;
				
		//----------------------------------------------------------------------/	
		//计算调速电压
		stru_Sample.ARR[CH_VCTR] = FRACMPY_Q10((stru_Sample.ADVal[CH_VCTR] - stru_Sample.REF[CH_VCTR]),stru_Sample.G_Vc);
		//计算母线电压
		stru_Sample.ARR[CH_VBUS] = FRACMPY_Q10((stru_Sample.ADVal[CH_VBUS] - stru_Sample.REF[CH_VBUS]),stru_Sample.G_Vb);
		
		//母线电压数字滤波
		Basic.Vbus   = DFILTER(stru_Sample.ARR[CH_VBUS], Basic.Vbus, Para.Lpf.Samp_Ibus);

		//调速信号数字滤波
		Basic.VCtr   = DFILTER(stru_Sample.ARR[CH_VCTR], Basic.VCtr, Para.Lpf.Samp_Vctrl);	
		
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : SysTick_1ms
* Description    : 1ms定时任务
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
u8 	u8_Fault_ZF_Cnt		= 0;
u8 	u8_Fault_Brk_Cnt	= 0;
u8 bctrlcap,bctrlpwm=1;
//---------------------------------------------------------------------------/
void SysTick_1ms(void)
{
	TimeFlag_1ms = 0;
	
	//=======================================================================//
	// ADC0通道采样
	ADC0_Sample();	
	//=======================================================================//
	if (INT8_VALUE(&breakMode) == Brake_Enable)
	{
    if(PORT_GetBitValue(PORT0 ,PIN6) == 1)
    {
      if(++u8_Fault_Brk_Cnt > 30)
      {
        u8_Fault_Brk_Cnt = 30;
        Fault.Brake = 1;
      }		
    }
    else
    {
      if(u8_Fault_Brk_Cnt > 0)	u8_Fault_Brk_Cnt--;
      if(u8_Fault_Brk_Cnt == 0)
      {
        u8_Fault_Brk_Cnt = 0;
        Fault.Brake = 0;
      }
    }
  }

	//------------------------------------------------------------------------/
	// 正反转功能 
	else if (INT8_VALUE(&reverseMode) == Motor_ZF_Enable)	
	{
    if(PORT_GetBitValue(PORT0 ,PIN6) == 1)
    {
      if(++u8_Fault_ZF_Cnt > 30)
      {
        u8_Fault_ZF_Cnt = 30;
        User_Motor_FRControl(1-(u8)INT8_VALUE(&motorFR));
      }		
    }
    else
    {
      if(u8_Fault_ZF_Cnt > 0)	u8_Fault_ZF_Cnt--;
      if(u8_Fault_ZF_Cnt == 0)
      {
        u8_Fault_ZF_Cnt = 0;
        User_Motor_FRControl((u8)INT8_VALUE(&motorFR));
      }
    }
  }

	//=======================================================================//	
	// 软件计算电机功率 
	#if (LIMIT_POWER_MODE == LIMIT_SOFTWARE)
	
	MC_MotorPower_Calc();

	#endif
	
	//=======================================================================//	
	// 10ms 任务处理 
	if(++Count.TimeBase[0]>=10) //10ms
	{ 
		Count.TimeBase[0] = 0;	

		//-------------------------------------------------------------------//		
		// 调速捕获--PWM
		#if (Config_CCP_Capture == CCP_Capture_Enable)	

		User_Speed_Capture();   
		
		#endif	
		
		//-------------------------------------------------------------------//		
		// 速度反馈--FG
		#if (Motor_Debug_Mode == Motor_Debug_Offline )

		#if (Config_CCP_PWM == CCP_PWM_Enable)

		User_Speed_Out();		
			
		#endif	
		
		#endif
		
		//-------------------------------------------------------------------//		
		// 噪音补偿
		#if (DeadComp_Mode != Dead_Disable)
		
		User_Voice_Control();	
		
		#endif
				
	}
  
	//=======================================================================//	
	/* 100ms 处理 */	
	if(++Count.TimeBase[1]>=100) //100ms
	{
		Count.TimeBase[1] = 0;
		
		//睡眠处理	
		if (INT8_VALUE(&sleepMode) == Sleep_Enable)
		{
			User_Sleep_Manage();			
		}		
	}
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_Ramp_Control
* Description    : 
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_Ramp_Control(struct_SpeedRamp *Ramp, s32 Ref)
{
	Ramp->RampTarget = Ref;
	
	if(Ramp->RampTarget > Ramp->RampOut)
	{
		if((Ramp->RampOut + Ramp->RampInc) <= Ramp->RampTarget)
		{
			Ramp->RampOut += Ramp->RampInc;
		}
		else
		{
			Ramp->RampOut = Ramp->RampTarget;
		}
	}
	else if(Ramp->RampTarget < Ramp->RampOut)
	{
		if((Ramp->RampOut - Ramp->RampDec) >= Ramp->RampTarget)
		{
			Ramp->RampOut -= Ramp->RampDec;
		}
		else
		{
			Ramp->RampOut = Ramp->RampTarget;
		}
	}
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : MC_Speed_Control
* Description    : 速度环 / 功率环 功能
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
u16 	u16_Speed_PI_Ramp  = 0;
u16 	u16_Speed_PI_Ramp2 = 0;
u8 		u8_Speed_PI_Flag2  = 0;
void  MC_Speed_Control(void)
{
	static u8  SpeedLimit_Flag = 0;
	
  static u16 SpeedLimit_Count = 0;
  static s32 SpeedTargetBeforeLimit;	
	
	//=========================================================================//	
	// 限功率--硬件功率计算
	#if (LIMIT_POWER_MODE == LIMIT_HARDWARE)
	
	s32 Temp_Power=0;
	#if (Power_Cal_Mode == LPF_Inner)			//内部LPF  拟合公式为 PGAAVG = (ULPF0 - ULPF) /1.55 + Voffset  

	stru_Sample.ADPGACal = (stru_Sample.REF[CH_IBUS] - stru_Sample.ADVal[CH_IBUS]) / 1.55;		
	
	stru_Sample.ARR[CH_IBUS] = FRACMPY_Q10(stru_Sample.ADPGACal,stru_Sample.G_Ib);

	Basic.Ibus  = stru_Sample.ARR[CH_IBUS];
	if(Basic.Ibus < 0)  
  {
    Basic.Ibus = 0;
  }
	Temp_Power  = FRACMPY_Q15(Basic.Vbus,Basic.Ibus);
	Basic.Power = DFILTER(Temp_Power, Basic.Power, Para.Lpf.Samp_Ibus);	
	#else 										//外部RC滤波计算
	stru_Sample.ARR[CH_IBUS] = FRACMPY_Q10((stru_Sample.ADVal[CH_IBUS] - stru_Sample.REF[CH_IBUS]),stru_Sample.G_Ib);
	
	Basic.Ibus  = stru_Sample.ARR[CH_IBUS];
	if(Basic.Ibus < 0)  
  {
    Basic.Ibus = 0;
  }
	Temp_Power  = FRACMPY_Q15(Basic.Vbus,Basic.Ibus) * POWERFACTOR;
	Basic.Power = DFILTER(Temp_Power, Basic.Power, Para.Lpf.Samp_Ibus) ;	
	
	#endif	
	
	 // 软件功率计算 
	#else   
	
	Basic.Power = Basic.dq_power;
	
	#endif

	//=========================================================================//	
	if(MOTOR_STATE == MC_RUN)
	{
		//=======================================================================//
		Para.Ctrl.Md_Svpwm = Config_SVPWM_Mode;

		//=======================================================================//
		// 加减速分段PI 
		if (Config.Mode_Control == Speed_Loop)
		{
		//-----------------------------------------------------------------------/
      if( (stru_Speed_Ctrl.RampOut - stru_FOC.Speed_Ref) > ((150*stru_Sample.G_Sp)>>10) )
      {
        Flag.Speed_Ramp 	= 1;
        u16_Speed_PI_Ramp = 0;
          
      }
      else if(stru_FOC.Speed_Ref - stru_Speed_Ctrl.RampOut == 0)
      {
        if(++u16_Speed_PI_Ramp >= 10000)
        {
          u16_Speed_PI_Ramp = 10000;
          Flag.Speed_Ramp 	= 0;
        }		
      }
		}
		
		//=========================================================================//
		
		// 调速控制模式，恒转矩/恒转速/恒功率 
		if(++stru_FOC.W_Cycle >= stru_FOC.WRAMP)
		{
			stru_FOC.W_Cycle = 0;
			
			// 调速信号爬坡处理 stru_FOC.Speed_Ref
			FOC_Ramp_Control(&stru_Speed_Ctrl, stru_FOC.Speed_Ref);
	
			//=======================================================================//	
			// 力矩环控制 
			if (Config.Mode_Control == Current_Loop)
			{					
				// 限功率使能,若限功率和限转速同时使能，则执行限功率控制
				#if (LIMIT_POWER_ENABLE)
				
				// 非限制状态下判断当前功率是否超过限定功率
				if(!SpeedLimit_Flag)
				{
					if(Basic.Power > stru_FOC.PowerLimit)
					{
						// 防止频繁进入与退出限速模式
						if(++SpeedLimit_Count > 500/stru_FOC.WRAMP)
						{
							SpeedLimit_Flag = 1;									
							SpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut;		//保存当前档位
							
							PID_WL.Kp = 16384;		//功率环KP = _Q15(0.5), 避开浮点运算
							PID_WL.Ki = 1638;		//功率环KI = _Q15(0.05), 避开浮点运算
							PID_WL.KiSum = stru_Curr_dq_ref.Iq<<14;	
							PID_WL.Integral = stru_Curr_dq_ref.Iq;
						}
					}
				}
				//限制状态下判断是否需要取消限制
				else 
				{
					//档位下降时取消限制
					if(stru_Speed_Ctrl.RampOut < (SpeedTargetBeforeLimit - 100))
					{
						SpeedLimit_Flag = 0;
					}									
							
					if(!SpeedLimit_Flag)
					{
						PID_WL.Kp = Para.FOC.Wkp;		//恢复转速环PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}
				
				// 限速使能
				#elif (LIMIT_SPEED_ENABLE)
				
				// 非限速状态下判断当前转速是否超过限定转速	
				if(!SpeedLimit_Flag)
				{
					if(Basic.Mech_Speed > LIMIT_SPEED_VALUE)
					{
						// 防止频繁进入与退出限速模式
						if(++SpeedLimit_Count > 500/stru_FOC.WRAMP)
						{
							SpeedLimit_Flag = 1;									
							SpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut;		//保存当前档位
							
							PID_WL.Kp = 16384;		//速度环KP = _Q15(0.5), 避开浮点运算
							PID_WL.Ki = 1638;		//速度环KI = _Q15(0.05), 避开浮点运算
							PID_WL.KiSum = stru_Curr_dq_ref.Iq<<14;	
							PID_WL.Integral = stru_Curr_dq_ref.Iq;
						}
					}
				}
				//限速状态下判断是否需要取消限速
				else 
				{
					//档位下降时取消限速
					if(stru_Speed_Ctrl.RampOut < (SpeedTargetBeforeLimit - 100))
					{
						SpeedLimit_Flag = 0;
					}																
					if(!SpeedLimit_Flag)
					{
						PID_WL.Kp = Para.FOC.Wkp;		//恢复功率环PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}
				
				#endif //end limit power/speed enable
				
				if(SpeedLimit_Flag)
				{
					// 限功率使能
					#if (LIMIT_POWER_ENABLE)
					
					//恒功率时的PI周期
					stru_FOC.WRAMP = TIME_POWER_LOOP;						
					
					//限功率时恒功率控制
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_FOC.PowerLimit, Basic.Power);
					
					// 限速使能
					#elif (LIMIT_SPEED_ENABLE)
					
					//恒转速时的PI周期
					stru_FOC.WRAMP = TIME_SPEED_LOOP;						
					
					//限速时恒转速控制
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, Basic.Calb_SpeedLimit, Basic.Calb_Speed);
					
					#endif					
				}
				else
				{	
					//恒力矩时的PI周期
					stru_FOC.WRAMP = 1;			
					
					//开机状态恒力矩控制
					stru_FOC.Curr_Is_Ref = stru_Speed_Ctrl.RampOut;
	
				}
			}
			//=======================================================================//	
			// 转速环控制 
			else if (Config.Mode_Control == Speed_Loop)
			{
				// 限功率使能
				#if (LIMIT_POWER_ENABLE)
				
				// 非限制状态下判断当前功率是否超过限定功率
				if(!SpeedLimit_Flag)
				{
					if(Basic.Power > stru_FOC.PowerLimit)
					{
						// 防止频繁进入与退出限速模式
						if(++SpeedLimit_Count > 500/stru_FOC.WRAMP)
						{
							SpeedLimit_Flag = 1;									
							SpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut;		//保存当前档位
							
							PID_WL.Kp = 16384;		//功率环KP = _Q15(0.5), 避开浮点运算
							PID_WL.Ki = 1638;		//功率环KI = _Q15(0.05), 避开浮点运算
						}
					}
				}
				//限制状态下判断是否需要取消限制
				else 
				{
					//档位下降时取消限制
					if(stru_Speed_Ctrl.RampOut < (SpeedTargetBeforeLimit - 100))
					{
						SpeedLimit_Flag = 0;
					}									
					//当前转速值比限速前档位转速大时退出功率限制
//					if(Basic.Calb_Speed > SpeedTargetBeforeLimit)
//					{
//						SpeedLimit_Flag = 0;
//					}							
					if(!SpeedLimit_Flag)
					{
						PID_WL.Kp = Para.FOC.Wkp;		//恢复转速环PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}
				#endif //end limit power enable
				
				if(SpeedLimit_Flag)
				{
					//恒功率时的PI周期
					stru_FOC.WRAMP = TIME_POWER_LOOP;						
					
					//限功率时恒功率控制		
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_FOC.PowerLimit, Basic.Power);
				}
				else
				{	
					//恒转速时的PI周期
					stru_FOC.WRAMP =	TIME_SPEED_LOOP;			
					
					//开机状态恒转速控制
					stru_FOC.Curr_Is_Ref = PID_SpeedLoop(&PID_WL, stru_Speed_Ctrl.RampOut, Basic.Calb_Speed);	
						
				}
			}
			//===========================================================================//	
			// 功率环控制 
			else if (Config.Mode_Control == Power_Loop)
			{
				// 限速使能
				#if (LIMIT_SPEED_ENABLE)
				
				// 非限速状态下判断当前转速是否超过限定转速	
				if(!SpeedLimit_Flag)
				{
					if(Basic.Mech_Speed > LIMIT_SPEED_VALUE)
					{
						// 防止频繁进入与退出限速模式
						if(++SpeedLimit_Count > 500/stru_FOC.WRAMP)
						{
							SpeedLimit_Flag = 1;									
							SpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut;		//保存当前档位
							
							PID_WL.Kp = 16384;		//速度环KP = _Q15(0.5), 避开浮点运算
							PID_WL.Ki = 1638;		//速度环KI = _Q15(0.05), 避开浮点运算
						}
					}
				}
				//限速状态下判断是否需要取消限速
				else 
				{
					//档位下降时取消限速
					if(stru_Speed_Ctrl.RampOut < (SpeedTargetBeforeLimit - 100))
					{
						SpeedLimit_Flag = 0;
					}									
					//当前功率值比限速前档位功率大时退出限速
					if(Basic.Power > (SpeedTargetBeforeLimit + 100))
					{
						SpeedLimit_Flag = 0;
					}							
					if(!SpeedLimit_Flag)
					{
						PID_WL.Kp = Para.FOC.Wkp;		//恢复功率环PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}
				#endif //end limit speed enable
				
				if(SpeedLimit_Flag)
				{
					//恒转速时的PI周期
					stru_FOC.WRAMP = TIME_SPEED_LOOP;						
					
					//限速时恒转速控制
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, Basic.Calb_SpeedLimit, Basic.Calb_Speed);	
				}
				else
				{	
					//恒功率时的PI周期
					stru_FOC.WRAMP =	TIME_POWER_LOOP;			
					
					//开机状态恒功率控制
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_Speed_Ctrl.RampOut, Basic.Power);

				}																
			}
			//===========================================================================//
			// VQ电压环控制 
			else if (Config.Mode_Control == VQ_Loop)
			{
				//非限电流或转速状态下判断当前电流或转速是否超过限定电流或转速
				if(!SpeedLimit_Flag)
				{
					if((stru_Curr_dq.Iq > stru_FOC.CurrentLimit) ||(Basic.Mech_Speed > INT32_VALUE(&vqLoopLimitSpeedValue)))
					{
						//防止频繁进入与退出限电流或转速模式
						if(++SpeedLimit_Count > 50/stru_FOC.WRAMP)
						{
							SpeedLimit_Flag = 1;									
							
							PID_WL.Kp       = 16384;	//KP = _Q15(0.5), 避开浮点运算
							PID_WL.Ki       = 327;		//KI = _Q15(0.01), 避开浮点运算
							PID_WL.KiSum    = stru_Volt_dq.Uq << 14;
							PID_WL.Integral = stru_Volt_dq.Uq;  							
						}
					}
				}
				//限电流或转速状态下判断是否需要取消限电流或转速
				else 
				{
					//档位下降时取消限电流或转速
					if(stru_Speed_Ctrl.RampOut < (SpeedTargetBeforeLimit - 400))
					{
						SpeedLimit_Flag  = 0;
						SpeedLimit_Count = 0;
					}									
				}							
				if(SpeedLimit_Flag)
				{
					PID_WL.Lower_Integral = 0;
					PID_WL.Lower_Output   = PID_WL.Lower_Integral;
					
					stru_FOC.WRAMP        = 1;
					if(Basic.Mech_Speed < (INT32_VALUE(&vqLoopLimitSpeedValue) - 50))
					{
						//限IQ电流时恒电流控制
						stru_FOC.Volt_Uq_Ref = PID_CALC(&PID_WL, stru_FOC.CurrentLimit, stru_Curr_dq.Iq);
					}
					else if (Basic.Mech_Speed >= (INT32_VALUE(&vqLoopLimitSpeedValue) - 5))
					{
						//限速时恒转速控制
						stru_FOC.Volt_Uq_Ref = PID_CALC(&PID_WL, Basic.Calb_SpeedLimit, Basic.Calb_Speed);						
					}
					
					SpeedTargetBeforeLimit = stru_FOC.Volt_Uq_Ref;		//保存当前档位					
				}
				else
				{	
					//VQ模式下处理周期
					stru_FOC.WRAMP        = TIME_SPEED_LOOP;	
					PID_WL.Lower_Output   = userHostSet.value.vqRunMin;	//速度环输出下限
					PID_WL.Upper_Output   = userHostSet.value.vqRunMax;	//速度环输出上限
					PID_WL.Lower_Integral = PID_WL.Lower_Output;	//速度环积分下限
					PID_WL.Upper_Integral = PID_WL.Upper_Output;	//速度环积分下限 
					//VQ模式控制
					stru_FOC.Volt_Uq_Ref  = stru_Speed_Ctrl.RampOut;                    
				}	         
			}
			
			//=======================================================================//		
			// 限制IQ电流输出
			if(stru_FOC.Curr_Is_Ref > stru_FOC.Curr_Iq_Max)	stru_FOC.Curr_Is_Ref = stru_FOC.Curr_Iq_Max;
			if(stru_FOC.Curr_Is_Ref < stru_FOC.Curr_Iq_Min)	stru_FOC.Curr_Is_Ref = stru_FOC.Curr_Iq_Min;
		}
		//===========================================================================//	
	}
	//===============================================================================//	
	// 电机工作在非运行态
	else
	{
		SpeedLimit_Flag = 0;
		stru_FOC.W_Cycle = 0;
		PID_WL.Kp = Para.FOC.Wkp;	
		PID_WL.Ki = Para.FOC.Wki;
		if (Config.Mode_Control == Current_Loop)	
		stru_FOC.WRAMP = 1;
		else if (Config.Mode_Control == Speed_Loop)
		stru_FOC.WRAMP = TIME_SPEED_LOOP;
		else if (Config.Mode_Control == Power_Loop)
		stru_FOC.WRAMP = TIME_POWER_LOOP;
			
		#if ((LIMIT_POWER_ENABLE) || (LIMIT_SPEED_ENABLE))
		SpeedLimit_Count = 0;
		#endif
		
		Flag.Speed_Ramp 	= 0;
		
	}
	//===============================================================================//	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : MC_BaseValue_Init
* Description    : 
* Function Call  : 初始化调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void MC_BaseValue_Init(void)
{
	//------------------------------------------------------------------------/
	stru_base_value.Us 		= FLOAT_VALUE(&busVoltage);
	stru_base_value.Rs 		= FLOAT_VALUE(&motorRS);
	stru_base_value.Ls 		= FLOAT_VALUE(&motorLS);
	stru_base_value.Ts 		= 1.0/EPWM_FREQ;
	stru_base_value.Poles	= Para.Base.Paris;
		
	//------------------------------------------------------------------------/	
	stru_base_value.Ub 		= stru_base_value.Us/1.732;
	stru_base_value.Ib 		= (float)((HW_ADC_REF-HW_AMP_REF_IP)/(HW_AMP_GAIN_IP*FLOAT_VALUE(&phaseResistance)));	
	stru_base_value.Fb 		= (float)((stru_base_value.Poles*INT32_VALUE(&speedBaseValue))/60);
	stru_base_value.Wb		= _2PI() * stru_base_value.Fb;

}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : MC_MotorInit_Process
* Description    : 
* Function Call  : 1ms进程调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void MC_MotorInit_Process(void)
{
	// 初始化
	if(MOTOR_STATE == MC_INIT)
	{
			FOC_MotorSpeed_Filter(stru_FOC.Elec_We);
	}
  
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : UartView
* Description    : 串口监测数据--调试用
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
u16 g_UartSum_u16 = 0;
u8  g_UartScopeArr[10] = {0};
void UartView(s16 view1,s16 view2,s16 view3,s16 view4)
{
	static u8  l_UartScopeCnt_u8 = 0;
	
	if(l_UartScopeCnt_u8 < 8)
    g_UartSum_u16 += g_UartScopeArr[l_UartScopeCnt_u8];	
	else if(l_UartScopeCnt_u8 == 8)
    g_UartScopeArr[8] = (u8)g_UartSum_u16;
  
	UART0->THR = g_UartScopeArr[l_UartScopeCnt_u8];  
		
	if(++l_UartScopeCnt_u8 >= 9)
	{					
		l_UartScopeCnt_u8 = 0;	
		g_UartSum_u16 = 0;
		g_UartScopeArr[0] = (u8)(view1);  g_UartScopeArr[1] = (u8)(view1>>8);	
		g_UartScopeArr[2] = (u8)(view2);  g_UartScopeArr[3] = (u8)(view2>>8);	
		g_UartScopeArr[4] = (u8)(view3);  g_UartScopeArr[5] = (u8)(view3>>8);	
		g_UartScopeArr[6] = (u8)(view4);  g_UartScopeArr[7] = (u8)(view4>>8);	 	
	}	
 
}

//===========================================================================//
//*********END   ************************************************************//
//===========================================================================//
