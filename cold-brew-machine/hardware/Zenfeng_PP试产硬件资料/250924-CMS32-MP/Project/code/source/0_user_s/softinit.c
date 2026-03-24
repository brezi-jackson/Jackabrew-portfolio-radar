//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    softinit.c
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

//---------------------------------------------------------------------------/
//	Global variable definitions(declared in header file with 'extern')
//---------------------------------------------------------------------------/
ByteFlag Time_Flag;

//---------------------------------------------------------------------------/
//	Local function prototypes('static')
//---------------------------------------------------------------------------/

/*****************************************************************************
 ** \brief	MC_SampPara_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_SampPara_Init(void)
{
	stru_Sample.G_Ip 	= _Q10(SW_GAIN_IPHASE);
	stru_Sample.G_Vb 	= _Q10(userHostSet.value.swGainVbus);
	stru_Sample.G_Ib 	= _Q10(userHostSet.value.swGainIbus);	
	stru_Sample.G_Sp 	= _Q10(userHostSet.value.gainSpeedBase);
	stru_Sample.G_Vc 	= _Q10(SW_GAIN_VCTRL);
	stru_Sample.G_Pw 	= FRACMPY_Q15(userHostSet.value.calcCoeffVbus,userHostSet.value.calcCoeffIbus);
	stru_Sample.IP_CH = (u8)IP_SAMP_CH;
  
}

/*****************************************************************************
 ** \brief	MC_FaultPara_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_FaultPara_Init(void)
{
  Fault.OverVoltage_Value  = _Q12((FLOAT_VALUE(&faultOverVoltage)*HW_VBUS_SDR*userHostSet.value.swGainVbus)/HW_ADC_REF);
	Fault.LowerVoltage_Value = _Q12((FLOAT_VALUE(&faultUnderVoltage)*HW_VBUS_SDR*userHostSet.value.swGainVbus)/HW_ADC_REF);
	Fault.OV_Recover_Value   = _Q12((FLOAT_VALUE(&faultOverVoltageRecover)*HW_VBUS_SDR*userHostSet.value.swGainVbus)/HW_ADC_REF);
	Fault.UV_Recover_Value   = _Q12((FLOAT_VALUE(&faultUnderVoltageRecover)*HW_VBUS_SDR*userHostSet.value.swGainVbus)/HW_ADC_REF);

	Fault.Phaseloss_Value    = FAULT_PHASELOSS_CURR;	
	Fault.OverCurrent_Value  = _Q15(userHostSet.value.iValue*FLOAT_VALUE(&faultOverCurrent));
	Fault.We_Rate						 = BLOCK_WE_RATE;
  
}

/*****************************************************************************
 ** \brief	MC_CtrlPara_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_CtrlPara_Init(void)
{
	Para.Ctrl.Md_Shunt 		= Config_Shunt_Mode;
	Para.Ctrl.Md_Control 	= INT8_VALUE(&controlMode);
	Para.Ctrl.Md_Svpwm 		= Config_SVPWM_Mode;
	Para.Ctrl.Md_Wind 		= Config_Wind_Mode;
	Para.Ctrl.MotorDir 		= (u8)INT8_VALUE(&motorFR);
	Para.Ctrl.Md_IPD 			= Config_IPD_Mode;
	Para.Ctrl.Md_Debug 		= Config_Debug_Mode;
	Para.Ctrl.Md_DeadComp = DeadComp_Mode;
	//安规测试模式 2022-06-28
	Para.Ctrl.Md_SafeTest = CONFIG_SAFE_TEST_MODE;
	
	Config.Mode_Control		=	INT8_VALUE(&controlMode);
	Config.Mode_IPD				= Config_IPD_Mode;
	Config.Mode_Shunt			= Config_Shunt_Mode;
	Config.Mode_Svpwm			= Config_SVPWM_Mode;
	Config.Mode_Wind			= Config_Wind_Mode;
	Config.MotorDir				= INT8_VALUE(&motorFR);
	Config.Mode_Weeken		= Config_Weak_Mode;	
	Config.Mode_Start			= INT8_VALUE(&startMode);
	Config.Mode_DeadComp	=	DeadComp_Mode;
	Config.Mode_Block			= BLOCK_WE_MODE;
	
}

/*****************************************************************************
 ** \brief	MC_BasePara_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_BasePara_Init(void)
{
	Para.Base.Ls 				= _Q15(FLOAT_VALUE(&motorLS)*1000);
	Para.Base.Rs 				= _Q15(FLOAT_VALUE(&motorRS));
	Para.Base.Paris	 		= (s32)INT8_VALUE(&motorPairs);	
	
	Basic.Speed_Close 	= (s32)MOTOR_SPEED_CLOSE;
	Basic.Speed_Stop 		= (s32)MOTOR_SPEED_STOP;
	Basic.Speed_Base 		= (s32)INT32_VALUE(&speedBaseValue);
	Basic.Speed_Back		= (s32)MOTOR_SPEED_BACK;
	Basic.Speed_Switch	=	(s32)MOTOR_SPEED_SWITCH;
	Basic.Speed_Brake		= (s32)INT32_VALUE(&breakSpeed);	
	Basic.Freq_EPWM 		= EPWM_FREQ;
	Basic.Tpwm_us 			= (2*1000000)/Basic.Freq_EPWM;
	Basic.MotorVolt 		= FLOAT_VALUE(&busVoltage)/1.732;
	Basic.MotorCurr 		= (float)((HW_ADC_REF-HW_AMP_REF_IP)/(HW_AMP_GAIN_IP*FLOAT_VALUE(&phaseResistance)));	
	Basic.Ke 						= (float)MOTOR_Ke;
	Basic.We_Ke 				= (s32)(MOTOR_Ke_K*MOTOR_WE_BASE);
	Basic.We 						= (INT32_VALUE(&speedBaseValue)*32767)/((EPWM_FREQ*60)/(2*Para.Base.Paris));	
	Basic.Calb_Vbus 		= (s32)(FLOAT_VALUE(&busVoltage)*userHostSet.value.calcCoeffVbus);
	Basic.Vbus          = Basic.Calb_Vbus;
	//VQ模式，限转速 2022-07-11
	if (VQ_Loop != Config.Mode_Control)
	{
    Basic.Calb_SpeedLimit = FRACMPY_Q10(LIMIT_SPEED_VALUE,stru_Sample.G_Sp);
  }
	else
  {
    Basic.Calb_SpeedLimit = FRACMPY_Q10(INT32_VALUE(&vqLoopLimitSpeedValue),stru_Sample.G_Sp);	
  }
	Basic.Calb_Speed_Min  = FRACMPY_Q10(FOC_RUN_SPEED_MIN,stru_Sample.G_Sp);	
	Basic.Calb_Speed_Max  = FRACMPY_Q10(FOC_RUN_SPEED_MAX,stru_Sample.G_Sp);
		
}

/*****************************************************************************
 ** \brief	MC_StartPara_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_RevWind_Init(void)
{	
	Para.Wind.Dout_max 			= _Q15(0.5);
	Para.Wind.Dout_min 			= _Q15(-0.5);
	Para.Wind.Qout_max 			= _Q15(0.8);
	Para.Wind.Qout_min 			= _Q15(0.0);	
	Para.Wind.Ekp 					= WIND_EKP;
	Para.Wind.Eki 					= WIND_EKI;
	Para.Wind.Eout_max 			= WIND_EOUT_MAX;
	Para.Wind.Eout_min 			= WIND_EOUT_MIN;
	Para.Wind.Start_Current = WIND_START_CURRENT;
  
}

/*****************************************************************************
 ** \brief	MC_FOCPara_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_FOCPara_Init(void)
{	
	Para.Start.run_current_init 	= _Q15(FLOAT_VALUE(&startCurrentInit));
	Para.Start.run_current_final	= _Q15(FLOAT_VALUE(&startCurrentFinal));
	Para.Start.Dkp 								= START_DKP;
	Para.Start.Dki 								= START_DKI;
	Para.Start.Dout_max 					= START_DOUT_MAX;
	Para.Start.Dout_min 					= START_DOUT_MIN;	
	Para.Start.Qkp 								= START_QKP;
	Para.Start.Qki 								= START_QKI;
	Para.Start.Qout_max 					= START_QOUT_MAX;
	Para.Start.Qout_min 					= START_QOUT_MIN;	
	Para.Start.Ekp								= (s32)INT32_VALUE(&satrtEKP)*10;
	Para.Start.Eki								= (s32)INT32_VALUE(&satrtEKI);
	Para.Start.Eout_min						=	(s32)INT32_VALUE(&satrtEOutMin);
	Para.Start.Hold_Time					= (u32)((TIME_START_HOLD*EPWM_FREQ)/1000);
	
	// FOC PARA
	Para.FOC.Dkp 									= FOC_DKp;
	Para.FOC.Dki 									= FOC_DKI;
	Para.FOC.Dout_max 						= FOC_DOUT_MAX;
	Para.FOC.Dout_min 						= FOC_DOUT_MIN;	
	Para.FOC.Qkp 									= FOC_QKP;
	Para.FOC.Qki 									= FOC_QKI;
	Para.FOC.Qout_max 						= FOC_QOUT_MAX;
	Para.FOC.Qout_min 						= FOC_QOUT_MIN;	
	Para.FOC.Wkp 									= _Q15(FLOAT_VALUE(&focWKP));
	Para.FOC.Wki 									= _Q15(FLOAT_VALUE(&focWKI));
	Para.FOC.Wkd 									= _Q15(FLOAT_VALUE(&focWKD));	
	Para.FOC.Wout_max 						= _Q15(userHostSet.value.iValue*FLOAT_VALUE(&focRunCurrentMax));;
	Para.FOC.Wout_min 						= _Q15(userHostSet.value.iValue*FLOAT_VALUE(&focRunCurrentMin));
	Para.FOC.Ekp									= FOC_EKP;
	Para.FOC.Eki									= FOC_EKI;
	Para.FOC.Fc_LPF								= FOC_FC_LPF;	
	Para.FOC.OffsetTheta					= _Q15((float)(INT32_VALUE(&focOffsetAngle))/360.0)*2;
	
	stru_FOC.SRAMP 								= (u16)START_SRAMP;	
	stru_FOC.IRAMP 								= (u16)INT32_VALUE(&startIRamp);
	stru_FOC.FRAMP 								= (u16)FOC_FRAMP;
	stru_FOC.Vs_Max 							= _Q15(FLOAT_VALUE(&vectorVoltageMax));
	stru_FOC.Vs_Square_Max 				= _Q15(FLOAT_VALUE(&vectorVoltageMax)*FLOAT_VALUE(&vectorVoltageMax));
	stru_FOC.EPWM_Period 					= (u16)EPWM_PERIOD;
	stru_FOC.EPWM_SAMPDLY					= (u16)EPWM_TGDLY;
	stru_FOC.TSAM 								= (u16)(((EPWM_TGDLY + EPWM_Tus*0.5)*32767)/EPWM_PERIOD);
  stru_FOC.DeadTime             = (uint32_t)((0x3ff & (u16)(EPWM_DT)) | ((0x3ff & (u16)(EPWM_DT)) << 10) | ((0x3ff & (u16)(EPWM_DT)) << 20) );
  stru_FOC.VoiceDownDeadTime    = (uint32_t)((0x3ff & (u16)(Voice_Down_DT)) | ((0x3ff & (u16)(Voice_Down_DT)) << 10) | ((0x3ff & (u16)(Voice_Down_DT)) << 20) );


	stru_FOC.Weakening 						= (s32)(WEAKENING_RANGE*stru_FOC.Vs_Max);
	stru_FOC.Curr_Iq_Max 					= _Q15(userHostSet.value.iValue*FLOAT_VALUE(&focRunCurrentMax));
	stru_FOC.Curr_Iq_Min 					= _Q15(userHostSet.value.iValue*FLOAT_VALUE(&focRunCurrentMin));
	stru_FOC.Speed_Ref_Min				= ((250*stru_Sample.G_Sp)>>10);
	
	#if (LIMIT_POWER_MODE == LIMIT_HARDWARE)
	stru_FOC.PowerLimit = Calc_Motor_Power(LIMIT_POWER_VALUE);
	#else
	stru_FOC.PowerLimit = LIMIT_SOFT_POWER;
	#endif
	
	//VQ模式电流限制 2022-07-11	
	if (Config.Mode_Control == VQ_Loop)
  {
    stru_FOC.CurrentLimit = _Q15(userHostSet.value.iValue*(FLOAT_VALUE(&vqLoopLimitCurrentValue)));	
	}
	//安规测试模式 2022-06-29
	stru_FOC.VoltageDutySafe      = (s32)SAFE_MODE_VOLTAGE_DUTY;

}

/*****************************************************************************
 ** \brief	MC_LPFPara_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_LPFPara_Init(void)
{
	// Lpf PARA
	Para.Lpf.Samp_Ibus 	= LPF_SAMP_IBUS;
	Para.Lpf.Samp_Vbus 	= LPF_SAMP_VBUS;
	Para.Lpf.Samp_Vctrl = LPF_SAMP_VCTRL;
	Para.Lpf.Samp_Vtem 	= LPF_SAMP_TEMP;
	Para.Lpf.Speed_Val 	= LPF_SPEED_VAL;
	Para.Lpf.Power_Val 	= LPF_POWER_VAL;	
  
}

/*****************************************************************************
 ** \brief	SpeedRampInit
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_SpeedRamp_Init(void)
{
	stru_Speed_Ctrl.RampInc 		= (s32)INT32_VALUE(&speedRampInc);
	stru_Speed_Ctrl.RampDec 		= (s32)INT32_VALUE(&speedRampDec);
	stru_Speed_Ctrl.RampOut 		= 0;
	stru_Speed_Ctrl.RampTarget 	= 0;
  
}

/*****************************************************************************
 ** \brief	MC_OpenLoop_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_OpenLoop_Init(void)
{	
	stru_OpenLoop.RevUp_T1 	= START_REVUP_TIME1;
	stru_OpenLoop.RevUp_T2 	= START_REVUP_TIME2;
	stru_OpenLoop.RevUp_T3 	= START_REVUP_TIME3;	
	stru_OpenLoop.RevUp_I1 	= START_REVUP_CURRENT1;
	stru_OpenLoop.RevUp_I2 	= START_REVUP_CURRENT2;
	stru_OpenLoop.RevUp_I3 	= START_REVUP_CURRENT3;
	stru_OpenLoop.We_K1 		= START_REVUP_WE_K1;
	stru_OpenLoop.We_K2 		= START_REVUP_WE_K2;
	stru_OpenLoop.We_K3 		= START_REVUP_WE_K3;
	stru_OpenLoop.We_Min   	= START_REVUP_WEMIN;
	stru_OpenLoop.We_Max   	= START_REVUP_WEMAX;
}

/*****************************************************************************
 ** \brief	MC_Para_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_Para_Init(void)
{
  MC_HEX_Host_Init();
    
	MC_CtrlPara_Init();
	
	MC_SampPara_Init();
	
	MC_FaultPara_Init();

	MC_BasePara_Init();

	MC_RevWind_Init();	
	
	MC_FOCPara_Init();
	
	MC_LPFPara_Init();	
	
	MC_SpeedRamp_Init();
	
	MC_OpenLoop_Init();
	
	MC_BaseValue_Init();	
	
	Time.Motor_Stophold 	= (u32)((TIME_STOP_HOLD * EPWM_FREQ) / 1000);
	Time.Motor_StartFail 	= (u32)(FAULT_START_FAIL_TIME * EPWM_FREQ);
	Time.Motor_Brake 			= (u32)((INT32_VALUE(&breakTime) * EPWM_FREQ) / 1000);
	Time.Motor_Block 			= (u32)((INT32_VALUE(&faultBlockTime) * EPWM_FREQ) / 1000);

	stru_Pulse.InjectTime = IPD_TIME;	
	stru_Pulse.InjectCurrent_Base = IPD_CURRENT;
	
}

/*****************************************************************************
 ** \brief	__EI_Set
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void __EI_Set(void)
{
	/* Enable EPWM_INT */	
	#if (Config_Shunt_Mode == Single_Shunt)
	{
		EPWM->ICLR |= 0xFFFFFFFF;
		EPWM_EnableZeroInt(EPWM_CH_0_MSK);	
		NVIC_EnableIRQ(EPWM_IRQn);
		NVIC_SetPriority(EPWM_IRQn,1);	
	}
	#endif	

	/* Enable ADC1_INT */	
	#if (Config_Shunt_Mode == Double_Shunt)
	{
		ADC_EnableChannelInt(ADC_SCAN_CHA);
		NVIC_EnableIRQ(ADC_IRQn);
		NVIC_SetPriority(ADC_IRQn,1);
	}
	#endif
	
	/* Enable TMR0_INT */
	if (INT8_VALUE(&startMode) == Start_Hall)
	{	
		TMR_EnableOverflowInt(TMR0);
		TMR_ClearOverflowIntFlag(TMR0);
		NVIC_EnableIRQ(TIMER0_IRQn);	
		NVIC_SetPriority(TIMER0_IRQn,2);
	}
	
	/* Enable UART1_INT */
	#if (Config_Comm_Mode == Comm_UI)	
	{
		//UART_EnableIRQ(UART_RBRIE_EN_Msk);  
		//NVIC_EnableIRQ(UART0_IRQn); 
		//NVIC_SetPriority(UART0_IRQn,2);
	}
	#endif
	
	/* Enable SysTick_INT */
	SysTick->CTRL  = 0x00000007;	
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn,3);
	
	/* Enable ACMP_INT */
	NVIC_EnableIRQ(ACMP_IRQn);
	NVIC_SetPriority(ACMP_IRQn,0);
	
	ACMP->LOCK = ACMP_WRITE_KEY;
	// 设置ACMP1 中断使能位  0:禁止  1:使能
	ACMP->IMSC &= ~(0x02ul);
	ACMP->IMSC |= (0x01ul<<1);
	// 设置ACMP1 中断清零控制位  	写0: 不影响 写1: 清零RIS_C1IF标志位
	ACMP->ICLR &= ~(0x02ul);
	ACMP->ICLR |= (0x01ul<<1);
	
	ACMP->LOCK = 0x00;
	
	#if (Config_CCP_Capture == CCP_Capture_Enable)	
	//CCP_EnableCAPMode0Int(CCP_CAPTURE_MODULE);		/*开启CCP模块捕获模式0中断*/	
	//NVIC_EnableIRQ(CCP_IRQn);		
	//NVIC_SetPriority(CCP_IRQn,0);					/*优先级0~3， 0最高、3最低*/			
	#endif	

}

/*****************************************************************************
 ** \brief	__DI_Set
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void __DI_Set(void)
{
	NVIC_DisableIRQ(EPWM_IRQn);
	NVIC_DisableIRQ(ADC_IRQn);	
	NVIC_DisableIRQ(SysTick_IRQn);	
	NVIC_DisableIRQ(UART0_IRQn);	
	NVIC_DisableIRQ(UART0_IRQn);
	NVIC_DisableIRQ(TIMER0_IRQn);	
	NVIC_DisableIRQ(TIMER1_IRQn);
	NVIC_DisableIRQ(LVI_IRQn);		
	NVIC_DisableIRQ(ACMP_IRQn);
	NVIC_DisableIRQ(CCP_IRQn);		
	NVIC_DisableIRQ(INTP0_IRQn);
	NVIC_DisableIRQ(INTP1_IRQn);
	NVIC_DisableIRQ(INTP2_IRQn);
	NVIC_DisableIRQ(INTP3_IRQn);
	
}

/*****************************************************************************
 ** \brief	
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void f_DelayTime(u32 delay)
{
   for(; delay > 0; delay--) 
	{
     __nop();
  }
}

/*****************************************************************************
 ** \brief	
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void DelayTime_ms(u32 delay)
{
	u32 i,j;
	
	for(i=0; i<delay; i++)
	{
		for(j=0; j<10000; j++) //1ms,64M
		{
			;
		}
	}
}

/*****************************************************************************
 ** \brief	ADC_IPA_SAMP
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_IPA_SAMP(void)
{

	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	
	ADC->SCAN = 0;
	
	ADC->SCAN |= ADC_SCAN_CHA;
	
	ADC->CON |= ADC_CON_ADCEN_Msk;
	
	ADC->LOCK = 0x00;	
	
	NVIC_DisableIRQ(ADC_IRQn);
  
}

/*****************************************************************************
 ** \brief	ADC_IPB_SAMP
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_IPB_SAMP(void)
{

	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	
	ADC->SCAN = 0;
	
	ADC->SCAN |= ADC_SCAN_CHB;
	
	ADC->CON |= ADC_CON_ADCEN_Msk;
	
	ADC->LOCK = 0x00;	
	
	NVIC_DisableIRQ(ADC_IRQn);
  
}

/*****************************************************************************
 ** \brief	void ADC_ClearAllInt_Flag(void)
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_ClearAllInt_Flag(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->ICLR |= 0xFFFFFFFF;
	ADC->LOCK = 0x00;
  
}

/*****************************************************************************
 ** \brief	ADC_TGSAMP_CONFIG
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_TGSAMP_CONFIG(void)
{
	/* 双电阻ADC1触发采样模式配置 */
	if (Para.Ctrl.Md_Shunt == Double_Shunt)
	{
		ADC_DisableEPWMCmp0TriggerChannel(0x0FFFFFF);
		ADC_DisableEPWMCmp1TriggerChannel(0x0FFFFFF);
		ADC_EnableEPWMCmp0TriggerChannel(ADC_SCAN_CHA | ADC_SCAN_CHB);
		ADC_EnableEPWMCmp1TriggerChannel(ADC_SCAN_VBUS);

		ADC_EnableHardwareTrigger(ADC_TG_EPWM_CMP0);
		ADC_EnableHardwareTrigger(ADC_TG_EPWM_CMP1);	
	}
	/* 单电阻ADC1触发采样模式配置 */
	else
	{	
		/* 计数比较器0/1触发转换通道配置 */
		ADC_DisableEPWMCmp0TriggerChannel(0x0FFFFFF);
		ADC_DisableEPWMCmp1TriggerChannel(0x0FFFFFF);
		ADC_EnableEPWMCmp0TriggerChannel(ADC_SCAN_CHA);
		ADC_EnableEPWMCmp1TriggerChannel(ADC_SCAN_CHB);
		ADC_EnableHardwareTrigger(ADC_TG_EPWM_CMP0);
		ADC_EnableHardwareTrigger(ADC_TG_EPWM_CMP1);
		
		//周期点触发母线电压采样
		ADC_DisableEPWMTriggerChannel(0x0FFFFFF);
		ADC_EnableEPWMTriggerChannel(ADC_SCAN_VBUS|ADC_SCAN_IBUS);
		ADC_EnableHardwareTrigger(ADC_TG_EPWM0_PERIOD);				
		EPWM_DisableIntAccompanyWithLoad();	
		
	}
}

/*****************************************************************************
 ** \brief	ADC_TGSAMP_VBUS_CONFIG
 **			
 ** \param [in] none
 ** \return  none
 ** \note	6526芯片内部母线采样通道特殊处理，母线电压通道要单独采样，采用轮流触发采样
*****************************************************************************/
void ADC_TGSAMP_VBUS_CONFIG(void)
{
	static uint8_t bSampleSwitchFlag = 0;
	#if (Config_Shunt_Mode == Double_Shunt)
	if(!bSampleSwitchFlag)
	{
		ADC_DisableEPWMCmp1TriggerChannel(0x0FFFFFF);
		ADC_EnableEPWMCmp1TriggerChannel(ADC_SCAN_VBUS);
		bSampleSwitchFlag = 1;
	}
	else
	{
		ADC_DisableEPWMCmp1TriggerChannel(0x0FFFFFF);
		if (INT8_VALUE(&speedControlMode) == VSP_Control)
		{
			ADC_EnableEPWMCmp1TriggerChannel(ADC_SCAN_CTRL | ADC_SCAN_HALL);//
		}
		else
		{
			ADC_EnableEPWMCmp1TriggerChannel(ADC_SCAN_HALL);//			
		}
		bSampleSwitchFlag = 0;
	}
	#else
	if(!bSampleSwitchFlag)
	{
		ADC_DisableEPWMTriggerChannel(0x0FFFFFF);
		ADC_EnableEPWMTriggerChannel(ADC_SCAN_VBUS);
		bSampleSwitchFlag = 1;
	}
	else
	{
		ADC_DisableEPWMTriggerChannel(0x0FFFFFF);
		ADC_EnableEPWMTriggerChannel(ADC_SCAN_IBUS);
		bSampleSwitchFlag = 0;
	}	
	#endif
}

/*****************************************************************************
 ** \brief	ADC_IBUS_SAMP
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_IBUS_SAMP(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	
	ADC->SCAN = 0;
	
	ADC->SCAN |= ADC_SCAN_IBUS;
	
	ADC->CON |= ADC_CON_ADCEN_Msk;
	
	ADC->LOCK = 0x00;	
	
	NVIC_DisableIRQ(ADC_IRQn);
	
}

/*****************************************************************************
 ** \brief	母线电流采样，单电阻恒功率模式
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_IBUS_SAMP(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	
	ADC->SCAN = ADC_SCAN_IBUS;
	
	ADC->CON |= ADC_CON_ADCEN_Msk;
	
	if(!ADC_IS_BUSY()) 
	{
		ADC_Go();
    
	}	
}

/*****************************************************************************
 ** \brief	GET_ADCREF_VAL
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void GET_ADCREF_VAL(void)
{
	u8 ADC_CH_NUM= 0,i,k;

	u32 l_Ibus_Val[10] = {0};
	u32 l_DataMax, l_DataMin, l_DataSum;
	
	for(ADC_CH_NUM=0; ADC_CH_NUM<3; ADC_CH_NUM++)
	{
		if(ADC_CH_NUM==0)
			ADC_IPA_SAMP();
		if(ADC_CH_NUM==1)
			ADC_IPB_SAMP();
		if(ADC_CH_NUM==2)
			ADC_IBUS_SAMP();
				
		for(i=0; i<10; i++)
		{
			if(!ADC_IS_BUSY()) 
			{
				ADC_Go();

				while(ADC_IS_BUSY());
				
				if(ADC_CH_NUM==0)	l_Ibus_Val[i] = ADC_GetResult(ADC_DATA_CHA);
				if(ADC_CH_NUM==1)	l_Ibus_Val[i] = ADC_GetResult(ADC_DATA_CHB);
				if(ADC_CH_NUM==2)	l_Ibus_Val[i] = ADC_GetResult(ADC_DATA_IBUS);
				
				DelayTime_ms(10);	//10MS
			}
		}		
		if(i >= 10)
		{
			l_DataMax = l_Ibus_Val[0];
			l_DataMin = l_Ibus_Val[0];
			l_DataSum = 0;
			for(k=0; k<10; k++)
			{
				if(l_Ibus_Val[k] > l_DataMax)	
					l_DataMax = l_Ibus_Val[k];
				if(l_Ibus_Val[k] < l_DataMin)	
					l_DataMin = l_Ibus_Val[k];
				
				l_DataSum += l_Ibus_Val[k];
			}	
	
			stru_Sample.REF[ADC_CH_NUM] = (l_DataSum - l_DataMax - l_DataMin)>>3;
		}
	}	
	
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->SCAN = 0;
	ADC->LOCK = 0x00;
	ADC_TGSAMP_CONFIG();	
	
}

/******************************** END OF FILE *******************************/

