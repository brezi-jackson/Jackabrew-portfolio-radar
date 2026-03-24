
#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
#define USART_1_TX_GPIO_PORT                  GPIOA
#define USART_1_TX_GPIO_PIN                   GPIO_PIN_9
#define USART_1_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART_1_RX_GPIO_PORT                  GPIOA
#define USART_1_RX_GPIO_PIN                   GPIO_PIN_10
#define USART_1_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART_1_UX                            USART1
#define USART_1_UX_IRQn                       USART1_IRQn
#define USART_1_UX_IRQHandler                 USART1_IRQHandler
#define USART_1_UX_CLK_ENABLE()               do{ __HAL_RCC_USART1_CLK_ENABLE(); }while(0)  /* USART1 时钟使能 */

/******************************************************************************************/

#define USART_REC_LEN               9			/* 定义最大接收字节数 200 */
#define USART_EN_RX                 1			/* 使能（1）/禁止（0）串口1接收 */
#define RXBUFFERSIZE   				9			/* 缓存大小 */
#define TXBUFFERSIZE   				9			/* 缓存大小 */

extern UART_HandleTypeDef g_uart2_handle;  /* UART句柄 */

extern uint16_t Error_Code;
extern uint8_t Pump_Error;
extern uint8_t Limit_Switch;
extern uint8_t NTC_1_Error;
extern uint8_t NTC_2_Error;

extern uint8_t temp_switch;
extern uint8_t pump_flag;
extern uint16_t pump_set;

extern float Temp_0;
extern float Temp_1;

void usart_init(uint32_t bound);                /* 串口初始化函数 */

#endif


