#ifndef _CS1231_H
#define _CS1231_H
#include "sys.h"

typedef struct {
    int32_t z1;
    float tc;
    int32_t in;
    int32_t out;
} lpf_1rd_t; 

typedef struct {
	int8_t Num;
    int32_t weight_g;
    int32_t weight_tare;
    int32_t weight_mid;
} Weight_g_t; 

extern Weight_g_t Weight_g_0;
extern Weight_g_t Weight_g_1;

void SCALE_Init(void);

uint32_t ReadCount(GPIO_TypeDef *SCK_GPIO_Port,GPIO_TypeDef *Dout_GPIO_Port, uint16_t SCK_Pin, uint16_t Dout_Pin);	//ÔöÒæ128
void Get_Weight_Real(Weight_g_t *Weight_g);
void BUF_Reset(int32_t weight_Reset);
void BUFInit(void);
int32_t Weight_Filter(int8_t Num);

#endif
