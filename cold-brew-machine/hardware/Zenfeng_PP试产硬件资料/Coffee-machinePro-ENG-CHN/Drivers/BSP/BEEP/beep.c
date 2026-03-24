#include "beep.h"

//蜂鸣器 IO初始化
void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_10; 			//PB10
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);	//蜂鸣器对应引脚GPIOB10拉高，
}
