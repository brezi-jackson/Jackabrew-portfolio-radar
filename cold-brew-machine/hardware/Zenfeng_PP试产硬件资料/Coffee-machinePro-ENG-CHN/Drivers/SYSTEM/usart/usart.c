#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "Init.h"
#include "task.h"
/* 如果使用os,则包括下面的头文件即可. */
#if SYS_SUPPORT_OS
#include "includes.h" /* os 使用 */
#endif

#if USART_EN_RX /*如果使能了接收*/

uint16_t g_usart_rx_sta = 0;

uint8_t g2_rx_buffer[RXBUFFERSIZE];  /* HAL库使用的串口接收缓冲 */

UART_HandleTypeDef g_uart2_handle;  /* UART句柄 */

/**
 * @brief       串口X初始化函数
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @note        注意: 必须设置正确的时钟源, 否则串口波特率就会设置异常.
 *              这里的USART的时钟源在sys_stm32_clock_init()函数中已经设置过了.
 * @retval      无
 */
void usart_init(uint32_t baudrate)
{
    /*UART 初始化设置*/
    g_uart2_handle.Instance = USART_1_UX;                                     /* USART_UX */
    g_uart2_handle.Init.BaudRate = baudrate;                                  /* 波特率 */
    g_uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* 字长为8位数据格式 */
    g_uart2_handle.Init.StopBits = UART_STOPBITS_1;                           /* 一个停止位 */
    g_uart2_handle.Init.Parity = UART_PARITY_NONE;                            /* 无奇偶校验位 */
    g_uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* 无硬件流控 */
    g_uart2_handle.Init.Mode = UART_MODE_TX_RX;                               /* 收发模式 */
    HAL_UART_Init(&g_uart2_handle);                                           /* HAL_UART_Init()会使能UART1 */

    /* 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量 */
    HAL_UART_Receive_IT(&g_uart2_handle, (uint8_t *)g2_rx_buffer, RXBUFFERSIZE);
}

/**
 * @brief       UART底层初始化函数
 * @param       huart: UART句柄类型指针
 * @note        此函数会被HAL_UART_Init()调用
 *              完成时钟使能，引脚配置，中断配置
 * @retval      无
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct;

    if (huart->Instance == USART_1_UX)                            /* 如果是串口1，进行串口1 MSP初始化 */
    {
        USART_1_TX_GPIO_CLK_ENABLE();                             /* 使能串口TX脚时钟 */
        USART_1_RX_GPIO_CLK_ENABLE();                             /* 使能串口RX脚时钟 */
        USART_1_UX_CLK_ENABLE();                                  /* 使能串口时钟 */

//		__HAL_RCC_AFIO_CLK_ENABLE();
//		__HAL_AFIO_REMAP_USART1_DISABLE();
		
        gpio_init_struct.Pin = USART_1_TX_GPIO_PIN;               /* 串口发送引脚号 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* IO速度设置为高速 */
        HAL_GPIO_Init(USART_1_TX_GPIO_PORT, &gpio_init_struct);
                
        gpio_init_struct.Pin = USART_1_RX_GPIO_PIN;               /* 串口RX脚 模式设置 */
        gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;    
        HAL_GPIO_Init(USART_1_RX_GPIO_PORT, &gpio_init_struct);   /* 串口RX脚 必须设置成输入模式 */
        
#if USART_EN_RX
        HAL_NVIC_EnableIRQ(USART_1_UX_IRQn);                      /* 使能USART1中断通道 */
        HAL_NVIC_SetPriority(USART_1_UX_IRQn, 3, 3);              /* 组2，最低优先级:抢占优先级3，子优先级3 */
#endif
    }
}

/**
 * @brief       串口数据接收回调函数
                数据处理在这里进行
 * @param       huart:串口句柄
 * @retval      无
 */

//主机->从机: tec控制指令 风扇控制指令 泵控制指令
//从机->主机: NTC1 NTC2 干簧管  tec电流 泵电流
uint8_t USART2_RX_BUF[RXBUFFERSIZE];

uint8_t pump_flag = 0, temp_switch = 0;
uint16_t pump_set;

float Temp_0, Temp_1,TEC_Current;

uint16_t Error_Code;

uint8_t Error, Pump_Error,  Limit_Switch, NTC_1_Error, NTC_2_Error;

uint8_t Error_rx;
uint8_t Pump_Cal_Done;

uint8_t  Main_Version_0;
uint8_t  Main_Version_1;
uint8_t  Main_Version_2;

extern uint8_t TEC_Error;
extern uint8_t Current_Error;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART_1_UX)                    /* 如果是串口1 */
    {
		for(uint8_t USART2_RX_STA = 0; USART2_RX_STA < RXBUFFERSIZE; USART2_RX_STA ++)
		{			
			if(g2_rx_buffer[USART2_RX_STA] == 0xEB)
			{
				for(uint8_t sta = 0; sta < RXBUFFERSIZE; sta ++)
				{
					if((USART2_RX_STA + sta) <= (RXBUFFERSIZE-1))
						USART2_RX_BUF[sta] = g2_rx_buffer[USART2_RX_STA + sta];
					else
						USART2_RX_BUF[sta] = g2_rx_buffer[USART2_RX_STA + sta - RXBUFFERSIZE];
				}
				
				if(USART2_RX_BUF[0] == 0xEB && USART2_RX_BUF[1] == 0xB0)
				{
					int32_t Temp0_U0_R;
					Temp0_U0_R = (USART2_RX_BUF[2] << 8) | USART2_RX_BUF[3];
					Temp_0 = Temp0_U0_R / 10.f;
					
					int32_t Temp1_U0_R;
					Temp1_U0_R = (USART2_RX_BUF[4] << 8) | USART2_RX_BUF[5];
					Temp_1 = Temp1_U0_R / 10.f;
					
					int32_t TEC_Current_U0_R;
					TEC_Current_U0_R = (USART2_RX_BUF[7] << 8) | USART2_RX_BUF[8];
					TEC_Current = TEC_Current_U0_R / 50.f;
					
					Error_Code = USART2_RX_BUF[6];
				}
				else
				{
					
				}
				
				if(TEC_Error)
					Error_Code |= 0x10;
				
				if(Current_Error)
					Error_Code |= 0x20;
				
				if((Error_Code & 0xFF) == 0x00)
					Error_rx = 0;
				else
					Error_rx = 1;
				
				if((Error_Code & 0x01) == 0x01)//水泵检测
					Pump_Error = 1;
				else
					Pump_Error = 0;
		
				if((Error_Code & 0x02) == 0x02)//水箱检测
					Limit_Switch = 1;
				else
					Limit_Switch = 0;
				
				if((Error_Code & 0x04) == 0x04)//NTC_1
					NTC_1_Error = 1;
				else
					NTC_1_Error = 0;
				
				if((Error_Code & 0x08) == 0x08)//NTC_2
					NTC_2_Error = 1;
				else
					NTC_2_Error = 0;
					
				break;
			}
			else if(g2_rx_buffer[USART2_RX_STA] == 0xEA)
			{
				for(uint8_t sta = 0; sta < RXBUFFERSIZE; sta ++)
				{
					if((USART2_RX_STA + sta) <= (RXBUFFERSIZE-1))
						USART2_RX_BUF[sta] = g2_rx_buffer[USART2_RX_STA + sta];
					else
						USART2_RX_BUF[sta] = g2_rx_buffer[USART2_RX_STA + sta - RXBUFFERSIZE];
				}
				
				if(USART2_RX_BUF[0] == 0xEA && USART2_RX_BUF[1] == 0x7B && USART2_RX_BUF[2] == 1)
				{
					Pump_Cal_Done = 0;
				}
				
				break;
			}
			else if(g2_rx_buffer[USART2_RX_STA] == 0xEC)
			{
				for(uint8_t sta = 0; sta < RXBUFFERSIZE; sta ++)
				{
					if((USART2_RX_STA + sta) <= (RXBUFFERSIZE-1))
						USART2_RX_BUF[sta] = g2_rx_buffer[USART2_RX_STA + sta];
					else
						USART2_RX_BUF[sta] = g2_rx_buffer[USART2_RX_STA + sta - RXBUFFERSIZE];
				}
				
				if(USART2_RX_BUF[0] == 0xEC && USART2_RX_BUF[1] == 0xA0)
				{
					Main_Version_0 = USART2_RX_BUF[2];
					Main_Version_1 = USART2_RX_BUF[3];
					Main_Version_2 = USART2_RX_BUF[4];
				}
			}
		}
		
		HAL_UART_Receive_IT(&g_uart2_handle, (uint8_t *)g2_rx_buffer, RXBUFFERSIZE);
    }
}

/**
 * @brief       串口X中断服务函数
                注意,读取USARTx->SR能避免莫名其妙的错误
 * @param       无
 * @retval      无
 */
void USART_1_UX_IRQHandler(void)
{
    HAL_UART_IRQHandler(&g_uart2_handle);                               /* 调用HAL库中断处理公用函数 */

//    while (HAL_UART_Receive_IT(&g_uart0_handle, (uint8_t *)g0_rx_buffer, RXBUFFERSIZE) != HAL_OK)     /* 重新开启中断并接收数据 */
//    {
//        /* 如果出错会卡死在这里 */
//    }
}
#endif
