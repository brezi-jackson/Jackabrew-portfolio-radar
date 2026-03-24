#include "motor.h"
#include "timer.h"
#include "GUI.h"
#include "usart.h"
#include "delay.h"
#include "lcd.h"
//MOTOR IO初始化

TIM_HandleTypeDef 	TIM_BUZZER_Handler;      	//定时器句柄 
TIM_OC_InitTypeDef 	TIM_BUZZER_CHNHandler;	//定时器3通道1句柄

TIM_HandleTypeDef 	TIM_VIBRAT_MOTOR_Handler;      	//定时器句柄 
TIM_OC_InitTypeDef 	TIM_VIBRAT_MOTOR_CH2Handler;	//定时器3通道1句柄


//TIM1 PWM部分初始化 
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz

void BUZZER_Init(uint16_t arr)
{
    TIM_BUZZER_Handler.Instance				= BUZZER_TIM;         		//定时器1
    TIM_BUZZER_Handler.Init.Prescaler		= 35;       				//定时器分频
    TIM_BUZZER_Handler.Init.CounterMode		= TIM_COUNTERMODE_UP;		//向上计数模式
    TIM_BUZZER_Handler.Init.Period			= arr;          			//自动重装载值
    TIM_BUZZER_Handler.Init.ClockDivision	= TIM_CLOCKDIVISION_DIV1;	//分频因子
    HAL_TIM_PWM_Init(&TIM_BUZZER_Handler);       	//初始化PWM
    
    TIM_BUZZER_CHNHandler.OCMode		= TIM_OCMODE_PWM1; 		//模式选择PWM1
    TIM_BUZZER_CHNHandler.Pulse			= 0;				//设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM_BUZZER_CHNHandler.OCPolarity	= TIM_OCPOLARITY_LOW;	//输出比较极性为低 
    
	HAL_TIM_PWM_ConfigChannel(&TIM_BUZZER_Handler, &TIM_BUZZER_CHNHandler, BUZZER_TIM_CHANNEL);//配置TIM3通道1
	
	HAL_TIM_PWM_Start(&TIM_BUZZER_Handler, BUZZER_TIM_CHANNEL);//开启PWM通道1
}

void VIBRAT_MOTOR_Init(void)
{
	TIM_VIBRAT_MOTOR_Handler.Instance			= VIBRAT_MOTOR_TIM;			//定时器1
	TIM_VIBRAT_MOTOR_Handler.Init.Prescaler		= 23;       				//定时器分频
	TIM_VIBRAT_MOTOR_Handler.Init.CounterMode	= TIM_COUNTERMODE_UP;		//向上计数模式
	TIM_VIBRAT_MOTOR_Handler.Init.Period		= 99;          				//自动重装载值
	TIM_VIBRAT_MOTOR_Handler.Init.ClockDivision	= TIM_CLOCKDIVISION_DIV1;	//分频因子
	HAL_TIM_PWM_Init(&TIM_VIBRAT_MOTOR_Handler);       	//初始化PWM
    
	TIM_VIBRAT_MOTOR_CH2Handler.OCMode		= TIM_OCMODE_PWM1; 		//模式选择PWM1
	TIM_VIBRAT_MOTOR_CH2Handler.Pulse		= 0;           		//设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
	TIM_VIBRAT_MOTOR_CH2Handler.OCPolarity	= TIM_OCPOLARITY_HIGH; 	//输出比较极性为低 
	
	HAL_TIM_PWM_ConfigChannel(&TIM_VIBRAT_MOTOR_Handler, &TIM_VIBRAT_MOTOR_CH2Handler, VIBRAT_MOTOR_TIM_CHANNEL);//配置TIM3通道3
	
	HAL_TIM_PWM_Start(&TIM_VIBRAT_MOTOR_Handler, VIBRAT_MOTOR_TIM_CHANNEL);//开启PWM通道3	
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	if(htim -> Instance == BUZZER_TIM)
	{
		BUZZER_TIM_CLK_ENABLE();			//使能定时3
		BUZZER_GPIO_CLK_ENABLE();			//开启GPIOC时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_AFIO_REMAP_TIM2_PARTIAL_1();

		GPIO_Initure.Pin	= BUZZER_GPIO_PIN;	//PA6
		GPIO_Initure.Mode	= GPIO_MODE_AF_PP;	//复用推挽输出
		GPIO_Initure.Pull	= GPIO_PULLUP;		//下拉
		GPIO_Initure.Speed	= GPIO_SPEED_HIGH;	//高速
		
		HAL_GPIO_Init(BUZZER_GPIO_PORT, &GPIO_Initure);
	}
	
	if(htim -> Instance == BL_TIM)
	{
		BL_TIM_CLK_ENABLE();				//使能定时3
		BL_GPIO_CLK_ENABLE();			//开启GPIOC时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
//		__HAL_AFIO_REMAP_TIM3_ENABLE();

		//FAN PWM
		GPIO_Initure.Pin	= BL_GPIO_PIN;	//PA6
		GPIO_Initure.Mode	= GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_Initure.Pull	= GPIO_PULLDOWN;		//下拉
		GPIO_Initure.Speed	= GPIO_SPEED_HIGH;		//高速
		
		HAL_GPIO_Init(BL_GPIO_PORT, &GPIO_Initure);
	}
	
	if(htim -> Instance == VIBRAT_MOTOR_TIM)
	{
		VIBRAT_MOTOR_TIM_CLK_ENABLE();
	    VIBRAT_MOTOR_GPIO_CLK_ENABLE();					/* KEY1时钟使能 */
	
		__HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_AFIO_REMAP_TIM2_PARTIAL_1();
		
		GPIO_Initure.Pin	= VIBRAT_MOTOR_GPIO_PIN;	/* KEY1引脚 */
		GPIO_Initure.Mode	= GPIO_MODE_AF_PP;			/* 复用推挽输出 */
		GPIO_Initure.Pull	= GPIO_PULLUP;				/* 上拉 */
		GPIO_Initure.Speed 	= GPIO_SPEED_FREQ_HIGH;		/* 高速 */
		
		HAL_GPIO_Init(VIBRAT_MOTOR_GPIO_PORT, &GPIO_Initure);	/* KEY1引脚模式设置,上拉输入 */
	}
}

//设置TIM1通道1的占空比
//compare:比较值

void BUZZER_Set(uint8_t buz_set)
{
	BUZZER_TIM->BUZZER_CCR = buz_set;
}

void VIBRAT_MOTOR_Set(u32 compare)
{
	VIBRAT_MOTOR_TIM->VIBRAT_MOTOR_CCR = compare;
}




// 播放一个音符（频率+时长）
void BUZZER_Play_Tone(uint16_t freq, uint16_t duration_ms)
{
    // 保存原始参数
    uint32_t old_arr = TIM_BUZZER_Handler.Instance->ARR;
    uint32_t old_ccr = __HAL_TIM_GET_COMPARE(&TIM_BUZZER_Handler, BUZZER_TIM_CHANNEL);

    if (freq == 0) {
        BUZZER_Set(0);
        HAL_Delay(duration_ms);
    } else {
        // 计算 ARR 和 50% 占空比
        uint32_t timer_clk = 72000000 / (TIM_BUZZER_Handler.Init.Prescaler + 1);
        uint32_t arr = timer_clk / freq - 1;

        __HAL_TIM_SET_AUTORELOAD(&TIM_BUZZER_Handler, arr);
        __HAL_TIM_SET_COMPARE(&TIM_BUZZER_Handler, BUZZER_TIM_CHANNEL, arr / 2);

        HAL_Delay(duration_ms);
        BUZZER_Set(0);
        HAL_Delay(5);
    }

    // 恢复原始定时器设置
    __HAL_TIM_SET_AUTORELOAD(&TIM_BUZZER_Handler, old_arr);
    __HAL_TIM_SET_COMPARE(&TIM_BUZZER_Handler, BUZZER_TIM_CHANNEL, old_ccr);
}
//void BUZZER_Play_Tone(uint16_t freq, uint16_t duration_ms)
//{
//    if (freq == 0) {
//        BUZZER_Set(0);  // 静音
//        HAL_Delay(duration_ms);
//        return;
//    }

//    // 计算 ARR：自动重载值
//    uint32_t timer_clk = 72000000 / (TIM_BUZZER_Handler.Init.Prescaler + 1);
//    uint32_t arr = timer_clk / freq - 1;

//    __HAL_TIM_SET_AUTORELOAD(&TIM_BUZZER_Handler, arr);
//    __HAL_TIM_SET_COMPARE(&TIM_BUZZER_Handler, BUZZER_TIM_CHANNEL, arr / 2); // 50% 占空比

//    HAL_Delay(duration_ms);
//    BUZZER_Set(0);  // 关闭输出
//    HAL_Delay(5);   // 加一点点间隔防止切换频率时产生杂音
//}

//// 播放旋律序列
//void BUZZER_Play_Melody(Note* melody, uint8_t count)
//{
//    for (uint8_t i = 0; i < count; i++) {
//        BUZZER_Play_Tone(melody[i].freq, melody[i].duration_ms);
//    }
//}
