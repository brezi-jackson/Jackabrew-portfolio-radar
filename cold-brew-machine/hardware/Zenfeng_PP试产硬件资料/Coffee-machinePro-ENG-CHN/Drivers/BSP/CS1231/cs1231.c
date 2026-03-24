//#include "cs1231.h"

//#define SCK_Pin			GPIO_PIN_10
//#define Dout_Pin		GPIO_PIN_11
//#define SCK_GPIO_Port	GPIOB
//#define Dout_GPIO_Port	GPIOB

//uint32_t ReadCount_CS1231(void)	//增益128
//{
//	uint32_t Count = 0;
//	uint8_t i = 0;
//	
//	HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET);
////	HAL_Delay(1);
//			
//	 uint8_t Scale_Lost = 0;

//	while(HAL_GPIO_ReadPin(Dout_GPIO_Port, Dout_Pin))
//	{
//		if(Scale_Lost > 10) //判断AD转换器是否准备好（DOUT应为低电平）
//			break;
//		else
//			Scale_Lost ++;
//	}	

//	for(i = 0; i < 24; i++)//24
//	{
//		HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_SET);
//			
////		HAL_Delay(1);

//		Count = Count << 1;		
//		if(HAL_GPIO_ReadPin(Dout_GPIO_Port, Dout_Pin))
//		{
//		    Count ++;
//		}
//		HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET);			
//	}
//	
//	HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_SET);
//	Count = Count ^ 0x800000;
//		
////	HAL_Delay(1);

//	HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET);
//	return Count;
//}

//#define N_Scale 10
//int32_t SUM;
//uint8_t Ni = 0;
//int32_t BUF[N_Scale];
//void BUFInit(void) //初始化队列数组BUF和SUM，即，往里面先存入N个采样值，SUM为N个采用值的和。
//{
////	uint8_t j = 0;
////	while (j < N_Scale)
////	{
////		j++;
////		
////		BUF[j] = ReadCount(SCK_GPIO_Port_1, Dout_GPIO_Port_1, SCK_Pin_1, Dout_Pin_1);
////		
////		SUM += BUF[j];
////	}
//}

//void BUF_Reset(int32_t weight_Reset) //初始化队列数组BUF和SUM，即，往里面先存入N个采样值，SUM为N个采用值的和。
//{
//	uint8_t j = 0;
//	while (j < N_Scale)
//	{
//		j++;
//		
//		BUF[j] = weight_Reset;
//		
//		SUM += BUF[j];
//	}
//}

//float lpf_1rd_calc(lpf_1rd_t *lpf1rd, int32_t new_sample)
//{
//    lpf1rd->in =  new_sample;
//	lpf1rd->out = lpf1rd->z1 + lpf1rd->tc * (lpf1rd->in - lpf1rd->z1);
//    lpf1rd->z1 =  lpf1rd->out;

//    return lpf1rd->out;
//}

//int32_t weight_Count, weight_Filter, AVG;
//int32_t weight_last, weight_test;

//int32_t Weight_Filter(int8_t Num)
//{
//	switch(Num)
//	{
//		case 0:
//			weight_Count = ReadCount(SCK_GPIO_Port_0, Dout_GPIO_Port_0, SCK_Pin_0, Dout_Pin_0);
//			weight_Filter = weight_Count / 200;
//			break;
//	
//		case 1:
//			weight_Count = ReadCount(SCK_GPIO_Port_1, Dout_GPIO_Port_1, SCK_Pin_1, Dout_Pin_1);
//		weight_Filter = weight_Count / 180;
//			break;
//		
//		default:
//			break;
//	}

//	AVG = weight_Filter;

//	return AVG;
//}
//int32_t weight, weight_1;
//extern lpf_1rd_t lpf1rd_0;
//extern lpf_1rd_t lpf1rd_1;
//void Get_Weight_Real(Weight_g_t *Weight_g)
//{
////	static uint8_t weight_real_tag;
//	
////	int32_t Weight_Real_Check;
//	
//	weight = Weight_Filter(Weight_g->Num);//实际质量计算

//	if(Weight_g->weight_tare >= weight)
//	{
//		weight_1 = Weight_g->weight_tare - weight;
//	}
//	else
//	{
//		weight_1 = weight - Weight_g->weight_tare;
//	}
//	
//	if(Weight_g->Num)
//		Weight_g->weight_g = lpf_1rd_calc(&lpf1rd_1, weight_1);
//	else
//		Weight_g->weight_g = lpf_1rd_calc(&lpf1rd_0, weight_1);
//	
////	Weight_Real_Check = weight_real % 10;
////	
////	if(Weight_Real_Check >= 8)
////	{
////		weight_real += (10 - Weight_Real_Check);
////	}
////	else if(Weight_Real_Check <= 2)
////	{
////		weight_real -= Weight_Real_Check;
////	}
//	
////	if(weight_real == 0)
////		weight_real_tag = 1;
////	
////	if(weight_real_tag)
////		return weight_real;
////	else
////		return 0;
//	
////	return weight_real;
////	}
//	
//	
//}
