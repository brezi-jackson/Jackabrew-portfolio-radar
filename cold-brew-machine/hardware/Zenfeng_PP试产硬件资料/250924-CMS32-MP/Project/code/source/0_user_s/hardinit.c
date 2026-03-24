//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    hardinit.c
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

/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/

/*****************************************************************************
 ** \brief	系统时钟初始化
 **			
 ** \param [in] none
 ** \return  none
 ** \note	64M主频
*****************************************************************************/
void SysClock_Init(void)
{
	SystemCoreClockUpdate();
  
}

/*****************************************************************************
 ** \brief	PORT初始化
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PORT_Init(void)
{  
  //GPIO_Init(PORT0,PIN3,OUTPUT);
  GPIO_Init(PORT2,PIN1,OUTPUT);
  
}

/*****************************************************************************
 ** \brief	EPWM模块初始化，双电阻方案
 **			
 ** \param [in] none
 ** \return  none
 ** \note	除了刹车和重映射之外，其余代码不允许改动
*****************************************************************************/
void EPWM_R2_Init(void)
{
	u32 T_CPMTG0,T_CPMTG1;
  
	/* 
	(1)设置EPWM时钟
	*/
	CGC_PER11PeriphClockCmd(CGC_PER11Periph_EPWM,ENABLE);	/*开启EPWM时钟*/
	
	/* 
	(2)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode(  EPWM_COUNT_UP_DOWN  | 				/*xx计数模式*/
						 EPWM_OCU_SYMMETRIC 	  |			/*对称计数模式*/
						 EPWM_WFG_COMPLEMENTARYK   |		/*互补模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
  
	/*
	(3)设置EPWM时钟周期
	*/	
	EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelClk( EPWM2, EPWM_CLK_DIV_1);	
	EPWM_ConfigChannelClk( EPWM4, EPWM_CLK_DIV_1);	
	
	EPWM_ConfigChannelPeriod(EPWM0,  EPWM_PERIOD);
	EPWM_ConfigChannelPeriod(EPWM2,  EPWM_PERIOD);
	EPWM_ConfigChannelPeriod(EPWM4,  EPWM_PERIOD);
	EPWM_ConfigChannelSymDuty(EPWM0, EPWM_HALFPERIOD);	
	EPWM_ConfigChannelSymDuty(EPWM2, EPWM_HALFPERIOD);		
	EPWM_ConfigChannelSymDuty(EPWM4, EPWM_HALFPERIOD);
	
	/*
	(4)设置EPWM反向输出
	*/
	#if 0
		EPWM_EnableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK |EPWM_CH_2_MSK|
								EPWM_CH_3_MSK| EPWM_CH_4_MSK|EPWM_CH_5_MSK);
	#else
		EPWM_DisableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK |EPWM_CH_2_MSK|
							EPWM_CH_3_MSK| EPWM_CH_4_MSK|EPWM_CH_5_MSK);
		#endif
		
	/*
	(5)设置EPWM死区
	*/
	EPWM_EnableDeadZone(0x3F, (uint32_t)EPWM_DT);

	/*
	(6)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK |EPWM_CH_2_MSK|EPWM_CH_4_MSK);				/*自动加载*/
	
	/*
	(7)设置EPWM比较器0
	*/			
	T_CPMTG0 = EPWM_CPMTG;
	if(T_CPMTG0 < 12)	T_CPMTG0 = 12; 	//触发计数器不允许为0
	if(T_CPMTG0 > 128)	T_CPMTG0 = 128; 
	T_CPMTG1 = EPWM_PERIOD - T_CPMTG0;
	EPWM_ConfigCompareTriger(EPWM_CMPTG_0,EPWM_CMPTG_FALLING,EPWM_CMPTG_EPWM0,T_CPMTG0);
	EPWM_ConfigCompareTriger(EPWM_CMPTG_1,EPWM_CMPTG_RISING,EPWM_CMPTG_EPWM0,T_CPMTG1);
	
	/*
	(8)设置EPWM刹车
	*/
	#if (ACMP_CH == ACMP_CH0)
	EPWM_EnableFaultBrake(EPWM_BRK_ACMP0EE);				/*ACMP0刹车*/	
	#else
	EPWM_EnableFaultBrake(EPWM_BRK_ACMP1EE);				/*ACMP1刹车*/	
	#endif
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_0_MSK | EPWM_CH_2_MSK |EPWM_CH_4_MSK,0);
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_1_MSK | EPWM_CH_3_MSK |EPWM_CH_5_MSK,0);
	EPWM_AllBrakeEnable();
	
	Brige_Output_Off();
	/*
	(9)设置IO口输出
	*/	
	GPIO_PinAFOutConfig(P10CFG, IO_OUTCFG_P10_EPWM0);				/*设置P10为EPWM0通道*/
	GPIO_PinAFOutConfig(P11CFG, IO_OUTCFG_P11_EPWM1);				/*设置P11为EPWM1通道*/
	GPIO_PinAFOutConfig(P12CFG, IO_OUTCFG_P12_EPWM2);				/*设置P12为EPWM2通道*/
	GPIO_PinAFOutConfig(P13CFG, IO_OUTCFG_P13_EPWM3);				/*设置P13为EPWM3通道*/
	GPIO_PinAFOutConfig(P14CFG, IO_OUTCFG_P14_EPWM4);				/*设置P14为EPWM4通道*/
	GPIO_PinAFOutConfig(P15CFG, IO_OUTCFG_P15_EPWM5);				/*设置P15为EPWM5通道*/

	GPIO_Init(PORT1,PIN0,OUTPUT);
	GPIO_Init(PORT1,PIN1,OUTPUT);
	GPIO_Init(PORT1,PIN2,OUTPUT);
	GPIO_Init(PORT1,PIN3,OUTPUT);
	GPIO_Init(PORT1,PIN4,OUTPUT);
	GPIO_Init(PORT1,PIN5,OUTPUT);
	
	EPWM_EnableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK|
					  EPWM_CH_2_MSK | EPWM_CH_3_MSK|
					  EPWM_CH_4_MSK | EPWM_CH_5_MSK);
	
	/*
	(10)重映射,客户根据实际应用配置 
	*/	
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->POREMAP = 0xAA452301;
	EPWM->LOCK = 0x0;
	
	/*
	(11)在zero点加载占空比
	*/	
	EPWM_ConfigLoadAndIntMode(EPWM0, EPWM_EACH_ZERO);			
	EPWM_ConfigLoadAndIntMode(EPWM2, EPWM_EACH_ZERO);
	EPWM_ConfigLoadAndIntMode(EPWM4, EPWM_EACH_ZERO);			/*加载点设置为每个零点*/	
	
	/*
	(12)开启EPWM
	*/		
	EPWM_Start(EPWM_CH_0_MSK | EPWM_CH_2_MSK | EPWM_CH_4_MSK);  
  
}

/*****************************************************************************
 ** \brief	EPWM模块初始化，单电阻方案
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_R1_Init(void)
{
  /* 
	(1)设置EPWM时钟
	*/
	CGC_PER11PeriphClockCmd(CGC_PER11Periph_EPWM,ENABLE);	/*开启EPWM时钟*/
  
	/* 
	(2)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode(  EPWM_COUNT_UP_DOWN  | 				/*xx计数模式*/
						 EPWM_OCU_ASYMMETRIC 	  |			/*非对称计数模式*/
						 EPWM_WFG_COMPLEMENTARYK   |		/*互补模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
	
	/*
	(3)设置EPWM时钟周期
	*/	
	EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelClk( EPWM2, EPWM_CLK_DIV_1);	
	EPWM_ConfigChannelClk( EPWM4, EPWM_CLK_DIV_1);	
	EPWM_ConfigChannelPeriod(EPWM0, EPWM_PERIOD);
	EPWM_ConfigChannelPeriod(EPWM2, EPWM_PERIOD);
	EPWM_ConfigChannelPeriod(EPWM4, EPWM_PERIOD);

	/*
	(4)设置EPWM反向输出
	*/
	EPWM_DisableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK |EPWM_CH_2_MSK|
								EPWM_CH_3_MSK| EPWM_CH_4_MSK|EPWM_CH_5_MSK);

	/*
	(5)设置EPWM死区
	*/
	EPWM_EnableDeadZone(0x3F, (uint32_t)EPWM_DT);

	/*
	(6)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK |EPWM_CH_2_MSK|EPWM_CH_4_MSK);				/*自动加载*/
		
	/*
	(7)设置EPWM比较器0
	*/			
	EPWM_ConfigCompareTriger(EPWM_CMPTG_0, EPWM_CMPTG_FALLING, EPWM_CMPTG_EPWM0, EPWM_HALFPERIOD>>1);
	EPWM_ConfigCompareTriger(EPWM_CMPTG_1, EPWM_CMPTG_FALLING, EPWM_CMPTG_EPWM2, EPWM_HALFPERIOD);

	/*
	(8)设置EPWM刹车
	*/
	#if (ACMP_CH == ACMP_CH0)
	EPWM_EnableFaultBrake(EPWM_BRK_ACMP0EE);				/*ACMP0刹车*/	
	#else
	EPWM_EnableFaultBrake(EPWM_BRK_ACMP1EE);				/*ACMP1刹车*/	
	#endif
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_0_MSK | EPWM_CH_2_MSK |EPWM_CH_4_MSK,0);
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_1_MSK | EPWM_CH_3_MSK |EPWM_CH_5_MSK,0);
	EPWM_AllBrakeEnable();
	
	Brige_Output_Off();
	
	/*
	(9)设置IO口输出
	*/	
	GPIO_PinAFOutConfig(P10CFG, IO_OUTCFG_P10_EPWM0);				/*设置P10为EPWM0通道*/
	GPIO_PinAFOutConfig(P11CFG, IO_OUTCFG_P11_EPWM1);				/*设置P11为EPWM1通道*/
	GPIO_PinAFOutConfig(P12CFG, IO_OUTCFG_P12_EPWM2);				/*设置P12为EPWM2通道*/
	GPIO_PinAFOutConfig(P13CFG, IO_OUTCFG_P13_EPWM3);				/*设置P13为EPWM3通道*/
	GPIO_PinAFOutConfig(P14CFG, IO_OUTCFG_P14_EPWM4);				/*设置P14为EPWM4通道*/
	GPIO_PinAFOutConfig(P15CFG, IO_OUTCFG_P15_EPWM5);				/*设置P15为EPWM5通道*/

	GPIO_Init(PORT1,PIN0,OUTPUT);
	GPIO_Init(PORT1,PIN1,OUTPUT);
	GPIO_Init(PORT1,PIN2,OUTPUT);
	GPIO_Init(PORT1,PIN3,OUTPUT);
	GPIO_Init(PORT1,PIN4,OUTPUT);
	GPIO_Init(PORT1,PIN5,OUTPUT);
	
	EPWM_EnableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK|
					  EPWM_CH_2_MSK | EPWM_CH_3_MSK|
					  EPWM_CH_4_MSK | EPWM_CH_5_MSK);
					  
	/*
	(10)重映射
	*/	
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->POREMAP = 0xAA452301;
	EPWM->LOCK = 0;
	
	/*
	(11)在zero点加载占空比
	*/	
	EPWM_ConfigLoadAndIntMode(EPWM0, EPWM_EACH_ZERO);			
	EPWM_ConfigLoadAndIntMode(EPWM2, EPWM_EACH_ZERO);
	EPWM_ConfigLoadAndIntMode(EPWM4, EPWM_EACH_ZERO);			/*加载点设置为每个零点*/	
	
	/*
	(12)开启EPWM
	*/					 
	EPWM_Start(EPWM_CH_0_MSK | EPWM_CH_2_MSK | EPWM_CH_4_MSK);
  
}

/********************************************************************************
 ** \brief	ADC配置，双电阻方案
 **
 ** \param [in]  none
 **
 ** \note  
 ******************************************************************************/
void ADC_R2_Init(void)
{
	/*
	(1)设置ADC1时钟
	*/
	CGC_PER13PeriphClockCmd(CGC_PER13Periph_ADCEN,ENABLE);

	ADC_ConfigRunMode(ADC_MODE_HIGH,ADC_CONVERT_CONTINUOUS,ADC_CLK_DIV_1,27);	/*连续模式+高速模式+1分频+12.5 ADCClk采样保持时间*/

	/*
	(2)设置ADC通道切换模式
	*/	
	ADC_ConfigChannelSwitchMode(ADC_SWITCH_HARDWARE);	/*硬件自动切换*/
  
	/*
	(3)设置ADC充电\放电功能
	*/
	ADC_DisableChargeAndDischarge();					/*关闭充电\放电功能*/
	
  /*
	(4)设置ADC参考源
	*/
	ADC_ConfigVREF(ADC_VREFP_VDD);		/*设置参考源*/
	
	/*
	(5)设置ADC1通道
	*/	
	GPIO_Init(PORT1,PIN6,ANALOG_INPUT); // 电压采样通道
	GPIO_Init(PORT0,PIN3,ANALOG_INPUT); // HallAD采样通道
	
	/*
	(6)开启ADC
	*/		
	ADC_Start();
	
}

/*****************************************************************************
 ** \brief	ADC模块配置，单电阻方案
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_R1_Init(void)
{
	/*
	(1)设置ADC1时钟
	*/
	CGC_PER13PeriphClockCmd(CGC_PER13Periph_ADCEN,ENABLE);

	ADC_ConfigRunMode(ADC_MODE_HIGH,ADC_CONVERT_CONTINUOUS,ADC_CLK_DIV_1,10);		/*连续模式+高速模式+1分频+10.5 ADCClk采样保持时间*/
	
	/*
	(2)设置ADC通道切换模式
	*/	
	ADC_ConfigChannelSwitchMode(ADC_SWITCH_HARDWARE);	/*硬件自动切换*/
	
	/*
	(3)设置ADC充电\放电功能
	*/
	ADC_DisableChargeAndDischarge();					/*关闭充电\放电功能*/
	  
  /*
	(4)设置ADC参考源
	*/
	ADC_ConfigVREF(ADC_VREFP_VDD);		/*设置参考源*/

	/*
	(5)设置ADC1通道
	*/	
	GPIO_Init(PORT1,PIN6,ANALOG_INPUT); // 电压采样通道
	GPIO_Init(PORT0,PIN3,ANALOG_INPUT); // HallAD采样通道
	
	/*
	(6)开启ADC
	*/		
	ADC_Start();
}

/*****************************************************************************
 ** \brief	PGA0模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  PGA0_Init(void)
{
  /*
	(1)设置PGA0时钟
	*/
	CGC_PER13PeriphClockCmd(CGC_PER13Periph_PGA0EN,ENABLE);
	 
  /*
	(2)设置PGA0正端、负端输入 IO口
	*/
	GPIO_Init(PORT0,PIN0,ANALOG_INPUT);  //PGA0 +
	GPIO_Init(PORT0,PIN1,ANALOG_INPUT);//PGA0 -
	  
  /*
	(3)设置PGA0增益
	*/
	PGA_ConfigGain(PGA0x,PGA_GAIN_5);		
  /*
	(4)设置PGA0参考电压
	*/
	PGA_VrefCtrl(PGA0x,PGA0BG);
	
  /*
	(5)设置PGA0模式
	*/
	PGA_ModeSet(PGA0x,PgaDiffer);	
	
	 /*
	(6)设置PGA0输出
	*/
	#if 0
	//PGA0输出到PAD串联电阻选择  0: 内部不串电阻  1: 内部串10K电阻
	PGA0_ConfigResistorPAD(PGA_R_10K);
	
	//PPGA0输出到PAD通道使能  0: 禁止  1: 使能
	PGA_EnableOutput(PGA0x);	
  
	GPIO_Init(PORT0,PIN2,ANALOG_INPUT);  //PGA_OUT

	#endif
		
  /*
	(7)使能PGA0
	*/
	PGA_Start(PGA0x);	
	
}

/*****************************************************************************
 ** \brief	PGA1模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PGA1_Init(void)
{
  /*
	(1)设置PGA1时钟
	*/
	CGC_PER13PeriphClockCmd(CGC_PER13Periph_PGA1EN,ENABLE);	
	  
  /*
	(2)设置PGA1正端、负端输入
	*/
	GPIO_Init(PORT2,PIN4,ANALOG_INPUT);  //PGA1 +
	GPIO_Init(PORT2,PIN5,ANALOG_INPUT);//PGA1 -
	 
  /*
	(3)设置PGA1增益
	*/
	PGA_ConfigGain(PGA1x,PGA_GAIN_5);	 
  
  /*
	(4)设置PGA1参考电压
	*/
	PGA_VrefCtrl(PGA1x,VrefHalf);
	
	/*
	(5)设置PGA1模式
	*/
	PGA_ModeSet(PGA1x,PgaDiffer); 
		
  /*
	(6)设置PGA1输出
	*/		
	#if (0)		
		
  PGA_EnableOutput(PGA1x);	
	GPIO_Init(PORT0,PIN4,ANALOG_INPUT);  //PGA_OUT
	
	#endif
	 
   /*
	(7)使能PGA1
	*/
	PGA_Start(PGA1x);
  
}

/*****************************************************************************
 ** \brief	PGA2模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PGA2_Init(void)
{
  /*
	(1)设置PGA2时钟
	*/
	CGC_PER13PeriphClockCmd(CGC_PER13Periph_PGA2EN,ENABLE);	
	  
  /*
	(2)设置PGA2正端、负端输入
	*/
	GPIO_Init(PORT2,PIN6,ANALOG_INPUT);  //PGA2 +
	GPIO_Init(PORT2,PIN7,ANALOG_INPUT);//PGA2 -
	 
  /*
	(3)设置PGA2增益
	*/
	PGA_ConfigGain(PGA2x,PGA_GAIN_5);
	
  /*
	(4)设置PGA2参考电压
	*/
	PGA_VrefCtrl(PGA2x,VrefHalf);
  	
  /*
	(5)设置PGA2模式
	*/
	PGA_ModeSet(PGA2x,PgaDiffer); 
	  
  /*
	(6)设置PGA2输出
	*/
	#if 0
	//PGA2输出使能
	PGA_EnableOutput(PGA2x);

	GPIO_Init(PORT0,PIN4,ANALOG_INPUT);  //PGA_OUT
	
	#endif
  
  /*
	(7)使能PGA2
	*/
	PGA_Start(PGA2x);

}

/*****************************************************************************
 ** \brief	ACMP0模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	用于EPWM刹车保护
*****************************************************************************/
void ACMP0_Init(void)
{
  /*
	(1)设置ACMP0时钟
	*/
	CGC_PER13PeriphClockCmd(CGC_PER13Periph_ACMPEN,ENABLE);
 
  /*
	(2)设置ACMP0正端输入
	*/
  ACMP_ConfigPositive(ACMP0,ACMP_POSSEL_0PGA2O);	
 
  /*
	(3)设置ACMP0负端输入
	*/
	ACMP_ConfigNegative(ACMP0,ACMP_NEGSEL_DAC_O);		/*负端选择DAC_O*/
 
  /*
	(4)设置ACMP0输出使能
	*/
 	ACMPOut_Enable(ACMP0);

  /*
	(5)设置ACMP0使能
	*/
	ACMP_Start(ACMP0);
	
  /*
	(6)设置ACMP0输出滤波
	*/
	ACMP_Filter_Config(ACMP0,ENABLE,ACMP_NGCLK_65_TSYS);

  /*
	(7)设置ACMP0输出极性
	*/
	ACMP_Polarity_Config(ACMP0,ACMP_POL_Pos);
	
  /*
	(8)设置ACMP0迟滞电压
	*/
	ACMP_EnableHYS(ACMP0,ACMP_HYS_POS,ACMP_HYS_S_10);

  /*
	(9)设置ACMP0 事件
	*/
	ACMP_ConfigEventAndIntMode(ACMP0,ACMP_EVENT_INT_RISING);	/*上升沿触发中断与事件，中断触发方式与事件触发方式共用*/		 
	
  /*
  (10)使能ACMP0 事件
	*/
	ACMP_EnableEventOut(ACMP0);
}

/*****************************************************************************
 ** \brief	DAC配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	同过DAC设置ACMP1比较器负端输入值
*****************************************************************************/
void DAC_Init(void)
{
	u16	VthQ10;
	u8 bLdoOn,bForT;
	  
  /*
	(1)计算LDO输出电压
	*/
	//读取当前LDO设置输出电压
	bLdoOn = ((*(u16*)0x40068340) >> 8) & 0x01;
	bForT = (*(u16*)0x40068340) & 0xff;
	
	if(!bLdoOn)					//未使能，5V
		VthQ10 = (FLOAT_VALUE(&faultHardOverCurrent) * HW_IBUS_RES * HW_AMP_GAIN_IBUS + HW_AMP_REF_IBUS) * 2550 / 50;
	else
	{
		if(bForT == 0x55)		//4.2V
			VthQ10 = (FLOAT_VALUE(&faultHardOverCurrent) * HW_IBUS_RES * HW_AMP_GAIN_IBUS + HW_AMP_REF_IBUS) * 2550 / 42;
			
		else					//3.6V
			VthQ10 = (FLOAT_VALUE(&faultHardOverCurrent) * HW_IBUS_RES * HW_AMP_GAIN_IBUS + HW_AMP_REF_IBUS) * 2550 / 36;
	}
	
	//防止数据溢出
	if(VthQ10 > 255)	
		VthQ10 = 255;

  /*
	(2)设置DAC时钟
	*/
	CGC_PER13PeriphClockCmd(CGC_PER13Periph_DAC,ENABLE);

   /*
	(3)计算DAC输入
	*/
	DAC_ConfigInput(VthQ10);
	
  /*
	(4)设置DAC使能
	*/
	DAC_Start();

}

/*****************************************************************************
 ** \brief	ACMP1模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	用于EPWM刹车保护
*****************************************************************************/
void ACMP1_Init(void)
{
	/*
	(1)设置ACMP1时钟
	*/
	CGC_PER13PeriphClockCmd(CGC_PER13Periph_ACMPEN,ENABLE);
	  
  /*
	(2)设置ACMP1正端输入
	*/
  ACMP_ConfigPositive(ACMP1,ACMP_POSSEL_1PGA0O);						
  
  /*
	(3)设置ACMP1负端输入
	*/
	ACMP_ConfigNegative(ACMP1,ACMP_NEGSEL_DAC_O);		/*负端选择DAC_O*/

	
  /*
	(4)设置ACMP1输出使能
	*/
 	ACMPOut_Enable(ACMP1);
 
  /*
	(5)设置ACMP1使能
	*/
	ACMP_Start(ACMP1);
  
  /*
	(6)设置ACMP1输出滤波
	*/	
	ACMP_Filter_Config(ACMP1,ENABLE,ACMP_NGCLK_65_TSYS);
 
  /*
	(7)设置ACMP1输出极性
	*/
	ACMP_Polarity_Config(ACMP1,ACMP_POL_Pos);

  /*
	(8)设置ACMP0迟滞电压
	*/
	ACMP_EnableHYS(ACMP1,ACMP_HYS_POS,ACMP_HYS_S_00);

  /*
	(9)设置ACMP0 事件
	*/
	ACMP_ConfigEventAndIntMode(ACMP1,ACMP_EVENT_INT_RISING);

  /*
	(10)使能ACMP1 事件
	*/
	ACMP_EnableEventOut(ACMP1);
	
}

/*****************************************************************************
 ** \brief	硬件触发器配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	无需改动
*****************************************************************************/
void HWDIV_Init(void)
{
	CGC_PER12PeriphClockCmd(CGC_PER12Periph_DIV,ENABLE);
	DIVSQRT_EnableDIVMode();
	DIVSQRT_EnableSingedMode();
  
}

/*****************************************************************************
 ** \brief	UART模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	该配置主要用于调试，用户可配置为通信
*****************************************************************************/
//void UART0_Init(void)
//{
//	uint32_t  BuadRate;
//	
//	BuadRate = 300000;

//	#if (Config_Comm_Mode == Comm_None)	
//	
//	//SYS_DisablePeripheralClk(SYS_CLK_UART1_MSK);
//	
//	#else
//	
//	/*
//	(1)开启UARTx时钟
//	*/
//	CGC_PER12PeriphClockCmd(CGC_PER12Periph_UART,ENABLE);
//	
//	/*
//	(2)设置UARTx模式
//	*/	
//	UART_ConfigRunMode(UART0, BuadRate, UART_WLS_8, UART_PARITY_NONE,UART_STOP_BIT_1);
//	
//	/*
//	(3)开启UARTx输出
//	*/			
//	GPIO_PinAFOutConfig(P04CFG,IO_OUTCFG_P04_TXD);	
//	GPIO_Init(PORT0,PIN4,OUTPUT);	
//  
//	/*
//	(4)
//	*/
//	UART0->END = 0x00;
//	
//	#endif

//}

/*****************************************************************************
 ** \brief	SysTick_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	无需改动
*****************************************************************************/
void SysTick_Init(void)
{
	SysTick->LOAD  = (uint32_t)(MCU_CLK/1000 - 1UL);
	SysTick->VAL   = 0UL;
	SysTick->CTRL  = 0x00000005;
	
}

/*****************************************************************************
 ** \brief	TMR0模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	用于hall平均角速度计算，不允许改动
*****************************************************************************/
void TMR0_Iint(void)
{
	/*
	(1)设置TMR 的时钟
	*/
	CGC_PER11PeriphClockCmd(CGC_PER11Periph_TIMER01,ENABLE);/*打开Timer0的时钟*/
	TMR_ConfigClk(TMR0,TMR_CLK_DIV_16);		/*64MHz*/
	
	/*
	(2)设置TMR 运行模式
	*/	
	TMR_ConfigRunMode(TMR0,TMR_COUNT_PERIOD_MODE, TMR_BIT_32_MODE);
	TMR_DisableOneShotMode(TMR0);
	
	/*
	(3)设置TMR 运行周期
	*/		
	TMR_SetPeriod(TMR0,40000);			/* 4us*12500=50ms*/
	
	/*
	(4)设置TMR 中断
	*/	
	TMR_EnableOverflowInt(TMR0);

	/*
	(5)开启TMR
	*/	
	TMR_Start(TMR0);
	
}

/*****************************************************************************
 ** \brief	TMR1模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	参考配置
*****************************************************************************/
void TMR1_Init(void)
{
	/*
	(1)设置TMR 的时钟
	*/
	CGC_PER11PeriphClockCmd(CGC_PER11Periph_TIMER01,ENABLE);/*打开Timer0的时钟*/
	TMR_ConfigClk(TMR1,TMR_CLK_DIV_1);		/*48MHz*/
	
	/*
	(2)设置TMR 运行模式
	*/	
	TMR_ConfigRunMode(TMR1,TMR_COUNT_PERIOD_MODE, TMR_BIT_16_MODE);
	TMR_DisableOneShotMode(TMR1);
	
	/*
	(3)设置TMR 运行周期
	*/		
	TMR_SetPeriod(TMR1,4000);			/*100us = 6400*/
	
	/*
	(4)设置TMR 中断
	*/	
	
	/*
	(5)开启TMR
	*/	
	TMR_Start(TMR1);	
}

/*****************************************************************************
 ** \brief	ADCLDO_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	无需改动
*****************************************************************************/
void ADCLDO_Init(void)
{	
  //打开时钟配置寄存器
	CGC_PER13PeriphClockCmd(CGC_PER13Periph_ADCLDO,ENABLE);	
	
	//ADCLDO输出电压选择 	0x55: LDO电压输出4.2V 其他: LDO电压输出3.6V
	
	#if (Config_LDO == LDO_3_6V)
	{
		//设置输出3.6V
		ADCLDO_OutVlotageSel(ADCLDO_OutV_3d6);
		//ADCLDO模块使能  
		ADCLDO_Enable();
		
	}
	#elif (Config_LDO == LDO_4_2V)
	{
		//设置输出4.2V
		ADCLDO_OutVlotageSel(ADCLDO_OutV_4d2);
		//ADCLDO模块使能  
		ADCLDO_Enable();
		
	}
	
	#elif (Config_LDO == LDO_5_0V)
	{
		//关闭LDO
		ADCLDO_Disable();
		//关闭时钟降低功耗
		CGC_PER13PeriphClockCmd(CGC_PER13Periph_ADCLDO,DISABLE);	

	}

	#endif
		
}

/*****************************************************************************
 ** \brief	系统的初始化配置，保护模块初始化，变量参数初始化等
 **			
 ** \param [in] none
 ** \return  none
 ** \note		基本的模块初始化顺序不允许改动，允许增加其他模块功能
*****************************************************************************/
void System_Init(void)
{
	
	//系统始终配置
	SysClock_Init();
	
	//取消P02复位功能			
	CGC->RSTM = 0x1;
	
	//调试时打开，避免上电后把仿真口刷新烧不了程序，正常模式改为300ms
	//单次延时不得超过看门狗复位时间
	DelayTime_ms(POWERON_DELAY_TIME);		
	
	//输入输出IO口线配置	
	PORT_Init();

	//ADCLDO配置
	ADCLDO_Init();
	
	//单电阻模式下PWM和ADC模块配置
	#if (Config_Shunt_Mode == Single_Shunt)
	
	EPWM_R1_Init();
	
	ADC_R1_Init();

	#endif

	//双电阻模式下PWM和ADC模块配置
	#if (Config_Shunt_Mode == Double_Shunt)
	
	EPWM_R2_Init();
	
	ADC_R2_Init();

	#endif
	
	#if (PGA_CH == PGA_CH0)
	
	PGA0_Init();
	
	#elif (PGA_CH == PGA_CH12)
	
	PGA1_Init();
	
	PGA2_Init();
	
	#elif (PGA_CH == PGA_CH012)
	
	PGA0_Init();
	
	PGA1_Init();
	
	PGA2_Init();
	
	#endif
	
	// 设置ACMP负端 参考电压，须在ACMP初始化之前，ADCLDO之后
	DAC_Init();
	
	//比较器ACMP0模块配置
	#if (ACMP_CH == ACMP_CH0)

	ACMP0_Init();

	#endif

	//比较器ACMP1模块配置
	#if (ACMP_CH == ACMP_CH1)

	ACMP1_Init();

	#endif

	//调速捕捉模块配置
	#if (Motor_Debug_Mode == Motor_Debug_Offline )
	
		#if (Config_CCP_Capture == CCP_Capture_Enable)
			CCP_Capture_Init();		
		#endif
		
	#endif	

	//速度反馈--FG输出模块配置
	#if (Motor_Debug_Mode == Motor_Debug_Offline )
	
		#if (Config_CCP_PWM == CCP_PWM_Enable)
			CCP_PWM_Init();		
		#endif
		
	#endif
	
	//硬件触发器配置
	HWDIV_Init();
	
	//串口通信配置--调试时使用
	#if (Config_Comm_Mode == Comm_Scope )
		UART0_Init();
	#endif
			
	//系统定时器--1ms时间片
	SysTick_Init();
	
	//定时器TMR0模块配置
	TMR0_Iint();
	
	//控制参数初始化
	MC_Para_Init();

	//延时等待ADC采样
	DelayTime_ms(200);
	
	//ADC参考电平读取
	GET_ADCREF_VAL();
	
}

/******************************** END OF FILE *******************************/
