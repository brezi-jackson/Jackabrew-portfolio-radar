#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

extern uint16_t Second, Minute_1, Minute_2 , Hour_1, Hour_2;

extern uint8_t Touch_s_L;
extern uint8_t Touch_s_R;
extern uint8_t Touch_l_L;
extern uint8_t Touch_l_R;

extern uint8_t Limit_Switch_0;
extern uint8_t time_start_tag;
extern uint8_t Progress_bar_tag;
extern uint8_t Progress_bar;

/******************************************************************************************/
/* 引脚 定义 */
/************ Touch1/2 **************/
#define Touch1_GPIO_PORT			GPIOA
#define Touch1_GPIO_PIN				GPIO_PIN_4
#define Touch1_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define Touch2_GPIO_PORT			GPIOB
#define Touch2_GPIO_PIN				GPIO_PIN_6
#define Touch2_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define Touch1	HAL_GPIO_ReadPin(Touch1_GPIO_PORT, Touch1_GPIO_PIN)
#define Touch2	HAL_GPIO_ReadPin(Touch2_GPIO_PORT, Touch2_GPIO_PIN)

/************ LED_T1/2 **************/
#define LED_T1_GPIO_PORT			GPIOA
#define LED_T1_GPIO_PIN				GPIO_PIN_3
#define LED_T1_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)	/* PC口时钟使能 */

#define LED_T1_ENABLE	HAL_GPIO_WritePin(LED_T1_GPIO_PORT, LED_T1_GPIO_PIN, GPIO_PIN_SET)
#define LED_T1_DISABLE	HAL_GPIO_WritePin(LED_T1_GPIO_PORT, LED_T1_GPIO_PIN, GPIO_PIN_RESET)

#define LED_T2_GPIO_PORT			GPIOB
#define LED_T2_GPIO_PIN				GPIO_PIN_7                                                                                                                                        
#define LED_T2_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)	/* PC口时钟使能 */

#define LED_T2_ENABLE	HAL_GPIO_WritePin(LED_T2_GPIO_PORT, LED_T2_GPIO_PIN, GPIO_PIN_SET)
#define LED_T2_DISABLE	HAL_GPIO_WritePin(LED_T2_GPIO_PORT, LED_T2_GPIO_PIN, GPIO_PIN_RESET)

/************ ENC_A/B **************/
#define ENC_A_GPIO_PORT				GPIOA
#define ENC_A_GPIO_PIN				GPIO_PIN_11
#define ENC_A_GPIO_CLK_ENABLE()		do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define ENC_B_GPIO_PORT				GPIOA
#define ENC_B_GPIO_PIN				GPIO_PIN_12
#define ENC_B_GPIO_CLK_ENABLE()		do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define ENC_A	HAL_GPIO_ReadPin(ENC_A_GPIO_PORT, ENC_A_GPIO_PIN)
#define ENC_B	HAL_GPIO_ReadPin(ENC_B_GPIO_PORT, ENC_B_GPIO_PIN)

/******************************************************************************************/

void TIM4_Init(u16 arr,u16 psc);
void Time_Reset(void);
void Touch_Init(void);
void Touch_Reset(void);

void ENCODER_Init(void);

#endif

