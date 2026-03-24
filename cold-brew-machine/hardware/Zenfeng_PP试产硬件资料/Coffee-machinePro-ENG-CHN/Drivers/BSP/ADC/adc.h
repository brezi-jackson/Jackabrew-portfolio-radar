#ifndef __ADC_H
#define __ADC_H
#include "sys.h"

/******************************************************************************************/
/* ADC及引脚 定义 */

/************ TEMP0 **************/

#define ADC_TEMP0_GPIO_PORT			GPIOC
#define ADC_TEMP0_GPIO_PIN			GPIO_PIN_3
#define ADC_TEMP0_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)	/* PC口时钟使能 */

#define ADC_TEMP0					ADC1 
#define ADC_CH_TEMP0				ADC_CHANNEL_13								/* 通道Y,  0 <= Y <= 17 */ 
#define ADC_CH_TEMP0_CLK_ENABLE()	do{ __HAL_RCC_ADC1_CLK_ENABLE(); }while(0)	/* ADC1 时钟使能 */

/************ TEMP1 **************/

#define ADC_TEMP1_GPIO_PORT			GPIOA
#define ADC_TEMP1_GPIO_PIN			GPIO_PIN_0
#define ADC_TEMP1_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)	/* PC口时钟使能 */

#define ADC_TEMP1					ADC1 
#define ADC_CH_TEMP1				ADC_CHANNEL_0                              	/* 通道Y,  0 <= Y <= 17 */ 
#define ADC_CH_TEMP1_CLK_ENABLE()	do{ __HAL_RCC_ADC1_CLK_ENABLE(); }while(0)	/* ADC1 时钟使能 */

/************ TEMP2 **************/

#define ADC_TEMP2_GPIO_PORT			GPIOA
#define ADC_TEMP2_GPIO_PIN			GPIO_PIN_1
#define ADC_TEMP2_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)	/* PC口时钟使能 */

#define ADC_TEMP2					ADC1 
#define ADC_CH_TEMP2				ADC_CHANNEL_1                             	/* 通道Y,  0 <= Y <= 17 */ 
#define ADC_CH_TEMP2_CLK_ENABLE()	do{ __HAL_RCC_ADC1_CLK_ENABLE(); }while(0)	/* ADC1 时钟使能 */

/************ TEC_CURR **************/

#define ADC_TEC0_GPIO_PORT			GPIOC
#define ADC_TEC0_GPIO_PIN			GPIO_PIN_2
#define ADC_TEC0_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)	/* PC口时钟使能 */

#define ADC_TEC0					ADC1 
#define ADC_CH_TEC0					ADC_CHANNEL_12								/* 通道Y,  0 <= Y <= 17 */ 
#define ADC_CH_TEC0_CLK_ENABLE()	do{ __HAL_RCC_ADC1_CLK_ENABLE(); }while(0)	/* ADC1 时钟使能 */

/************ PUMP_CURR **************/

#define ADC_PUMP_GPIO_PORT			GPIOC
#define ADC_PUMP_GPIO_PIN			GPIO_PIN_1
#define ADC_PUMP_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)	/* PC口时钟使能 */

#define ADC_PUMP					ADC1 
#define ADC_CH_PUMP					ADC_CHANNEL_11								/* 通道Y,  0 <= Y <= 17 */ 
#define ADC_CH_PUMP_CLK_ENABLE()	do{ __HAL_RCC_ADC1_CLK_ENABLE(); }while(0)	/* ADC1 时钟使能 */

/******************************************************************************************/

static uint16_t NTC_List[171]=
{
3648,
3633,
3616,
3600,
3582,
3564,
3545,
3525,
3504,
3483,
3461,
3438,
3414,
3390,
3364,
3338,
3311,
3283,
3254,
3225,
3194,
3163,
3131,
3099,
3065,
3031,
2996,
2960,
2924,
2886,
2849,
2810,
2771,
2732,
2692,
2652,
2611,
2569,
2528,
2486,
2444,
2401,
2359,
2316,
2273,
2230,
2187,
2144,
2101,
2059,
2016,
1974,
1932,
1890,
1848,
1807,
1766,
1725,
1685,
1646,
1606,
1568,
1529,
1492,
1455,
1418,
1382,
1347,
1312,
1278,
1244,
1211,
1179,
1148,
1117,
1087,
1058,
1029,
1001,
973,
946,
920,
895,
870,
846,
822,
799,
776,
754,
733,
712,
692,
672,
653,
635,
617,
599,
582,
566,
549,
534,
519,
504,
490,
476,
462,
449,
436,
424,
412,
400,
389,
378,
368,
357,
347,
338,
328,
319,
310,
304,
293,
285,
277,
270,
262,
255,
248,
242,
235,
229,
223,
217,
211,
205,
200,
195,
190,
185,
180,
175,
170,
166,
162,
158,
154,
150,
146,
142,
139,
135,
132,
128,
125,
122,
119,
116,
113,
111,
108,
105,
103,
100,
98,
96,
93,
91,
89,
87,
85,
83
};

static uint16_t NTC_List_1[146]=
{
4135,
4133,
4130,
4127,
4124,
4121,
4118,
4115,
4111,
4108,
4104,
4100,
4096,
4091,
4086,
4082,
4077,
4071,
4066,
4060,
4054,
4048,
4041,
4034,
4027,
4020,
4012,
4004,
3996,
3987,
3978,
3969,
3959,
3949,
3938,
3928,
3916,
3905,
3893,
3881,
3868,
3854,
3841,
3827,
3812,
3797,
3782,
3766,
3749,
3733,
3715,
3697,
3679,
3660,
3641,
3621,
3601,
3580,
3559,
3537,
3515,
3492,
3469,
3445,
3421,
3397,
3371,
3346,
3320,
3293,
3267,
3239,
3211,
3183,
3155,
3126,
3096,
3067,
3036,
3006,
2975,
2944,
2913,
2882,
2850,
2818,
2785,
2753,
2720,
2687,
2654,
2621,
2588,
2555,
2521,
2488,
2454,
2421,
2387,
2354,
2320,
2287,
2254,
2220,
2187,
2154,
2121,
2088,
2056,
2023,
1991,
1959,
1927,
1896,
1864,
1833,
1803,
1772,
1742,
1712,
1682,
1653,
1624,
1595,
1567,
1539,
1511,
1484,
1457,
1430,
1404,
1378,
1352,
1327,
1302,
1278,
1254,
1230,
1207,
1184,
1161,
1139,
1117,
1096,
1075,
1054,
};

extern float Temp_0, Temp_1, Temp_2;

void adc_init(void);                                                /* ADC初始化 */
void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch,uint32_t rank, uint32_t stime); /* ADC通道设置 */
uint32_t adc_get_result(uint32_t ch);                               /* 获得某个通道值  */
uint32_t adc_get_result_average(uint32_t ch, uint8_t times);/* 得到某个通道给定次数采样的平均值 */

void GET_TEMP(void);
void GET_TEMP_1(void);
float GET_CURRENT(void);

#endif 
