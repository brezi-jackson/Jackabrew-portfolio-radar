#ifndef _TASK_H
#define _TASK_H

#include "sys.h"
#include "Init.h"
#include "test.h"
#include "timer.h"

extern int16_t Encoder;

extern uint16_t Task;
extern uint16_t Task_Last;

extern int16_t temp_set;

extern uint8_t variable_tx;
extern float pump_var_a,pump_var_b;

extern Coffee_mode mode_Now;

void Task_Mode_Set_work(void);
void Task_Factory_test_work(void);

void Task_CoffeeType_work(void);

void Task_Tea_Default_work(void);

void Task_Custom_work(void);
void Task_Setting_work(void);

void Task_LanguageSwitch_work(void);

void Task_Bean_Add_work(void);

void Task_Tea_Pods_Add_work(void);
void Task_Tea_Add_work(void);

void Task_Summary_work(void);
void Task_Ready_work(void);

void Task_Choose_work(void);
void Task_Saved_work(void);
void Task_Named_work(void);
void Task_NameOver_work(void);

void Task_ExecuteWay_work(void); //存储后执行选择界面

void Task_NameReplace_work(void);
void Task_AckReplace_work(void);
void Task_EditOrDelete_work(void);
void Task_NameDelete_work(void);

void Task_Reservation_work(void);
void Task_Cancel_work(void);
void Task_Execute_work(void);


uint8_t Task_0xFFFF_Work(uint8_t Work_state, uint32_t Reservation_Second, Coffee_mode mode_now);

uint16_t Custom_Data_Process(uint8_t state_now, uint8_t state_last);

void Custom_Data_Reset(void);

void LED_Set(uint8_t sleep);

#endif
