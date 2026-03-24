#include "timer.h"
#include "exti.h"
#include "motor.h"
#include "scale.h"
#include "Init.h"
#include "gtim.h"
#include "adc.h"
#include "task.h"

TIM_HandleTypeDef TIM4_Handler;      //定时器句柄 

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!

void TIM4_Init(u16 arr,u16 psc)
{
	TIM4_Handler.Instance=TIM4;                          //通用定时器3
	TIM4_Handler.Init.Prescaler=psc;                     //分频系数
	TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
	TIM4_Handler.Init.Period=arr;                        //自动装载值
	TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
	HAL_TIM_Base_Init(&TIM4_Handler);

	HAL_TIM_Base_Start_IT(&TIM4_Handler);
}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM4_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启ITM3中断   
	}
}

//定时器3中断服务函数
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM4_Handler);
}

void Touch_Init()
{
    GPIO_InitTypeDef gpio_init_struct;
	
	__HAL_RCC_AFIO_CLK_ENABLE();
    Touch1_GPIO_CLK_ENABLE();
	__HAL_AFIO_REMAP_SWJ_NOJTAG();

	/* KEY0时钟使能 */
    gpio_init_struct.Pin 	= Touch1_GPIO_PIN;/* KEY0引脚 */
    gpio_init_struct.Mode 	= GPIO_MODE_INPUT;                    /* 输入 */
    gpio_init_struct.Pull 	= GPIO_PULLUP;                        /* 上拉 */
    gpio_init_struct.Speed 	= GPIO_SPEED_FREQ_HIGH;              /* 高速 */
	HAL_GPIO_Init(Touch1_GPIO_PORT, &gpio_init_struct);           /* WKUP引脚模式设置,下拉输入 */
	
	Touch2_GPIO_CLK_ENABLE();                                     /* KEY0时钟使能 */
	gpio_init_struct.Pin	= Touch2_GPIO_PIN;/* KEY0引脚 */
	gpio_init_struct.Mode	= GPIO_MODE_INPUT;                    /* 输入 */
	gpio_init_struct.Pull	= GPIO_PULLUP;                        /* 上拉 */
	gpio_init_struct.Speed	= GPIO_SPEED_FREQ_HIGH;              /* 高速 */
	HAL_GPIO_Init(Touch2_GPIO_PORT, &gpio_init_struct);           /* WKUP引脚模式设置,下拉输入 */

    LED_T1_GPIO_CLK_ENABLE();								/* KEY1时钟使能 */
	gpio_init_struct.Pin	= LED_T1_GPIO_PIN;				/* KEY1引脚 */
    gpio_init_struct.Mode 	= GPIO_MODE_OUTPUT_PP;                    /* 输入 */
    gpio_init_struct.Pull 	= GPIO_PULLUP;						/* 上拉 */
	gpio_init_struct.Speed	= GPIO_SPEED_FREQ_HIGH;              /* 高速 */
	HAL_GPIO_Init(LED_T1_GPIO_PORT, &gpio_init_struct);	/* KEY1引脚模式设置,上拉输入 */

	LED_T2_GPIO_CLK_ENABLE();								/* KEY1时钟使能 */
	gpio_init_struct.Pin 	= LED_T2_GPIO_PIN;				/* KEY1引脚 */
    gpio_init_struct.Mode 	= GPIO_MODE_OUTPUT_PP;                    /* 输入 */
    gpio_init_struct.Pull 	= GPIO_PULLUP;						/* 上拉 */
    gpio_init_struct.Speed	= GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(LED_T2_GPIO_PORT, &gpio_init_struct);	/* KEY1引脚模式设置,上拉输入 */
}

int16_t Encoder;
int16_t Encoder_A_now, Encoder_A_last;
int16_t Encoder_B_now, Encoder_B_last;

void ENCODER_Init()
{
    GPIO_InitTypeDef gpio_init_struct;
    ENC_A_GPIO_CLK_ENABLE();                                     /* KEY0时钟使能 */
	ENC_B_GPIO_CLK_ENABLE();                                     /* KEY1时钟使能 */

    gpio_init_struct.Pin = ENC_A_GPIO_PIN;                       /* KEY0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpio_init_struct.Pull = GPIO_PULLUP;                        /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(ENC_A_GPIO_PORT, &gpio_init_struct);           /* KEY0引脚模式设置,上拉输入 */

    gpio_init_struct.Pin = ENC_B_GPIO_PIN;                       /* KEY1引脚 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpio_init_struct.Pull = GPIO_PULLUP;                        /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(ENC_B_GPIO_PORT, &gpio_init_struct);           /* KEY1引脚模式设置,上拉输入 */
	
	Encoder_A_now = Encoder_A_last = ENC_A;
	Encoder_B_now = Encoder_B_last = ENC_B;
	
	Encoder = 0;
}

extern uint16_t Task, Task_Last;

void Sleep(void)
{
	OLED_DisPlay_Off();
	temp_switch = 0;
	pump_set = 0;
	time_start_tag = 0;
	LED_T1_DISABLE;
	LED_T2_DISABLE;
}

void Wake(void)
{
	OLED_Init();
	Task_Last++;
}

//回调函数，定时器中断服务函数调用

uint16_t Second, Minute_1, Minute_2 , Hour_1, Hour_2, weight_tag;

uint8_t Touch_now_L = 1, Touch_now_R = 1, Touch_last_L = 1, Touch_last_R = 1, Touch_flag_L, Touch_flag_R;

int Touch_tag_L, Touch_tag_R;

uint8_t Touch_s_L, Touch_s_R, Touch_l_L, Touch_l_R, Limit_Switch_0;

uint8_t VIBRAT_MOTOR_test = 80;
uint8_t fan_set;

uint8_t Encoder_A_tag, Encoder_B_tag;
uint16_t sleep_tag_0, sleep_tag_1;
uint8_t sleep_time, sleep_flag;
uint16_t fan_test = 90;

uint8_t g1_tx_buffer[TXBUFFERSIZE] = {0xAE, 0xB7, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xDD};//cal variation
uint8_t g2_tx_buffer[TXBUFFERSIZE] = {0xBE, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0xFF};//cal
uint8_t g3_tx_buffer[TXBUFFERSIZE] = {0xBE, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB, 0xEE};//use

uint8_t pump_flag_last, pump_start, pump_set_send;

uint8_t Current_Error;

extern uint8_t Factory_test_start;
extern float TEC_Current;
extern uint8_t TEC_Working_tag, TEC_Working_time;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t tim4_flag;
	static uint8_t Current_Error_tag;

    if(htim==(&TIM4_Handler))
    {
		tim4_flag++;
		
		if(tim4_flag % 1 == 0)
		{			
			Touch_now_L = Touch1;
			Touch_now_R = Touch2;
						
			if(Touch_last_L != Touch_now_L || Touch_last_R != Touch_now_R)
			{
				if((Touch_now_L == 0) && (Touch_last_L == 1))
					Touch_flag_L = 1;
				else if((Touch_now_R == 0) && (Touch_last_R == 1))
					Touch_flag_R = 1;
				else if((Touch_now_L == 1) && (Touch_last_L == 0))
					Touch_flag_L = 2;
				else if((Touch_now_R == 1) && (Touch_last_R == 0))
					Touch_flag_R = 2;
			}
			
			Encoder_A_now = ENC_A;
			Encoder_B_now = ENC_B;

			if(Touch_flag_L==0 && Touch_flag_R ==0 && Encoder_A_last == Encoder_A_now && Encoder_B_last == Encoder_B_now && temp_switch == 0 && pump_set == 0 && Error_Code == 0)
			{	
				if(sleep_tag_0 < 100)
					sleep_tag_0 ++;
			}
			else
			{
				sleep_tag_0 = sleep_tag_1 = 0;


				if(Touch_flag_L == 1)
				{
					if(sleep_flag)
					{
						sleep_time = 0;
						Wake();
						sleep_flag = 0;
					}
					else
						Touch_tag_L ++;
				}
				
				else if(Touch_flag_L == 2)
				{	
					if(Touch_tag_L>50 && Touch_tag_L < 300)
						Touch_s_L++;
				
					Touch_tag_L = 0;
					Touch_flag_L = 0;
				}
				if(Touch_flag_L == 1 && Touch_tag_L >= 700)
				{
					Touch_l_L = 1;
				}
				
				if(Touch_flag_R == 1)
				{
					if(sleep_flag)
					{
						sleep_time = 0;
						Wake();
						sleep_flag = 0;
					}
					else
						Touch_tag_R ++;
				}
				else if(Touch_flag_R == 2)
				{	
					if(Touch_tag_R>50 && Touch_tag_R < 300)
						Touch_s_R++;
					Touch_tag_R = 0;
					Touch_flag_R = 0;
				}
				
				if(Touch_flag_R == 1 && Touch_tag_R >= 700)
					Touch_l_R = 1;
				
				if((Encoder_A_now != Encoder_A_last)&& Encoder_A_now == 0)
				{
					if(sleep_flag)
					{
						sleep_time = 0;
						Wake();
						sleep_flag = 0;
					}
					else
					{
						if((Encoder_B_now != Encoder_B_last) && Encoder_B_now == 0)
							Encoder ++;
					}
				}
				if((Encoder_A_now != Encoder_A_last) && Encoder_A_now)
				{
					if(sleep_flag)
					{
						sleep_time = 0;
						Wake();
						sleep_flag = 0;
					}
					else
					{
						if((Encoder_B_now != Encoder_B_last) && Encoder_B_now)
							Encoder --;
					}
				}
			}
			
			Touch_last_L = Touch_now_L;
			Touch_last_R = Touch_now_R;
			
			Encoder_A_last = Encoder_A_now;
			Encoder_B_last = Encoder_B_now;	
		}
		
		if(tim4_flag % 4 == 0)
		{
			static uint16_t VIBRAT_MOTOR_tag;
			
			if(Touch_flag_L || Touch_flag_R)
			{
				VIBRAT_MOTOR_tag++;
				
				if(VIBRAT_MOTOR_tag < 20)
				{
					VIBRAT_MOTOR_Set(VIBRAT_MOTOR_test);
				}
				else
				{
					VIBRAT_MOTOR_Set(0);
				}
			}
			else
			{
				VIBRAT_MOTOR_tag = 0;
				VIBRAT_MOTOR_Set(0);
			}
		}
		
		if(tim4_flag % 10 == 0)
		{
			if(TEC_Current > 13)
				Current_Error_tag++;
			else
				Current_Error_tag = 0;
			
			if(Current_Error_tag > 200)
				Current_Error = 1;
			
			static uint8_t tx_tag;	
			
			if(tx_tag < TXBUFFERSIZE-1)
				tx_tag++;
			else
				tx_tag = 0;
			
			if(temp_switch)
			{
				if(pump_set>25)
					fan_set = fan_test;
				else if(pump_set <= 25 && pump_set > 20)
					fan_set = fan_test - 5;
				else if(pump_set <= 20 && pump_set > 15)
					fan_set = fan_test - 10;
				else if(pump_set <= 15 && pump_set > 10)
					fan_set = fan_test - 15;
				else if(pump_set <= 10 && pump_set >= 5)
					fan_set = fan_test - 20;
				else
					fan_set = fan_test;
			}
			else
				fan_set = 0;
			
			if(pump_set)
			{
				if(pump_flag != 2)
					pump_flag = 1;
			}
			else
				pump_flag = 0;
			
			if(Factory_test_start)
			{
				if(variable_tx)
				{
					g1_tx_buffer[2] = (uint16_t)(pump_var_a * 10000) >> 8;
					g1_tx_buffer[3] = (uint16_t)(pump_var_a * 10000);
					g1_tx_buffer[4] = (uint16_t)(pump_var_b * 100) >> 8;
					g1_tx_buffer[5] = (uint16_t)(pump_var_b * 100);
					g1_tx_buffer[6] = pump_flag;
					
					HAL_UART_Transmit_IT(&g_uart2_handle, &g1_tx_buffer[tx_tag], 1);
				}
				else
				{
					g2_tx_buffer[2] = temp_switch;
					g2_tx_buffer[3] = temp_set;
					g2_tx_buffer[4] = pump_flag;
					g2_tx_buffer[5] = pump_set;
					g2_tx_buffer[6] = 90;
					
					HAL_UART_Transmit_IT(&g_uart2_handle, &g2_tx_buffer[tx_tag], 1);
				}
			}
			else // 常规发送
			{	
				g3_tx_buffer[2] = temp_switch;
				g3_tx_buffer[3] = temp_set;
				g3_tx_buffer[4] = pump_flag;
				g3_tx_buffer[5] = pump_set;
				g3_tx_buffer[6] = fan_set;
				
				HAL_UART_Transmit_IT(&g_uart2_handle, &g3_tx_buffer[tx_tag], 1);
			}
		
			pump_flag_last = pump_flag;
		}
		
		if(tim4_flag % 12 == 0)
		{		
			if(Progress_bar_tag)
				Progress_bar++;
			else
				Progress_bar = 0;
		}
		
		if(tim4_flag % 100 == 0)
		{
			if(sleep_tag_0 >= 100)
				sleep_tag_1++;
			else
				sleep_tag_1 =0;
		}
		
		if(tim4_flag % 1000 == 0)
		{			
			if(TEC_Working_tag)
			{
				TEC_Working_time ++;
			}
			else
			{
				TEC_Working_time = 0;
			}
			
			if(time_start_tag)
			{
				Second ++;
				
				if(Second == 60)
				{
					Minute_1 ++;
					Second = 0;
				}
				
				if(Minute_1 == 10)
				{
					Minute_1 = 0;
					Minute_2 ++;
				}
				if(Minute_2 == 6)
				{
					Minute_2 = 0;
					Hour_1 ++;
				}
				
				if(Hour_1 == 10)
				{
					Hour_1 = 0;
					Hour_2 ++;
				}
				
				if(Hour_2 ==6)
				{
					Hour_2 =0;
				}
			}
		}
		  
		if(tim4_flag % 60000 == 0)
		{
			if(sleep_tag_1 >= 590)
			{
				sleep_tag_1 = 0;
				if(sleep_time < 9)//休眠
				{
					sleep_time ++;
				}
				else
				{
					sleep_time = 0;
					Sleep();
					sleep_flag = 1;
				}
			}

			tim4_flag = 0;
		}
    }
}

void Time_Reset(void)
{
	Second = Minute_1 = Minute_2 = Hour_1 = Hour_2 = 0;
}

void Touch_Reset(void)
{
	Touch_l_L = Touch_l_R = Touch_s_L = Touch_s_R = 0;
}
