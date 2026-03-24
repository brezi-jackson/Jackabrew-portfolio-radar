//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    user_hostinithex.c
* @author  CMS Appliction Team 
* @version 0.0.1
* @date    2023年1月
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

//软件版本号：1
const int8_t   version                  __attribute__((at(0x00008000))) = 1;                          //
//软件ID号：1
const int8_t   softID                   __attribute__((at(0x00008004))) = 1;                          //

//功能模式参数
const int8_t   pcbHallMode              __attribute__((at(0x00008008))) = Config_PCB_Hall_Mode;       //PCB霍尔表格选择(预留)
const int8_t   motorFR                  __attribute__((at(0x0000800C))) = Config_Motor_FR;            //电机旋转方向
const int8_t   sleepMode                __attribute__((at(0x00008010))) = Sleep_Control_Mode;         //休眠功能
const int8_t   reverseMode              __attribute__((at(0x00008014))) = Motor_ZF_Control_Mode;      //正反转功能
const int8_t   breakMode                __attribute__((at(0x00008018))) = Brake_Control_Mode;         //刹车功能
const int32_t  breakSpeed               __attribute__((at(0x0000801C))) = Motor_Brake_Speed;          //刹车转速
const int32_t  breakTime                __attribute__((at(0x00008020))) = Motor_Brake_Time;           //刹车时间

//电机及硬件参数
const int8_t   motorPairs               __attribute__((at(0x00008024))) = MOTOR_PAIRS;                //电机极对数
const float    motorRS                  __attribute__((at(0x00008028))) = MOTOR_RS;                   //电机相电阻
const float    motorLS                  __attribute__((at(0x0000802C))) = MOTOR_LS;                   //电机相电感
const float    busVoltage               __attribute__((at(0x00008030))) = HW_VBUS_VOLT;               //母线电压
const float    phaseResistance          __attribute__((at(0x00008034))) = HW_PHASE_RES;               //相采样电阻
const float    ibusResistance           __attribute__((at(0x00008038))) = HW_IBUS_RES;                //母线采样电阻

//启动参数
const int8_t   startMode                __attribute__((at(0x0000803C))) = Config_Start_Mode;          //启动方式
const float    startCurrentInit         __attribute__((at(0x00008040))) = START_CURRENT_INIT;         //启动电流初始值
const float    startCurrentFinal        __attribute__((at(0x00008044))) = START_CURRENT_FINAL;        //启动电流最大值
const int32_t  startIRamp               __attribute__((at(0x00008048))) = START_IRAMP;                //启动电流爬坡周期
const int32_t  satrtEKP                 __attribute__((at(0x0000804C))) = START_EKP;                  //启动估算器KP
const int32_t  satrtEKI                 __attribute__((at(0x00008050))) = START_EKI;                  //启动估算器KI
const int32_t  satrtEOutMin             __attribute__((at(0x00008054))) = START_EOUT_MIN;             //启动初始角速度

//运行参数
const int32_t  speedRampInc             __attribute__((at(0x00008058))) = SPEED_RAMP_INC;             //速度爬坡曲线，加速斜率
const int32_t  speedRampDec             __attribute__((at(0x0000805C))) = SPEED_RAMP_DEC;             //速度爬坡曲线，下降斜率
const float    focWKP                   __attribute__((at(0x00008060))) = FOC_WKP;                    //速度环KP
const float    focWKI                   __attribute__((at(0x00008064))) = FOC_WKI;                    //速度环KI
const float    focWKD                   __attribute__((at(0x00008068))) = FOC_WKD;                    //速度环KD
const float    focRunCurrentMin         __attribute__((at(0x0000806C))) = FOC_RUN_CURRENT_MIN;        //电机运行最小相电流峰值
const float    focRunCurrentMax         __attribute__((at(0x00008070))) = FOC_RUN_CURRENT_MAX;        //电机运行最大相电流峰值
const float    vectorVoltageMax         __attribute__((at(0x00008074))) = MAX_VOLTAGE_VECTOR;         //占空比开度

//故障保护参数
const int32_t  faultBlockTime           __attribute__((at(0x00008078))) = FAULT_BLOCK_TIME;           //堵转判别时间
const int32_t  faultRestartDelayTime    __attribute__((at(0x0000807C))) = FAULT_RESTART_DELAY_TIME;   //故障保护重启间隔时间
const int32_t  faultSpeedMax            __attribute__((at(0x00008080))) = FAULT_SPEED_MAX;            //堵转保护最大转速值
const int32_t  faultSpeedMin            __attribute__((at(0x00008084))) = FAULT_SPEED_MIN;            //堵转保护最小转速值
const float    faultOverVoltage         __attribute__((at(0x00008088))) = FAULT_VBUS_OVOT;            //过压保护值
const float    faultUnderVoltage        __attribute__((at(0x0000808C))) = FAULT_VBUS_UVOT;            //欠压保护值
const float    faultOverVoltageRecover  __attribute__((at(0x00008090))) = FAULT_VBUS_OV_REV;          //过压保护恢复值
const float    faultUnderVoltageRecover __attribute__((at(0x00008094))) = FAULT_VBUS_UV_REV;          //欠压保护恢复值
const float    faultOverCurrent         __attribute__((at(0x00008098))) = FAULT_OVER_CURRENT;         //相电流峰值过流保护值

//调速控制参数
const int8_t   speedControlMode         __attribute__((at(0x0000809C))) = Speed_Control_Mode;         //调速模式选择
const float    vspStop                  __attribute__((at(0x000080A0))) = VSP_REF_L;                  //VSP停止电压点
const float    vspStart                 __attribute__((at(0x000080A4))) = VSP_REF_M;                  //VSP启动电压点
const float    vspMax                   __attribute__((at(0x000080A8))) = VSP_REF_H;                  //VSP最大电压点
const int32_t  speedStart               __attribute__((at(0x000080AC))) = SPEED_START;                //启动速度
const int32_t  speedMax                 __attribute__((at(0x000080B0))) = SPEED_MAX;                  //最大速度
const int32_t  pwmStop                  __attribute__((at(0x000080B4))) = PWM_REF_L;                  //PWM停止占空比
const int32_t  pwmStart                 __attribute__((at(0x000080B8))) = PWM_SPEED_HOLD;             //PWM启动占空比
const int32_t  pwmHold                  __attribute__((at(0x000080BC))) = PWM_REF_M;                  //PWM保持占空比
const int32_t  pwmMax                   __attribute__((at(0x000080C0))) = PWM_REF_H;                  //PWM最大占空比
const int32_t  clkStop                  __attribute__((at(0x000080C4))) = CLK_REF_L;                  //CLK停止频率
const int32_t  clkStart                 __attribute__((at(0x000080C8))) = CLK_REF_M;                  //CLK启动频率
const int32_t  clkMax                   __attribute__((at(0x000080CC))) = CLK_REF_H;                  //CLK最大频率
const int8_t   controlMode              __attribute__((at(0x000080D0))) = Config_Contorl_Mode;        //控制方式
const int8_t   lockSpeedMode            __attribute__((at(0x000080D4))) = Lock_Speed_Mode;            //速度锁定
const int32_t  speedBaseValue           __attribute__((at(0x000080D8))) = MOTOR_SPEED_BASE;           //转速基值
const float    faultHardOverCurrent     __attribute__((at(0x000080DC))) = FAULT_HARD_OVER_CURRENT;    //硬件过流保护值
const int32_t  focOffsetAngle           __attribute__((at(0x000080E0))) = FOC_OFFSET_ANGLE;           //FOC补偿角
const float    vqRunVoltageMin          __attribute__((at(0x000080E4))) = VQ_RUN_VOLTAGE_MIN;         //电机运行Vq最小值
const float    vqRunVoltageMax          __attribute__((at(0x000080E8))) = VQ_RUN_VOLTAGE_MAX;         //电机运行Vq最大值
const int32_t  vqLoopLimitSpeedValue    __attribute__((at(0x000080EC))) = VQ_LOOP_LIMIT_SPEED_VALUE;	//VQ控制 限制最大转速值
const float    vqLoopLimitCurrentValue  __attribute__((at(0x000080F0))) = VQ_LOOP_LIMIT_CURRENT_VALUE; //VQ控制  限制最大相电流值

//---------------------------------------------------------------------------/
//	Global variable definitions(declared in header file with 'extern')
//---------------------------------------------------------------------------/
volatile   struct_Config_t userHostSet = {0};    //HEX上位机配置变量结构体
signed int HALL_Angle_CW[8][2]         = {0};
signed int HALL_Angle_CCW[8][2]        = {0};
signed int HALL_AD_Value[8]            = {0};

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
	}
	else if(INT8_VALUE(&reverseMode) == Motor_ZF_Enable)
	{
		//正反转
		GPIO_Init(PORT0,PIN6,PULLDOWN_INPUT);
	}
	
	//PCB霍尔表格选择
	if(INT8_VALUE(&pcbHallMode))
	{
		//CW方向扇区表格
		HALL_Angle_CW[0][0]  = 0; HALL_Angle_CW[0][1]  = DEG_330;
		HALL_Angle_CW[1][0]  = 1; HALL_Angle_CW[1][1]  = DEG_30;
		HALL_Angle_CW[2][0]  = 2; HALL_Angle_CW[2][1]  = DEG_270;
		HALL_Angle_CW[3][0]  = 3; HALL_Angle_CW[3][1]  = DEG_90;
		HALL_Angle_CW[4][0]  = 4; HALL_Angle_CW[4][1]  = DEG_210;
		HALL_Angle_CW[5][0]  = 5; HALL_Angle_CW[5][1]  = DEG_150;
		HALL_Angle_CW[6][0]  = 6; HALL_Angle_CW[6][1]  = DEG_150;
		HALL_Angle_CW[7][0]  = 7; HALL_Angle_CW[7][1]  = DEG_90;
		//CCW方向扇区表格
		HALL_Angle_CCW[0][0] = 0; HALL_Angle_CCW[0][1] = DEG_150;
		HALL_Angle_CCW[1][0] = 1; HALL_Angle_CCW[1][1] = DEG_90;
		HALL_Angle_CCW[2][0] = 2; HALL_Angle_CCW[2][1] = DEG_210;
		HALL_Angle_CCW[3][0] = 3; HALL_Angle_CCW[3][1] = DEG_30;
		HALL_Angle_CCW[4][0] = 4; HALL_Angle_CCW[4][1] = DEG_270;
		HALL_Angle_CCW[5][0] = 5; HALL_Angle_CCW[5][1] = DEG_330;
		HALL_Angle_CCW[6][0] = 6; HALL_Angle_CCW[6][1] = DEG_30;
		HALL_Angle_CCW[7][0] = 7; HALL_Angle_CCW[7][1] = DEG_90;
		//HALL电压采样值
		HALL_AD_Value[0]     = 0;
		HALL_AD_Value[1]     = 54;
		HALL_AD_Value[2]     = 598;
		HALL_AD_Value[3]     = 1072;
		HALL_AD_Value[4]     = 2644;
		HALL_AD_Value[5]     = 3298;
		HALL_AD_Value[6]     = 4042;
		HALL_AD_Value[7]     = 4500;
	}
	else
	{
		//CW方向扇区表格
		HALL_Angle_CW[0][0]  = 0; HALL_Angle_CW[0][1]  = DEG_150;
		HALL_Angle_CW[1][0]  = 1; HALL_Angle_CW[1][1]  = DEG_270;
		HALL_Angle_CW[2][0]  = 2; HALL_Angle_CW[2][1]  = DEG_210;
		HALL_Angle_CW[3][0]  = 3; HALL_Angle_CW[3][1]  = DEG_30;
		HALL_Angle_CW[4][0]  = 4; HALL_Angle_CW[4][1]  = DEG_90;
		HALL_Angle_CW[5][0]  = 5; HALL_Angle_CW[5][1]  = DEG_330;
		HALL_Angle_CW[6][0]  = 6; HALL_Angle_CW[6][1]  = DEG_150;
		HALL_Angle_CW[7][0]  = 7; HALL_Angle_CW[7][1]  = DEG_90;
		//CCW方向扇区表格
		HALL_Angle_CCW[0][0] = 0; HALL_Angle_CCW[0][1] = DEG_270;
		HALL_Angle_CCW[1][0] = 1; HALL_Angle_CCW[1][1] = DEG_150;
		HALL_Angle_CCW[2][0] = 2; HALL_Angle_CCW[2][1] = DEG_210;
		HALL_Angle_CCW[3][0] = 3; HALL_Angle_CCW[3][1] = DEG_30;
		HALL_Angle_CCW[4][0] = 4; HALL_Angle_CCW[4][1] = DEG_330;
		HALL_Angle_CCW[5][0] = 5; HALL_Angle_CCW[5][1] = DEG_90;
		HALL_Angle_CCW[6][0] = 6; HALL_Angle_CCW[6][1] = DEG_30;
		HALL_Angle_CCW[7][0] = 7; HALL_Angle_CCW[7][1] = DEG_90;
		//HALL电压采样值
		HALL_AD_Value[0]     = 0;
		HALL_AD_Value[1]     = 585;
		HALL_AD_Value[2]     = 1085;
		HALL_AD_Value[3]     = 1658;
		HALL_AD_Value[4]     = 1973;
		HALL_AD_Value[5]     = 2631;
		HALL_AD_Value[6]     = 3313;
		HALL_AD_Value[7]     = 4000;		
	}
	
	//VQ环运行值
  userHostSet.value.vqRunMin = _Q15(FLOAT_VALUE(&vqRunVoltageMin));
  userHostSet.value.vqRunMax = _Q15(FLOAT_VALUE(&vqRunVoltageMax));
	
	if(INT8_VALUE(&controlMode) != VQ_Loop)
	{
		userHostSet.value.runSpeedStart = INT32_VALUE(&speedStart);
		userHostSet.value.runSpeedMax   = INT32_VALUE(&speedMax);
	}
	else
	{
		userHostSet.value.runSpeedStart = userHostSet.value.vqRunMin;
		userHostSet.value.runSpeedMax   = userHostSet.value.vqRunMax;
	}
	
	userHostSet.value.swGainVbus    = (HW_ADC_REF*8)/(FLOAT_VALUE(&busVoltage)*HW_VBUS_SDR*2);
	userHostSet.value.swGainIbus    = ((HW_ADC_REF*8)/((FLOAT_VALUE(&ibusResistance)*(MOTOR_POWER_BASE/FLOAT_VALUE(&busVoltage))*HW_AMP_GAIN_IBUS*1.2) + HW_AMP_REF_IBUS));
	userHostSet.value.calcCoeffVbus = _Q12((HW_VBUS_SDR*userHostSet.value.swGainVbus)/HW_ADC_REF);
	userHostSet.value.calcCoeffIbus = _Q12((FLOAT_VALUE(&ibusResistance)*HW_AMP_GAIN_IBUS*userHostSet.value.swGainIbus)/HW_ADC_REF);	
	userHostSet.value.iValue        = ((FLOAT_VALUE(&phaseResistance)*HW_AMP_GAIN_IP)/(HW_ADC_REF-HW_AMP_REF_IP));
  userHostSet.value.gainSpeedBase = ((_Q15_VAL)/ (INT32_VALUE(&speedBaseValue)*2));
	
	//VSP调速
	userHostSet.value.vspStopQ15    = (FLOAT_VALUE(&vspStop)*_Q15_VAL/HW_ADC_REF);
	userHostSet.value.vspStartQ15   = (FLOAT_VALUE(&vspStart)*_Q15_VAL/HW_ADC_REF);
	userHostSet.value.vspMaxQ15     = (FLOAT_VALUE(&vspMax)*_Q15_VAL/HW_ADC_REF);
	userHostSet.value.vspSpeedK     = ((userHostSet.value.runSpeedMax - userHostSet.value.runSpeedStart)*_Q15_VAL/(userHostSet.value.vspMaxQ15 - userHostSet.value.vspStartQ15));
	userHostSet.value.vspSpeedB     = (userHostSet.value.runSpeedStart - (userHostSet.value.vspStartQ15*userHostSet.value.vspSpeedK/_Q15_VAL));
	//PWM调速
	userHostSet.value.pwmSpeedK     = ((userHostSet.value.runSpeedMax - userHostSet.value.runSpeedStart)*1000/(INT32_VALUE(&pwmMax) - INT32_VALUE(&pwmHold)));
	userHostSet.value.pwmSpeedB     = (userHostSet.value.runSpeedStart - (INT32_VALUE(&pwmHold)*userHostSet.value.pwmSpeedK/1000));
	//CLK调速
	if(INT8_VALUE(&controlMode) != VQ_Loop)
	{
		userHostSet.value.clkSpeedStart = INT32_VALUE(&clkStart)*4;
		userHostSet.value.clkSpeedMax   = INT32_VALUE(&clkMax)*4;
	}
	else
	{
		userHostSet.value.clkSpeedStart = userHostSet.value.runSpeedStart;
		userHostSet.value.clkSpeedMax   = userHostSet.value.runSpeedMax;
	}
	userHostSet.value.clkSpeedK     = ((userHostSet.value.clkSpeedMax - userHostSet.value.clkSpeedStart)*INT32_VALUE(&clkMax)/(INT32_VALUE(&clkMax) - INT32_VALUE(&clkStart)));
	userHostSet.value.clkSpeedB     = (userHostSet.value.clkSpeedStart - (INT32_VALUE(&clkStart)*userHostSet.value.clkSpeedK/INT32_VALUE(&clkMax)));

	/*开启CCP模块时钟*/
	CGC_PER11PeriphClockCmd(CGC_PER11Periph_CCP,ENABLE);					
	if (INT8_VALUE(&speedControlMode) == CLK_Control)
  {
		CCP_ConfigCLK(CCP_CAPTURE_MODULE,CCP_CLK_DIV_64,CCP_RELOAD_CCPLOAD,0xffff);			/*Fahb=64M*/	
    GPIO_PinAFInConfig(CCP0BINCFG, 0x01);    /*设置P04为CCP0b捕获通道*/
    GPIO_Init(PORT0,PIN4,INPUT);	
  }
	else if(INT8_VALUE(&speedControlMode) == PWM_Control)
  {
		CCP_ConfigCLK(CCP_CAPTURE_MODULE,CCP_CLK_DIV_4,CCP_RELOAD_CCPLOAD,0xffff);			/*Fahb=64M*/
    GPIO_PinAFInConfig(CCP0BINCFG, 0x01);    /*设置P04为CCP0b捕获通道*/
    GPIO_Init(PORT0,PIN4,INPUT);	
  }
  else if(INT8_VALUE(&speedControlMode) == VSP_Control)
  {
    GPIO_Init(PORT0,PIN4,ANALOG_INPUT);
  }
	
}

/******************************** END OF FILE *******************************/
