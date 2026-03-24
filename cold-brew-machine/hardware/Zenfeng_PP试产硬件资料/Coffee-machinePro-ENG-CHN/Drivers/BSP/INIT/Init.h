#ifndef _INIT_H
#define _INIT_H
#include "sys.h"
#include "usart.h"
#include "delay.h"
//#include "usmart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"

#include "myiic.h"
#include "motor.h"
#include "oled.h"

#include "timer.h"
#include "adc.h"
#include "spi.h"
#include "gtim.h"

#define mode_num 6
#define tea_mode_num 4

#define SCREEN_VERSION_0 00
#define SCREEN_VERSION_1 01
#define SCREEN_VERSION_2 05

typedef struct {
    int32_t z1;
    float tc;
    int32_t in;
    int32_t out;
} lpf_1rd_t; 

//typedef struct {
//	uint8_t *name;
//	uint8_t *title_1;
//	uint8_t *title_2;
//	
//	int16_t Heating_Temp;
//	int16_t Cooling_Temp;
//						
//	uint8_t Heating_spd;
//	uint8_t Cooling_spd;
//			
//	int16_t Heating_Time;
//	int16_t Cooling_Time;	
//	
//	uint16_t Heating_CWratio;
//	uint16_t Cooling_CWratio;
//	
//	uint32_t Total_Time;
//	uint16_t Total_ml;
//	
//	uint8_t mode_tag;
//} Coffee_mode;

typedef struct {
	uint16_t data_default;
	
	uint16_t max;
	uint16_t min;
	
	float Resolution;
} Data_set;

extern Coffee_mode Default_modes_Regular[];
extern Coffee_mode Default_modes_Concentrate[];
extern Coffee_mode Default_modes_SlowDrip[];

extern Coffee_mode Tea_Default_modes[];

extern Coffee_mode Default_modes[];
extern Data_set Custom_Setting[];
	
void Drivers_Init(void);

extern lpf_1rd_t lpf1rd_0;
extern lpf_1rd_t lpf1rd_1;

extern PidObject Temp_pid;

float lpf_1rd_calc(lpf_1rd_t *lpf1rd, int32_t new_sample);

#endif
