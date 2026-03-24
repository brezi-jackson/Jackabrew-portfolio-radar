#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"

typedef volatile struct
{
	float desired;     ///期望
	float offset;      //
	float prevError;    // 上次偏差
	float integ;        //误差积分累加值
	float kp;           //p参数
	float ki;           //i参数
	float kd;           //d参数
	float IntegLimitHigh;       //< integral limit
	float IntegLimitLow;
	float measured;     ////pid反馈量
	float out;
	float OutLimitHigh;
	float OutLimitLow;	
	float Control_OutPut;//控制器总输出
	float Last_Control_OutPut;//上次控制器总输出
	float Control_OutPut_Limit;//输出限幅
		/***************************************/
	float Last_FeedBack;//上次反馈值
	float Dis_Err;//微分量
	float Dis_Error_History[5];//历史微分量
	float Err_LPF;
	float Last_Err_LPF;
	float Dis_Err_LPF;

//	int8_t Err_Limit_Flag :1;//偏差限幅标志
//	int8_t Integrate_Limit_Flag :1;//积分限幅标志
//	int8_t Integrate_Separation_Flag :1;//积分分离标志		
}PidObject;


/******************************************************************************************/
/* 引脚 定义 */
/******************************* BUZZER ***********************************/

#define BUZZER_TIM					TIM2
#define BUZZER_TIM_CLK_ENABLE()		do{ __HAL_RCC_TIM2_CLK_ENABLE(); }while(0)   /* TEC时钟使能 */
	
#define BUZZER_CCR					CCR1
#define BUZZER_TIM_CHANNEL			TIM_CHANNEL_1
#define BUZZER_GPIO_PORT			GPIOA
#define BUZZER_GPIO_PIN				GPIO_PIN_15
#define BUZZER_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PC口时钟使能 */

#define BUZZER_ENABLE 				HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_SET)
#define BUZZER_DISABLE				HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_RESET)

/************ VIBRAT_MOTOR **************/
#define VIBRAT_MOTOR_TIM					TIM2
#define VIBRAT_MOTOR_TIM_CLK_ENABLE()		do{ __HAL_RCC_TIM2_CLK_ENABLE(); }while(0)   /* TEC时钟使能 */
	
#define VIBRAT_MOTOR_CCR					CCR3
#define VIBRAT_MOTOR_TIM_CHANNEL			TIM_CHANNEL_3

#define VIBRAT_MOTOR_GPIO_PORT				GPIOA
#define VIBRAT_MOTOR_GPIO_PIN				GPIO_PIN_2
#define VIBRAT_MOTOR_GPIO_CLK_ENABLE()		do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)	/* PC口时钟使能 */

#define VIBRAT_MOTOR_ENABLE		HAL_GPIO_WritePin(VIBRAT_MOTOR_GPIO_PORT, VIBRAT_MOTOR_GPIO_PIN, GPIO_PIN_SET)
#define VIBRAT_MOTOR_DISABLE	HAL_GPIO_WritePin(VIBRAT_MOTOR_GPIO_PORT, VIBRAT_MOTOR_GPIO_PIN, GPIO_PIN_RESET)

/******************************************************************************************/

extern TIM_HandleTypeDef TIM_TEC_Handler;	//定时器句柄 

extern PidObject Temp_pid;

extern float Temp_0, Temp_1;

extern float TEC_0, TEC_1;

void TEC_Init(uint16_t arr);
void TEC_Set_A(u32 compare);
void TEC_Set_B(u32 compare);

void FAN_Init(uint16_t arr);
void FAN_Speed_Set(u32 compare);

void PUMP_Init(uint16_t arr);
void PUMP_Speed_Set_A(u32 compare);
void PUMP_Speed_Set_B(u32 compare);

void MOTOR_Init(uint16_t arr);
void MOTOR_Speed_Set_A(u32 compare);
void MOTOR_Speed_Set_B(u32 compare);

void BUZZER_Init(uint16_t arr);

void VIBRAT_MOTOR_Init(void);
void VIBRAT_MOTOR_Set(u32 compare);

//void PID_Controler(PidObject* pidproject);
int16_t Temp_PID(int16_t Temp_target);
void Temp_Set(int16_t Temp_target, uint8_t TEC_Switch);

void BUZZER_Set(uint8_t buz_set);

typedef struct {
    uint16_t freq;
    uint16_t duration_ms;
} Note;

void BUZZER_Play_Tone(uint16_t freq, uint16_t duration_ms);
//void BUZZER_Play_Melody(Note* melody, uint8_t count);

#endif
