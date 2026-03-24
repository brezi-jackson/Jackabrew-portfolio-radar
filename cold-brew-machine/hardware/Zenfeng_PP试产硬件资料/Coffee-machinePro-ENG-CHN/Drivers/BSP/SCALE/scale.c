#include "scale.h"
#include "delay.h"
#include "init.h"

Weight_g_t Weight_g_0, Weight_g_1;

//SCALE IO初始化
void SCALE_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	
    GPIO_Initure.Pin=Dout_Pin_0;				//PB4 重量参数输入
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  	//输入
	HAL_GPIO_Init(Dout_GPIO_Port_0, &GPIO_Initure);
	
	GPIO_Initure.Pin=SCK_Pin_0;				//PB6
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;	//输出
    GPIO_Initure.Pull=GPIO_PULLUP;
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(SCK_GPIO_Port_0, &GPIO_Initure);
	
	GPIO_Initure.Pin=Dout_Pin_1;			//PB4 重量参数输入
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  	//输入
	HAL_GPIO_Init(Dout_GPIO_Port_1, &GPIO_Initure);
	
	GPIO_Initure.Pin=SCK_Pin_1;				//PB6
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;	//输出
    GPIO_Initure.Pull=GPIO_NOPULL;
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(SCK_GPIO_Port_1, &GPIO_Initure);
	
	Weight_g_0.Num = 0;
	Weight_g_1.Num = 1;
}

uint32_t ReadCount(GPIO_TypeDef *SCK_GPIO_Port,GPIO_TypeDef *Dout_GPIO_Port, uint16_t SCK_Pin, uint16_t Dout_Pin)	//增益128
{
	uint32_t Count = 0;
	uint8_t i = 0;
	
	HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET);
//	HAL_Delay(1);
			
	 uint8_t Scale_Lost = 0;

	while(HAL_GPIO_ReadPin(Dout_GPIO_Port, Dout_Pin))
	{
		if(Scale_Lost > 10) //判断AD转换器是否准备好（DOUT应为低电平）
			break;
		else
			Scale_Lost ++;
	}	

	for(i = 0; i < 24; i++)//24
	{
		HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_SET);
			
//		HAL_Delay(1);

		Count = Count << 1;		
		if(HAL_GPIO_ReadPin(Dout_GPIO_Port, Dout_Pin))
		{
		    Count ++;
		}
		HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET);			
	}
	
	HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_SET);
	Count = Count ^ 0x800000;
		
//	HAL_Delay(1);

	HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET);
	return Count;
}

//float lpf_1rd_calc(lpf_1rd_t *lpf1rd, int32_t new_sample)
//{
//    lpf1rd->in =  new_sample;
//	lpf1rd->out = lpf1rd->z1 + lpf1rd->tc * (lpf1rd->in - lpf1rd->z1);
//    lpf1rd->z1 =  lpf1rd->out;

//    return lpf1rd->out;
//}

int32_t weight_Count_Init, weight_Filter_Init;

int32_t Weight_Filter_Init(int8_t Num)
{
	switch(Num)
	{
		case 0:
			weight_Count_Init = ReadCount(SCK_GPIO_Port_0, Dout_GPIO_Port_0, SCK_Pin_0, Dout_Pin_0);
			break;
	
		case 1:
			weight_Count_Init = ReadCount(SCK_GPIO_Port_1, Dout_GPIO_Port_1, SCK_Pin_1, Dout_Pin_1);
			break;
		
		default:
			break;
	}

	return weight_Count_Init;
}

int32_t weight_Count, weight_Filter, AVG, weight_Count_0_last, weight_Count_1_last;

int32_t weight_Count_0, weight_Count_1;

int32_t Weight_Filter(int8_t Num)
{
	switch(Num)
	{
		case 0:
			weight_Count = ReadCount(SCK_GPIO_Port_0, Dout_GPIO_Port_0, SCK_Pin_0, Dout_Pin_0);
			
			weight_Count_0 = weight_Count;
		
			if(weight_Count <= Weight_g_0.weight_Count_init)
				weight_Count = weight_Count_0_last;
			
			weight_Filter = weight_Count / 100;
			weight_Count_0_last = weight_Count;
			break;
	
		case 1:
			weight_Count = ReadCount(SCK_GPIO_Port_1, Dout_GPIO_Port_1, SCK_Pin_1, Dout_Pin_1);
			
			weight_Count_1 = weight_Count;
		
			if(weight_Count <= Weight_g_1.weight_Count_init)
				weight_Count = weight_Count_1_last;
			
			weight_Filter = weight_Count / 72;
			weight_Count_1_last = weight_Count;
			break;
		
		default:
			break;
	}

	AVG = weight_Filter;

	return AVG;
}

int32_t weight, weight_1;

extern lpf_1rd_t lpf1rd_0;
extern lpf_1rd_t lpf1rd_1;

void Get_Weight_Real(Weight_g_t *Weight_g)
{
	weight = Weight_Filter(Weight_g->Num);//实际质量计算

	if(Weight_g->weight_tare >= weight)
		weight_1 = 0;
	else
		weight_1 =  weight - Weight_g->weight_tare;
	
	if(Weight_g->Num)
		Weight_g->weight_g = lpf_1rd_calc(&lpf1rd_1, weight_1);
	else
		Weight_g->weight_g = lpf_1rd_calc(&lpf1rd_0, weight_1);
}
