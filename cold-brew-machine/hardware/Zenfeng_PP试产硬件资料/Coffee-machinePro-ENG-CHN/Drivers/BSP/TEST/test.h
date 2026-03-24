/**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#ifndef __TEST_H__
#define __TEST_H__

#include "sys.h"
#include "init.h"

void DrawTestPage(u8 *str);
void Display_ButtonUp(u16 x1,u16 y1,u16 x2,u16 y2);
void menu_test(void);
void Test_Color(void);
void Test_FillRec(void);
void Test_Circle(void);
void Test_Triangle(void);
void English_Font_test(void);
void Chinese_Font_test(void);
void Pic_test(const unsigned char *p);
void Load_Drow_Dialog(void);
void Touch_Test(void);
void main_test(void);
void Rotate_Test(void);
void Task_Str(u8 *str);

void Weight_Show_16(uint8_t x, uint8_t y, int32_t weight);
void Weight_Show_12(uint8_t x, uint8_t y, int32_t weight);

void Temp_Show(float Temp, uint8_t state);
void Temp_Show_0(int32_t Temp, uint8_t size, uint8_t symbol);
void Temp_Show_1(int32_t Temp, uint8_t size, uint8_t symbol);

void Drip_Show_0(int32_t Drip, uint8_t size, uint8_t symbol);
void Drip_Show_1(int32_t Drip, uint8_t size, uint8_t symbol);

void Min_Show_0(int32_t Min, uint8_t size, uint8_t symbol);
void Min_Show_1(int32_t Min, uint8_t size, uint8_t symbol);

void clean_16_to_12(void);

void Time_Show(void);

uint8_t Count_Down_Show(uint32_t minute);

uint8_t Count_Down_unShow(uint32_t minute);

void Task_0xFFF0_Show(Coffee_mode mode_now);

void Task_0xFF1F_Show(uint16_t Task);

void Task_CoffeeType_Show(Coffee_mode mode_now);
void Task_Tea_Add_Show(uint16_t tea_add, uint8_t state);

void Task_0xFF16_Show(uint8_t Custom_state, uint16_t Data);
void Task_SETTING_Show(uint8_t Setting_state, uint8_t Setting_Data);				//设置页面 Setting

void Task_LanguageSwitch_Show(void);


void Task_0xF1FF_Show(uint16_t coffee_add);

void Task_Choose_Show(uint8_t init_tag, uint8_t progress_bar);
void Task_Saved_Show(uint8_t selected_option);
void Task_Named_Show(uint8_t init_tag, uint8_t progress_bar);
void Task_NameOver_Show(void);

void Task_ExecuteWay_Show(uint8_t selected_option); //存储后执行选择界面

void Task_NameReplace_Show(void);
void Task_AckReplace_Show(void);
void Task_EditOrDelete_Show(uint8_t selected_option);
void Task_NameDelete_Show(void);

void Task_0x1FFF_Show(Coffee_mode mode_now);
void Task_0x2FFF_Show(uint8_t init_tag, uint8_t progress_bar);											//准备页面
void Task_0x3FFF_Show(uint8_t init_tag, uint32_t Reservation_Time);		//reserve
void Task_0xEFFF_Show(uint8_t init_tag, uint8_t progress_bar); 											//Cancel
void Task_0xFFFF_Show(uint16_t state, Coffee_mode mode_now)	;		//执行页面

void Task_Finish_Show(void);

void Task_test_Show(void);
void ERROR_Show(uint8_t error);

void Task_Factory_test_Show(uint8_t check_done);

#endif
