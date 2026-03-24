/*******************************************************************************
* Copyright (C) 2019 China Micro Semiconductor Limited Company. All Rights Reserved.
*
* This software is owned and published by:
* CMS LLC, No 2609-10, Taurus Plaza, TaoyuanRoad, NanshanDistrict, Shenzhen, China.
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with CMS
* components. This software is licensed by CMS to be adapted only
* for use in systems utilizing CMS components. CMS shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. CMS is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/

/*****************************************************************************/
/** \file demo_adcb.h
**
** History:
** 
*****************************************************************************/
#ifndef __DEMO_ADC_H_
#define __DEMO_ADC_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "common.h"
#include "cgc.h"
#include "adc.h"
#include "gpio.h"
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
#define Temp1_ADC_DATA		(ADC_CH_4)
#define	Temp1_ADC_CH_MSK	(ADC_CH_4_MSK)
#define Temp2_ADC_DATA		(ADC_CH_5)
#define	Temp2_ADC_CH_MSK	(ADC_CH_5_MSK)
#define Pump_ADC_DATA		(ADC_CH_17)
#define	Pump_ADC_CH_MSK		(ADC_CH_17_MSK)
#define TEC_ADC_DATA		(ADC_CH_10)
#define	TEC_ADC_CH_MSK		(ADC_CH_10_MSK)

#define	ADC_ClearIntFlag_CH() 		{ ADC->LOCK = ADC_LOCK_WRITE_KEY; ADC->ICLR |= Temp1_ADC_CH_MSK; ADC->LOCK = 0x00;}

/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/


/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
static uint16_t NTC_List_1[146]=
{
0xE84,//-20℃
0xE70,//-19℃
0xE5A,//-18℃
0xE44,//-17℃
0xE2D,//-16℃
0xE15,//-15℃
0xDFC,//-14℃
0xDE2,//-13℃
0xDC8,//-12℃
0xDAC,//-11℃
0xD90,//-10℃
0xD72,//-9℃
0xD54,//-8℃
0xD35,//-7℃
0xD15,//-6℃
0xCF4,//-5℃
0xCD2,//-4℃
0xCAF,//-3℃
0xC8C,//-2℃
0xC67,//-1℃
0xC42,//0℃
0xC1C,//1℃
0xBF5,//2℃
0xBCE,//3℃
0xBA6,//4℃
0xB7D,//5℃
0xB53,//6℃
0xB29,//7℃
0xAFF,//8℃
0xAD3,//9℃
0xAA8,//10℃
0xA7C,//11℃
0xA4F,//12℃
0xA23,//13℃
0x9F6,//14℃
0x9C8,//15℃
0x99B,//16℃
0x96D,//17℃
0x93F,//18℃
0x911,//19℃
0x8E4,//20℃
0x8B6,//21℃
0x888,//22℃
0x85B,//23℃
0x82D,//24℃
0x800,//25℃
0x7D3,//26℃
0x7A7,//27℃
0x77A,//28℃
0x74E,//29℃
0x723,//30℃
0x6F8,//31℃
0x6CD,//32℃
0x6A3,//33℃
0x67A,//34℃
0x651,//35℃
0x629,//36℃
0x601,//37℃
0x5DA,//38℃
0x5B3,//39℃
0x58D,//40℃
0x568,//41℃
0x544,//42℃
0x520,//43℃
0x4FD,//44℃
0x4DB,//45℃
0x4B9,//46℃
0x498,//47℃
0x478,//48℃
0x458,//49℃
0x43A,//50℃
0x41C,//51℃
0x3FE,//52℃
0x3E2,//53℃
0x3C6,//54℃
0x3AA,//55℃
0x390,//56℃
0x376,//57℃
0x35D,//58℃
0x344,//59℃
0x32C,//60℃
0x315,//61℃
0x2FE,//62℃
0x2E8,//63℃
0x2D3,//64℃
0x2BE,//65℃
0x2AA,//66℃
0x296,//67℃
0x283,//68℃
0x270,//69℃
0x25E,//70℃
0x24D,//71℃
0x23C,//72℃
0x22B,//73℃
0x21B,//74℃
0x20C,//75℃
0x1FD,//76℃
0x1EE,//77℃
0x1E0,//78℃
0x1D2,//79℃
0x1C5,//80℃
0x1B8,//81℃
0x1AB,//82℃
0x19F,//83℃
0x193,//84℃
0x188,//85℃
0x17D,//86℃
0x172,//87℃
0x168,//88℃
0x15D,//89℃
0x154,//90℃
0x14A,//91℃
0x141,//92℃
0x138,//93℃
0x12F,//94℃
0x127,//95℃
0x11F,//96℃
0x117,//97℃
0x10F,//98℃
0x108,//99℃
0x100,//100℃
0xF9,//101℃
0xF3,//102℃
0xEC,//103℃
0xE6,//104℃
0xDF,//105℃
0xD9,//106℃
0xD4,//107℃
0xCE,//108℃
0xC8,//109℃
0xC3,//110℃
0xBE,//111℃
0xB9,//112℃
0xB4,//113℃
0xAF,//114℃
0xAB,//115℃
0xA6,//116℃
0xA2,//117℃
0x9E,//118℃
0x9A,//119℃
0x96,//120℃
0x92,//121℃
0x8E,//122℃
0x8B,//123℃
0x87,//124℃
0x84,//125℃
};

static uint16_t NTC_List_2[146]=
{
0xFD7,//-20℃
0xFD4,//-19℃
0xFD2,//-18℃
0xFCF,//-17℃
0xFCC,//-16℃
0xFC9,//-15℃
0xFC6,//-14℃
0xFC3,//-13℃
0xFBF,//-12℃
0xFBC,//-11℃
0xFB8,//-10℃
0xFB4,//-9℃
0xFB0,//-8℃
0xFAC,//-7℃
0xFA7,//-6℃
0xFA2,//-5℃
0xF9D,//-4℃
0xF98,//-3℃
0xF93,//-2℃
0xF8D,//-1℃
0xF87,//0℃
0xF81,//1℃
0xF7B,//2℃
0xF74,//3℃
0xF6D,//4℃
0xF65,//5℃
0xF5E,//6℃
0xF56,//7℃
0xF4E,//8℃
0xF45,//9℃
0xF3C,//10℃
0xF33,//11℃
0xF29,//12℃
0xF1F,//13℃
0xF15,//14℃
0xF0A,//15℃
0xEFF,//16℃
0xEF3,//17℃
0xEE7,//18℃
0xEDB,//19℃
0xECE,//20℃
0xEC1,//21℃
0xEB3,//22℃
0xEA5,//23℃
0xE96,//24℃
0xE87,//25℃
0xE77,//26℃
0xE67,//27℃
0xE57,//28℃
0xE46,//29℃
0xE34,//30℃
0xE22,//31℃
0xE10,//32℃
0xDFD,//33℃
0xDEA,//34℃
0xDD6,//35℃
0xDC1,//36℃
0xDAC,//37℃
0xD97,//38℃
0xD81,//39℃
0xD6A,//40℃
0xD53,//41℃
0xD3B,//42℃
0xD23,//43℃
0xD0B,//44℃
0xCF2,//45℃
0xCD8,//46℃
0xCBE,//47℃
0xCA4,//48℃
0xC89,//49℃
0xC6E,//50℃
0xC52,//51℃
0xC35,//52℃
0xC19,//53℃
0xBFC,//54℃
0xBDE,//55℃
0xBC0,//56℃
0xBA2,//57℃
0xB83,//58℃
0xB65,//59℃
0xB45,//60℃
0xB26,//61℃
0xB06,//62℃
0xAE6,//63℃
0xAC5,//64℃
0xAA5,//65℃
0xA84,//66℃
0xA63,//67℃
0xA42,//68℃
0xA20,//69℃
0x9FF,//70℃
0x9DD,//71℃
0x9BB,//72℃
0x999,//73℃
0x978,//74℃
0x956,//75℃
0x934,//76℃
0x912,//77℃
0x8F0,//78℃
0x8CE,//79℃
0x8AC,//80℃
0x88A,//81℃
0x869,//82℃
0x847,//83℃
0x826,//84℃
0x804,//85℃
0x7E3,//86℃
0x7C2,//87℃
0x7A2,//88℃
0x781,//89℃
0x761,//90℃
0x741,//91℃
0x721,//92℃
0x701,//93℃
0x6E2,//94℃
0x6C3,//95℃
0x6A4,//96℃
0x686,//97℃
0x668,//98℃
0x64A,//99℃
0x62C,//100℃
0x60F,//101℃
0x5F2,//102℃
0x5D6,//103℃
0x5BA,//104℃
0x59E,//105℃
0x583,//106℃
0x568,//107℃
0x54E,//108℃
0x533,//109℃
0x51A,//110℃
0x500,//111℃
0x4E7,//112℃
0x4CF,//113℃
0x4B6,//114℃
0x49F,//115℃
0x487,//116℃
0x470,//117℃
0x459,//118℃
0x443,//119℃
0x42D,//120℃
0x418,//121℃
0x402,//122℃
0x3EE,//123℃
0x3D9,//124℃
0x3C5,//125℃
};

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/

/*****************************************************************************
 ** \brief	ADCB_Software_Trigger_Mode
 **			设置ADCB软件触发转换
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_Software_Trigger_Mode(void);
void ADC_Config(void);
float GET_TEMP_ADC(uint16_t adcx, uint16_t *NTC_List);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_ADCB_H_ */

