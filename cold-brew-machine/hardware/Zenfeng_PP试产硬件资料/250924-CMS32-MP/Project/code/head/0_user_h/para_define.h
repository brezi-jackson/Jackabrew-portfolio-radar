/***********************************************************************************/
//---------------------------------------------------------------------------------// 
/** \file 
*  File Name  : para_define.h
*  Author     : CMS  Appliction Team
*  Version    : V2.0
*  Date       : 01/03/2020
** Description: 本页参数为硬件相关参数。
** 
//---------------------------------------------------------------------------------// 
************************************************************************************/
#ifndef   __PARA_DEFINE_H__
#define   __PARA_DEFINE_H__

//============================================================================// 
//电阻采样方式                                                                                                                                                                  
#define   Single_Shunt                          (0)               						// 单电阻                  
#define   Double_Shunt                          (1)               						// 双电阻                   
#define   Config_Shunt_Mode                     (Double_Shunt)    					                 

//----------------------------------------------------------------------------//
//SVPWM调制方式  
#define   Segment_7                             (0)               						// 七段式                    
#define   Segment_5                             (1)               						// 五段式                                          
#define   Config_SVPWM_Mode                     (Segment_7)       					                     

//----------------------------------------------------------------------------//
//初始位置侦测方式																																					 
#define   IPD_Disable                           (0)                                     
#define   IPD_Enable                            (1)            								// IPD使能
#define   IPD_Locking                           (2) 													// 预定位
#define   Config_IPD_Mode                       (IPD_Disable)      						// 初始位置检测                  

//----------------------------------------------------------------------------//
//弱磁功能	
#define   Weaken_Disable                        (0)                         	// 弱磁功能关闭            
#define   Weaken_Enable                         (1)                           // 弱磁功能开启          
#define   Config_Weak_Mode                      (Weaken_Disable)                  

//----------------------------------------------------------------------------//
//顺逆风检测功能  
#define   Start_NoWind                          (0)														// 顺逆风检测功能关闭
#define   Start_Wind                            (1)														// 顺逆风检测功能开启
#define   Config_Wind_Mode                      (Start_Wind)

//----------------------------------------------------------------------------//
//通信	
#define   Comm_None                             (0)														// 无通信模式
#define   Comm_Scope                            (1)														// 数据监测
#define   Comm_UI                               (2)														// 用户界面
#define   Config_Comm_Mode                      (Comm_None)

//----------------------------------------------------------------------------//
//HALL调试模式
#define   HALL_Debug_Disable                    (0)                          	// HALL调试关闭	          
#define   HALL_Debug_Enable                     (1)                           // HALL调试开启            
#define   Config_HallDebug_Mode                 (HALL_Debug_Disable)                     
 
//----------------------------------------------------------------------------//
//硬件调试模式
#define   Debug_Disable                         (0)                          	// 硬件调试功能关闭	          
#define   Debug_Enable                          (1)                           // 硬件调试功能开启            
#define   Config_Debug_Mode                     (Debug_Disable)                     
 
//----------------------------------------------------------------------------//
//降速音补偿
#define   Voice_Down_Disable                    (0)                          	// 降速音补偿关闭	          
#define   Voice_Down_Enable                     (1)                           // 降速音补偿开启                             
#define   Voice_Down_DT                         (EPWM_Tus*0.5)                // 降速音补偿死区设置
#define   Config_Voice_Down_Mode                (Voice_Down_Disable)
 
//============================================================================//
//MCU 硬件相关配置 
//----------------------------------------------------------------------------//
//PGA通道选择，
#define   PGA_CH0                               (0)
#define   PGA_CH12                              (1)
#define   PGA_CH012                             (2)
#define   PGA_NONE                              (3)
#define   PGA_CH                                (PGA_CH012)				

//----------------------------------------------------------------------------//
//比较器通道选择,用于EPWM故障刹车保护 
#define   ACMP_CH0                              (0)
#define   ACMP_CH1                              (1)
#define   ACMP_NONE                             (2)
#define   ACMP_CH                               (ACMP_CH1)			

//----------------------------------------------------------------------------//
//双电阻方案：PGA1与PGA2 对应的采样通道，包含六种情况 
//单电阻方案无需配置 
#define   IP_UV                                 (0)
#define   IP_UW                                 (1)
#define   IP_VU                                 (2)
#define   IP_VW                                 (3)
#define   IP_WU                                 (4)
#define   IP_WV                                 (5)
#define   IP_SAMP_CH                            (IP_UV)				

//----------------------------------------------------------------------------//
//单电阻/双电阻采样双通道配置 
//其中双电阻方案默认PGA1/2，单电阻方案默认PGA0
#if (Config_Shunt_Mode == Single_Shunt)

#define   ADC_DATA_CHA                          (ADC_CH_0)
#define   ADC_SCAN_CHA                          (ADC_CH_0_MSK)

#define   ADC_DATA_CHB                          (ADC_CH_1)
#define   ADC_SCAN_CHB                          (ADC_CH_1_MSK)

//----------------------------------------------------------------------------//
#elif (Config_Shunt_Mode == Double_Shunt)

#define   ADC_DATA_CHA                          (ADC_CH_3)			
#define   ADC_SCAN_CHA                          (ADC_CH_3_MSK)	

#define   ADC_DATA_CHB                          (ADC_CH_2)
#define   ADC_SCAN_CHB                          (ADC_CH_2_MSK)

#endif

//----------------------------------------------------------------------------//
//模拟信号采样通道配置，按照硬件修改配置
#define   ADC_DATA_VBUS                        (ADC_CH_18)			
#define   ADC_SCAN_VBUS                        (ADC_CH_18_MSK)						  	// 母线电压采样 

#define   ADC_DATA_IBUS                        (ADC_CH_0)			
#define   ADC_SCAN_IBUS                        (ADC_CH_0_MSK)		             // 母线电流采样 

#define   ADC_DATA_HALL                        (ADC_CH_10)			
#define   ADC_SCAN_HALL                        (ADC_CH_10_MSK)		            // HallAD值采样 

#define   ADC_DATA_CTRL                        (ADC_CH_12)			
#define   ADC_SCAN_CTRL                        (ADC_CH_12_MSK)						  	// 调速信号采样 

//----------------------------------------------------------------------------//
#define   ADC_DATA_UBEMF                       (ADC_CH_15)			
#define   ADC_SCAN_UBEMF                       (ADC_CH_15_MSK)							  // MCU_BEMF_U (未使用)

#define   ADC_DATA_VBEMF                       (ADC_CH_16)			
#define   ADC_SCAN_VBEMF                       (ADC_CH_16_MSK)							  // MCU_BEMF_V (未使用)

#define   ADC_DATA_WBEMF                       (ADC_CH_17)			
#define   ADC_SCAN_WBEMF                       (ADC_CH_17_MSK)						  	// MCU_BEMF_W (未使用)

//============================================================================//
//调速信号捕获 
#define   CCP_Capture_Disable                   (0)					
#define   CCP_Capture_Enable                    (1)					
#define   Config_CCP_Capture                    (CCP_Capture_Enable)					// 捕获使能
#define   CCP_CAPTURE_MODULE                    (CCP1)												// 捕获的模块 CCP0、CCP1
#define   CCP_CAPTURE_CH                        (CCPxA)												// 捕获的通道 CCPxA、CCPxB

//----------------------------------------------------------------------------//
//FG信号输出 
#define   CCP_PWM_Disable                       (0)					
#define   CCP_PWM_Enable                        (1)					
#define   Config_CCP_PWM                        (CCP_PWM_Enable)							// PWM使能
#define   CCP_PWM_MODULE                        (CCP0)												// PWM的模块 CCP0、CCP1
#define   CCP_PWM_CH                            (CCPxB)												// PWM的通道 CCPxA、CCPxB

//============================================================================//
// MCU Config 
#define   MCU_CLK                               (64000000ul)									// MCU CLK 
#define   EPWM_FREQ                             (19000)												// EPWM FREQ 
#define   EPWM_PERIOD                           (MCU_CLK/EPWM_FREQ/2)
#define   EPWM_HALFPERIOD                       (EPWM_PERIOD/2)
#define   EPWM_Tus                              (64)													// 64: 1us 	
#define   EPWM_DT                               (EPWM_Tus*0.5) 								// 死区时间，64；1us 
#define   EPWM_CPMTG                            (EPWM_Tus*0.5) 								// 双电阻方案：采样提前时间 单位：us 
#define   EPWM_TGDLY                            (EPWM_Tus*1.5) 								// 单电阻方案：采样延迟时间 单位：us 

//============================================================================//               
//硬件参数配置
/* 
	PGA放大倍数校正 (30K * PGAgain)/30K = (30K * PGAgain)/(30K + R0) PGAgain：PGA放大倍数设置值 R0：PGA正负端输入电阻
	例如 PGAgain = 5，R0 = 1K，  PGA实际放大倍数：GAIN = (30 * 5)/(30 + 1) = 4.8387
*/
#define   HW_AMP_GAIN_IP                        (4.838)            						// 相电流采样放大倍数                        
#define   HW_AMP_GAIN_IBUS                      (4.838)            						// Ibus电流采样放大倍数                        
#define   HW_AMP_REF_IP                         (2.5)         								// 相电流采样偏置电压 (V)            
#define   HW_AMP_REF_IBUS                       (0.8)           							// 母线电流采样偏置电压 (V)                      
#define   HW_ADC_REF                            (5.0)          								// ADC参考电压 (V)                                                                                                                     

#define   HW_VBUS_SDR                           (0.0865824)       						// 母线电压采样，分压电阻比   
                                                                                                     
//============================================================================//                
//电机参数                                                                                                        
#define   MOTOR_Ke_VPP                          (3.86)          							// 反电势电压峰峰值 (V) 
#define   MOTOR_Ke_Freq                         (23.15)         							// 反电势频率 (Hz) 
#define   MOTOR_Ke                              (((float)MOTOR_Ke_VPP/2/_SQRT_3)/(_2PI()*MOTOR_Ke_Freq))

#define   MOTOR_Ke_K                            (1.0) 		 							    	// 校正系数 
	
#define   MOTOR_WE_BASE                         (MOTOR_VOLT_BASE*2*_SQRT_3*_2PI()*MOTOR_Ke_Freq/MOTOR_Ke_VPP) 

//============================================================================//              
//基值参数                                                                                          
#define   MOTOR_SPEED_BASE                      (3000)		  									// 转速基值 (RPM)                             
#define   MOTOR_POWER_BASE                      (50)           								// 功率基值 (W)                            
#define   MOTOR_VOLT_BASE                       ((float)HW_VBUS_VOLT/1.732)           
#define   MOTOR_IBUS_BASE                       ((float)MOTOR_POWER_BASE/HW_VBUS_VOLT)                                                                                                     
                                                                                          
//============================================================================//    
#define   MOTOR_SPEED_CLOSE                     (150)          								// 马达启动开环切闭环转速                      
#define   MOTOR_SPEED_STOP                      (50)           								// 马达停机判段转速 （封6管MOS）                     
#define   MOTOR_SPEED_BACK                      (120)           							// 马达回到运行态的最低转速                                                                                                                                  
#define   MOTOR_SPEED_SWITCH                    (1500)           							// 马达高于该转速直接切到运行态 

#define   SPEED_OFFSET                          (20)              							// 转速偏差校正                     

//----------------------------------------------------------------------------//
#define   TIME_SPEED_LOOP                       (10)        	 								// 速度环执行周期(ms)                                  
#define   TIME_POWER_LOOP                       (1)         	  							// 功率环执行周期(ms)                                  
 
//----------------------------------------------------------------------------//
#define   TIME_START_HOLD                       (300)													// 启动完成保持时间 (ms),100~5000ms 
#define   TIME_STOP_HOLD                        (500)            							// 停机延时时间 (ms)

//============================================================================//               
//BEMF反电动势顺风参数 
#define   BEMF_CHECK_TIME                       (3000)												// BEMF检测顺风态时间 单位:Tpwm 
#define   BEMF_SPEED_START                      (50)												// BEMF检测顺风启动转速(低于该转速进入启动态) 单位:RPM 
#define   BEMF_SPEED_MIN                        (100)												// BEMF检测顺风最小投切转速(大于该转速进入切换态) 单位:RPM 
#define   BEMF_SPEED_MAX                        (150)												// BEMF检测顺风最大投切转速(大于该转速进入切换态) 单位:RPM 

//============================================================================//               
//启动参数                                                                                                                                                                                                          
#define   START_DKP                             _Q15(0.5)      								// D轴PI参数：Kp     0.76                       
#define   START_DKI                             _Q15(0.005)    								// D轴PI参数：Ki        0.01                     
#define   START_DOUT_MAX                        _Q15(0.0)      								// D轴PI参数：最大输出                                
#define   START_DOUT_MIN                        _Q15(-0.5)     								// D轴PI参数：最小输出                                 

//----------------------------------------------------------------------------//
#define   START_QKP                             _Q15(0.5)      								// Q轴PI参数：Kp                               
#define   START_QKI                             _Q15(0.005)    								// Q轴PI参数：Ki                              
#define   START_QOUT_MAX                        _Q15(0.5)      								// Q轴PI参数：最大输出                                 
#define   START_QOUT_MIN                        _Q15(0.0)      							  // Q轴PI参数：最小输出                                  
                                                                                                                                                                                                                    
//----------------------------------------------------------------------------//                                                                                                   
#define   IPD_TIME                              (30)           								// IPD时间 (ms)                        
#define   IPD_CURRENT                           _Q15(0.35)     								// IPD电流                         
  
//============================================================================//
//开环启动参数 
#define   START_VF                              (0)
#define   START_IF                              (1)
#define   START_MODE                            (START_VF)										// 默认START_IF, Debug模式START_VF 

//----------------------------------------------------------------------------//
#define   START_RPM_INIT                        (0)														// 启动初始转速 (RPM), 0 
#define   START_RPM_FINAL                       (200)													// 启动限制转速 (RPM), 1%~10% MOTOR_SPEED_BASE 

//----------------------------------------------------------------------------//
#define   START_ALIGN_TADD                      (100)													// 预定位加速时间 (ms), 0~1000ms 
#define   START_ALIGN_THOLD                     (500)													// 预定位保持时间 (ms), 0~5000ms 
#define   START_RUN_TIME                        (2000)												// 启动运行时间 (ms), 100~3000ms 
#define   START_ALIGN_CURRENT                   _Q15(0.10)										// 预定位电流 (%), 0.05~0.5 

//----------------------------------------------------------------------------//
#define   START_ALIGN_TIME                      (START_ALIGN_TADD+START_ALIGN_THOLD)
#define   START_ALIGN_KADD                      (START_ALIGN_CURRENT/START_ALIGN_TADD)

//============================================================================//               
//FOC控制参数                                                                                      
#define   FOC_DKp                               _Q15(0.3)      								// D轴PI参数：Kp                              
#define   FOC_DKI                               _Q15(0.001)    								// D轴PI参数：Ki                             
#define   FOC_DOUT_MAX                          _Q15(0.8)      								// D轴PI参数：最大输出                                
#define   FOC_DOUT_MIN                          _Q15(-0.9)     								// D轴PI参数：最小输出                                

//----------------------------------------------------------------------------//
#define   FOC_QKP                               _Q15(0.3)      								// Q轴PI参数：Kp                              
#define   FOC_QKI                               _Q15(0.001)    								// Q轴PI参数：Ki                               
#define   FOC_QOUT_MAX                          _Q15(0.95)     								// Q轴PI参数：最大输出                                
#define   FOC_QOUT_MIN                          _Q15(-0.8)     								// Q轴PI参数：最小输出                                 
                                                                                                     
//----------------------------------------------------------------------------//                                                                                               
#define   FOC_EKP                               _Q15(0.5)      								// 运行估算器PI参数：Kp                               
#define   FOC_EKI                               _Q15(0.03)    								// 运行估算器PI参数：Ki                               
                                                                                                     
#define   FOC_FC_LPF                            (20)    											// 估算器截至频率：Hz

//----------------------------------------------------------------------------//
#define   FOC_FRAMP                             (6)            								                            								                       

//----------------------------------------------------------------------------//
#define   FOC_WOUT_MAX                          (FOC_RUN_CURRENT_MAX)                  
#define   FOC_WOUT_MIN                          (FOC_RUN_CURRENT_MIN)                  
     
//============================================================================//                 
//功率/转速限制方式 
#define   LIMIT_SOFTWARE                        (0)														// 软件限制
#define   LIMIT_HARDWARE                        (1)														// 硬件限制
#define   LIMIT_POWER_MODE                      (LIMIT_HARDWARE)

#define   LIMIT_SOFT_POWER                      _Q15(1.0)								    	// 软件限制功率值
                                                                                                                         
#define   LIMIT_POWER_ENABLE                    (0)            								// 限制功率使能                  
#define   LIMIT_POWER_VALUE                     (0)            								// 硬件限制功率值 (W)                  
#define   LIMIT_SPEED_ENABLE                    (0)            								// 限制转速使能                           
#define   LIMIT_SPEED_VALUE                     (3000)         								// 限制转速值 (RPM)                 
                                                                                                     
//============================================================================//                                                                                                              
//缺相保护参数                                                                                       
#define   FAULT_PHASELOSS_CURR                  I_Value(0.01)  								// 缺相保护电流 单位：A                       
                                                                                                                          
//============================================================================//
//堵转处理--针对堵转的判断，处理无感堵转异音问题，该功能根据实际情况配置
//如果默认的堵转检测功能正常，则不建议开启该模块功能
#define   BLOCK_WE_DISABLE                      (0)														// 堵转We方式关闭
#define   BLOCK_WE_ENABLE                       (1)														// 堵转We方式使能
#define   BLOCK_WE_MODE                         (BLOCK_WE_DISABLE)							

#define   BLOCK_WE_RATE                         (25)        									// 转速变化率,百分比单位
                                                                                                  
//============================================================================//
//充电使能--PN驱动无需开启，NN驱动可开启
#define   CHARGE_DISABLE                        (0)														// 预充电关闭
#define   CHARGE_ENABLE                         (1)														// 预充电使能
#define   CHARGE_MODE                           (CHARGE_DISABLE)							

#define   CHARGE_TIME                           (10)        									// 预充电时间(ms)

//============================================================================//
//HALL功能
#define   HALL_START_INC_MIN                    (10)													// HALL启动角度增量初始值 
#define   HALL_START_INC_MAX                    (100)													// HALL启动角度增量最大值 

#define   HALL_OFFSET_THETA                     _Q15(0.0)											// HALL相位补偿 

//============================================================================//
/* Wind PI control for Angle est */
#define   WIND_EKP                              _Q15(1.0)											// 0.1~1.0 
#define   WIND_EKI                              _Q15(0.00)										// 0.01~1.0 
#define   WIND_EOUT_MAX                         _Q15(0.5)											// 0.1~1.0 
#define   WIND_EOUT_MIN                         _Q15(-0.5)										// -0.1~-1.0 

/* RevWind  control para */
#define   WIND_STOP_SPEED                       (-50)													// 反向启动的最大转速 (RPM) 
#define   WIND_REVSPEED_MIN                     (-180)												// 逆风启动判定条件：反向转速低于该值允许启动 (RPM) 
#define   WIND_REVSPEED_MAX                     (-200)												// 逆风启动判定条件：反向转速高于该值先刹车再启动 (RPM) 

#define   WIND_SPEED_START                      (-20)	
#define   WIND_SPEED_MIN                        (-120)												// 逆风启动判定条件：反向转速低于该值允许启动 (RPM) 
#define   WIND_SPEED_MAX                        (-200)												// 逆风启动判定条件：反向转速高于该值先刹车再启动 (RPM) 

#define   WIND_CHECK_TIME                       (10000)												// 顺逆风检测时间 

#define   WIND_START_CURRENT                    _Q15(0.20)      							// 逆风启动电流 

//============================================================================//
//LDO输出电压
#define	  LDO_5_0V                              (0)												  	//5V
#define	  LDO_4_2V                              (1)												  	//4.2V
#define	  LDO_3_6V                              (2)												  	//3.6V
#define   Config_LDO                            (LDO_5_0V)

//============================================================================//
//硬件功率计算
#define	  LPF_Inner                             (1)											  		//内部LPF
#define	  RCF_Outside                           (2)											  		//外部RC滤波
#define   Power_Cal_Mode                        (RCF_Outside)									//硬件计算模式

#define   POWERFACTOR                           (1.27)												//硬件计算补偿系数

//============================================================================//
											
#endif  /* __PARA_DEFINE_H__ */
