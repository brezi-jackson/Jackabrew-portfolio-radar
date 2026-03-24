#include "exti.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "timer.h"

//外部中断初始化
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();               //开启GPIOA时钟
    
    GPIO_Initure.Pin=GPIO_PIN_15;	//PA11 PA15 GPIO_PIN_11|
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;		//下降沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
    //中断线0-PA0
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,1,2);	//抢占优先级为2，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);			//使能中断线10-15
}


//中断服务函数

void EXTI15_10_IRQHandler(void)
{
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
	
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}


//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
int16_t Touch;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	delay_ms(2);
	switch(GPIO_Pin)
	{
//		case GPIO_PIN_11:
//		
//			if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12))
//				Touch ++;
//			else
//				Touch --;
//			break;
//			
		case GPIO_PIN_15:
			Touch++;
			break;
		
		default:
			break;
	}
}
