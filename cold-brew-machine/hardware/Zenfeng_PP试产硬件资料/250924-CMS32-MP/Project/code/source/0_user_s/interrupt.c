//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    interrupt.c
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

//---------------------------------------------------------------------------/
//	Local function prototypes('static')
//---------------------------------------------------------------------------/
void FOC_User_Control(void);
				
/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : EPWM_IRQHandler
* Description    : R1:30us/64M(2~3us_week)(2us_comm)
* Function Call  : EPWM_IRQHandler中断函数
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void EPWM_IRQHandler(void)
{
	//零点中断任务
	if(EPWM->MIS & EPWM_ZIFn_Flag)
	{
		//清除标志
		EPWM_ClearZIFn_Flag();
			
		//电流读取
		FOC_Read_Current();
		
		//FOC控制流程
		FOC_Control_Process();
		
		//用户进程
		FOC_User_Control();
		
	}
	//清所有中断标志
	else
	{
		EPWM_ClearAllInt_Flag();
    
	}
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : ADC_IRQHandler
* Description    : 
* Function Call  : ADC_IRQHandler中断函数
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
//uint32_t Temp1;
//uint32_t Temp2;
//uint32_t Pump_Current;

//void ADC_IRQHandler(void)
//{
//	if(ADC->MIS & Temp1_ADC_CH_MSK)
//	{		
//		//清除中断标志
//		ADC_ClearIntFlag_CH();
//		
//		//电流读取
//		Pump_Current = ADC_GetResult(Pump_ADC_DATA);
//		Temp1 = ADC_GetResult(Temp1_ADC_DATA);
//		Temp2 = ADC_GetResult(Temp2_ADC_DATA);
//	}
//	//清所有中断标志
//	else
//	{
//		ADC_ClearAllInt_Flag();
//  }				
//0
/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : ACMP_IRQHandler
* Description    :
* Function Call  : 故障刹车比较器中断
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void ACMP_IRQHandler(void)
{

	ACMP->LOCK = ACMP_WRITE_KEY;
	// 设置ACMP1 中断清零控制位  	写0: 不影响 写1: 清零RIS_C1IF标志位
	ACMP->ICLR &= ~(0x02ul);
	ACMP->ICLR |= (0x01ul<<1);
	
	ACMP->LOCK = 0x00;
	
	//关闭输出 
	System_Modules_off();
	
	//短路标志置位
	Fault_ShortCircuit = 1;
	
	//系统状态重启
	MOTOR_STATE = MC_INIT;
	SYSTEM_STATE = SYS_FAULT; 
	
	//参数初始化
	FOC_ControlPara_Init();
  
}

/*****************************************************************************
//*-----------------------------------------------------------------------------
//* Function Name  : SysTick_Handler
//* Description    :
//* Function Call  : SysTick_Handler中断
//* Input Paragram : 140us
//* Return Value   : 
//*-----------------------------------------------------------------------------
//******************************************************************************/
//void SysTick_Handler(void)
//{
//	//--------------------------------------------------------------------------/
//	SysTick->CTRL|=SysTick_CTRL_COUNTFLAG_Msk;
//	
//	TimeFlag_1ms = 1;

//	//--------------------------------------------------------------------------/
//	// 电机故障检测 	
//	#if (Config_Debug_Mode == Debug_Disable)
//			
//	MC_MotorFault_Check();
//	
//	#endif
//	
//	//--------------------------------------------------------------------------/
//	// 电机初始状态任务
//	MC_MotorInit_Process();
//	
//	//--------------------------------------------------------------------------/
//	// 电机转速计算 
//	MC_MotorSpeed_Calc();
//	
//	//--------------------------------------------------------------------------/	
//	// 电机调速模块控制 
//	MC_Speed_Control();
//  
//	//--------------------------------------------------------------------------/		
//  //调速信号捕获完成标志
//  Capture_Mode2_Handle();  
//  
//	//--------------------------------------------------------------------------/
//	// 时序相关处理 
//	if(Time.Charge_Count > 0)      	Time.Charge_Count --;
//	if(Time.Motor_Restart > 0)      Time.Motor_Restart --;

//}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : TIMER0_IRQHandler
* Description    :
* Function Call  : TMR0中断
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void TIMER0_IRQHandler(void)
{
	TMR_ClearOverflowIntFlag(TMR0);
	
	TMR0->LOAD = 460210;//4000000; 1HZ-1ms
	
	stru_Hall.We_Out = 65536;
		
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : TIMER0_IRQHandler
* Description    :
* Function Call  : TMR1中断
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void TIMER1_IRQHandler(void)
{
	TMR_ClearOverflowIntFlag(TMR1);
	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : CCP_IRQHandler
* Description    :
* Function Call  : CCP中断
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void CCP_IRQHandler(void)
{
	u32 temp = CCP->LOCK;								//保存锁的状态，防止在中断外操作模块造成状态丢失		
	
	if(CCP->MIS & (0x1UL<<CCP_CAPTURE_MODULE))
	{
		Capture.OverFlowFlag = 0;
		Capture.OverFlowTime = 0;
		
		if (!Capture.CompleteFlag)
		{
			if (Capture.CaptureCount == 0)
			{
				#if (CCP_CAPTURE_MODULE == CCP0)
					#if (CCP_CAPTURE_CH == CCPxA)
						Capture.CaptureValue[0] = 65000 - ((uint16_t)CCP->D0A);
					#else	
						Capture.CaptureValue[0] = 65000 - ((uint16_t)CCP->D0B);
					#endif	
				#else
					#if (CCP_CAPTURE_CH == CCPxA)
						Capture.CaptureValue[0] = 65000 - ((uint16_t)CCP->D1A);
					#else	
						Capture.CaptureValue[0] = 65000 - ((uint16_t)CCP->D1B);
					#endif					
				#endif
			}
			else if (Capture.CaptureCount == 1)
			{
				#if (CCP_CAPTURE_MODULE == CCP0)
					#if (CCP_CAPTURE_CH == CCPxA)
						Capture.CaptureValue[1] = 65000 - ((uint16_t)CCP->D0A);
					#else	
						Capture.CaptureValue[1] = 65000 - ((uint16_t)CCP->D0B);
					#endif	
				#else
					#if (CCP_CAPTURE_CH == CCPxA)
						Capture.CaptureValue[1] = 65000 - ((uint16_t)CCP->D1A);
					#else	
						Capture.CaptureValue[1] = 65000 - ((uint16_t)CCP->D1B);
					#endif					
				#endif
				//下降到上升沿捕获
				#if (CCP_CAPTURE_MODULE == CCP0)
					CCP->LOCK = CCP_P1AB_WRITE_KEY;
					CCP->CON0 &= ~(CCP_CCPCON0_CCP0CM0ES_Msk); 
					CCP->CON0 |= CCP_CAP_MODE0_FALLING_TO_RISING;
					CCP->LOCK = 0x00;					
				#else
					CCP->LOCK = CCP_P1AB_WRITE_KEY;				
					CCP->CON1 &= ~(CCP_CCPCON1_CCP1CM0ES_Msk); 
					CCP->CON1 |= CCP_CAP_MODE0_FALLING_TO_RISING;	
					CCP->LOCK = 0x00;
				#endif
			}
			else if (Capture.CaptureCount == 2)
			{
				#if (CCP_CAPTURE_MODULE == CCP0)
					#if (CCP_CAPTURE_CH == CCPxA)
						Capture.CaptureValue[2] = 65000 - ((uint16_t)CCP->D0A);
					#else	
						Capture.CaptureValue[2] = 65000 - ((uint16_t)CCP->D0B);
					#endif	
				#else
					#if (CCP_CAPTURE_CH == CCPxA)
						Capture.CaptureValue[2] = 65000 - ((uint16_t)CCP->D1A);
					#else	
						Capture.CaptureValue[2] = 65000 - ((uint16_t)CCP->D1B);
					#endif					
				#endif
			}
			else if (Capture.CaptureCount == 3)
			{
				#if (CCP_CAPTURE_MODULE == CCP0)
					#if (CCP_CAPTURE_CH == CCPxA)
						Capture.CaptureValue[3] = 65000 - ((uint16_t)CCP->D0A);
					#else	
						Capture.CaptureValue[3] = 65000 - ((uint16_t)CCP->D0B);
					#endif	
				#else
					#if (CCP_CAPTURE_CH == CCPxA)
						Capture.CaptureValue[3] = 65000 - ((uint16_t)CCP->D1A);
					#else	
						Capture.CaptureValue[3] = 65000 - ((uint16_t)CCP->D1B);
					#endif					
				#endif	
				//上升到下降沿捕获
				#if (CCP_CAPTURE_MODULE == CCP0)
					CCP->LOCK = CCP_P1AB_WRITE_KEY;
					CCP->CON0 &= ~(CCP_CCPCON0_CCP0CM0ES_Msk); 
					CCP->CON0 |= CCP_CAP_MODE0_RISING_TO_FALLING;
					CCP->LOCK = 0x00;					
				#else
					CCP->LOCK = CCP_P1AB_WRITE_KEY;				
					CCP->CON1 &= ~(CCP_CCPCON1_CCP1CM0ES_Msk); 
					CCP->CON1 |= CCP_CAP_MODE0_RISING_TO_FALLING;	
					CCP->LOCK = 0x00;
				#endif				
			}			
			
			if (++Capture.CaptureCount > 3)
			{
				Capture.CaptureCount = 0;			//重新开始采集
				Capture.CompleteFlag = 1;			//置捕获完成标志
			}
		}
		
		CCP->ICLR |= (0x1UL<< CCP_CAPTURE_MODULE);
		
		CCP->LOCK = CCP_P1AB_WRITE_KEY;
		CCP->RUN &= ~(0x1<<CCP_CAPTURE_MODULE); 
		CCP->LOCK = 0x00;	
		
		CCP->LOCK = CCP_P1AB_WRITE_KEY;
		CCP->RUN |= (0x1<<CCP_CAPTURE_MODULE); 
		CCP->LOCK = 0x00;			
	}
	
	CCP->LOCK = temp;			//恢复锁的状态
  
}

/****************************************************************************
 ** \brief	INTP3_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void INTP3_IRQHandler(void)
{
	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : UART_VIEW
* Description    : 串口数据打印
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void UART_VIEW(void)
{
	UartView(Basic.Mech_Speed ,Basic.FG_Speed ,stru_FOC.Elec_We,stru_Curr_abc.Ia);

}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : FOC_User_Control
* Description    : 用户操作模块
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void FOC_User_Control(void)
{
		UART_VIEW();	/* 数据监测 */
	
}

/******************************** END OF FILE *******************************/
