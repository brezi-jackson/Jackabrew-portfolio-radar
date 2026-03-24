//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    user_hostinithex.c
* @author  CMS Appliction Team 
* @version 0.0.1
* @date    2020年6月
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

//功能模式参数
const int8_t   pcbHallMode              __attribute__((at(0x00007000))) = Config_PCB_Hall_Mode;       //PCB霍尔表格选择(预留)
const int8_t   motorFR                  __attribute__((at(0x00007004))) = Config_Motor_FR;            //电机旋转方向
const int8_t   sleepMode                __attribute__((at(0x00007008))) = Sleep_Control_Mode;         //休眠功能
const int8_t   reverseMode              __attribute__((at(0x0000700C))) = Motor_ZF_Control_Mode;      //正反转功能
const int8_t   breakMode                __attribute__((at(0x00007010))) = Brake_Control_Mode;         //刹车功能
const int32_t  breakSpeed               __attribute__((at(0x00007014))) = Motor_Brake_Speed;          //刹车转速
const int32_t  breakTime                __attribute__((at(0x00007018))) = Motor_Brake_Time;           //刹车时间
//电机及硬件参数
const int8_t   motorPairs               __attribute__((at(0x0000701C))) = MOTOR_PAIRS;                //电机极对数
const float    motorRS                  __attribute__((at(0x00007020))) = MOTOR_RS;                   //电机相电阻
const float    motorLS                  __attribute__((at(0x00007024))) = MOTOR_LS;                   //电机相电感
const float    busVoltage               __attribute__((at(0x00007028))) = HW_VBUS_VOLT;               //母线电压
const float    phaseResistance          __attribute__((at(0x0000702C))) = HW_PHASE_RES;               //相采样电阻
const float    ibusResistance           __attribute__((at(0x00007030))) = HW_IBUS_RES;                //母线采样电阻
//启动参数
const int8_t   startMode                __attribute__((at(0x00007034))) = Config_Start_Mode;          //启动方式
const float    startCurrentInit         __attribute__((at(0x00007038))) = START_CURRENT_INIT;         //启动电流初始值
const float    startCurrentFinal        __attribute__((at(0x0000703C))) = START_CURRENT_FINAL;        //启动电流最大值
const int32_t  startIRamp               __attribute__((at(0x00007040))) = START_IRAMP;                //启动电流爬坡周期
const int32_t  satrtEKP                 __attribute__((at(0x00007044))) = START_EKP;                  //启动估算器KP
const int32_t  satrtEKI                 __attribute__((at(0x00007048))) = START_EKI;                  //启动估算器KI
const int32_t  satrtEOutMin             __attribute__((at(0x0000704C))) = START_EOUT_MIN;             //启动初始角速度
//运行参数
const int32_t  speedRampInc             __attribute__((at(0x00007050))) = SPEED_RAMP_INC;             //速度爬坡曲线，加速斜率
const int32_t  speedRampDec             __attribute__((at(0x00007054))) = SPEED_RAMP_DEC;             //速度爬坡曲线，下降斜率
const float    focWKP                   __attribute__((at(0x00007058))) = FOC_WKP;                    //速度环KP
const float    focWKI                   __attribute__((at(0x0000705C))) = FOC_WKI;                    //速度环KI
const float    focWKD                   __attribute__((at(0x00007060))) = FOC_WKD;                    //速度环KD
const float    focRunCurrentMin         __attribute__((at(0x00007064))) = FOC_RUN_CURRENT_MIN;        //电机运行最小相电流峰值
const float    focRunCurrentMax         __attribute__((at(0x00007068))) = FOC_RUN_CURRENT_MAX;        //电机运行最大相电流峰值
const float    vectorVoltageMax         __attribute__((at(0x0000706C))) = MAX_VOLTAGE_VECTOR;         //占空比开度
//故障保护参数
const int32_t  faultBlockTime           __attribute__((at(0x00007070))) = FAULT_BLOCK_TIME;           //堵转判别时间
const int32_t  faultRestartDelayTime    __attribute__((at(0x00007074))) = FAULT_RESTART_DELAY_TIME;   //故障保护重启间隔时间
const int32_t  faultSpeedMax            __attribute__((at(0x00007078))) = FAULT_SPEED_MAX;            //堵转保护最大转速值
const int32_t  faultSpeedMin            __attribute__((at(0x0000707C))) = FAULT_SPEED_MIN;            //堵转保护最小转速值
const float    faultOverVoltage         __attribute__((at(0x00007080))) = FAULT_VBUS_OVOT;            //过压保护值
const float    faultUnderVoltage        __attribute__((at(0x00007084))) = FAULT_VBUS_UVOT;            //欠压保护值
const float    faultOverVoltageRecover  __attribute__((at(0x00007088))) = FAULT_VBUS_OV_REV;          //过压保护恢复值
const float    faultUnderVoltageRecover __attribute__((at(0x0000708C))) = FAULT_VBUS_UV_REV;          //欠压保护恢复值
const float    faultOverCurrent         __attribute__((at(0x00007090))) = FAULT_OVER_CURRENT;         //相电流峰值过流保护值
//调速控制参数
const int8_t   speedControlMode         __attribute__((at(0x00007094))) = Speed_Control_Mode;         //调速模式选择
const float    vspStop                  __attribute__((at(0x00007098))) = VSP_REF_L;                  //VSP停止电压点
const float    vspStart                 __attribute__((at(0x0000709C))) = VSP_REF_M;                  //VSP启动电压点
const float    vspMax                   __attribute__((at(0x000070A0))) = VSP_REF_H;                  //VSP最大电压点
const int32_t  speedStart               __attribute__((at(0x000070A4))) = SPEED_START;               //VSP启动速度
const int32_t  speedMax                 __attribute__((at(0x000070A8))) = SPEED_MAX;                  //VSP最大速度
const int32_t  pwmStop                  __attribute__((at(0x000070AC))) = PWM_REF_L;                  //PWM停止占空比
const int32_t  pwmStart                 __attribute__((at(0x000070B0))) = PWM_SPEED_HOLD;             //PWM启动占空比
const int32_t  pwmHold                  __attribute__((at(0x000070B4))) = PWM_REF_M;                  //PWM保持占空比
const int32_t  pwmMax                   __attribute__((at(0x000070B8))) = PWM_REF_H;                  //PWM最大占空比
const int32_t  clkStop                  __attribute__((at(0x000070BC))) = CLK_REF_L;                  //CLK停止频率
const int32_t  clkStart                 __attribute__((at(0x000070C0))) = CLK_REF_M;                  //CLK启动频率
const int32_t  clkMax                   __attribute__((at(0x000070C4))) = CLK_REF_H;                  //CLK最大频率
const int8_t   controlMode              __attribute__((at(0x000070C8))) = Config_Contorl_Mode;        //控制方式
const int8_t   lockSpeedMode            __attribute__((at(0x000070CC))) = Lock_Speed_Mode;            //速度锁定
const int32_t  speedBaseValue           __attribute__((at(0x000070D0))) = MOTOR_SPEED_BASE;           //转速基值
const float    faultHardOverCurrent     __attribute__((at(0x000070D4))) = FAULT_HARD_OVER_CURRENT;    //硬件过流保护值
//软件版本号：001
const int8_t   version1                 __attribute__((at(0x000070D8))) = 0;                          //第一位
const int8_t   version2                 __attribute__((at(0x000070DC))) = 0;                          //第二位
const int8_t   version3                 __attribute__((at(0x000070E0))) = 1;                          //第三位
//软件ID号：1001
const int8_t   softID1                  __attribute__((at(0x000070E4))) = 1;                          //第一位
const int8_t   softID2                  __attribute__((at(0x000070E8))) = 0;                          //第二位
const int8_t   softID3                  __attribute__((at(0x000070EC))) = 0;                          //第三位
const int8_t   softID4                  __attribute__((at(0x000070F0))) = 1;                          //第四位


//---------------------------------------------------------------------------/
//	Global variable definitions(declared in header file with 'extern')
//---------------------------------------------------------------------------/
volatile struct_Config_t userHostSet = {0};    //HEX上位机配置变量结构体

//---------------------------------------------------------------------------/
//	Local function prototypes('static')
//---------------------------------------------------------------------------/

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : MC_HEX_Host_Init
* Description    : 速度环 / 功率环 功能
* Function Call  : 
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void  MC_HEX_Host_Init(void)
{

	//IO口配置
	if(INT8_VALUE(&breakMode) == Brake_Enable)
	{
		//刹车BRK
		GPIO_Init(PORT0,PIN6,PULLDOWN_INPUT);
//        GPIO_Init(PORT0,PIN4,PULLDOWN_INPUT);
	}
	else if(INT8_VALUE(&reverseMode) == Motor_ZF_Enable)
	{
		//正反转
		GPIO_Init(PORT0,PIN6,PULLDOWN_INPUT);
//        GPIO_Init(PORT0,PIN4,PULLDOWN_INPUT);
	}
//	if(INT8_VALUE(&controlMode) != VQ_Loop)
//    {
//        INT32_VALUE(&speedStart) = SPEED_START;
//        INT32_VALUE(&speedMax) = SPEED_MAX;
//             
//    }
//    else if(INT8_VALUE(&controlMode) == VQ_Loop)
//    {
//        INT32_VALUE(&speedStart) = VQ_RUN_VOLTAGE_MIN;
//        INT32_VALUE(&speedMax) = VQ_RUN_VOLTAGE_MAX;
//    }
	
	userHostSet.value.swGainVbus    = (HW_ADC_REF*8)/(FLOAT_VALUE(&busVoltage)*HW_VBUS_SDR*2);
	userHostSet.value.swGainIbus    = ((HW_ADC_REF*8)/((FLOAT_VALUE(&ibusResistance)*(MOTOR_POWER_BASE/FLOAT_VALUE(&busVoltage))*HW_AMP_GAIN_IBUS*1.2) + HW_AMP_REF_IBUS));
	userHostSet.value.calcCoeffVbus = _Q12((HW_VBUS_SDR*userHostSet.value.swGainVbus)/HW_ADC_REF);
	userHostSet.value.calcCoeffIbus = _Q12((FLOAT_VALUE(&ibusResistance)*HW_AMP_GAIN_IBUS*userHostSet.value.swGainIbus)/HW_ADC_REF);	
	userHostSet.value.iValue        = ((FLOAT_VALUE(&phaseResistance)*HW_AMP_GAIN_IP)/(HW_ADC_REF-HW_AMP_REF_IP));
	//VSP调速
	userHostSet.value.vspStopQ15    = (FLOAT_VALUE(&vspStop)*_Q15_VAL/HW_ADC_REF);
	userHostSet.value.vspStartQ15   = (FLOAT_VALUE(&vspStart)*_Q15_VAL/HW_ADC_REF);
	userHostSet.value.vspMaxQ15     = (FLOAT_VALUE(&vspMax)*_Q15_VAL/HW_ADC_REF);
	userHostSet.value.vspSpeedK     = ((INT32_VALUE(&speedMax) - INT32_VALUE(&speedStart))*_Q15_VAL/(userHostSet.value.vspMaxQ15 - userHostSet.value.vspStartQ15));
	userHostSet.value.vspSpeedB     = (INT32_VALUE(&speedStart) - (userHostSet.value.vspStartQ15*userHostSet.value.vspSpeedK/_Q15_VAL));
	//PWM调速
	userHostSet.value.pwmSpeedK     = ((INT32_VALUE(&speedMax) - INT32_VALUE(&speedStart))*1000/(INT32_VALUE(&pwmMax) - INT32_VALUE(&pwmHold)));
	userHostSet.value.pwmSpeedB     = (INT32_VALUE(&speedStart) - (INT32_VALUE(&pwmHold)*userHostSet.value.pwmSpeedK/1000));
	//CLK调速
	userHostSet.value.clkSpeedStart = INT32_VALUE(&clkStart)*4;
	userHostSet.value.clkSpeedMax   = INT32_VALUE(&clkMax)*4;
	userHostSet.value.clkSpeedK     = ((userHostSet.value.clkSpeedMax - userHostSet.value.clkSpeedStart)*INT32_VALUE(&clkMax)/(INT32_VALUE(&clkMax) - INT32_VALUE(&clkStart)));
	userHostSet.value.clkSpeedB     = (userHostSet.value.clkSpeedStart - (INT32_VALUE(&clkStart)*userHostSet.value.clkSpeedK/INT32_VALUE(&clkMax)));

  userHostSet.value.gainSpeedBase = ((_Q15_VAL)/ (INT32_VALUE(&speedBaseValue)*2));

	/*开启CCP模块时钟*/
	CGC_PER11PeriphClockCmd(CGC_PER11Periph_CCP,ENABLE);					
	if (INT8_VALUE(&speedControlMode) == CLK_Control)
  {
		CCP_ConfigCLK(CCP_CAPTURE_MODULE,CCP_CLK_DIV_64,CCP_RELOAD_CCPLOAD,0xffff);			/*Fahb=64M*/	
    GPIO_PinAFInConfig(CCP0BINCFG, 0x01);    /*设置P02为CCP0b捕获通道*/
    GPIO_Init(PORT0,PIN4,INPUT);	    
  }
	else if(INT8_VALUE(&speedControlMode) == PWM_Control)
  {
		CCP_ConfigCLK(CCP_CAPTURE_MODULE,CCP_CLK_DIV_4,CCP_RELOAD_CCPLOAD,0xffff);			/*Fahb=64M*/
    GPIO_PinAFInConfig(CCP0BINCFG, 0x01);    /*设置P02为CCP0b捕获通道*/
    GPIO_Init(PORT0,PIN4,INPUT);
  }
  else if(INT8_VALUE(&speedControlMode) == VSP_Control)
  {
    GPIO_Init(PORT0,PIN4,ANALOG_INPUT);
  }
	
}


/******************************** END OF FILE *******************************/

