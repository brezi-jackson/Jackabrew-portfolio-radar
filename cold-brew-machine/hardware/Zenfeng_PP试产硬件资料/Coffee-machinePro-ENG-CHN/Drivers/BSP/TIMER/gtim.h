#ifndef __GTIM_H
#define __GTIM_H

#include "./SYSTEM/sys/sys.h"

/*********************************以下是通用定时器脉冲计数实验相关宏定义*************************************/

/* TIMX 输入计数定义
* 这里的输入计数使用定时器TIM2_CH1,捕获WK_UP按键的输入
* 默认是针对TIM2~TIM5, 只有CH1和CH2通道可以用做输入计数, CH3/CH4不支持!
* 注意: 通过修改这几个宏定义,可以支持TIM1~TIM8任意一个定时器,CH1/CH2对应IO口做输入计数
*       特别要注意:默认用的PA0,设置的是下拉输入!如果改其他IO,对应的上下拉方式也得改!
*/
#define GTIM_TIMX_CNT_CHY_GPIO_PORT            GPIOA
#define GTIM_TIMX_CNT_CHY_GPIO_PIN             GPIO_PIN_0
#define GTIM_TIMX_CNT_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define GTIM_TIMX_CNT                          TIM5
#define GTIM_TIMX_CNT_IRQn                     TIM5_IRQn
#define GTIM_TIMX_CNT_IRQHandler               TIM5_IRQHandler
#define GTIM_TIMX_CNT_CHY                      TIM_CHANNEL_1                                 /* 通道Y,  1<= Y <=2 */
#define GTIM_TIMX_CNT_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM5_CLK_ENABLE(); }while(0)    /* TIM2 时钟使能 */

/******************************************************************************************/

void gtim_timx_cnt_chy_init(uint16_t psc);                  /* 通用定时器 脉冲计数初始化函数 */
uint32_t gtim_timx_cnt_chy_get_count(void);                 /* 通用定时器 获取脉冲计数 */
void gtim_timx_cnt_chy_restart(void);                       /* 通用定时器 重启计数器 */

#endif

















