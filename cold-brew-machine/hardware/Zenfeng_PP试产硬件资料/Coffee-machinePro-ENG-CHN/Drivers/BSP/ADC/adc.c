#include "adc.h"
#include "delay.h"

ADC_HandleTypeDef g_adc_handle;   /* ADC句柄 */

/**
 * @brief       ADC初始化函数
 *   @note      本函数支持ADC1/ADC2任意通道, 但是不支持ADC3
 *              我们使用12位精度, ADC采样时钟=12M, 转换时间为: 采样周期 + 12.5个ADC周期
 *              设置最大采样周期: 239.5, 则转换时间 = 252 个ADC周期 = 21us
 * @param       无
 * @retval      无
 */
void adc_init(void)
{
    g_adc_handle.Instance = ADC1;							/* 选择哪个ADC */
    g_adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;		/* 数据对齐方式：右对齐 */
    g_adc_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;		/* 非扫描模式，仅用到一个通道 */
    g_adc_handle.Init.ContinuousConvMode = DISABLE;			/* 关闭连续转换模式 */
    g_adc_handle.Init.NbrOfConversion = 1;					/* 赋值范围是1~16，本实验用到1个规则通道序列 */
    g_adc_handle.Init.DiscontinuousConvMode = DISABLE;		/* 禁止规则通道组间断模式 */
    g_adc_handle.Init.NbrOfDiscConversion = 0;				/* 配置间断模式的规则通道个数，禁止规则通道组间断模式后，此参数忽略 */
    g_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;/* 触发转换方式：软件触发 */
    HAL_ADC_Init(&g_adc_handle);							/* 初始化 */

    HAL_ADCEx_Calibration_Start(&g_adc_handle);				/* 校准ADC */
}

/**
 * @brief       ADC底层驱动，引脚配置，时钟使能
                此函数会被HAL_ADC_Init()调用
 * @param       hadc:ADC句柄
 * @retval      无
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance == ADC1)
	{
		GPIO_InitTypeDef gpio_init_struct;
		RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
		
		/************ TEMP0 **************/

		ADC_CH_TEMP0_CLK_ENABLE();                           	/* 开启GPIO时钟 */
		ADC_TEMP0_GPIO_CLK_ENABLE();							/* 使能ADCx时钟 */

        /* 设置ADC时钟 */
        adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;	/* ADC外设时钟 */
        adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;		/* 分频因子6时钟为72M/6=12MHz */
        HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);				/* 设置ADC时钟 */

        /* 设置AD采集通道对应IO引脚工作模式 */
        gpio_init_struct.Pin = ADC_TEMP0_GPIO_PIN;				/* ADC通道IO引脚 */
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;				/* 模拟 */
        HAL_GPIO_Init(ADC_TEMP0_GPIO_PORT, &gpio_init_struct);
		
		ADC_CH_TEMP1_CLK_ENABLE();                           	/* 开启GPIO时钟 */
		ADC_TEMP1_GPIO_CLK_ENABLE();							/* 使能ADCx时钟 */

		/************ TEMP1 **************/
		
//		ADC_CH_TEMP1_CLK_ENABLE();                           	/* 开启GPIO时钟 */
//		ADC_TEMP1_GPIO_CLK_ENABLE();							/* 使能ADCx时钟 */
//		
//		/* 设置ADC时钟 */
//		adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;	/* ADC外设时钟 */
//		adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;		/* 分频因子6时钟为72M/6=12MHz */
//		HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);				/* 设置ADC时钟 */

//		/* 设置AD采集通道对应IO引脚工作模式 */
//		gpio_init_struct.Pin = ADC_TEMP1_GPIO_PIN;				/* ADC通道IO引脚 */
//		gpio_init_struct.Mode = GPIO_MODE_ANALOG;				/* 模拟 */
//		HAL_GPIO_Init(ADC_TEMP1_GPIO_PORT, &gpio_init_struct);
//		
//		ADC_TEC0_GPIO_CLK_ENABLE();
//		ADC_CH_TEC0_CLK_ENABLE();

//		/************ TEMP2 **************/
//		
////		ADC_CH_TEMP2_CLK_ENABLE();                           	/* 开启GPIO时钟 */
////		ADC_TEMP2_GPIO_CLK_ENABLE();							/* 使能ADCx时钟 */
//		
//		/* 设置ADC时钟 */
//		adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;	/* ADC外设时钟 */
//		adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;		/* 分频因子6时钟为72M/6=12MHz */
//		HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);				/* 设置ADC时钟 */

//		/* 设置AD采集通道对应IO引脚工作模式 */
//		gpio_init_struct.Pin = ADC_TEMP2_GPIO_PIN;				/* ADC通道IO引脚 */
//		gpio_init_struct.Mode = GPIO_MODE_ANALOG;				/* 模拟 */
//		HAL_GPIO_Init(ADC_TEMP2_GPIO_PORT, &gpio_init_struct);

		/************ TEC_CURR **************/
		
		ADC_TEC0_GPIO_CLK_ENABLE();
		ADC_CH_TEC0_CLK_ENABLE();

		/* 设置ADC时钟 */
		adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;	/* ADC外设时钟 */
		adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;		/* 分频因子6时钟为72M/6=12MHz */
		HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);				/* 设置ADC时钟 */

		/* 设置AD采集通道对应IO引脚工作模式 */
		gpio_init_struct.Pin = ADC_TEC0_GPIO_PIN;				/* ADC通道IO引脚 */
		gpio_init_struct.Mode = GPIO_MODE_ANALOG;				/* 模拟 */
		HAL_GPIO_Init(ADC_TEC0_GPIO_PORT, &gpio_init_struct);
		
		/************ PUMP_CURR **************/
		
		ADC_PUMP_GPIO_CLK_ENABLE();
		ADC_CH_PUMP_CLK_ENABLE();

		/* 设置ADC时钟 */
		adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;	/* ADC外设时钟 */
		adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;		/* 分频因子6时钟为72M/6=12MHz */
		HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);				/* 设置ADC时钟 */

		/* 设置AD采集通道对应IO引脚工作模式 */
		gpio_init_struct.Pin = ADC_PUMP_GPIO_PIN;				/* ADC通道IO引脚 */
		gpio_init_struct.Mode = GPIO_MODE_ANALOG;				/* 模拟 */
		HAL_GPIO_Init(ADC_PUMP_GPIO_PORT, &gpio_init_struct);
    }
}

/**
 * @brief       设置ADC通道采样时间
 * @param       adcx : adc句柄指针,ADC_HandleTypeDef
 * @param       ch   : 通道号, ADC_CHANNEL_0~ADC_CHANNEL_17
 * @param       stime: 采样时间  0~7, 对应关系为:
 *   @arg       ADC_SAMPLETIME_1CYCLE_5, 1.5个ADC时钟周期        ADC_SAMPLETIME_7CYCLES_5, 7.5个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_13CYCLES_5, 13.5个ADC时钟周期     ADC_SAMPLETIME_28CYCLES_5, 28.5个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_41CYCLES_5, 41.5个ADC时钟周期     ADC_SAMPLETIME_55CYCLES_5, 55.5个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_71CYCLES_5, 71.5个ADC时钟周期     ADC_SAMPLETIME_239CYCLES_5, 239.5个ADC时钟周期
 * @param       rank: 多通道采集时需要设置的采集编号,
                假设你定义channle1的rank=1，channle2 的rank=2，
                那么对应你在DMA缓存空间的变量数组AdcDMA[0] 就i是channle1的转换结果，AdcDMA[1]就是通道2的转换结果。 
                单通道DMA设置为 ADC_REGULAR_RANK_1
 *   @arg       编号1~16：ADC_REGULAR_RANK_1~ADC_REGULAR_RANK_16
 * @retval      无
 */
void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch, uint32_t rank, uint32_t stime)
{
    ADC_ChannelConfTypeDef adc_ch_conf;
    
    adc_ch_conf.Channel = ch;                            /* 通道 */
    adc_ch_conf.Rank = rank;                             /* 序列 */
    adc_ch_conf.SamplingTime = stime;                    /* 采样时间 */
    HAL_ADC_ConfigChannel(adc_handle, &adc_ch_conf);     /* 通道配置 */
}

/**
 * @brief       获得ADC转换后的结果
 * @param       ch: 通道值 0~17，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_17
 * @retval      无
 */
uint32_t adc_get_result(uint32_t ch)
{
    adc_channel_set(&g_adc_handle , ch, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5);    /* 设置通道，序列和采样时间 */

    HAL_ADC_Start(&g_adc_handle);                            /* 开启ADC */
    HAL_ADC_PollForConversion(&g_adc_handle, 10);            /* 轮询转换 */
    return (uint16_t)HAL_ADC_GetValue(&g_adc_handle);        /* 返回最近一次ADC1规则组的转换结果 */
}

/**
 * @brief       获取通道ch的转换值，取times次,然后平均
 * @param       ch      : 通道号, 0~17
 * @param       times   : 获取次数
 * @retval      通道ch的times次转换结果平均值
 */
uint32_t adc_get_result_average(uint32_t ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++)     /* 获取times次数据 */
    {
        temp_val += adc_get_result(ch);
        delay_ms(5);
    }

    return temp_val / times;        /* 返回平均值 */
}

float GET_TEMP_ADC_0(uint32_t adcx)
{	
	float adcx1, temp;
	
	double x1,x2,x3,x4;
	
	adcx1 = (float)adcx / 1000;
	
//	x4 = adcx1 * adcx1 * adcx1 * adcx1 * 1.798;
//	x3 = adcx1 * adcx1 * adcx1 * 18.921;
//	x2 = adcx1 * adcx1 * 69.259;
//	x1 = adcx1 * 127.258;

//	temp = x4 - x3 + x2 - x1 + 126.86;

	x4 = adcx1 * adcx1 * adcx1 * adcx1 * 1.739;
	x3 = adcx1 * adcx1 * adcx1 * 18.45;
	x2 = adcx1 * adcx1 * 68.105;
	x1 = adcx1 * 126.194;

	temp = x4 - x3 + x2 - x1 + 126.86;
	
	return temp;	
}

//float GET_TEMP_ADC(uint32_t adcx)
//{	
//	float adcx1, temp;
//	
//	double x1,x2,x3,x4, x5, x6;
//	
//	adcx1 = (float)adcx / 1000;

//	x1 = adcx1;
//	x2 = x1 * adcx1;
//	x3 = x2 * adcx1;
//	x4 = x3 * adcx1;
//	x5 = x4 * adcx1;
//	x6 = x5 * adcx1;

//	temp = x6 * 2.02f - x5 * 26.34f + x4 * 134.12f - x3 * 340.0f + x2 * 455.0f - x1 * 334.42f + 170.27f;
//	
//	return temp;	
//}		

float GET_TEMP_ADC(uint32_t adcx)
{
	uint8_t middle=0;
	uint8_t indexL=0;
	uint8_t indexR=170;
	
	if(adcx>=NTC_List[0])
    	return -20;
	if(adcx<=NTC_List[170])
   		return 150;
   	
//	while((indexR-indexL) > 1)
//	{
//		middle=(indexL + indexR)>>1;
//		if(adcx==NTC_List[middle])
//			indexL =  middle;
//		else if(adcx > NTC_List[middle])
//			indexR = middle;
//		else if(adcx < NTC_List[middle])
//			indexL = middle;
//	}
	
	for(int i = 0; i <=170; i++)
	{
		if(adcx > NTC_List[i])
		{
			indexL = i - 1;
			break;
		}
	}
	
	float data=0;
	float t=0;
	float result=0;

	data = NTC_List[indexL] - NTC_List[indexL + 1];
	t = (float)(NTC_List[indexL] - adcx) / data;

	result = (float)indexL + t - 20.f;
	return result;
}

float GET_TEMP_ADC_1(uint32_t adcx)
{
	uint8_t middle=0;
	uint8_t indexL=0;
	uint8_t indexR=145;
	
	if(adcx>=NTC_List_1[0])
    	return -20;
	if(adcx<=NTC_List_1[145])
   		return 125;
   	
//	while((indexR-indexL) > 1)
//	{
//		middle=(indexL + indexR)>>1;
//		if(adcx==NTC_List[middle])
//			indexL =  middle;
//		else if(adcx > NTC_List[middle])
//			indexR = middle;
//		else if(adcx < NTC_List[middle])
//			indexL = middle;
//	}
	
	for(int i = 0; i <=145; i++)
	{
		if(adcx > NTC_List_1[i])
		{
			indexL = i - 1;
			break;
		}
	}
	
	float data=0;
	float t=0;
	float result=0;

	data = NTC_List_1[indexL] - NTC_List_1[indexL + 1];
	t = (float)(NTC_List_1[indexL] - adcx) / data;

	result = (float)indexL + t - 20.f;
	return result;
}

float Temp_0, Temp_1, Temp_2;
uint32_t Temp_ad_0, Temp_ad_1;

void GET_TEMP(void)
{	
	uint32_t Temp;
	static uint8_t Temp_tag;
	
	switch(Temp_tag)
	{
		case 0:
			Temp_tag++;
			Temp_ad_0 = adc_get_result_average(ADC_CH_TEMP0, 5);
			Temp_0 =  GET_TEMP_ADC_0(Temp_ad_0);// * 3.3f /4096;
			break;
		
		case 1:
			Temp_tag = 0;
			Temp_ad_1 = adc_get_result_average(ADC_CH_TEMP1, 3);
			Temp_1 = GET_TEMP_ADC(Temp_ad_1);
			break;
//		
//		case 2:
//			Temp_tag = 0;
//			Temp = adc_get_result_average(ADC_CH_TEMP2, 5);
//			Temp_2 = GET_TEMP_ADC(Temp);
//			break;
	}
}

#define Temp_ADC_Table_Size 126

uint16_t Temp_ADC_Table[Temp_ADC_Table_Size] = {
3751, 3730, 3709, 3686, 3663, 3639, 3614, 3588, 3561, 3533, //-20 ~ -11
3505, 3475, 3444, 3413, 3380, 3347, 3313, 3278, 3242, 3205, //-10 ~ -1
3167, 														//  0
3129, 3090, 3050, 3009, 2968, 2926, 2883, 2840, 2796, 2752, //  1 ~ 10
2708, 2663, 2617, 2572, 2526, 2480, 2434, 2387, 2341, 2295, // 11 ~ 20
2248, 2202, 2156, 2110, 2065, 2019, 1974, 1930, 1885, 1841, // 21 ~ 30
1798, 1755, 1713, 1671, 1630, 1589, 1549, 1509, 1471, 1432, // 31 ~ 40
1395, 1358, 1322, 1287, 1252, 1219, 1185, 1153, 1121, 1090, // 41 ~ 50
1060, 1031, 1002,  974,  946,  920,  893,  868,  844,  820, // 51 ~ 60
 796,  773,  751,  730,  709,  688,  669,  649,  631,  613, // 61 ~ 70
 595,  578,  561,  545,  530,  514,  500,  485,  472,  458, // 71 ~ 80
 445,  433,  420,  408,  397,  386,  375,  364,  354,  344, // 81 ~ 90
 335,  325,  316,  308,  299,  291,  283,  275,  268,  260, // 91 ~ 100
 253, 247, 240, 233, 227									//101 ~ 105
};

//float GET_TEMP_ADC_1(uint32_t adcx)
//{	
//	int temp, i;
//	
//	for(i = 0; i <= Temp_ADC_Table_Size; i++)
//	{
//		if(adcx < Temp_ADC_Table[i])
//		{
//			temp = i - 20;
//		}
//	}
//	
//	return temp;	
//}

//void GET_TEMP_1(void)
//{
//	uint32_t Temp;

//	Temp = adc_get_result_average(ADC_CH_TEMP0, 5);
//	Temp_0 =  GET_TEMP_ADC_1(Temp);// * 3.3f /4096;
//}

float GET_CURRENT(void)
{
	uint32_t Current;
	
	float current;

	Current = adc_get_result(ADC_CH_TEC0);
	
	current = (((float)Current / 4096) - 0.1f) * 16.5f;

	return current;	
}
