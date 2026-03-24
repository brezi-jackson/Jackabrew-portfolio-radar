//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    mc_ctrl.c
* @author  CMS Motor Control Team
* @version V3.0.0
* @date    2021年3月
* @brief   
*---------------------------------------------------------------------------//
*****************************************************************************/
//==========================================================================//

#ifndef  __MOTOR_CONTROL_H__
#define  __MOTOR_CONTROL_H__

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include 	<stdint.h>
#include  "base_define.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
#define Fault_Flag		    				u_Fault_Flag.Byte
#define	Fault_ShortCircuit				u_Fault_Flag.Bits.Bit0
#define	Fault_UnderVoltage				u_Fault_Flag.Bits.Bit7	
#define	Fault_StartFail						u_Fault_Flag.Bits.Bit6 
#define	Fault_OverVoltage					u_Fault_Flag.Bits.Bit5 
#define	Fault_TorgueLoss					u_Fault_Flag.Bits.Bit4 
#define	Fault_OverCurrent					u_Fault_Flag.Bits.Bit3 
#define	Fault_PhaseLoss						u_Fault_Flag.Bits.Bit2 
#define	Fault_Block								u_Fault_Flag.Bits.Bit1



/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/

//---------------------------------------------------------------------------/

typedef enum 
{
	SYS_INIT		= 0,
	SYS_RESTART	= 1,
	SYS_RUN			= 2,
	SYS_FAULT		= 3,
	SYS_STOP		= 4,
	SYS_WAIT		= 5
}SystStates;

//---------------------------------------------------------------------------/
typedef enum 
{
	MC_INIT			= 0, 
	MC_CHARGE		= 1, 
	MC_WIND			= 2,
	MC_ALIGN		= 3,	
	MC_START		= 4, 
	MC_SW				= 5, 
	MC_RUN			= 6,   
	MC_STOP			= 7, 
	MC_BRAKE		= 8,
	MC_FAULT		= 9
}MotorStates;

//---------------------------------------------------------------------------/
typedef struct
{
		//----------------------------------------------------------------------/
		float			Ub;
		float			Ib;
		float			Rb;	
		float			Lb;	
		float			Fb;	
		float			Tb;	
		float			Wb;	
	
		//----------------------------------------------------------------------/	
		float			Us;
		float			Rs;	
		float			Ls;
		float			Ts;	
		float			Poles;
	
		//----------------------------------------------------------------------/	
		int32_t		Rp;
		int32_t		Lp;	
		int32_t		Tp;		
		int32_t		Fp;	
	
		//----------------------------------------------------------------------/
		int32_t		B;
		int32_t		C;	
		int32_t		G;		
		
		//----------------------------------------------------------------------/
		
}struct_Base_Value;


//---------------------------------------------------------------------------/
typedef struct 
{
		int32_t 	RampInc;
		int32_t 	RampDec;
		int32_t 	RampTarget;
		int32_t 	RampOut;
		int32_t 	RampBack;
		int32_t 	RampRef;
}struct_SpeedRamp;

//---------------------------------------------------------------------------/
typedef struct 
{
		uint8_t		Mode_Control;
		uint8_t		Mode_Shunt;
		uint8_t		Mode_Svpwm;
		uint8_t		Mode_Wind;
		uint8_t		MotorDir;
		uint8_t		Mode_IPD;
		uint8_t		Mode_Weeken;
		uint8_t		Mode_DeadComp;	
		uint8_t		Mode_Start;			
		uint8_t		Mode_Block;		
}struct_Config;



//---------------------------------------------------------------------------/
typedef struct
{
		int32_t 	Vbus;
		int32_t		Ibus;
		int32_t		VCtr;
		int32_t		Power;	
		int32_t		dq_power;	
		int32_t		Speed_Close;	
		int32_t		Speed_Min;
		int32_t		Speed_Base;
		int32_t		Speed_Stop;	
		int32_t		Speed_Back;	
		int32_t 	Speed_Switch;
		int32_t		Speed_Brake;	
		int32_t		Calb_Speed;
		int32_t		Calb_SpeedLimit;
		int32_t		Calb_Speed_Min;
		int32_t		Calb_Speed_Max;	
		int32_t		Calb_Vbus;
		int32_t		Mech_Speed;
		int32_t		FG_Speed;
		int32_t 	We;
		int32_t		Freq_EPWM;
		int32_t 	Tpwm_us;
		int32_t 	We_Ke;	
		int32_t		Speed_Ke;
		float			MotorVolt;
		float			MotorCurr;	
		float			Ke;

}struct_Basic_Para;

//---------------------------------------------------------------------------/
typedef	union
{
	unsigned char Byte;
	struct
	{
		unsigned char	Bit0	:1;
		unsigned char	Bit1	:1;
		unsigned char	Bit2	:1;
		unsigned char	Bit3	:1;
		unsigned char	Bit4	:1;
		unsigned char	Bit5	:1;
		unsigned char	Bit6	:1;
		unsigned char	Bit7	:1;
	}Bits;
}Union_Fault;

//---------------------------------------------------------------------------/
typedef struct
{
	s32			Speed_Ref; 
	s32			Speed_Ref_Min; 	
	s32			Curr_Is_Ref; 
	s32			Curr_Iq_Max;
	s32			Curr_Iq_Min;
	s32 		Elec_Angle;
	u32 		MotorRunTime;	
	s32 		Is_beta;
	s32 		IPD_Angle;
	u16 		W_Cycle;
	u16 		I_Cycle;	
	u16 		WRAMP;
	u16 		IRAMP;
	u16 		FRAMP;
	u16 		SRAMP;	
	s32 		Vs_Max;
	s32 		Vs_Square_Max;
	u16 		EPWM_Period;
	u16 		EPWM_SAMPDLY;
	u16 		TSAM;

	s32			Weakening;
	s32			Vout;
	s32			PowerLimit;
	s32			Elec_We;
	s32			Offset_Angle;
	s32			Volt_Uq_Ref;   		//Vq模式
	s32			CurrentLimit; 		//2022-06-27		
	s32			VoltageDutySafe;  //2022-06-29
  u32			DeadTime;  //2023-03-30
  u32			VoiceDownDeadTime;  //2023-03-30
}struct_FOC_Ctrl;

//---------------------------------------------------------------------------/
typedef struct
{
	u8  		IP_CH;
	s32 		ADVal[6];
	s32 		ARR[6];
	s32 		REF[6];
	s32			G_Ip; 
	s32			G_Ib; 
	s32			G_Vb; 
	s32			G_Vc; 
	s32			G_Sp; 
	s32			G_Pw;
	
	s32			ADPGACal;
}struct_ADC_Sample;

//---------------------------------------------------------------------------/
typedef struct
{
	u8			MC_RunStop	:1;
	u8			Speed_Ramp	:1;
	u8			IPD					:1;	
	u8			Charge			:1;
	u8			MotorBack		:1;
	u8			MotorFR;
	u8			MotorFR_Pre;

}struct_Falg;

//---------------------------------------------------------------------------/
typedef struct
{
	u8	 		Sample_Chenal;
	u16  		Charge_Count;
	u16	 		Motor_Restart;
	u16			FG_Count;
	u32	 		Motor_Stophold;
	u32	 		Motor_StartFail;	
	u32	 		Motor_Brake;
	u32	 		Motor_Block;	
}struct_Time;

//---------------------------------------------------------------------------/
typedef struct
{
	u16  		Charge_Count;
	u32	 		Motor_Restart;

	u16  		TimeBase[5];

}struct_Count;

//---------------------------------------------------------------------------/
typedef struct
{
	u8			Block_Cnt;
	u8			PhaseLoss_Cnt;
	u8			StartFail_Cnt;
	u8			Brake_Cnt;
	u8			TorgueLoss_Cnt;
	u8			Brake;
	u8			Braking;
	u16			PhaseA_LossCnt;
	u16			PhaseB_LossCnt;
	u16			PhaseC_LossCnt;
	u16			Phaseloss_Value;
	u32			OverVoltage_Value;
	u32			LowerVoltage_Value;
	u32			OV_Recover_Value;
	u32			UV_Recover_Value;
	s32			OverCurrent_Value;
	s32			We_Rate;
	s32			Ia_max;
	s32			Ib_max;
	s32			Ic_max;	
}struct_Fault;

//---------------------------------------------------------------------------/
typedef struct 
{
	u8		CompleteFlag:1;				//捕获完成标志
	u8		OverFlowFlag:1;				//计数溢出标志
	u16		CaptureCount;					//采集顺序
	u16 	WeakenTime;						//溢出电平拉低消抖计时
	u16 	HeightenTime;					//溢出电平拉高消抖计时
	u32		CaptureValue[4];			//捕获的值
	u32 	OverFlowTime;					//溢出时间计数	
	u32		CaptureTimeL;					//捕获的低电平时间
	u32		CaptureTimeH;					//捕获的高电平时间	
	
	u32		Frequency;						//频率
	u32		DutyCycle;						//占空比
	u32		FrequencyOld;					//频率旧值
	u32		DutyCycleOld;					//占空比旧值
	u32		FrequencyTemp;				//频率缓冲
	u32		DutyCycleTemp;				//占空比缓冲

	u32		FG_SpeedOld;					//速度旧值
	u16		FG_SpeedMin;					//FG输出速度最小值
	u16		FG_Period;            //FG输出周期
	u32		CaptureTimeCycle;			//捕获的高低电平时间	
  
}struct_Capture;

//---------------------------------------------------------------------------/
typedef struct 
{
	u8		SleepFlag:1;				//
	u16		SleepCount;					//
}struct_Sleep;

//---------------------------------------------------------------------------/
typedef struct 
{
	s32		Ls;
	s32		Rs;
	s32		Paris;
	s32		Ke;
	s32		Voltage;
	s32		Current;
	s32		Power;
	
}BASEPARA;

//---------------------------------------------------------------------------/
typedef struct 
{
	u8		RunFlag:1;
	u8		Md_Sensor:1;
	u8		Md_Shunt:1;
	u8		Md_Svpwm:1;
	u8		Md_Wind	:1;
	u8		MotorDir:1;
	u8		Md_Debug;
	u8		Md_IPD;	
	u8		Md_DeadComp;	
	u8		Md_Control;
	u8		Md_SafeTest;
}CTRPARA;

//---------------------------------------------------------------------------/
typedef struct 
{	
	s32		Dkp;
	s32		Dki;
	s32		Dout_max;
	s32		Dout_min;
	s32		Qkp;
	s32		Qki;
	s32		Qout_max;
	s32		Qout_min;
	s32		Ekp;
	s32		Eki;
	s32		Eout_max;
	s32		Eout_min;
	s32		Start_Current;
}WINDPARA;

//---------------------------------------------------------------------------/
typedef struct 
{
	u8		mode;
	u16		rpm_init;
	u16		rpm_final;
	s32 	align_current;
	u32		align_time;
	u32		align_addtime;
	u32		align_holdtime;
	u32		align_add_k;	
	s32		run_current_init;
	s32		run_current_final;
	u32		run_time;
	s32		Dkp;
	s32		Dki;
	s32		Dout_max;
	s32		Dout_min;
	s32		Qkp;
	s32		Qki;
	s32		Qout_max;
	s32		Qout_min;
	s32		Ekp;
	s32		Eki;
	s32		Eout_max;
	s32		Eout_min;
	s32		EInt_max;
	s32		EInt_min;
	s32		Etheta;
	s32		Offset_Ki;
	s32		Offset_Ku;
	s32		ERampInc;
	s32		ERampDec;
	u32		Hold_Time;
}STARTPARA;

//---------------------------------------------------------------------------/
typedef struct 
{
	s32		Dkp;
	s32		Dki;
	s32		Dout_max;
	s32		Dout_min;
	s32		Qkp;
	s32		Qki;
	s32		Qout_max;
	s32		Qout_min;
	s32		Wkp;
	s32		Wki;
	s32		Wkd;
	s32		Wout_max;
	s32		Wout_min;
	s32		OffsetTheta;
	s32		Ekp;
	s32		Eki;
	s32		Fc_LPF;	
}FOCPARA;

//---------------------------------------------------------------------------/
typedef struct 
{	

	s32		Samp_Ibus;
	s32		Samp_Vbus;
	s32		Samp_Vctrl;
	s32		Samp_Vtem;
	s32		Speed_Val;
	s32		Power_Val;
	s32		PhaseK;
}LPFPARA;


//---------------------------------------------------------------------------/
typedef struct	
{
	BASEPARA	Base;
	CTRPARA		Ctrl;
	WINDPARA	Wind;
	STARTPARA	Start;
	FOCPARA		FOC;
	LPFPARA		Lpf;

}struct_MotCtrl_Para;

//---------------------------------------------------------------------------/


/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern volatile Union_Fault				u_Fault_Flag;
extern volatile SystStates 				SYSTEM_STATE;
extern volatile MotorStates 			MOTOR_STATE;
extern struct_Base_Value					stru_base_value;
extern struct_SpeedRamp						stru_Speed_Ctrl;
extern struct_ADC_Sample 					stru_Sample;
extern struct_FOC_Ctrl						stru_FOC;
extern struct_Basic_Para					Basic;
extern struct_Config							Config;
extern struct_Falg 								Flag;
extern struct_Time    						Time; 
extern struct_Count   						Count; 
extern struct_Fault 							Fault;
extern struct_MotCtrl_Para				Para;
extern struct_Capture							Capture;


/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
void	MC_BaseValue_Init(void);
void 	MC_MotorInit_Process(void);
void 	System_Control(void);
void 	MC_MotorFault_Check(void);
void 	MC_MotorSpeed_Calc(void);
void  MC_Speed_Control(void);
void 	MC_Clear_FaultMessage(void);
void 	SysTick_1ms(void);
void 	UartView(s16 view1,s16 view2,s16 view3,s16 view4);
void 	User_Motor_On(void);
void 	User_Motor_Off(void);
void 	User_Motor_FRControl(u8 Input_Val);
void 	User_MotorSpeed_Set(s32 Input_Val);
void 	System_Modules_off(void);
void 	User_Speed_Control(void);
void  Capture_Mode2_Handle(void);
void 	User_Speed_Capture(void);
void 	User_Speed_Out(void);
void 	CCP_Capture_Init(void);
void 	CCP_PWM_Init(void);
void 	User_Voice_Control(void);
void 	User_Sleep_Manage(void);
u8   	API_Get_MotorStates(void);
u8 	 	API_Get_FaultStates(void);
s32  	API_Get_MotorSpeed(void);
void 	API_Speed_Control(s32 speed_val, u8 speed_en);
void  WDT_Restart(void);
	
void 	FMC_Write(uint32_t fmc_adress,uint32_t fmc_data);
void 	FMC_Erase(uint32_t fmc_adress,u8 Erase_Mode);
#endif	/* __MOTOR_CONTROL_H__ */

