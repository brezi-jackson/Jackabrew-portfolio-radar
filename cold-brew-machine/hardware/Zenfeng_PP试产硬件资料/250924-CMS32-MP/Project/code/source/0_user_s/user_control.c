//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    user_control.c
* @author  wenshuhong
* @version 
* @date    2020年3月
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
#define	 Voice_Tab_Len  		(20)


//---------------------------------------------------------------------------/
//	Local variable  definitions
//---------------------------------------------------------------------------/
struct_Capture		Capture 	= {0};
struct_Sleep			Sleep 		=	{0};

volatile s32 			Set_Speed = 0;
volatile s32 			Lock_Speed = 0;
//---------------------------------------------------------------------------/
//	Global variable definitions(declared in header file with 'extern')
//---------------------------------------------------------------------------/

//===========================================================================/
// 死区补偿的表格--推荐值
const signed int Voice_Buffer[Voice_Tab_Len][3] = 
{
//--转速--补偿系数--补偿角度--//
	{0, 		0,		0},
	{100, 	0,		0},
	{200, 	0,		0}, 
	{300, 	200,	1000},
	{400, 	400,	1000},
	{500, 	400,	1000},
	{600, 	400,	1000},
	{700, 	400,	1000},	
	{800, 	400,	1000},
	{900, 	400,	1000},
	{1000, 	400,	1000},	
	{1100, 	400,	1000},
	{1200, 	400,	1000}, 
	{1300, 	400,	1000},
	{1400, 	400,	1000},
	{1500, 	400,	1000},
	{2000, 	400,	1000},	
	{3500, 	400,	1000},
};

//===========================================================================/
//转速锁定设定
#define Speed_Lock_BufLen 5    //转速档位锁定数目，目前支持5个档位的速度锁定，若不够用户可自行添加
const signed int Speed_Lock_Buf[Speed_Lock_BufLen][3]=
{
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0}
};
/* VSP速度锁定设定说明 */
/*
	例如设定参数如下：
	{6554,9831,480}
	0.1V = 655.4
	6554：表示转速锁定的输入VSP电压下限值 (单位：无) 6554 = (1.0V / 0.1V) * 655.4
	9831：表示转速锁定的输入VSP电压上限值 (单位：无) 9831 = (1.5V / 0.1V) * 655.4
	480：表示转速锁定的目标转速值 (单位：RPM)
	上述设定参数表示 在 1.0V <= 输入VSP电压 < 1.5V 范围内，转速都保持在 480 转。
*/

/* PWM速度锁定设定说明 */
/*
	例如设定参数如下：
	{100,150,480}
	100：表示转速锁定的输入PWM占空比下限值 (单位：0.1%)
	150：表示转速锁定的输入PWM占空比上限值 (单位：0.1%)
	480：表示转速锁定的目标转速值 (单位：RPM)
	上述设定参数表示 在 10.0% <= 输入PWM占空比 < 15.0% 范围内，转速都保持在 480 转。
*/

/* CLK速度锁定设定说明 */
/*
	例如设定参数如下：
	{100,150,480}
	100：表示转速锁定的输入CLK频率下限值 (单位：Hz)
	150：表示转速锁定的输入CLK频率上限值 (单位：Hz)
	480：表示转速锁定的目标转速值 (单位：RPM)
	上述设定参数表示 在 100Hz <= 输入CLK频率 < 150Hz 范围内，转速都保持在 480 转。
*/

//===========================================================================/

//---------------------------------------------------------------------------/
//	Local function prototypes('static')
//---------------------------------------------------------------------------/
void VSP_Speed_Control(void);
void PWM_Speed_Control(void);
void CLK_Speed_Control(void);

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :User_Speed_Control
* Description    :调速信号控制
* Function Call  :主函数调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
s32 s32_Debug_Speed = 500;	// 调试时设置的转速
s32 s32_Debug_VQ = 5000;	// 调试时设置的VQ值
void User_Speed_Control(void)
{
	#if (Motor_Debug_Mode == Motor_Debug_Offline)
	{
		if (INT8_VALUE(&speedControlMode) == VSP_Control)
		{
			VSP_Speed_Control();						//VSP调速
		}
		else if (INT8_VALUE(&speedControlMode) == PWM_Control)
		{
			PWM_Speed_Control();						//PWM调速
		}
		else if (INT8_VALUE(&speedControlMode) == CLK_Control)
		{
			CLK_Speed_Control();						//频率调速
		}
		else
		{
			if (VQ_Loop != INT8_VALUE(&controlMode))
      {
        User_MotorSpeed_Set(s32_Debug_Speed);		//调试用
      }
      else
      {
        User_MotorSpeed_Set(s32_Debug_VQ);			//调试用
      }
			User_Motor_On();						
		}
	}
	#endif
	
	#if (Motor_Debug_Mode == Motor_Debug_Online)
	{
		#if (VQ_Loop != Config_Contorl_Mode)
			User_MotorSpeed_Set(s32_Debug_Speed);		//调试用
		#else
			User_MotorSpeed_Set(s32_Debug_VQ);			//调试用
		#endif
	}
	#endif	
  
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :VSP_Speed_Control
* Description    :VSP调速控制
* Function Call  :主函数调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
//---------------------------------------------------------------------------//  
void VSP_Speed_Control(void)
{
	u8 i=0;

	if(Basic.VCtr <= userHostSet.value.vspStopQ15)
	{
		Set_Speed  = 0;
		Lock_Speed = 0;
		User_Motor_Off();
	}
	else
	{
		if(Basic.VCtr >= userHostSet.value.vspStartQ15)
		{
			if(INT8_VALUE(&lockSpeedMode) == Lock_Speed_Disable)
			{				
				if (Basic.VCtr > userHostSet.value.vspMaxQ15)
					Set_Speed = userHostSet.value.runSpeedMax;				//超过设定最大电压给最大速度
				else 
        {
					Set_Speed = ((userHostSet.value.vspSpeedK * Basic.VCtr) >> 15) + userHostSet.value.vspSpeedB;
        }
				
				if(Set_Speed > userHostSet.value.runSpeedMax)
					Set_Speed = userHostSet.value.runSpeedMax;
			}
			else
			{
				for(i = 0 ;i < Speed_Lock_BufLen ;i++)
				{
					//查找锁定转速
					if((Basic.VCtr >= Speed_Lock_Buf[i][0] )&& (Basic.VCtr < Speed_Lock_Buf[i][1]))
					{
						Lock_Speed = Speed_Lock_Buf[i][2];
						break;
					}
				}
				//未查找到锁定转速，锁定转速清零
				if(i >= Speed_Lock_BufLen)
				{
					if(Lock_Speed != Speed_Lock_Buf[i][2])
					{
						Lock_Speed = 0;
					}
				}

				if(Lock_Speed != 0)
				{
					Set_Speed = Lock_Speed;
				}
				else
				{
					if (Basic.VCtr > userHostSet.value.vspMaxQ15)
						Set_Speed = userHostSet.value.runSpeedMax;				//超过设定最大电压给最大速度
					else
						Set_Speed = ((userHostSet.value.vspSpeedK * Basic.VCtr) >> 15) + userHostSet.value.vspSpeedB;
					
					if(Set_Speed > userHostSet.value.runSpeedMax)
						Set_Speed = userHostSet.value.runSpeedMax;					
				}
			}
            
			User_MotorSpeed_Set(Set_Speed);
			User_Motor_On();
		}
		else
		{
			Set_Speed = userHostSet.value.runSpeedStart;
			User_MotorSpeed_Set(Set_Speed);
      
		}
	}	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :PWM_Speed_Control
* Description    :PWM调速控制
* Function Call  :主函数调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
//---------------------------------------------------------------------------// 
void PWM_Speed_Control(void)
{	
	u8 i=0;
	
	if(Capture.DutyCycle <= INT32_VALUE(&pwmStop))
	{
		Set_Speed  = 0;
		Lock_Speed = 0;
		User_Motor_Off();
	}
	else 
	{
		if(Capture.DutyCycle >=  INT32_VALUE(&pwmStart))
		{
			if(INT8_VALUE(&lockSpeedMode) == Lock_Speed_Disable)
			{
				if (Capture.DutyCycle > INT32_VALUE(&pwmMax))
				{		
					Set_Speed = userHostSet.value.runSpeedMax;				//超过设定最大占空比给最大速度
				}
				else
				{
					if(Capture.DutyCycle <= INT32_VALUE(&pwmHold))
					{
						Set_Speed = userHostSet.value.runSpeedStart;
					}
					else
					{
						Set_Speed = ((userHostSet.value.pwmSpeedK  * Capture.DutyCycle)/1000) + userHostSet.value.pwmSpeedB;
					}			
				}
				
				if(Set_Speed > userHostSet.value.runSpeedMax)	
					Set_Speed = userHostSet.value.runSpeedMax;			
			}
			else
			{
				for(i = 0 ;i < Speed_Lock_BufLen ;i++)
				{
					//查找锁定转速
					if((Capture.DutyCycle >= Speed_Lock_Buf[i][0]) && (Capture.DutyCycle < Speed_Lock_Buf[i][1]))
					{
						Lock_Speed = Speed_Lock_Buf[i][2];
						break;
					}
				}
				//未查找到锁定转速，锁定转速清零
				if(i >= Speed_Lock_BufLen)
				{
					if(Lock_Speed != Speed_Lock_Buf[i][2])
					{
						Lock_Speed = 0;
					}
				}

				if(Lock_Speed != 0)
				{
					Set_Speed = Lock_Speed;
				}
				else
				{
					if (Capture.DutyCycle > INT32_VALUE(&pwmMax))
					{		
						Set_Speed = userHostSet.value.runSpeedMax;				//超过设定最大占空比给最大速度
					}
					else
					{
						if(Capture.DutyCycle <= INT32_VALUE(&pwmHold))
						{
							Set_Speed = userHostSet.value.runSpeedStart;
						}
						else
						{
							Set_Speed = ((userHostSet.value.pwmSpeedK  * Capture.DutyCycle)/1000) + userHostSet.value.pwmSpeedB;
						}			
					}
					
					if(Set_Speed > userHostSet.value.runSpeedMax)	
						Set_Speed = userHostSet.value.runSpeedMax;						
				}				
			}
           		
			User_MotorSpeed_Set(Set_Speed);
			User_Motor_On();				
		}
 		else
		{
			Set_Speed = userHostSet.value.runSpeedStart;
			User_MotorSpeed_Set(Set_Speed);
      
		}
	}		
}


/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :CLK_Speed_Control
* Description    :CLK调速控制
* Function Call  :主函数调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
//---------------------------------------------------------------------------//  
void CLK_Speed_Control(void)
{
	u8 i=0;	
	
	if(Capture.Frequency <= INT32_VALUE(&clkStop)*10)
	{
		Set_Speed  = 0;
		Lock_Speed = 0;
		User_Motor_Off();
	}
	else 
	{
		if(Capture.Frequency >= INT32_VALUE(&clkStart)*10)
		{			
			if(INT8_VALUE(&lockSpeedMode) == Lock_Speed_Disable)
			{
				if (Capture.Frequency > INT32_VALUE(&clkMax)*10)		
					Set_Speed = userHostSet.value.clkSpeedMax;				//超过设定最大频率给最大速度
				else
					Set_Speed = ((userHostSet.value.clkSpeedK  * Capture.Frequency)/(INT32_VALUE(&clkMax)*10)) + userHostSet.value.clkSpeedB;

				if(Set_Speed > userHostSet.value.clkSpeedMax)	
					Set_Speed = userHostSet.value.clkSpeedMax;	
			}
			else
			{
				for(i = 0 ;i < Speed_Lock_BufLen ;i++)
				{
					//查找锁定转速
					if((Capture.Frequency >= Speed_Lock_Buf[i][0]*10) && (Capture.Frequency < Speed_Lock_Buf[i][1]*10))
					{
						Lock_Speed = Speed_Lock_Buf[i][2];
						break;
					}
				}
				//未查找到锁定转速，锁定转速清零
				if(i >= Speed_Lock_BufLen)
				{
					if(Lock_Speed != Speed_Lock_Buf[i][2])
					{
						Lock_Speed = 0;
					}
				}

				if(Lock_Speed != 0)
				{
					Set_Speed = Lock_Speed;
				}
				else
				{
					if (Capture.Frequency > INT32_VALUE(&clkMax)*10)
						Set_Speed = userHostSet.value.clkSpeedMax;				//超过设定最大频率给最大速度
					else
						Set_Speed = ((userHostSet.value.clkSpeedK  * Capture.Frequency)/(INT32_VALUE(&clkMax)*10)) + userHostSet.value.clkSpeedB;

					if(Set_Speed > userHostSet.value.clkSpeedMax)	
						Set_Speed = userHostSet.value.clkSpeedMax;							
				}				
			}
            
			User_MotorSpeed_Set(Set_Speed);
			User_Motor_On();
		}
 		else
		{
			Set_Speed = userHostSet.value.clkSpeedStart;
			User_MotorSpeed_Set(Set_Speed);
      
		}		
	}
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :CCP捕获模式0配置
* Description    :需要将相应的IO配置成CCP口，其它不需要改动
* Function Call  :程序初始化调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
void CCP_Capture_Init(void)
{	
	/*
  (1)开启CCP模块时钟
  */
	CGC_PER11PeriphClockCmd(CGC_PER11Periph_CCP,ENABLE);				
	
	if (INT8_VALUE(&speedControlMode) == CLK_Control)
		CCP_ConfigCLK(CCP_CAPTURE_MODULE,CCP_CLK_DIV_64,CCP_RELOAD_CCPLOAD,0xffff);			/*Fahb=64M*/	
	else
		CCP_ConfigCLK(CCP_CAPTURE_MODULE,CCP_CLK_DIV_4,CCP_RELOAD_CCPLOAD,0xffff);			/*Fahb=64M*/				
	
	/*
	(2)设置CCP运行模式
	*/
	CCP_EnableCAPMode2();								/* 触发模式2  */					
	CCP_ConfigCAPMode2(CAP1,CCP_CAP_MODE2_RISING);		/* 上升沿触发 */
	CCP_ConfigCAPMode2(CAP2,CCP_CAP_MODE2_FALLING);		/* 下降沿触发 */
	CCP_ConfigCAPMode2(CAP3,CCP_CAP_MODE2_RISING);		/* 上升沿触发 */	
  
	/*
	(3)设置CCP捕获通道
	*/
	CCP_SelCAPMode2Channel(CAP1_CCP0B);
	
	/*
	(4)设置捕获通道IO
	*/
//	GPIO_PinAFInConfig(CCP0BINCFG, 0x00);    /*设置P02为CCP0b捕获通道*/
//	GPIO_Init(PORT0,PIN2,INPUT);
	
//	GPIO_PinAFInConfig(CCP0BINCFG, 0x01);    /*设置P04为CCP0b捕获通道*/
//	GPIO_Init(PORT0,PIN4,INPUT);	

	/*
	(5)开启CCP模块
	*/	
	CCP_EnableRun(CCP1);			
	CCP_Start(CCP1);			/*CCP模块使能*/

}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :CCP PWM模块配置
* Description    :需要将相应的IO配置成CCP口，其它不需要改动
* Function Call  :程序初始化调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
void CCP_PWM_Init(void)
{
	/*
	(1)设置CCP的时钟
	*/
	CGC_PER11PeriphClockCmd(CGC_PER11Periph_CCP,ENABLE);				/*开启CCP模块时钟*/
	CCP_ConfigCLK(CCP_PWM_MODULE,CCP_CLK_DIV_64,CCP_RELOAD_CCPLOAD,10);			/*Fahb=64M*/	
  
	/*
	(2)设置CCP运行模式
	*/
	CCP_EnablePWMMode(CCP_PWM_MODULE);	
	
	/*
	(3)设置CCP 比较值
	*/	
	CCP_ConfigCompare(CCP_PWM_MODULE,CCP_PWM_CH,5); 	/*50%*/	
  
	/*
	(4)设置CCP 反相输出
	*/		
	CCP_DisableReverseOutput(CCP_PWM_MODULE,CCP_PWM_CH);
  
	/*
	(5)设置CCP IO口复用
	*/	
	
	GPIO_PinAFOutConfig(P07CFG, IO_OUTCFG_P07_CCP0B_O);	
	GPIO_Init(PORT0,PIN7,OUTPUT);
	
	/*
	(6)开启CCP模块
	*/		

	Capture.FG_SpeedMin = 120;//(u16)(60*1000000/(MOTOR_PAIRS*65000)) + 1;	//FG输出最小转速
  
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :Capture_Mode2_Handle
* Description    :调速信号捕获完成标志
* Function Call  :1ms中断调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
void Capture_Mode2_Handle(void)
{
	if((CCP->RIS & (0x1UL<<(CAP3+8)))? 1:0)
	{
		Capture.OverFlowFlag = 0;
		Capture.OverFlowTime = 0;
					
		CCP_ClearCAPMode2IntFlag(CAP3);
		Capture.CompleteFlag = 1;			//置捕获完成标志;
    
	}
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :User_Speed_Capture
* Description    :调速信号捕获计算
* Function Call  :主函数10ms调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
void User_Speed_Capture(void)
{
	if (++Capture.OverFlowTime >= 50)			//100ms溢出计数周期
	{
		Capture.OverFlowTime = 0;
		Capture.OverFlowFlag = 1;
		Capture.CaptureCount = 0;
		Capture.CompleteFlag = 0;	
		
		CCP_DisableRun(CCP1);	
		CCP_EnableRun(CCP1);		/*CCP模块使能*/
	}
		
	if (Capture.OverFlowFlag)
	{
		if (PORT_GetBitValue(PORT0 ,PIN4))				//判断占空比是否为100%
		{
			if (++Capture.HeightenTime >= 8)	//80ms计数消抖
			{		
				Capture.DutyCycle = 1000;			
				Capture.Frequency = 0;				
			}	

			Capture.WeakenTime = 0;
		}
		else
		{
			if (++Capture.WeakenTime >= 8)		//80ms计数消抖
			{
				Capture.DutyCycle = 0;
				Capture.Frequency = 0;						
			}
			
			Capture.HeightenTime = 0;
		}
	}
	else if (Capture.CompleteFlag)
	{
		Capture.CaptureTimeH = CCP->CAP0DAT0 & 0xffff;
		Capture.CaptureTimeCycle = CCP->CAP0DAT0 >> 16;
		
		//过窄捕获错误处理
		if(Capture.CaptureTimeH > Capture.CaptureTimeCycle)
		{
			if (PORT_GetBitValue(PORT0 ,PIN4))				//判断占空比是否为100%
			{
				if (++Capture.HeightenTime >= 8)	//80ms计数消抖
				{		
					Capture.DutyCycle = 1000;			
					Capture.Frequency = 0;	
					Capture.FrequencyTemp = 0;
					Capture.DutyCycleTemp	= 1000;	
				}	
				Capture.WeakenTime = 0;
			}
			else
			{
				if (++Capture.WeakenTime >= 8)		//80ms计数消抖
				{
					Capture.DutyCycle = 0;
					Capture.Frequency = 0;	
					Capture.FrequencyTemp = 0;
					Capture.DutyCycleTemp	= 0;						
				}
				Capture.HeightenTime = 0;
			}
		}
		else		//正常捕获
		{
			Capture.WeakenTime = 0;
			Capture.HeightenTime = 0;
			
			if (INT8_VALUE(&speedControlMode) == CLK_Control)
      {
        Capture.FrequencyTemp = (u32)(1000000*10/Capture.CaptureTimeCycle);				//0.1Hz	1/(计数值*CCP时钟周期)
      }
      else	
      {
        Capture.FrequencyTemp = (u32)(16000000*10/Capture.CaptureTimeCycle);				//0.1Hz	1/(计数值*CCP时钟周期)
      }

			Capture.DutyCycleTemp = (u32)(Capture.CaptureTimeH*1000/Capture.CaptureTimeCycle) + 1;	//分辨率 0.1%		
			
			if(Capture.FrequencyTemp == Capture.FrequencyOld)						
			{
				if (ABSFUN((s32)Capture.FrequencyOld - (s32)Capture.Frequency) > 2)       	//偏差判断  ±2
					Capture.Frequency = Capture.FrequencyTemp;				    										//更新数据
			}
			else 
				Capture.FrequencyOld = Capture.FrequencyTemp;																//更新旧值
			
			if(Capture.DutyCycleTemp == Capture.DutyCycleOld)
			{
				if (ABSFUN((s32)Capture.DutyCycleOld - (s32)Capture.DutyCycle) > 2)       	//偏差判断	 ±2
					Capture.DutyCycle = Capture.DutyCycleTemp;                  							//更新数据
			}
			else
				Capture.DutyCycleOld = Capture.DutyCycleTemp;						
		}
		
		//clk模式关机小发波
		if (INT8_VALUE(&speedControlMode) == CLK_Control)
    {
      if(Capture.CaptureTimeH < 15)
      {
        Capture.Frequency = 0;
        Capture.DutyCycle = 0;
      }
    }
		
		Capture.CaptureCount = 0;			//重新开始采集
		Capture.CompleteFlag = 0;			//清除采集完成标志
    
	}
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :CCP_PWM_Config
* Description    :设置CCPn模块装载值
* Function Call  :FG输出调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
void CCP_PWM_Config(uint32_t CCPn, uint32_t CCPChannel ,uint32_t ReloadMode, uint32_t Period,uint32_t Compare)
{
	volatile u32 temp;
	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	
	if(CCP0 == CCPn)
	{
		CCP->LOAD0 = ((uint16_t)Period) | (ReloadMode<< CCP_CCPLOAD0_RELOAD_Pos);	

		if( CCPxA == CCPChannel)
		{
			temp = CCP->D0A;
			temp &= ~(CCP_CCPD0A_DATA_Msk);
			temp |= Compare;
			CCP->D0A = temp;
		}
		if( CCPxB == CCPChannel)
		{
			temp = CCP->D0B;
			temp &= ~(CCP_CCPD0B_DATA_Msk);
			temp |= Compare;
			CCP->D0B = temp;			
		}		
	}
	
	if(CCP1 == CCPn)
	{
		CCP->LOAD1 = ((uint16_t)Period) | (ReloadMode<< CCP_CCPLOAD1_RELOAD_Pos);	
		
		if( CCPxA == CCPChannel)
		{
			temp = CCP->D1A;
			temp &= ~(CCP_CCPD1A_DATA_Msk);
			temp |= Compare;
			CCP->D1A = temp;				
		}
		if( CCPxB == CCPChannel)
		{
			temp = CCP->D1B;
			temp &= ~(CCP_CCPD1B_DATA_Msk);
			temp |= Compare;
			CCP->D1B = temp;			
		}		
	}	
	CCP->LOCK = 0x00;		
  
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :User_Speed_Out
* Description    :转速（FG）输出
* Function Call  :主函数调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
int8_t FG_Flag = 0;
void User_Speed_Out(void)
{
  if (Basic.FG_Speed >= Capture.FG_SpeedMin)	
  {
    FG_Flag = 1;
  }  
  if(FG_Flag == 1)
  {
    if(Basic.FG_Speed >= (Capture.FG_SpeedMin - 40))
    {
      PORT_SetBit(PORT2,PIN1);
      GPIO_PinAFOutConfig(P07CFG, IO_OUTCFG_P07_CCP0B_O);	
      GPIO_Init(PORT0,PIN7,OUTPUT);
      
      //-------------------------------------------------------------------//		
      if (Capture.FG_SpeedOld != Basic.FG_Speed)		//与保留的旧值不一致则更新
      {
        Capture.FG_SpeedOld = Basic.FG_Speed;		

        Capture.FG_Period  = (u16)(60*1000000/(3*Para.Base.Paris*Capture.FG_SpeedOld));

        CCP_PWM_Config(CCP_PWM_MODULE,CCP_PWM_CH,CCP_RELOAD_CCPLOAD,Capture.FG_Period,Capture.FG_Period >> 1);
      }
      
      if (CCP_PWM_MODULE == CCP0)
      {
        if (!(CCP->CON0 & 0x40))									//运行标志位判断
          CCP_Start(CCP_PWM_MODULE);							//使能CCP模块				
      }
      else
      {
        if (!(CCP->CON1 & 0x40))									//运行标志位判断
          CCP_Start(CCP_PWM_MODULE);							//使能CCP模块					
      }
      
      if (!(CCP->RUN & (0x1<<CCP_PWM_MODULE)))    //运行标志位判断
        CCP_EnableRun(CCP_PWM_MODULE);						//运行CCP		
    }
    else
    {
      FG_Flag = 0;
    }
  }
  else 
  {	
    //-------------------------------------------------------------------//	
    Capture.FG_SpeedOld = 0;
    CCP_Stop(CCP_PWM_MODULE);										//停止CCP
    CCP_DisableRun(CCP_PWM_MODULE);							//关闭CCP模块    

    PORT_ClrBit(PORT0,PIN7);
    GPIO_PinAFOutConfig(P07CFG, IO_OUTCFG_P07_GPIO);
    GPIO_Init(PORT0,PIN7,OUTPUT);    

  }	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :User_Voice_Control
* Description    :调音控制
* Function Call  :主函数调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
s32 s32_Comp_Coeff_tmp = 400;
s32 s32_Comp_Angle_tmp = 0;
void User_Voice_Control(void)
{
	//=======================================================================//
	// 死区补偿--查表
	#if (Motor_Debug_Mode==Motor_Debug_Offline)
	
	u16 i;
	
	for(i=0; i<(Voice_Tab_Len-1); i++)
	{
		if((Basic.Mech_Speed > Voice_Buffer[i][0]) && (Basic.Mech_Speed <= Voice_Buffer[i+1][0]))
		{
			stru_DeadComp.s32_Comp_Coeff = (((Voice_Buffer[i+1][1] - Voice_Buffer[i][1])*(Basic.Mech_Speed - Voice_Buffer[i][0]))/(Voice_Buffer[i+1][0] - Voice_Buffer[i][0])) + Voice_Buffer[i][1];
			stru_DeadComp.s32_Comp_Angle = Voice_Buffer[i+1][2];
	
			break;
		}
	}
	#endif
	
	//=======================================================================//	
	// 调试模式
	#if (Motor_Debug_Mode == Motor_Debug_Online)
	
	//-----------------------------------------------------------------------/
	// 在线调试--确认补偿系数
	#if 1
	stru_DeadComp.s32_Comp_Coeff = s32_Comp_Coeff_tmp;
	stru_DeadComp.s32_Comp_Angle = s32_Comp_Angle_tmp;
	
	#else
	
	//-----------------------------------------------------------------------/
	// 在线听音--走表格
	u16 i;
	
	for(i=0; i<(Voice_Tab_Len-1); i++)
	{
		if((Basic.Mech_Speed > Voice_Buffer[i][0]) && (Basic.Mech_Speed <= Voice_Buffer[i+1][0]))
		{
			stru_DeadComp.s32_Comp_Coeff = (((Voice_Buffer[i+1][1] - Voice_Buffer[i][1])*(Basic.Mech_Speed - Voice_Buffer[i][0]))/(Voice_Buffer[i+1][0] - Voice_Buffer[i][0])) + Voice_Buffer[i][1];
			stru_DeadComp.s32_Comp_Angle = Voice_Buffer[i+1][2];
	
			break;
		}
	}
	
	#endif
	//-----------------------------------------------------------------------/
	
	#endif
	//=======================================================================//	
	
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :User_Sleep_Manage			//未更新
* Description    :睡眠管理
* Function Call  :主函数调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
void User_Sleep_Manage(void)
{	
	if ((MOTOR_STATE == MC_INIT) && (Basic.Mech_Speed == 0) && (Capture.Frequency == 0) && (Capture.DutyCycle==0) && (Basic.VCtr <= userHostSet.value.vspStopQ15))
	{
		if(++Sleep.SleepCount >= 30)				//100*30 = 3s
		{
			Sleep.SleepCount = 0;
			//关管
			Brige_Output_Off();
			
			//关闭中断
			NVIC->ICER[0] = 0xFFFFFFFF;
			//特殊处理tick
			SysTick->CTRL  = 0x00000005;
		//***********************************************//	
			//设置部分引脚为输入高阻态
		//	GPIO_Init(PORT0,PIN0,ANALOG_INPUT);
		//**********************************************//
			
			//关闭PWM
			EPWM_Stop(EPWM_CH_0_MSK | EPWM_CH_1_MSK|
						EPWM_CH_2_MSK | EPWM_CH_3_MSK|
						EPWM_CH_4_MSK | EPWM_CH_5_MSK);
			//关闭ADC
			ADC_Stop();
			
			//关闭PGA0
			PGA_Stop(PGA0x);

			//关闭PGA1
			PGA_Stop(PGA1x);
      
      //关闭PGA2
      PGA_Stop(PGA2x);
			
			//关闭ACMP1
			#if (ACMP_CH == ACMP_CH0)
			
				ACMP_Stop(ACMP0);

			#endif			
			#if (ACMP_CH == ACMP_CH1)
			
				ACMP_Stop(ACMP1);

			#endif
			
			//关闭DAC
			DAC_Stop();
			
      //关闭FG输出
			#if (Config_CCP_PWM == CCP_PWM_Enable)
				CCP_DisableRun(CCP_PWM_MODULE);
				CCP_Stop(CCP_PWM_MODULE);	

			#endif	
			
			//关闭CCP捕获
			#if (Config_CCP_Capture == CCP_Capture_Enable)
				CCP_DisableRun(CCP_CAPTURE_MODULE);
				CCP_Stop(CCP_CAPTURE_MODULE);			
			#endif
			
			//关闭除法时钟
			CGC_PER12PeriphClockCmd(CGC_PER12Periph_DIV,ENABLE);
			
			//关闭Timer
			TMR_Stop(TMR0);
			
		//-----------------------------------------------------------------------//	
			//设置唤醒引脚
			#if 1
			//p04输入中断
			GPIO_Init(PORT0,PIN4,PULLDOWN_INPUT);
			GPIO_PinAFInConfig(INT3CFG, 0x04);    /*设置P04为INPUT03中端口*/

			INTM->EGP0 &= ~(0x01 << 3);			//设置INPUT03 上升沿触发
			INTM->EGP0 |= 0x01 << 3;
			
			NVIC_EnableIRQ(INTP3_IRQn);
			#endif		

		//-----------------------------------------------------------------------//			
		//深度睡眠模式
//			CGC->PMUKEY = 0x0192A;
//			CGC->PMUKEY = 0x3E4F;
//			CGC->PMUCTL = 1;
			SCB->SCR   |= 0x04;
			__WFI();	
		
		//-----------------------------------------------------------------------//
			//唤醒之后
		//-----------------------------------------------------------------------//	
    	DelayTime_ms(2);
			//关中断
			NVIC_DisableIRQ(INTP3_IRQn);
      
      GPIO_Init(PORT0,PIN4,INPUT);
		//-----------------------------------------------------------------------//	
			//恢复功能
			//除法（关时钟之后需要重新配置）
			CGC_PER12PeriphClockCmd(CGC_PER12Periph_DIV,ENABLE);
			DIVSQRT_EnableDIVMode();
			DIVSQRT_EnableSingedMode();
				
			//恢复PWM
			EPWM_Start(EPWM_CH_0_MSK | EPWM_CH_1_MSK|
						EPWM_CH_2_MSK | EPWM_CH_3_MSK|
						EPWM_CH_4_MSK | EPWM_CH_5_MSK);
						
			Brige_Output_Off();	
			//恢复ADC
			ADC_Start();	
			
			//恢复PGA0
			PGA_Start(PGA0x);
			
      //恢复PGA1
			PGA_Start(PGA1x);
      
      //恢复PGA2
      PGA_Start(PGA2x);

      //恢复DAC
			DAC_Start();	
			
      //恢复ACMP
      #if (ACMP_CH == ACMP_CH0)
    
      ACMP_Start(ACMP0);

      #endif			
      #if (ACMP_CH == ACMP_CH1)
    
      ACMP_Start(ACMP1);

      #endif
			
      //恢复FG输出
			#if (Config_CCP_PWM == CCP_PWM_Enable)
				CCP_EnableRun(CCP_PWM_MODULE);
				CCP_Start(CCP_PWM_MODULE);	
				
				GPIO_Init(PORT0,PIN7,OUTPUT);
				GPIO_PinAFOutConfig(P07CFG, IO_OUTCFG_P07_CCP0B_O);	

			#endif	
			
			//恢复CCP捕获
			#if (Config_CCP_Capture == CCP_Capture_Enable)
				CCP_EnableRun(CCP_CAPTURE_MODULE);
				CCP_Start(CCP_CAPTURE_MODULE);			
			#endif
      
			//timer
			TMR_Start(TMR0);

			MOTOR_STATE = MC_INIT;
			SYSTEM_STATE = SYS_RUN; 
				
			DelayTime_ms(2);
		//-----------------------------------------------------------------------//	
			//恢复引脚功能	
			
		//-----------------------------------------------------------------------//		
			//恢复中断
			__EI_Set();
			
			SysTick->LOAD  = (uint32_t)(MCU_CLK/1000 - 1UL);
			SysTick->VAL   = 0UL;
			SysTick->CTRL  = 0x00000007;
		}
	}
	else
		Sleep.SleepCount = 0;

  
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :API_Speed_Control
* Description    :电机调速运行控制
* Function Call  :主函数调用
* Input Paragram :Speed_Val：设置转速值; Speed_En: 1-run, 0-stop
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
void API_Speed_Control(s32 Speed_Val, u8 Speed_En)
{
	/* 设置电机运行转速 */
	User_MotorSpeed_Set(Speed_Val);
	
	/* 设置电机运行使能信号 */
	if(Speed_En)
		User_Motor_On();	// 电机运行
	else
		User_Motor_Off();	// 电机停止
  
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :API_Get_MotorSpeed
* Description    :获取电机转速
* Function Call  :主函数调用
* Input Paragram :无
* Return Value   :电机转速
*-----------------------------------------------------------------------------
******************************************************************************/
s32 API_Get_MotorSpeed(void)
{
	return (Basic.Mech_Speed);
  
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :API_Get_MotorStates
* Description    :获取电机当前运行状态
* Function Call  :主函数调用
* Input Paragram :无
* Return Value   :电机转速
	MC_INIT		= 0, 	//初始化态
	MC_CHARGE	= 1, 	//预充电，未使用
	MC_WIND		= 2, 	//顺逆风检测
	MC_START	= 3, 	//启动态
	MC_SW			= 4, 	//切换态（启动到运行过渡）
	MC_RUN		= 5,  //运行态
	MC_STOP		= 6, 	//停机态
*-----------------------------------------------------------------------------
******************************************************************************/
u8 API_Get_MotorStates(void)
{
	return (MOTOR_STATE);

}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :API_Get_FaultStates
* Description    :获取电机故障信息
* Function Call  :主函数调用
* Input Paragram :无
* Return Value   :电机故障标志
	Fault_Flag		    			u_Fault_Flag.Byte				//总故障代码
	Fault_ShortCircuit			u_Fault_Flag.Bits.Bit0	//硬件短路
	Fault_UnderVoltage			u_Fault_Flag.Bits.Bit7	//欠压
	Fault_StartFail					u_Fault_Flag.Bits.Bit6 	//启动失败
	Fault_OverVoltage				u_Fault_Flag.Bits.Bit5 	//过压
	Fault_TorgueLoss				u_Fault_Flag.Bits.Bit4 	//转矩丢失，未使用
	Fault_OverCurrent				u_Fault_Flag.Bits.Bit3 	//软件过流
	Fault_PhaseLoss					u_Fault_Flag.Bits.Bit2 	//缺相
	Fault_Block							u_Fault_Flag.Bits.Bit1	//堵转
*-----------------------------------------------------------------------------
******************************************************************************/
u8 API_Get_FaultStates(void)
{
	return (Fault_Flag);

}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :FMC_Erase
* Description    :flash擦除
* Function Call  :用户调用
* Input Paragram :目标地址、擦除方式
				  擦除方式 0 : sector不校验 1：chip擦除 2：sector擦除，硬件校验 3 ：禁止
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
void FMC_Erase(uint32_t fmc_adress,u8 Erase_Mode)
{

	//擦除方式 0 : sector不校验 1：chip擦除 2：sector擦除，硬件校验 3 ：禁止
	FMC->FLERMD = (Erase_Mode << 3);
	
    //解锁保护寄存器
	FMC->FLPROT = 0xf1;
	
	//设置擦除操作方式
	FMC->FLOPMD1 = 0x55;
	FMC->FLOPMD2 = 0xAA;
	
	//向目标地址写任意输出擦除
	*((uint32_t *)fmc_adress) = 0xFFFFFFFF;
	
	//等待擦除完成
	while(!(FMC->FLSTS & 0x01))
		;
	
	//清除完成标志位
	FMC->FLSTS |= 0x01;
	
	FMC->FLERMD = 0x00;
	FMC->FLPROT = 0x00;
	//上锁
	FMC->FLPROT = 0x00;
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :FMC_Write
* Description    :写入
* Function Call  :用户调用
* Input Paragram :目标地址、目标数据
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
void FMC_Write(uint32_t fmc_adress,uint32_t fmc_data)
{
	
    //解锁保护寄存器
	FMC->FLPROT = 0xf1;
	
	//设置操作方式
	FMC->FLOPMD1 = 0xAA;
	FMC->FLOPMD2 = 0x55;
	
	//目标地址写入数据
	*((uint32_t *)fmc_adress) = (uint32_t)fmc_data;

	//等待写入完成
	while(!(FMC->FLSTS & 0x01))
		;
	
	//清除完成标志位
	FMC->FLSTS |= 0x01;
	
	//上锁
	FMC->FLPROT = 0x00;

}


/******************************** END OF FILE *******************************/
