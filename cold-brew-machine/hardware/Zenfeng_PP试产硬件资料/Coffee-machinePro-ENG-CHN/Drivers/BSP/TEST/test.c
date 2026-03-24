#include "lcd.h"
#include "delay.h"
#include "gui.h"
#include "task.h"
#include "test.h"
#include "timer.h"
#include "motor.h"
#include "oled.h"
#include "usart.h"
#include "math.h"

extern uint8_t languageMark;

extern uint8_t UnitMark;

extern uint8_t SoundMark;

/*****************************************************************************
 * @name       :void Rotate_Test(void)
 * @date       :2018-08-09 
 * @function   :rotate test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Temp_Show(float Temp, uint8_t state)
{
	uint8_t size = 28;
	uint8_t size_1 = 20;
	uint8_t x = 46;
	uint8_t x_1 = 64;
	uint8_t y_2 = 88;
	uint8_t size_2 = 16;
	
	uint8_t y_1 = 128 - size_1; //
	
	if(UnitMark == 0){
		if(state == 1)
		{
			OLED_ShowString(x+14*2+4,50, ".", size, 1,0);
			
			OLED_ShowNum(x,		 50, Temp   , 2, size, 1);
			OLED_ShowNum(x+14*3, 50, Temp*10, 1, size, 1);
			
			gui_circle(108, 63, 1, 2, 0);
			OLED_ShowString(112,60, "C",  16, 1,0);
		}
		else if(state == 2)
		{
			OLED_ShowString(x_1+8*2,y_2, ".", size_2, 1,0);
			
			OLED_ShowNum(x_1,		 y_2, Temp   , 2, size_2, 1);
			OLED_ShowNum(x_1+8*3, y_2, Temp*10, 1, size_2, 1);
			
			gui_circle(108, y_2 + 3, 1, 2, 0);
			OLED_ShowString(112,y_2, "C",  16, 1,0);
			
			OLED_ShowString(x_1+8*2,y_2+size_2, ".", size_2, 1,0);
			
			OLED_ShowNum(x_1,		y_2+size_2, Temp_1   , 2, size_2, 1);
			OLED_ShowNum(x_1+8*3, 	y_2+size_2, Temp_1*10, 1, size_2, 1);
			
			gui_circle(108, 	y_2+size_2+3, 1, 2, 0);
			OLED_ShowString(112,y_2+size_2, "C",  16, 1,0);
		}
		else
		{
			if(Temp_0/10)
					OLED_ShowNum(6,	y_1, (uint16_t)Temp_0/10,	1, size_1, 1);
			OLED_ShowNum(6+size_1/2,	y_1, (uint16_t)Temp_0%10,	1, size_1, 1);
		}
	}
	else{
			if(state == 1)
			{
				OLED_ShowString(x+14*2+4,50, ".", size, 1,0);
				
				OLED_ShowNum(x-14,		 50, (uint16_t)(roundf((float)Temp * 1.8f + 32.0f))   , 3, size, 1);
				OLED_ShowNum(x+14*3, 50, (uint16_t)(roundf((float)Temp * 18.0f + 32.0f)), 1, size, 1);
				
				gui_circle(108, 63, 1, 2, 0);
				OLED_ShowString(112,60, "F",  16, 1,0);
			}
			else if(state == 2)
			{
				OLED_ShowString(x_1+8*2,y_2, ".", size_2, 1,0);
				
				OLED_ShowNum(x_1-14,		 y_2, (uint16_t)(roundf((float)Temp * 1.8f + 32.0f))   , 3, size_2, 1);
				OLED_ShowNum(x_1+8*3, y_2, (uint16_t)(roundf((float)Temp * 18.0f + 32.0f)), 1, size_2, 1);
				
				gui_circle(108, y_2 + 3, 1, 2, 0);
				OLED_ShowString(112,y_2, "F",  16, 1,0);
				
				OLED_ShowString(x_1+8*2,y_2+size_2, ".", size_2, 1,0);
				
				OLED_ShowNum(x_1-14,		y_2+size_2, (uint16_t)(roundf((float)Temp_1 * 1.8f + 32.0f))   , 3, size_2, 1);
				OLED_ShowNum(x_1+8*3, 	y_2+size_2, (uint16_t)(roundf((float)Temp_1 * 18.0f + 32.0f)), 1, size_2, 1);
				
				gui_circle(108, 	y_2+size_2+3, 1, 2, 0);
				OLED_ShowString(112,y_2+size_2, "F",  16, 1,0);
			}
			else
			{
				OLED_ShowNum(6,	y_1, (uint16_t)(roundf((float)((uint16_t)Temp_0) * 1.8f + 32.0f)) ,	3, size_1, 1);
			}
	}
	
	OLED_Refresh();
}

uint8_t Count_Down_Show(uint32_t minute)
{
	static uint16_t x = 32;
	static uint16_t y = 64;
	static uint8_t size = 24;
	
	static uint32_t second_last;
	
	uint8_t size_half = size / 2;
	
	uint32_t second, second_a, minute_t, hour_t;
		
	second_a = minute - Second - (Minute_2 * 10 + Minute_1 + ((Hour_2*10 + Hour_1) * 60)) * 60;
	
	hour_t = second_a / 3600;
	
	minute_t = (second_a % 3600)/ 60;

	second = second_a % 60;
		
	if(second != second_last)
	{
		OLED_ShowString(x+size_half*2,y-size/2, ":", size, 1,0);
		
		OLED_ShowString(x+size_half*5,y-size/2, ":", size, 1,0);
		
		OLED_ShowNum(x, 			y-size/2, hour_t / 10,		1, size, 1);
		OLED_ShowNum(x+size_half, 	y-size/2, hour_t,			1, size, 1);
		OLED_ShowNum(x+size_half*3,	y-size/2, minute_t / 10,	1, size, 1);
		OLED_ShowNum(x+size_half*4,	y-size/2, minute_t,		1, size, 1);
		OLED_ShowNum(x+size_half*6,	y-size/2, second/10,		1, size, 1);
		OLED_ShowNum(x+size_half*7,	y-size/2, second,			1, size, 1);
	}
	
	second_last = second;
	
	OLED_Refresh();
	
	if(second_a == 0)
		return 1;
	else
		return 0;
}

uint8_t Count_Down_unShow(uint32_t minute)
{	
	uint32_t second;
	
	second = minute - Second - (Minute_2 * 10 + Minute_1 + ((Hour_2*10 + Hour_1) * 60)) * 60;
		
	if(second == 0)
		return 1;
	else
		return 0;
}

extern unsigned char bmpSetting[];

void Task_0xFFF0_Show(Coffee_mode mode_now)		//mode 选择
{
	static uint16_t y_0		= 44; //52
	static uint16_t y_1		= 66; //80
	static uint8_t size_0 	= 24; //24
	static uint8_t size_1 	= 24; //18
	static uint16_t fc 		= WHITE;
	static uint16_t bc 		= BLACK;
	static uint16_t mode 	= 1;
	
	OLED_ClearArea();
	
	Gui_StrCenter(0, 10, fc, bc, "MODE",16, 1,languageMark);//80

	if(mode_Now.mode_tag != 6)
	{
		switch(languageMark)
		{
			case 0:
				if(mode_now.title_1 == mode_now.title_2)
				{
					Gui_StrCenter(0, y_0 + size_0 / 2,	fc, bc, mode_now.name,	size_0, mode,0);
				}
				else
				{	
					Gui_StrCenter(0, y_0,	fc, bc, mode_now.name,	size_0, mode,0);
					Gui_StrCenter(0, y_1,	fc, bc, mode_now.title_1,	size_1, mode,0);
				}
				break;

			case 1:
				Gui_StrCenter(0, y_0 + size_0 / 2,	fc, bc, mode_now.name,	16, mode,languageMark);
				break;

			default:
				break;
		}
	}
	else if(mode_Now.mode_tag == 6)
	{
		OLED_ShowPicture(62,52,36,36,bmpSetting,1);
	}
	
	OLED_Refresh();
}

void Task_CoffeeType_Show(Coffee_mode mode_now) 
{
		static int16_t display_offset = 0;
		static int16_t selected_idx;
		
    uint8_t size_val = (languageMark == 1) ? 16 : 24;
    OLED_ClearArea();
    Gui_StrCenter(0, 10, WHITE, BLACK, "ROAST LEVEL", 16, 1, languageMark);

    char* all_options[] = {"DARK", "MEDIUM", "LIGHT"};
    uint8_t option_count = sizeof(all_options) / sizeof(all_options[0]);

    if (Encoder >= option_count) Encoder = option_count - 1;
    if (Encoder < 0) Encoder = 0;
    selected_idx = Encoder;

    // 滚动控制
    if (selected_idx < display_offset) display_offset = selected_idx;
    else if (selected_idx > display_offset + 2) display_offset = selected_idx - 2;

    for (uint8_t i = 0; i < option_count; i++) {
        if (i < display_offset || i >= display_offset + 3) continue;

        uint8_t y = 40 + (i - display_offset) * 30;
		
        if (i == selected_idx)
		{
            Gui_StrCenter(0, y - (size_val-16)/2, WHITE, BLACK, all_options[i], size_val, 1, languageMark);
            OLED_DrawRoundedRect(10, y - 7, 140, 30, 3, 1);
        } 
		else
		{
            Gui_StrCenter(0, y, GRAY, BLACK, all_options[i], 16, 1, languageMark);
        }
    }

    // 滚动条
    if (option_count > 3) {
        for (uint8_t y = 40; y < 110; y += 2) OLED_DrawPoint(158, y, 1);
        uint8_t scroll_y = 40 + (selected_idx * (110 - 40 - 8)) / (option_count - 1);
        OLED_DrawLine(159, scroll_y, 160, scroll_y + 8, 1);
        OLED_DrawLine(157, scroll_y, 158, scroll_y + 8, 1);
        OLED_DrawLine(158, scroll_y, 159, scroll_y + 8, 1);
    }

    OLED_Refresh();
}

void Task_TeaType_Show()
{
	static int16_t tea_display_offset = 0;
	static int16_t tea_selected_idx;
	
    uint8_t size_val = (languageMark == 1) ? 16 : 24;
    OLED_ClearArea();
    Gui_StrCenter(0, 10, WHITE, BLACK, "TEA TYPE", 16, 1, languageMark);

    char* all_options[] = {"GREEN TEA", "WHITE TEA", "OOLONG", "BLACK TEA", "HERBAL"};
    uint8_t option_count = sizeof(all_options) / sizeof(all_options[0]);

    if (Encoder >= option_count) Encoder = option_count - 1;
    if (Encoder < 0) Encoder = 0;
    tea_selected_idx = Encoder;

    // 滚动逻辑
    if (tea_selected_idx < tea_display_offset)
        tea_display_offset = tea_selected_idx;
    else if (tea_selected_idx > tea_display_offset + 2)
        tea_display_offset = tea_selected_idx - 2;

    for (uint8_t i = 0; i < option_count; i++)
	{
        if (i < tea_display_offset || i >= tea_display_offset + 3) continue;

        uint8_t y = 40 + (i - tea_display_offset) * 30;
		
        if (i == tea_selected_idx)
		{
            Gui_StrCenter(0, y - (size_val-16)/2, WHITE, BLACK, all_options[i], size_val, 1, languageMark);
            OLED_DrawRoundedRect(10, y - 7, 140, 30, 3, 1);
        } 
		else 
		{
            Gui_StrCenter(0, y, GRAY, BLACK, all_options[i], 16, 1, languageMark);
        }
    }

    if (option_count > 3) 
	{
        for (uint8_t y = 40; y < 110; y += 2) OLED_DrawPoint(158, y, 1);
        uint8_t scroll_y = 40 + (tea_selected_idx * (110 - 40 - 8)) / (option_count - 1);
        OLED_DrawLine(159, scroll_y, 160, scroll_y + 8, 1);
        OLED_DrawLine(157, scroll_y, 158, scroll_y + 8, 1);
        OLED_DrawLine(158, scroll_y, 159, scroll_y + 8, 1);
    }

    OLED_Refresh();
}

// 示例调用方式：
// uint8_t current_encoder_value = 0; // 初始值
// Task_CoffeeType_Show(current_mode, current_encoder_value);

extern Coffee_mode mode_Now;

extern unsigned char bmpRatio[];
extern unsigned char bmpBloomTemp[];
extern unsigned char bmpBloomFlow[];
extern unsigned char bmpBrewFlow[];
extern unsigned char bmpBrewTemp[];

void Task_0xFF16_Show(uint8_t Custom_state, uint16_t Data)	//客制页面 custom
{
	uint8_t size_0 = 16;
	uint8_t size_1 = 28;
//	uint8_t x_0 = 64;
	uint8_t y_0 = 10;
			
	OLED_ClearArea();	
	
	switch(Custom_state)
	{
		case 0:
			Gui_StrCenter(0,y_0,WHITE, BLACK, "COFFEE:WATER", 16, 1,languageMark);
			Gui_StrCenter(0,y_0+16,WHITE, BLACK, "BLOOM RATIO", 16, 1,languageMark);
			OLED_ShowPicture(70,94,24,24,bmpRatio,1);
			if(Data >0)
			{
				OLED_ShowString(56, 60, "1", size_0, 1,0);
				OLED_ShowString(56 + 13, 60, ":", size_0, 1,0);	
				OLED_ShowNum(80, 50, Data / 10, 1, size_1, 1);
				OLED_ShowString(80+size_1/2, 50, ".", size_1, 1,0);
				OLED_ShowNum(80+size_1/2+4, 50, Data, 1, size_1, 1);
				mode_Now.Heating_CWratio = Data;
			}
			else
			{
				mode_Now.Heating_CWratio = 0;
				Gui_StrCenter(0,50,WHITE, BLACK, "OFF", size_1, 1,0);
			}
	
			break;

		case 1:
			Gui_StrCenter(0,y_0,WHITE, BLACK, "BLOOM TEMP", 16, 1,languageMark);
			OLED_ShowPicture(70,94,24,24,bmpBloomTemp,1);
			if(Data <= Custom_Setting[1].min)
			{
				Gui_StrCenter(0,50,WHITE, BLACK, "ROOM", size_1, 1,0);
			}
			else
			{	
				if(UnitMark == 0){
					OLED_ShowNum(56, 50, Data, 2, size_1, 1);
					gui_circle(94, 63, 1, 2, 0);
					OLED_ShowString(98,60, "C",  size_0, 1,0);
				}
				else{
					//Data = (uint16_t)(roundf((float)Data * 1.8f + 32.0f));
//					Data = Data * (9/5) + 32;
					//OLED_ShowNum(56, 50, Data * (9/5) + 32, 2, size_1, 1);
					OLED_ShowNum(56, 50, (uint16_t)(roundf((float)Data * 1.8f + 32.0f)), 3, size_1, 1);
					gui_circle(102, 63, 1, 2, 0);
					OLED_ShowString(106,60, "F",  size_0, 1,0);
				}

			}
			mode_Now.Heating_Temp = Data;			
			break;
		case 2:
			mode_Now.Heating_spd = Data;
			
			Gui_StrCenter(0,y_0,WHITE, BLACK, "BLOOM FLOW", 16, 1,languageMark);
			OLED_ShowPicture(70,94,24,24,bmpBloomFlow,1);
			OLED_ShowString(50, 50, ".", size_1, 1,0);
			OLED_ShowNum(56, 50, Data, 2, size_1, 1);
			OLED_ShowString(94, 60, "ml", size_0, 1,0);
			OLED_ShowString(94+size_0, 60, "/s", size_0, 1,0);
			break;
		case 3:
			Gui_StrCenter(0,y_0,WHITE, BLACK, "COLD BREW TEMP", 16, 1,languageMark);
			OLED_ShowPicture(70,94,24,24,bmpBrewTemp,1);
			if(Data >= Custom_Setting[3].max)
			{
				Gui_StrCenter(0,50,WHITE, BLACK, "ROOM", size_1, 1,0);
			}
			else
			{
				if(UnitMark == 0){
					OLED_ShowNum(56, 50, Data, 2, size_1, 1);
					gui_circle(94, 63, 1, 2, 0);
					OLED_ShowString(98,60, "C",  size_0, 1,0);
				}
				else{
					//Data = Data * (9/5) + 32;
					OLED_ShowNum(56, 50, (uint16_t)(roundf((float)Data * 1.8f + 32.0f)), 2, size_1, 1);
					gui_circle(94, 63, 1, 2, 0);
					OLED_ShowString(98,60, "F",  size_0, 1,0);
				}
			}
			mode_Now.Cooling_Temp = Data;
			break;
		case 4:
			mode_Now.Cooling_spd = Data;
		
			Gui_StrCenter(0,y_0,WHITE, BLACK, "COLD BREW FLOW", 16, 1,languageMark);
			OLED_ShowPicture(70,94,24,24,bmpBrewFlow,1);
			OLED_ShowString(50, 50, ".", size_1, 1,0);
			OLED_ShowNum(56, 50, Data, 2, size_1, 1);
			OLED_ShowString(94, 60, "ml", size_0, 1,0);
			OLED_ShowString(94+size_0, 60, "/s", size_0, 1,0);
			break;
		case 5:
			mode_Now.Cooling_CWratio = Data - mode_Now.Heating_CWratio;
		
//			OLED_ShowString(33, y_0, "COFFEE", size_0, 1);
//			OLED_ShowString(83, y_0, ":", size_0, 1);
//			OLED_ShowString(88, y_0, "WATER", size_0, 1);

			if((Data / 10)>25)
				Gui_StrCenter(0,y_0,WHITE, BLACK, "TEA:WATER", 16, 1,languageMark);
			else 
				Gui_StrCenter(0,y_0,WHITE, BLACK, "COFFEE:WATER", 16, 1,languageMark);
		  //Gui_StrCenter(0,y_0,WHITE, BLACK, "COFFEE:WATER", 16, 1,languageMark);
			Gui_StrCenter(0,y_0+16,WHITE, BLACK, "TOTAL RATIO", 16, 1,languageMark);
			OLED_ShowPicture(70,94,24,24,bmpRatio,1);
			OLED_ShowString(56, 60, "1", size_0, 1,0);
			OLED_ShowString(56 + 13, 60, ":", size_0, 1,0);	
			//OLED_ShowNum(80, 50, Data / 10, 2, size_1, 1);
	
			OLED_ShowNum(80, 50, Data / 10, 2, size_1, 1);
			OLED_ShowString(80+size_1, 50, ".", size_1, 1,0);
			OLED_ShowNum(80+size_1+4, 50, Data, 1, size_1, 1);		
			break;

		default:
			break;
	}
	
	OLED_Refresh();	
}

// 定义语言选项
typedef struct {
    char *name;
    uint8_t lang_mark;
} LanguageOption;

LanguageOption language_options[] = {
    {"ENG", 0},
    {"CHN", 1},
};
#define LANGUAGE_COUNT (sizeof(language_options)/sizeof(language_options[0]))

void Task_LanguageSwitch_Show(void)
{
	static int16_t display_offset = 0;
	static int16_t selected_idx;

	uint8_t size_val = (languageMark == 1) ? 16 : 24;
	OLED_ClearArea();
	Gui_StrCenter(0, 10, WHITE, BLACK, "LANGUAGE", 16, 1, languageMark);

	char* all_options[] = {"ENGLISH", "CHINESE"};
	uint8_t option_count = sizeof(all_options) / sizeof(all_options[0]);

	/* 修复2：严格的范围控制 */
	selected_idx = Encoder;
	if (selected_idx >= option_count) 
	{
		Encoder = selected_idx = option_count - 1;
	} 
	else if (selected_idx < 0)
	{
		Encoder = selected_idx = 0;
	}

	// 滚动控制
	if (selected_idx < display_offset) display_offset = selected_idx;
	else if (selected_idx > display_offset + 2) display_offset = selected_idx - 2;

	for (uint8_t i = 0; i < option_count; i++) 
	{
		if (i < display_offset || i >= display_offset + 3) continue;

		uint8_t y = 40 + (i - display_offset) * 30;
		if (i == selected_idx)
		{
			Gui_StrCenter(0, y - (size_val-16)/2, WHITE, BLACK, all_options[i], size_val, 1, languageMark);
			OLED_DrawRoundedRect(10, y - 7, 140, 30, 3, 1);
		} 
		else 
		{
			Gui_StrCenter(0, y, WHITE, BLACK, all_options[i], 16, 1, languageMark);
		}
	}

	// 滚动条
	if (option_count > 3) 
	{
		for (uint8_t y = 40; y < 110; y += 2) OLED_DrawPoint(158, y, 1);
		
		uint8_t scroll_y = 40 + (selected_idx * (110 - 40 - 8)) / (option_count - 1);
		OLED_DrawLine(159, scroll_y, 160, scroll_y + 8, 1);
		OLED_DrawLine(157, scroll_y, 158, scroll_y + 8, 1);
		OLED_DrawLine(158, scroll_y, 159, scroll_y + 8, 1);
	}

	OLED_Refresh();
}

void Task_Unit_Show() 
{
	int16_t display_offset = 0;
	int16_t selected_idx;

	uint8_t size_val = (languageMark == 1) ? 16 : 24;
	OLED_ClearArea();
	Gui_StrCenter(0, 10, WHITE, BLACK, "UNIT", 16, 1, languageMark);

	char* all_options[] = {"METRIC", "IMPERIAL"};
	uint8_t option_count = sizeof(all_options) / sizeof(all_options[0]);

	/* 统一范围控制 */
	selected_idx = Encoder;
	if (selected_idx >= option_count) Encoder = selected_idx = option_count - 1;
	else if (selected_idx < 0) Encoder = selected_idx = 0;

	// 滚动控制
	if (selected_idx < display_offset) display_offset = selected_idx;
	else if (selected_idx > display_offset + 2) display_offset = selected_idx - 2;

	for (uint8_t i = 0; i < option_count; i++)
	{
		if (i < display_offset || i >= display_offset + 3) continue;

		uint8_t y = 40 + (i - display_offset) * 30;
		
		if (i == selected_idx) 
		{
			Gui_StrCenter(0, y - (size_val-16)/2, WHITE, BLACK, all_options[i], size_val, 1, languageMark);
			OLED_DrawRoundedRect(10, y - 7, 140, 30, 3, 1);
		} 
		else 
		{
			Gui_StrCenter(0, y, GRAY, BLACK, all_options[i], 16, 1, languageMark);
		}
	}

    OLED_Refresh();
}

void Task_Sound_Show() 
{
		int16_t display_offset = 0;
		int16_t selected_idx;
		
    uint8_t size_val = (languageMark == 1) ? 16 : 24;
    OLED_ClearArea();
    Gui_StrCenter(0, 10, WHITE, BLACK, "SOUND", 16, 1, languageMark);

    char* all_options[] = {"ON", "OFF"};
    uint8_t option_count = sizeof(all_options) / sizeof(all_options[0]);

    /* 统一范围控制 */
    selected_idx = Encoder;
    if (selected_idx >= option_count) Encoder = selected_idx = option_count - 1;
    else if (selected_idx < 0) Encoder = selected_idx = 0;

    // 滚动控制
    if (selected_idx < display_offset) display_offset = selected_idx;
    else if (selected_idx > display_offset + 2) display_offset = selected_idx - 2;

    for (uint8_t i = 0; i < option_count; i++) {
        if (i < display_offset || i >= display_offset + 3) continue;

        uint8_t y = 40 + (i - display_offset) * 30;
        if (i == selected_idx)
		{
            Gui_StrCenter(0, y - (size_val-16)/2, WHITE, BLACK, all_options[i], size_val, 1, languageMark);
            OLED_DrawRoundedRect(10, y - 7, 140, 30, 3, 1);
        } 
		else 
		{
            Gui_StrCenter(0, y, GRAY, BLACK, all_options[i], 16, 1, languageMark);
        }
    }

    OLED_Refresh();
}

void Task_Reset_Show(uint8_t init_tag, uint8_t progress_bar){

	static uint8_t progress_bar_last;
	
		OLED_ClearArea();
	
		Gui_StrCenter(0,10,WHITE, BLACK, "RESET ALL?",16, 1,languageMark);//80
		Gui_StrCenter(0,30,WHITE, BLACK, "THIS WILL ERASE",16, 1,languageMark);//80
		Gui_StrCenter(0,52,WHITE, BLACK, "ALL SETTINGS",16, 1,languageMark);//80
		Gui_StrCenter(0,80,WHITE, BLACK, "HOLD TO CONFIRM",16, 1,languageMark);//80
		OLED_DrawLine(0,101,160, 101, 1);
	
	if(init_tag)
	{
		if(progress_bar < progress_bar_last || progress_bar == 0)
		{
			OLED_DrawLine(0,100,160, 100, 0);
			OLED_DrawLine(0,102,160, 102, 0);
		}
		else
		{
			OLED_DrawLine(0,100,progress_bar, 100, 1);
			OLED_DrawLine(0,102,progress_bar, 102, 1);
		}
	}
//	else
//	{
//		Gui_StrCenter(0,10,WHITE, BLACK, "RESET ALL?",16, 1,0);//80
//		Gui_StrCenter(0,30,WHITE, BLACK, "THIS WILL ERASE",16, 1,0);//80
//		Gui_StrCenter(0,52,WHITE, BLACK, "ALL SETTINGS",16, 1,0);//80
//		Gui_StrCenter(0,80,WHITE, BLACK, "HOLD TO CONFIRM",16, 1,0);//80
//		OLED_DrawLine(0,101,160, 101, 1);
//	}
	
//			OLED_ClearArea();
//		
//		Gui_StrCenter(0,10,WHITE, BLACK, "RESET ALL?",16, 1,0);//80
//		Gui_StrCenter(0,30,WHITE, BLACK, "THIS WILL ERASE",16, 1,0);//80
//		Gui_StrCenter(0,52,WHITE, BLACK, "ALL SETTINGS",16, 1,0);//80
//		Gui_StrCenter(0,80,WHITE, BLACK, "HOLD TO CONFIRM",16, 1,0);//80
//		OLED_DrawLine(0,101,160, 101, 1);
	
	progress_bar_last = progress_bar;
	
	OLED_Refresh();

}


//// 将 Show_Descale_UI 和 Show_SystemCheck_UI 从原来 Task_SETTING_Show 拆出
//void Show_Descale_UI(uint8_t step,uint8_t Setting_Data)
//{
//	
//}

//void Show_SystemCheck_UI(uint8_t step)
//{
//    // 原来的 SYSTEM CHECK 逻辑搬到这里
//}

uint8_t Descale_Done, System_Check_Done;
uint8_t setting_led_close;
uint8_t Descale_Finish;

extern uint8_t bmpClock[];

uint8_t Pump_cal;
extern float TEC_Current;
static uint8_t TEC_Current_Error = 1;
static uint8_t TEC_Cooling_Error = 1;

extern uint8_t  Main_Version_0;
extern uint8_t  Main_Version_1;
extern uint8_t  Main_Version_2;

void Task_SETTING_Show(uint8_t Setting_state, uint8_t Setting_Data)
{
	static uint8_t descaling_temp_tag;
	//static uint8_t check_done;
	static uint8_t selected_idx;
	static int16_t display_offset = 0;

	uint8_t size_val = (languageMark == 1) ? 16 : 24;
	uint8_t size_0 = (languageMark == 1) ? 16 : 24;

	uint8_t size_40 = 20;
	uint8_t size_41 = 16;
	uint8_t x_0 = 92;
	uint8_t y_0 = 32;
	uint8_t y_1 = 52;
	uint8_t y_2 = 72;
	uint8_t y_3 = 92;
	uint8_t y_4 = 112;
	
//	OLED_ClearArea();
	
    Gui_StrCenter(0, 10, WHITE, BLACK, "SETTING", 16, 1, languageMark);

    char* all_options[] = {"LANGUAGE", "UNIT", "SOUND", "DESCALE","RESET ALL"};//, "SYSTEM CHECK","RESET ALL"};
    uint8_t option_count = sizeof(all_options) / sizeof(all_options[0]);
	
	if (Setting_Data == 0) 
	{
        // 主菜单滚动列表显示
        selected_idx = Setting_state;

        // 滚动控制
        if (selected_idx < display_offset) 
		{
			
            display_offset = selected_idx;
        } 
		else if (selected_idx > display_offset + 2) 
		{
            display_offset = selected_idx - 2;
        }
				
		for (uint8_t i = 0; i < option_count; i++) 
		{
			if (i < display_offset || i >= display_offset + 3) 
				continue;

			uint8_t y = 40 + (i - display_offset) * 30;
			
			if (i == selected_idx) 
			{
				Gui_StrCenter(0, y - (size_val-16)/2, WHITE, BLACK, all_options[i], size_val, 1, languageMark);
				OLED_DrawRoundedRect(5, y - 7, 150, 30, 3, 1);
			} 
			else 
			{
				Gui_StrCenter(0, y, GRAY, BLACK, all_options[i], 16, 1, languageMark);
			}
		}

		// 滚动条
		if (option_count > 3) 
		{
			for (uint8_t y = 40; y < 110; y += 2) 
				OLED_DrawPoint(158, y, 1);
			
			uint8_t scroll_y = 40 + (selected_idx * (110 - 40 - 8)) / (option_count - 1);
			
			OLED_DrawLine(159, scroll_y, 160, scroll_y + 8, 1);
			OLED_DrawLine(157, scroll_y, 158, scroll_y + 8, 1);
			OLED_DrawLine(158, scroll_y, 159, scroll_y + 8, 1);
		}

		OLED_Refresh();
		return;
	}
    else
	{
		// 子界面处理
		switch (Setting_state) 
		{
			case 0: // Language 子界面
				Task_LanguageSwitch_Show();
				break;
			
			case 1: // UNIT 设置界面
				Task_Unit_Show();
				break;
			
			case 2: // SOUND 设置界面
				Task_Sound_Show();
				break;
			
			case 3: // Descale 子界面
				Gui_StrCenter(0,10,WHITE, BLACK, "DESCALE", 16, 1,languageMark);
				
				if(Descale_Done)
				{
					Gui_StrCenter(0,52,WHITE, BLACK, "FINISHED", size_val, 1,languageMark);
					
					if(Count_Down_unShow(120))
					{
						time_start_tag = 0;
						Time_Reset();
						Touch_Reset();
						
						Descale_Done = 0;
						setting_led_close = 0;
						
						Descale_Finish = 1;
					}
					else
					{
						time_start_tag = 1;
					}
				}
				else if(Setting_Data)
				{
					if(Setting_Data == 1)
					{
						temp_switch = 0;
						pump_set = 0;
						descaling_temp_tag = 0;
						time_start_tag = 0;
						Time_Reset();
										
						setting_led_close = 0;

						OLED_ShowPicture(35,55,24,24,bmpClock,1);
						OLED_ShowString(65,60, "00h 05m", 16, 1,0);
						Gui_StrCenter(0,100,WHITE, BLACK, "PRESS TO START", 16, 1,languageMark);
					}
					else if(Setting_Data == 2)
					{
						temp_switch = 2;
						temp_set = 65;
						
						setting_led_close = 1;
						
						if(Temp_0 >= temp_set && descaling_temp_tag == 0)
						{
							descaling_temp_tag = 1;
							OLED_ClearArea();
						}							
							
						if(descaling_temp_tag && Descale_Done == 0)
						{
							if(Count_Down_Show(300))
							{
								temp_switch = 0;
								pump_set = 0;
								time_start_tag = 0;
								Time_Reset();
								
								Descale_Done = 1;
								setting_led_close = 1;
							}
							else
							{
								pump_set = 65;
								time_start_tag = 1;
							}
						}
						else
							Temp_Show(Temp_0, 1);
					}
				}
				else
				{
					Gui_StrCenter(0,10,WHITE, BLACK, "SETTING", 16, 1,languageMark);
					temp_switch = 0;
					time_start_tag = 0;
					Time_Reset();
					Descale_Done = 0;
					Gui_StrCenter(0,64 - size_0 / 2,WHITE, BLACK, "DESCALE", size_0, 1,languageMark);
				}
				
				OLED_Refresh();
				break;
				
			case 4: // System Check 子界面
//				OLED_ClearArea();

//				Gui_StrCenter(0,10,WHITE, BLACK, "SYSTEM CHECK", 16, 1,languageMark);
//					
//				if(Second < 1)			BUZZER_Set(100);		//蜂鸣器测试:开启1秒
//				else					BUZZER_Set(0);
//					
//				if(TEC_Current > 9)		TEC_Current_Error = 0;	//全功率开启tec, 检测
//				if(Temp_0 < 18)			TEC_Cooling_Error = 0;
//					
//					if(Count_Down_unShow(8))
//					{									
//						System_Check_Done = 1;
//						
//						setting_led_close = 1;
//						
//						pump_set = 0;
//						temp_switch = 0;
//						time_start_tag = 0;
//						
//						{
//							OLED_ClearArea();
//							Gui_StrCenter(0,10,WHITE, BLACK, "SYSTEM CHECK", size_41, 1,0);
//							
//							{
//								Gui_StrCenter(0, y_0,WHITE, BLACK, "PUMP   ", size_41, 1,0);

//								if(Pump_Error)
//									OLED_ShowString(x_0, y_0, "NG", size_40, 1,0);
//								else
//									OLED_ShowString(x_0, y_0, "OK", size_41, 1,0);
//							}

//							{							
//								Gui_StrCenter(0, y_1,WHITE, BLACK, "NTC1   ", size_41, 1,0);
//								if(NTC_1_Error)
//									OLED_ShowString(x_0, y_1, "NG", size_40, 1,0);
//								else
//									OLED_ShowString(x_0, y_1, "OK", size_41, 1,0);
//							}

//							{
//								Gui_StrCenter(0, y_2,WHITE, BLACK, "NTC2   ", size_41, 1,0);
//								if(NTC_2_Error)
//									OLED_ShowString(x_0,y_2, "NG", size_40, 1,0);
//								else
//									OLED_ShowString(x_0,y_2, "OK", 16, 1,0);
//							}

//							{
//								Gui_StrCenter(0,y_3,WHITE, BLACK, "TANK   ", size_41, 1,0);
//								if(Limit_Switch)
//									OLED_ShowString(x_0,y_3, "NG", size_40, 1,0);
//								else
//									OLED_ShowString(x_0,y_3, "OK", size_41, 1,0);
//							}
//							
//							{
//								Gui_StrCenter(0,y_4,WHITE, BLACK, "TEC    ", size_41, 1,0);
//								if(TEC_Current_Error || TEC_Cooling_Error)
//									OLED_ShowString(x_0,y_4, "NG", size_40, 1,0);
//								else
//									OLED_ShowString(x_0,y_4, "OK", size_41, 1,0);
//							}

//							OLED_Refresh();
//						}
//					}
//					else
//					{
//						temp_switch = 1;
//						temp_set = 5;
//						
//						time_start_tag = 1;
//						
//						OLED_ShowNum(70, 100, (uint16_t)TEC_Current, 2, 16, 1);
//						OLED_ShowString(86,100, ".", 16, 1,0);
//						OLED_ShowNum(92, 100, (uint16_t)(TEC_Current * 10), 1, 16, 1);
//						OLED_ShowString(108,100, "A", 16, 1,0);
//						
//						OLED_ShowString(20, 26, "SCREEN", size_41, 1,0);
//						OLED_ShowNum(70, 26, SCREEN_VERSION_0, 2, 16, 1);
//						OLED_ShowString(86,26, ".", 16, 1,0);
//						OLED_ShowNum(92, 26, SCREEN_VERSION_1, 2, 16, 1);
//						OLED_ShowString(108,26, ".", 16, 1,0);
//						OLED_ShowNum(114, 26, SCREEN_VERSION_2, 2, 16, 1);
//						
//						OLED_ShowString(20, 82, "MAIN"	, size_41, 1,0);
//						OLED_ShowNum(70, 82, Main_Version_0, 2, 16, 1);
//						OLED_ShowString(86,82, ".", 16, 1,0);
//						OLED_ShowNum(92, 82, Main_Version_1, 2, 16, 1);
//						OLED_ShowString(108,82, ".", 16, 1,0);
//						OLED_ShowNum(114, 82, Main_Version_2, 2, 16, 1);
//						
//						Temp_Show(Temp_0, 1);
//						pump_set = 80;
//				}
//				OLED_Refresh();
//				break;
//					case 5: // 重置机器 设置界面

Task_Reset_Show(Progress_bar_tag,Progress_bar);
						break;
		}
    }
}

//void Task_SETTING_Show(uint8_t Setting_state, uint8_t Setting_Data)	//设置页面 Setting
//{
//	static uint8_t descaling_temp_tag;
//	static uint8_t check_done;
//	uint8_t size_0 = 24;;
//	
//	switch(languageMark){
//		case 0:
//					size_0 = 24;
//					break;
//		case 1:
//					size_0 = 16;
//					break;
//		default:
//			break;
//	}
//	
//	switch(Setting_state)
//	{
//		case 0:	
//			if(Setting_Data)
//			{
//				Gui_StrCenter(0,10,WHITE, BLACK, "DESCALE", 16, 1,languageMark);
//				
//				if(Setting_Data == 1)
//				{
//					temp_switch = 0;
//					pump_set = 0;
//					descaling_temp_tag = 0;
//					time_start_tag = 0;
//									
//					setting_led_close = 0;

//					OLED_ShowString(15,64, "NO", size_0, 1,languageMark);
//					OLED_ShowString(125,64, "YES", size_0, 1,languageMark);
//				}
//				else if(Setting_Data == 2)
//				{
//					temp_switch = 2;
//					temp_set = 65;
//					setting_led_close = 1;
//					if(Temp_0 >= temp_set && descaling_temp_tag == 0)
//						descaling_temp_tag = 1;						
//						
//					if(descaling_temp_tag && setting_done == 0)
//					{
//						if(Count_Down_Show(300))
//						{
//							temp_switch = 0;
//							pump_set = 0;
//							time_start_tag = 0;
//							Time_Reset();
//							setting_done = 1;
//							setting_led_close = 0;
//						}
//						else
//						{
//							pump_set = 65;
//							time_start_tag = 1;
//						}
//					}
//					else
//						Temp_Show(Temp_0, 1);
//				}
//			}
//			else
//			{
//				Gui_StrCenter(0,10,WHITE, BLACK, "SETTING", 16, 1,languageMark);
//				temp_switch = 0;
//				time_start_tag = 0;
//				Time_Reset();
//				setting_done = 0;
//				Gui_StrCenter(0,64 - size_0 / 2,WHITE, BLACK, "DESCALE", size_0, 1,languageMark);
//			}
//			
//			OLED_Refresh();
//			break;
//		
//		case 1:
//			Gui_StrCenter(0,10,WHITE, BLACK, "SETTING", 16, 1,languageMark);
//			if(Setting_Data)
//			{		
//				if(check_done == 0)
//				{
//					temp_switch = 1;
//					temp_set = 5;
//			
//					if(Count_Down_unShow(5))
//					{
//						pump_set = 0;
//						temp_switch = 0;
//						
//						time_start_tag = 0;
//						Time_Reset();

//						check_done = 1;
//						OLED_ClearArea();
//						{
//							Gui_StrCenter(0,64 - size_0 *2,WHITE, BLACK, "PUMP    ", 16, 1,languageMark);

//							if(Pump_Error)
//								OLED_ShowString(100,64 - size_0 *2, "NG", 24, 1,0);
//							else
//								OLED_ShowString(100,64 - size_0 *2, "OK", 16, 1,0);
//						}
//						
//						{							
//							Gui_StrCenter(0,64 - size_0 *1,WHITE, BLACK, "NTC1    ", 16, 1,0);
//							if(NTC_1_Error)
//								OLED_ShowString(100,64 - size_0 *1, "NG", 24, 1,0);
//							else
//								OLED_ShowString(100,64 - size_0 *1, "OK", 16, 1,0);
//						}
//						
//						{
//							Gui_StrCenter(0,64,WHITE, BLACK, "NTC2    ", 16, 1,0);
//							if(NTC_2_Error)
//								OLED_ShowString(100,64, "NG", 24, 1,0);
//							else
//								OLED_ShowString(100,64, "OK", 16, 1,0);
//						}
//						
//						{
//							Gui_StrCenter(0,64 + size_0 *1,WHITE, BLACK, "TANK    ", 16, 1,0);
//							if(Limit_Switch)
//								OLED_ShowString(100,64 + size_0 *1, "NG", 24, 1,0);
//							else
//								OLED_ShowString(100,64 + size_0 *1, "OK", 16, 1,0);
//							OLED_Refresh();
//						}
//					}
//					else
//					{
//						Temp_Show(Temp_0, 1);
//						pump_set = 65;
//						
//						time_start_tag = 1;
//					}
//				}
//			}
//			else
//			{
//				Gui_StrCenter(0,64 - size_0 / 2,WHITE, BLACK, "SYSTEM CHECK", size_0, 1,languageMark);
//				Temp_Show(Temp_0, 2);
//				check_done = 0;
//				time_start_tag = 0;
//				Time_Reset();
//			}
//			break;
//		case 2:  // 第三个设置界面：语言切换z
//						if(Setting_Data == 0){
////								OLED_ClearArea();
//								// 显示语言入口界面
//								Gui_StrCenter(0, 10, WHITE, BLACK, "SETTING", 16, 1, languageMark);
//								Gui_StrCenter(0, 64 - size_0 / 2, WHITE, BLACK, "LANGUAGE", size_0, 1, languageMark);
//								OLED_Refresh();
//						}
//						else if(Setting_Data == 1){
//								Task_LanguageSwitch_Show();
//						}
//				break;
//		default:
//			break;
//	}
//}

//void Task_LanguageSwitch_Show(uint8_t selected_option) // 语言切换界面
//{
//	OLED_ClearArea();

//	// 显示标题
//	Gui_StrCenter(0, 10, WHITE, BLACK, "UNIT", 16, 1,0);

//	if (selected_option == 1)
//	{
//		// 画选中框 YES
//			// 第三行 - YES
//		Gui_StrCenter(0, 60, WHITE, BLACK, "ENG", 24, 1,0);
//		OLED_DrawRoundedRect(10, 55, 150, 30, 3, 1);
//			// 从 (60,10) 到 (40,30) 画一条右上到左下的斜线
//		OLED_DrawSolidDiagonalLine(125, 70, 130, 65, 1);
//		OLED_DrawSolidDiagonalLine(124, 70, 129, 65, 1);
//		OLED_DrawSolidDiagonalLine(125, 70, 130, 75, 1);
//		OLED_DrawSolidDiagonalLine(124, 70, 129, 75, 1);
//		OLED_DrawLine(124, 70, 145, 70, 1); // 横线中间
//	}
//	else{
//		Gui_StrCenter(0, 60, WHITE, BLACK, "ENG", 16, 1,0);
//	}


//	if (selected_option == 0)
//	{
//			// 第四行 - NO
//		//Gui_StrCenter(0, 90, WHITE, BLACK, "DELETE", 24, 1);
//		OLED_ShowChinese(40, 90, 6, 24, 1); // 显示“中”
//		OLED_ShowChinese(64, 90, 7, 24, 1); // 显示“文”
//		
//		// 画选中框 NO
//		OLED_DrawRoundedRect(10, 85, 150, 30, 3, 1);
//		OLED_DrawSolidDiagonalLine(125, 100, 130, 95, 1);
//		OLED_DrawSolidDiagonalLine(124, 100, 129, 95, 1);
//		OLED_DrawSolidDiagonalLine(125, 100, 130, 105, 1);
//		OLED_DrawSolidDiagonalLine(124, 100, 129, 105, 1);
//		OLED_DrawLine(124, 100, 145, 100, 1); // 横线中间
//	}
//	else{
//		//Gui_StrCenter(0, 100, WHITE, BLACK, "DELETE", 16, 1);
//		OLED_ShowChinese(48, 100, 6, 16, 1); // 显示“中”
//		OLED_ShowChinese(64, 100, 7, 16, 1); // 显示“文”
//	}

//	OLED_Refresh();
//}

static int16_t display_offset = 0;
extern int16_t selected_idx;
//extern uint8_t need_refresh;
extern Coffee_mode custom_Kind[10];

char read_result[32] = {0};

void Task_Choose_Show(uint8_t init_tag, uint8_t progress_bar)
{

	// 首先计算实际有效的选项数量
	uint8_t valid_items = 0;
	uint8_t last_valid_idx = 0;
	
	for (uint8_t i = 0; i < 10; i++)
	{
		if (custom_Kind[i].name != NULL && strlen(custom_Kind[i].name) > 0 && strcmp(custom_Kind[i].name, "None") != 0)
		{
			valid_items++;
			last_valid_idx = i; // 记录最后一个有效选项的索引
		}
	}
		
    uint8_t size_val = (languageMark == 1) ? 16 : 24;
    OLED_ClearArea();
    Gui_StrCenter(0, 10, WHITE, BLACK, "SAVED PROFILE", 16, 1, languageMark);

    //char* all_options[] = {"DARK", "MEDIUM", "LIGHT"};
    //uint8_t option_count = sizeof(all_options) / sizeof(all_options[0]);

    if (Encoder >= valid_items) Encoder = valid_items - 1;
    if (Encoder < 0) Encoder = 0;
    selected_idx = Encoder;

    // 滚动控制
    if (selected_idx < display_offset) display_offset = selected_idx;
    else if (selected_idx > display_offset + 2) display_offset = selected_idx - 2;

    for (uint8_t i = 0; i < valid_items; i++) {
        if (i < display_offset || i >= display_offset + 3) continue;

        uint8_t y = 40 + (i - display_offset) * 30;
        if (i == selected_idx) {
            Gui_StrCenter(0, y, WHITE, BLACK, custom_Kind[i].name, 24, 1, 0);
            OLED_DrawRoundedRect(10, y - 5, 140, 30, 3, 1);
        } else {
            Gui_StrCenter(0, y, GRAY, BLACK, custom_Kind[i].name, 16, 1, 0);
        }
    }

    // 滚动条
    if (valid_items > 3) {
        for (uint8_t y = 40; y < 110; y += 2) OLED_DrawPoint(158, y, 1);
        uint8_t scroll_y = 40 + (selected_idx * (110 - 40 - 8)) / (valid_items - 1);
        OLED_DrawLine(159, scroll_y, 160, scroll_y + 8, 1);
        OLED_DrawLine(157, scroll_y, 158, scroll_y + 8, 1);
        OLED_DrawLine(158, scroll_y, 159, scroll_y + 8, 1);
    }

		if(valid_items == 0){
				Gui_StrCenter(0, 52, WHITE, BLACK, "<EMPTY>", 24, 1,0);
		}

    OLED_Refresh();

}

extern int16_t selected_idx_replace;

unsigned char bmpArrow[]={

0x80,0xC0,0xE0,0xB0,0x98,0x88,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,
0x01,0x03,0x07,0x0D,0x19,0x11,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,/*"箭头.bmp",0*/
	
};

void Task_NameReplace_Show() {
    // 首先计算实际有效的选项数量
    uint8_t valid_items = 0;
    uint8_t last_valid_idx = 0;
    for (uint8_t i = 0; i < 10; i++) {
        if (custom_Kind[i].name != NULL && strlen(custom_Kind[i].name) > 0 && strcmp(custom_Kind[i].name, "None") != 0) {
            valid_items++;
            last_valid_idx = i; // 记录最后一个有效选项的索引
        }
    }

    // 如果没有有效项目，直接返回
    if (valid_items == 0) {
        OLED_ClearArea();
        Gui_StrCenter(0, 10, WHITE, BLACK, "MEMORY FULL,REPLACE", 16, 1,languageMark);
        //Gui_StrCenter(0, 50, WHITE, BLACK, "NO ITEMS TO REPLACE", 16, 1);
        OLED_Refresh();
        return;
    }

    // 处理编码器输入
    if (Encoder < 0) {
        Encoder = 0;
    }
    else if (Encoder >= valid_items) {
        Encoder = valid_items - 1;
    }
    selected_idx_replace = Encoder;

    // 保持选中项在可见范围内
    if (selected_idx_replace < display_offset) {
        display_offset = selected_idx_replace;
    }
    else if (selected_idx_replace > display_offset + 4) {  // 一次显示5个，所以偏移量最大为4
        display_offset = selected_idx_replace - 4;
    }
//    need_refresh = 1;

    // 刷新显示
    if (1) {
        OLED_ClearArea();

        // 显示标题
        Gui_StrCenter(0, 10, WHITE, BLACK, "MEMORY FULL,REPLACE", 16, 1,languageMark);
        // 显示操作提示
       // Gui_StrCenter(0, 120, WHITE, BLACK, "RIGHT TO SELECT", 12, 1);

        uint8_t displayed_items = 0;
        // 显示5个列表项
        for (uint8_t i = 0; i < 10; i++) {
            uint8_t idx = i;
            if (custom_Kind[idx].name == NULL || strlen(custom_Kind[idx].name) == 0 || strcmp(custom_Kind[idx].name, "None") == 0) {
                continue;
            }
            if (displayed_items < display_offset) {
                displayed_items++;
                continue;
            }
            if (displayed_items >= display_offset + 5) {  // 一次最多显示5个
                break;
            }

            uint8_t y = 32 + (displayed_items - display_offset) * 18; // 缩小间距为18像素
            
            // 显示编号 (1. 2. 3. 等) - 始终正常显示
            char index_str[4];
            snprintf(index_str, sizeof(index_str), "%d.", idx + 1);
            OLED_ShowString(5, y, index_str, 16, 1,0);
            
            // 显示名称
            uint8_t name_x = 30; // 编号后留出空间
            
            if (idx == selected_idx_replace) {
                // 选中项：反色显示名称
                uint8_t len = strlen(custom_Kind[idx].name);
                // 计算反色区域
                uint8_t start_x = name_x;
                uint8_t end_x = name_x + len * 8; // 16像素字体每个字符约8像素宽
                
                // 绘制反色背景
                for (uint8_t px = start_x; px < end_x; px++) {
                    for (uint8_t py = y; py < y + 16; py++) {
                        OLED_DrawPoint(px, py, 1); // 反色背景
                    }
                }
                
                // 显示反色文字
                //Gui_Str(name_x, y, 1, 0, custom_Kind[idx].name, 16, 0);
								OLED_ShowString(name_x, y, custom_Kind[idx].name, 16, 0,0);
                // 显示选择箭头
                //OLED_ShowString(132, y, "<-", 16, 1,0);
								OLED_ShowPicture(132,y,16,16,bmpArrow,1);
								
            } else {
                // 非选中项：正常显示
                Gui_Str(name_x, y, 1, 0, custom_Kind[idx].name, 16, 1);
            }
            displayed_items++;
        }

        // 滚动条虚线绘制代码
        if (valid_items > 0) {
            uint8_t scroll_y = 32 + (selected_idx_replace * (120 - 32 - 8)) / (valid_items - 1);
            // 1. 绘制虚线轨道（从y=32到y=120，每隔2像素画一个点）
            for (uint8_t y = 32; y < 120; y += 2) {
                OLED_DrawPoint(158, y, 1);
            }
            // 2. 绘制实心滑块（2像素宽，8像素高）
            OLED_DrawLine(159, scroll_y, 160, scroll_y + 8, 1);
            OLED_DrawLine(157, scroll_y, 158, scroll_y + 8, 1);
            OLED_DrawLine(158, scroll_y, 159, scroll_y + 8, 1);
        }

        OLED_Refresh();
//        need_refresh = 0;
    }
}

void Task_AckReplace_Show(){

	// 创建临时缓冲区（确保足够大）
	char combined_str[64];  // 假设最大长度不超过64字节
	
	// 安全拼接字符串
	snprintf(combined_str, sizeof(combined_str), "[%s]?", custom_Kind[selected_idx_replace].name);

	OLED_ClearArea();

	// 第一行
	Gui_StrCenter(0, 10, WHITE, BLACK, "REPLACED", 16, 1,languageMark);

	// 第二行
	// 居中显示
	Gui_StrCenter(0, 50, WHITE, BLACK, combined_str, 16, 1,0);
	//Gui_StrCenter(0, 30, WHITE, BLACK, "REPLACE THE", 16, 1);

	//Gui_StrCenter(0, 90, WHITE, BLACK, " NO     YES", 20, 1,0);
	//Gui_StrCenter(0, 90, WHITE, BLACK, " NO     YES", 20, 1,0);
	//Gui_StrCenter(0, 90, WHITE, BLACK, " NO     YES", 20, 1,0);
	OLED_ShowString(15, 90, "NO", 20, 1,0);
	OLED_ShowString(125, 90, "YES", 20, 1,0);
	OLED_Refresh();


}

void Task_EditOrDelete_Show(uint8_t selected_option) // 编辑删除选择界面
{
	uint8_t size_val;
	
	switch(languageMark){
		case 0:
					size_val = 24;
					break;
		case 1:
					size_val = 16;
					break;
		default:
			break;
	}
	
	OLED_ClearArea();

	// 显示标题
	Gui_StrCenter(0, 10, WHITE, BLACK, "SAVED PROFILE", 16, 1,languageMark);

	if (selected_option == 1)
	{
		// 画选中框 YES
			// 第三行 - YES
		Gui_StrCenter(0, 60, WHITE, BLACK, "EDIT", size_val, 1,languageMark);
		OLED_DrawRoundedRect(10, 55, 140, 30, 3, 1);
			// 从 (60,10) 到 (40,30) 画一条右上到左下的斜线
//		OLED_DrawSolidDiagonalLine(125, 70, 130, 65, 1);
//		OLED_DrawSolidDiagonalLine(124, 70, 129, 65, 1);
//		OLED_DrawSolidDiagonalLine(125, 70, 130, 75, 1);
//		OLED_DrawSolidDiagonalLine(124, 70, 129, 75, 1);
//		OLED_DrawLine(124, 70, 145, 70, 1); // 横线中间
	}
	else{
		Gui_StrCenter(0, 60, WHITE, BLACK, "EDIT", 16, 1,languageMark);
	}


	if (selected_option == 0)
	{
			// 第四行 - NO
		Gui_StrCenter(0, 90, WHITE, BLACK, "DELETE", size_val, 1,languageMark);
		
		// 画选中框 NO
		OLED_DrawRoundedRect(10, 85, 140, 30, 3, 1);
//		OLED_DrawSolidDiagonalLine(125, 100, 130, 95, 1);
//		OLED_DrawSolidDiagonalLine(124, 100, 129, 95, 1);
//		OLED_DrawSolidDiagonalLine(125, 100, 130, 105, 1);
//		OLED_DrawSolidDiagonalLine(124, 100, 129, 105, 1);
//		OLED_DrawLine(124, 100, 145, 100, 1); // 横线中间
	}
	else{
		Gui_StrCenter(0, 90, WHITE, BLACK, "DELETE", 16, 1,languageMark);
	}

	OLED_Refresh();
}


void Task_NameDelete_Show(){
	
	uint8_t size_val;
	
	switch(languageMark){
		case 0:
					size_val = 20;
					break;
		case 1:
					size_val = 16;
					break;
		default:
			break;
	}

	// 创建临时缓冲区（确保足够大）
	char combined_str[64];  // 假设最大长度不超过64字节
	
	// 安全拼接字符串
	snprintf(combined_str, sizeof(combined_str), "[%s]?", custom_Kind[selected_idx].name);

	
	OLED_ClearArea();

	// 第一行
	Gui_StrCenter(0, 10, WHITE, BLACK, "DELETE", 16, 1,languageMark);

	// 第二行
	// 居中显示
	Gui_StrCenter(0, 50, WHITE, BLACK, combined_str, 16, 1,0);
	
	// 第四行 - NO
	//Gui_StrCenter(0, 90, WHITE, BLACK, " NO     YES", 20, 1,0);
	OLED_ShowString(15,64, "NO", size_val, 1,languageMark);
	OLED_ShowString(125,64, "YES", size_val, 1,languageMark);

	OLED_Refresh();


}

void Task_NameDeleteOver_Show(void){

	uint8_t size_val;
	
	switch(languageMark){
		case 0:
					size_val = 24;
					break;
		case 1:
					size_val = 16;
					break;
		default:
			break;
	}
	
	OLED_ClearArea();

	// 第一行 - 打勾图标
	OLED_DrawDiagonalLine(60, 10, 72, 20, 1);
	OLED_DrawDiagonalLine(60, 11, 72, 21, 1);
	OLED_DrawDiagonalLine(60, 12, 72, 22, 1);
	OLED_DrawDiagonalLine(72, 20, 92, 5, 1);
	OLED_DrawDiagonalLine(72, 21, 92, 6, 1);
	OLED_DrawDiagonalLine(72, 22, 92, 7, 1);
	// 第二行 - 保存的名称
	Gui_StrCenter(0, 40, WHITE, BLACK, custom_Kind[selected_idx].name, 16, 1,0);
	// Gui_StrCenter(0, 40, WHITE, BLACK, save_name, 16, 1);
	//  第三行
	Gui_StrCenter(0, 70, WHITE, BLACK, "DELETE!", size_val, 1,languageMark);

	OLED_Refresh();


}

static void _swap(u16 *a, u16 *b) {
    u16 tmp = *a;
    *a = *b;
    *b = tmp;
}
//void GUI_DrawArcOptimized(u16 xc, u16 yc, u16 r, u16 start_angle, u16 end_angle, u16 color) {
//    // 确保角度范围合法
//    if (start_angle > end_angle) {
//        _swap(&start_angle, &end_angle);
//    }

//    // 遍历圆弧上的每个点
//    for (int angle = start_angle; angle <= end_angle; angle++) {
//        float rad = angle * 3.1415926f / 180.0f;
//        int x = xc + r * cosf(rad);
//        int y = yc + r * sinf(rad);
//        GUI_DrawPoint(x, y, color);
//    }
//}

/**
 * @brief 绘制可控制凹凸方向的圆弧
 * @param xc, yc  圆心坐标
 * @param r       半径
 * @param start_angle 起始角度（0-360度）
 * @param end_angle   终止角度（0-360度）
 * @param color   颜色
 * @param is_convex true=凸圆弧，false=凹圆弧
 */
void GUI_DrawArcOptimized(u16 xc, u16 yc, u16 r, u16 start_angle, u16 end_angle, u16 color, int is_convex) {
    if (start_angle > end_angle) {
        _swap(&start_angle, &end_angle);
    }

    for (int angle = start_angle; angle <= end_angle; angle++) {
        float rad = angle * 3.1415926f / 180.0f;
        int x, y;
        
        if (is_convex) {
            // 凸圆弧：正常计算
            x = xc + r * cosf(rad);
            y = yc + r * sinf(rad);
        } else {
            // 凹圆弧：反向计算（圆心在另一侧）
            x = xc - r * cosf(rad);
            y = yc - r * sinf(rad);
        }
        GUI_DrawPoint(x, y, color);
    }
}

// 你的点阵数据
//static const unsigned char bitmap_bytes[] = {
//    0x00, 0x00, 0x00, 
//    0x00, 0x00, 0x00, 
//    0x00, 0x00, 0x00, 
//    0x00, 0x00, 0x00, 
//    0x00, 0x00, 0x00, 
//    0x00, 0x00, 0x00, 
//    0x00, 0x00, 0x00, 
//    0x00, 0x00, 0x00, 
//    0x00, 0x00, 0x00, 
//    0x3f, 0xff, 0xc0, 
//    0x3f, 0xff, 0xc0, 
//    0x3f, 0xff, 0xfc, 
//    0x3f, 0xff, 0xfe, 
//    0x3f, 0xff, 0xff, 
//    0x3f, 0xff, 0xe7, 
//    0x3f, 0xff, 0xe7, 
//    0x3f, 0xff, 0xe7, 
//    0x3f, 0xff, 0xff, 
//    0x3f, 0xff, 0xfe, 
//    0x3f, 0xff, 0xc0, 
//    0x3f, 0xff, 0xc0, 
//    0x1f, 0xff, 0x80, 
//    0x0f, 0xff, 0x00, 
//    0x00, 0x00, 0x00
//};

unsigned char BMP[]={
0x00,0x10,0x18,0x08,0x0C,0xC6,0xE2,0xE2,0xE1,0xF1,0xF1,0xF1,0xF1,0xF1,0xE1,0xE2,
0xE2,0x86,0x24,0x28,0x30,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x07,0xFD,0x01,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x01,0xFD,0x03,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x1F,0x3C,0x47,0xC6,0x93,0xBB,0xBB,0xBB,0xBB,0xBB,0x92,0xC7,
0x66,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,/*"加量",0*/

};

/**
  * @brief  显示阴码、逆向、列行格式的点阵图像
  * @param  x: 起始列坐标(0-127)
  * @param  y: 起始页坐标(0-7, 每页8行)
  * @param  width: 图像宽度(像素)
  * @param  height: 图像高度(像素)
  * @param  img: 图像数据数组
  * @param  mode: 显示模式(0:反色显示,1:正常显示)
  * @retval 无
  */
void OLED_ShowInverseColumnImage(u8 x, u8 y, u8 width, u8 height, const u8 *img, u8 mode) {
    u16 index = 0;
    u8 i, j, k;
    u8 temp;
    
    // 计算需要多少页(每页8行)
    u8 pages = height / 8;
    if (height % 8 != 0) {
        pages++;
    }
    
    for (i = 0; i < pages; i++) {
        for (j = 0; j < width; j++) {
            temp = img[index++];  // 读取数据
            
            // 阴码转阳码
            temp = ~temp;
            
            // 处理逆向(低位在前)
            // 如果OLED驱动是高位在前，需要反转字节位序
            // temp = (temp & 0x55) << 1 | (temp & 0xAA) >> 1;
            // temp = (temp & 0x33) << 2 | (temp & 0xCC) >> 2;
            // temp = (temp & 0x0F) << 4 | (temp & 0xF0) >> 4;
            
            // 写入显存
            for (k = 0; k < 8; k++) {
                if (temp & (1 << k)) {
                    OLED_DrawPoint(x + j, y * 8 + i * 8 + k, mode);
                } else {
                    OLED_DrawPoint(x + j, y * 8 + i * 8 + k, !mode);
                }
            }
        }
    }
}


void Task_0xF1FF_Show(uint16_t coffee_add)	//加豆页面coffee_add
{
	uint8_t size_0 = 16;
	uint8_t size_1 = 28;
	uint8_t x_0 = 64;
	uint8_t x_1 = x_0 + 4 + size_1;
	uint8_t y_0 = 10;
	uint8_t y_1 = 64 - size_1/2;
	uint8_t y_2 = 64 + size_1/2 - size_0;
	
	OLED_ClearArea();
	
	Gui_StrCenter(0,y_0,WHITE, BLACK, "COFFEE ADDED", 16, 1,languageMark);
	
	OLED_ShowNum(x_0, y_1, coffee_add, 2, size_1, 1);
	OLED_ShowString(x_1, y_2, "g", size_0, 1,0);
	
	OLED_ShowPicture(70,94,24,24,BMP,1);
	
	
//	OLED_ShowChinese(0, 0, 3, 16, 1); // 显示“冲”
//	OLED_ShowChinese(16, 0, 4, 16, 1); // 显示“泡”
//	OLED_ShowChinese(32, 0, 5, 16, 1); // 显示“中”
	// 优化版调用
	//GUI_DrawArcOptimized(80, 110, 20, 30, 150, WHITE, 0);
	//OLED_Show4DigitNum(80, 110, 1688, 16, 1); 
	    // 显示图像
  //OLED_ShowPicture(0, 0, 24, 24, (u8*)bitmap_bytes, 1);
    // 显示图像(从坐标(0,0)开始)
    //OLED_ShowInverseColumnImage(0, 0, 16, 64, BMP, 1);
	
	OLED_Refresh();
}


void Task_Tea_Add_Show(uint16_t tea_add, uint8_t state)	//加茶包
{
	uint8_t size_0 = 16;
	uint8_t size_1 = 28;
	uint8_t x_0 = 64;
	uint8_t x_1 = x_0 + 4 + size_1;
	uint8_t y_0 = 10;
	uint8_t y_1 = 64 - size_1/2;
	uint8_t y_2 = 64 + size_1/2 - size_0;
	
	OLED_ClearArea();
	
	if(state)
	{
		Gui_StrCenter(0,y_0,WHITE, BLACK, "TEA ADDED", 16, 1,languageMark);
		
		OLED_ShowNum(x_0, y_1, tea_add, 2, size_1, 1);
		OLED_ShowString(x_1, y_2, "g", size_0, 1,0);
	}
	else
	{
		Gui_StrCenter(0,y_0,WHITE, BLACK, "TEAPODS ADDED", 16, 1,languageMark);
		
		OLED_ShowString(59, y_1, "X", size_1, 1,0);
		OLED_ShowNum(73, y_1, tea_add, 2, size_1, 1);
	}
	OLED_ShowPicture(70,94,24,24,BMP,1);
	
	OLED_Refresh();
}


extern uint8_t Coffee_Added;
extern uint8_t uesr_chooseMark;

extern uint8_t Added_Mark;
extern uint8_t Save_flag;

extern unsigned char bmpCup[];
extern unsigned char bmpClock[];
extern unsigned char bmpBloom[];
extern unsigned char bmpBrew[];
extern unsigned char bmpTotal[];
void Task_0x1FFF_Show(Coffee_mode mode_now)							//总结页面
{
	
	if(Added_Mark != 1){
		
	uint8_t size_2 	= 20; // 数字 
	uint8_t size_3 	= 16; // 单位（ml & hour & minute） 
	uint8_t size_4 	= 16; // title

	
	uint8_t x_0 = 6+20; // subtitle location
	uint8_t x_1 = 66+20 ; // Content location
	uint8_t x_2 = x_1+ size_3 + 10; // Unit location
	
	uint8_t y_1	= 30; //MODE Location
	uint8_t y_2	= y_1 + size_3; //MODE Location	
	uint8_t y_3 = y_2 + size_3;  //Brew的位置
	uint8_t y_4 = y_3 + size_3;  //Time 的位置
	uint8_t y_5 = y_4 + size_3;  //Time 的位置
	uint8_t y_6 = y_5 + size_3;  //Time 的位置
	
	uint16_t fc 	= WHITE;
	uint16_t bc 	= BLACK;
	
	OLED_ClearArea();
	
	if(uesr_chooseMark)
		Gui_StrCenter(0,10,fc, bc, mode_now.name,16, 1,0);//80
	else
		Gui_StrCenter(0,10,fc, bc, "SUMMARY",16, 1,languageMark);//80
	//Gui_StrCenter(0,10,fc, bc, mode_now.name,16, 1,0);//80

	OLED_ShowPicture(5,y_1,16,16,bmpBloom,1);
	OLED_ShowString(x_0,				y_1, "BLOOM",							size_3, 1,languageMark);
	OLED_ShowString(x_1-10,				y_1, ":",							size_3, 1,0);
	OLED_ShowString(x_1,				y_1, "1:",									size_3, 1,0);
	OLED_ShowNum(x_1 + size_3,			y_1, mode_now.Heating_CWratio / 10 , 1, 	size_3, 1);
	OLED_ShowString(x_1 + size_3 + 10,	y_1, ".",									size_3, 1,0);
	OLED_ShowNum(x_1 +size_3 * 2 ,	y_1, mode_now.Heating_CWratio, 1, 			size_3, 1);
	
	if(UnitMark == 0){
			//OLED_ShowNum(x_1,    	y_2, mode_now.Heating_Temp, 	size_3, 1);
			OLED_ShowNum(x_1,    	y_2, mode_now.Heating_Temp,2, 	size_3, 1);
			OLED_ShowString(x_2+4,	y_2, "C",  						size_3, 1,0);
	}
	else{
			//mode_now.Heating_Temp = (mode_now.Heating_Temp * 9.0/5.0) + 32;
			OLED_ShowNum(x_1,    	y_2, (uint16_t)(roundf((float)mode_now.Heating_Temp * 1.8f + 32.0f)),3, 	size_3, 1);
			//mode_now.Heating_Temp = (uint16_t)(roundf((float)mode_now.Heating_Temp * 1.8f + 32.0f));
			OLED_ShowString(x_2+6,	y_2, "F",  						size_3, 1,0);
	}
	
	//OLED_ShowNum(x_1,    	y_2, mode_now.Heating_Temp,2, 	size_3, 1);
	gui_circle(x_2+2, 		y_2 + 3, 1, 2, 0);
//	OLED_ShowString(x_2+4,	y_2, "C",  						size_3, 1,0);
	
	OLED_ShowString(x_1,					y_3, "0.", 						size_3, 1,0);
	OLED_ShowNum(x_1 + size_3,				y_3, mode_now.Heating_spd, 2, 	size_3, 1);
	OLED_ShowString(x_1 + size_3 * 2 + 4,	y_3, "ml", 						size_3, 1,0);
	OLED_ShowString(x_1 + size_3 * 3 + 4,	y_3, "/s", 						size_3, 1,0);
	
	OLED_ShowPicture(5,y_4,16,16,bmpBrew,1);
	OLED_ShowString(x_0, 	y_4, "BREW",   				size_4, 1,languageMark);
	OLED_ShowString(x_1-10,				y_4, ":",							size_3, 1,0);
	
	if(UnitMark == 0){
			OLED_ShowNum(x_1,    	y_4, mode_now.Cooling_Temp,2, 	size_3, 1);
			gui_circle(x_2, y_4+3, 1, 2, 0);
			OLED_ShowString(x_2+4,	y_4, "C",  						size_3, 1,0);
	}
	else{
			//mode_now.Cooling_Temp = mode_now.Cooling_Temp * (9/5) + 32;
			OLED_ShowNum(x_1,    	y_4, (uint16_t)(roundf((float)mode_now.Cooling_Temp * 1.8f + 32.0f)),2, 	size_3, 1);
			gui_circle(x_2, y_4+3, 1, 2, 0);
			OLED_ShowString(x_2+4,	y_4, "F",  						size_3, 1,0);
	}
	
	
	OLED_ShowString(x_1,					y_5, "0.", 						size_3, 1,0);
	OLED_ShowNum(x_1 + size_3,				y_5, mode_now.Cooling_spd, 2, 	size_3, 1);
	OLED_ShowString(x_1 + size_3 * 2 + 4,	y_5, "ml", 						size_3, 1,0);
	OLED_ShowString(x_1 + size_3 * 3 + 4,	y_5, "/s", 						size_3, 1,0);
	
	OLED_ShowPicture(5,y_6-1,16,16,bmpTotal,1);
	OLED_ShowString(x_0,					y_6, "TOTAL",     													size_4, 1,languageMark);
	OLED_ShowString(x_1-10,				y_6, ":",							size_3, 1,0);
	OLED_ShowString(x_1,					y_6, "1:",	 															size_3, 1,0);
	OLED_ShowNum(x_1 + size_3,				y_6, (mode_now.Heating_CWratio + mode_now.Cooling_CWratio) / 10 , 2, 	size_3, 1);
	OLED_ShowString(x_1 + size_3 * 2 +2,	y_6, ".",	 															size_3, 1,0);
	OLED_ShowNum(x_1 +size_3 * 3-6,			y_6, mode_now.Heating_CWratio + mode_now.Cooling_CWratio, 1, 			size_3, 1);

	OLED_Refresh();
	
	}
	else{
	
//	uint8_t size_0 	= 24; // 标题第一行
//	uint8_t size_1 	= 20; // 标题第二行(如有)
	uint8_t size_2 	= 20; // 数字 
	uint8_t size_3 	= 16; // 单位（ml & hour & minute） 
	uint8_t size_4 	= 16; // title
		
//	uint8_t x = 90; //80
//	uint8_t y_0	= 44; //+20
//	uint8_t y_1	= 66; //72
//	uint8_t y_2 = 100; //+20 //数字的位置
//	uint8_t y_3 = 100 + size_2 - size_3; //unit的位置
	
	
	uint8_t x_0 = 10; // subtitle location
	uint8_t x_1 = x_0+size_4/2*6+4 ; // Content location
	uint8_t x_2 = x_1+ size_2/2*3; // Unit location
	
	uint8_t y_1	= 40; //MODE Location
	
	uint8_t y_15 = y_1+2*size_3+2;  //Bean的位置
//	uint8_t y_152 = y_15 + size_2 - size_3; //ml unit的位置
	
	
	uint8_t y_2 =  y_15+ size_3+2;  //Brew的位置
//	uint8_t y_22 = y_2 + size_2 - size_3; //ml unit的位置
	
	uint8_t y_3 =  y_2+ size_3+2 -25;  //Time 的位置
//	uint8_t y_32 = y_3 + size_2 - size_3; //hour & minute unit的位置
	
	uint16_t fc 	= WHITE;
	uint16_t bc 	= BLACK;
	
	OLED_ClearArea();
	

	Gui_StrCenter(0,10,fc, bc, "READY TO BREW?",16, 1,languageMark);//80

	//Gui_StrCenter(0,10,fc, bc, "PREVIEW",16, 1,languageMark);//80
	
	if(UnitMark == 0){
		OLED_ShowNum(62,    55,   mode_now.Total_ml,4,       size_3,1);
		OLED_ShowString(100, 55,  "ml",       size_3, 1,0);
	}
	else{
		//mode_now.Total_ml = mode_now.Total_ml * 29.5735;
		//(uint16_t)(roundf((float)mode_now.Heating_Temp * 1.8f + 32.0f))
		OLED_ShowNum(62,    55,(uint16_t)ceilf((float)mode_now.Total_ml * 0.033814f),2,       size_3,1);
		OLED_ShowString(80, 55,  "oz",       size_3, 1,0);
	}
	
	OLED_ShowPicture(30,50,24,24,bmpCup,1);
	//OLED_ShowNum(62,    55,   mode_now.Total_ml,4,       size_3,1);
	//OLED_ShowString(100, 55,  "ml",       size_3, 1,0);

	OLED_ShowPicture(30,80,24,24,bmpClock,1);

	OLED_ShowNum(x_1,               y_3,   mode_now.Total_Time / 3600 / 10,1,       size_3,1);
	OLED_ShowNum(x_1+size_2/2,      y_3,   mode_now.Total_Time / 3600,1,       size_3,1);
	OLED_ShowString(x_1+size_2/2*2, y_3,  "h",       size_3, 1,0);
	OLED_ShowNum(x_1+size_2/2*3,    y_3,   mode_now.Total_Time / 60 % 60 / 10,1,       size_3,1);
	OLED_ShowNum(x_1+size_2/2*4,    y_3,   mode_now.Total_Time / 60 % 60 ,1,       size_3,1);
	OLED_ShowString(x_1+size_2/2*5, y_3,  "m",       size_3, 1,0);
	
//	OLED_ShowString(x_0,y_1, "MODE :",size_4,1,0);
//	OLED_ShowString(x_1,y_1, mode_now.name,  	size_3, 1,0);
//	OLED_ShowString(x_1,y_1+size_3, mode_now.title_1,	size_3, 1,0);
//	
//	OLED_ShowString(x_0, y_15,   "BEAN :",   size_4,1,0);
//	OLED_ShowNum(x_1,    y_15,   Coffee_Added,2,       size_3,1);
//	OLED_ShowString(x_2, y_15,  "g",       size_3, 1,0);
//	
//	OLED_ShowString(x_0, y_2,   "BREW :",   size_4,1,0);
//	OLED_ShowNum(x_1,    y_2,   mode_now.Total_ml,4,       size_3,1);
//	//OLED_Show4DigitNum(x_1, y_2, mode_now.Total_ml, 16, 1); 
//	OLED_ShowString(x_2+6, y_2,  "ml",       size_3, 1,0);
//	
//	
//	OLED_ShowString(x_0,            y_3,   "TIME :",     size_4,1,0);
//	OLED_ShowNum(x_1,               y_3,   mode_now.Total_Time / 3600 / 10,1,       size_3,1);
//	OLED_ShowNum(x_1+size_2/2,      y_3,   mode_now.Total_Time / 3600,1,       size_3,1);
//	OLED_ShowString(x_1+size_2/2*2, y_3,  "h",       size_3, 1,0);
//	OLED_ShowNum(x_1+size_2/2*3,    y_3,   mode_now.Total_Time / 60 % 60 / 10,1,       size_3,1);
//	OLED_ShowNum(x_1+size_2/2*4,    y_3,   mode_now.Total_Time / 60 % 60 ,1,       size_3,1);
//	OLED_ShowString(x_1+size_2/2*5, y_3,  "m",       size_3, 1,0);
	
	OLED_Refresh();	
	}
}

extern int8_t uesr_EditMark;

void Task_Saved_Show(uint8_t selected_option) // 品种保存页面
{
	uint8_t size_val;
	
	switch(languageMark){
		case 0:
					size_val = 24;
					break;
		case 1:
					size_val = 16;
					break;
		default:
			break;
	}
	
	OLED_ClearArea();

	if(uesr_EditMark == 1){
		
				// 创建临时缓冲区（确保足够大）
		char combined_str[64];  // 假设最大长度不超过64字节
		
		// 安全拼接字符串
		snprintf(combined_str, sizeof(combined_str), "[%s]?", custom_Kind[selected_idx].name);

		// 第一行
		Gui_StrCenter(0, 10, WHITE, BLACK, "RENAMED", 16, 1,languageMark);

		// 第二行
		// 居中显示
		Gui_StrCenter(0, 30, WHITE, BLACK, combined_str, 16, 1,0);
		//Gui_StrCenter(0, 30, WHITE, BLACK, "REPLACE THE", 16, 1);

		if (selected_option == 1)
			{
				// 画选中框 YES
					// 第三行 - YES
				Gui_StrCenter(0, 60, WHITE, BLACK, "YES", size_val, 1,languageMark);
				OLED_DrawRoundedRect(10, 55, 140, 30, 3, 1);
					// 从 (60,10) 到 (40,30) 画一条右上到左下的斜线
//				OLED_DrawSolidDiagonalLine(125, 70, 130, 65, 1);
//				OLED_DrawSolidDiagonalLine(124, 70, 129, 65, 1);
//				OLED_DrawSolidDiagonalLine(125, 70, 130, 75, 1);
//				OLED_DrawSolidDiagonalLine(124, 70, 129, 75, 1);
//				OLED_DrawLine(124, 70, 145, 70, 1); // 横线中间
			}
			else{
				Gui_StrCenter(0, 60, WHITE, BLACK, "YES", 16, 1,languageMark);
			}


			if (selected_option == 0)
			{
					// 第四行 - NO
				Gui_StrCenter(0, 90, WHITE, BLACK, "NO", size_val, 1,languageMark);
				
				// 画选中框 NO
				OLED_DrawRoundedRect(10, 85, 140, 30, 3, 1);
//				OLED_DrawSolidDiagonalLine(125, 100, 130, 95, 1);
//				OLED_DrawSolidDiagonalLine(124, 100, 129, 95, 1);
//				OLED_DrawSolidDiagonalLine(125, 100, 130, 105, 1);
//				OLED_DrawSolidDiagonalLine(124, 100, 129, 105, 1);
//				OLED_DrawLine(124, 100, 145, 100, 1); // 横线中间
			}
			else{
				Gui_StrCenter(0, 90, WHITE, BLACK, "NO", 16, 1,languageMark);
			}
	
	}
	else{
	
				// 第一行
//			Gui_StrCenter(0, 10, WHITE, BLACK, "FINISHED", 16, 1);

//			// 第二行
//			Gui_StrCenter(0, 30, WHITE, BLACK, "SAVE THE MODE?", 16, 1);
				Gui_StrCenter(0, 20, WHITE, BLACK, "SAVE PROFILE?", 16, 1,languageMark);

			if (selected_option == 1)
			{
				// 画选中框 YES
					// 第三行 - YES
				Gui_StrCenter(0, 60, WHITE, BLACK, "YES", size_val, 1,languageMark);
				OLED_DrawRoundedRect(10, 55, 140, 30, 3, 1);
					// 从 (60,10) 到 (40,30) 画一条右上到左下的斜线
//				OLED_DrawSolidDiagonalLine(125, 70, 130, 65, 1);
//				OLED_DrawSolidDiagonalLine(124, 70, 129, 65, 1);
//				OLED_DrawSolidDiagonalLine(125, 70, 130, 75, 1);
//				OLED_DrawSolidDiagonalLine(124, 70, 129, 75, 1);
//				OLED_DrawLine(124, 70, 145, 70, 1); // 横线中间
			}
			else{
				Gui_StrCenter(0, 60, WHITE, BLACK, "YES", 16, 1,languageMark);
			}


			if (selected_option == 0)
			{
					// 第四行 - NO
				Gui_StrCenter(0, 90, WHITE, BLACK, "LATER", size_val, 1,languageMark);
				
				// 画选中框 NO
				OLED_DrawRoundedRect(10, 85, 140, 30, 3, 1);
//				OLED_DrawSolidDiagonalLine(125, 100, 130, 95, 1);
//				OLED_DrawSolidDiagonalLine(124, 100, 129, 95, 1);
//				OLED_DrawSolidDiagonalLine(125, 100, 130, 105, 1);
//				OLED_DrawSolidDiagonalLine(124, 100, 129, 105, 1);
//				OLED_DrawLine(124, 100, 145, 100, 1); // 横线中间
			}
			else{
				Gui_StrCenter(0, 90, WHITE, BLACK, "LATER", 16, 1,languageMark);
			}
	
	}

	OLED_Refresh();
}


extern char save_name[11];
extern uint8_t name_length;
extern uint8_t current_char;
extern uint32_t sleep_tag_1;

void Task_Named_Show(uint8_t init_tag, uint8_t progress_bar) // 品种命名页面
{
	static uint8_t progress_bar_last;
	const char char_set[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>+-/!?.:_=";
	
	static uint32_t sleep_tag_1_last = 0;
	static uint8_t show_state = 0;
	
		// 检查是否达到切换时间（每3个计数切换）
	if (sleep_tag_1 - sleep_tag_1_last >= 5) {
			sleep_tag_1_last = sleep_tag_1;
			show_state = !show_state; // 切换状态
	}


	OLED_ClearArea();

	// 第一行
	Gui_StrCenter(0, 10, WHITE, BLACK, "NAME", 16, 1,languageMark);

	// 第二行 - 输入框 (用四条线绘制矩形)
	DrawBasicRectangle(20, 30, 140, 50, 1);
	// OLED_DrawLine(20, 50, 140, 47, 1);

	// 显示已输入的名称
	uint8_t x_pos = 24;
	for (uint8_t i = 0; i < name_length; i++)
	{
		OLED_ShowChar(x_pos, 35, save_name[i], 16, 1,0);
		x_pos += 8;
	}

//	if(char_set[current_char] == '!' || char_set[current_char] == '.'|| char_set[current_char] == ':')
//			OLED_ShowChar(x_pos+3, 32, char_set[current_char], 16, 1,0);
//	else
//			OLED_ShowChar(x_pos, 32, char_set[current_char], 16, 1,0);
	
	OLED_ShowChar(x_pos, 32, char_set[current_char], 16, 1,0);
	
//	if (show_state) {
//			// 显示当前正在调整的字母（即使已满也显示）
//			OLED_ShowChar(x_pos, 32, char_set[current_char], 16, 1,0);
//		//OLED_DrawLine(x_pos, 47, x_pos + 10, 47, 1);
//	} else {
//			// 显示当前正在调整的字母（即使已满也显示）
//			//OLED_ShowChar(x_pos, 32,'|', 16, 0,0);
//		OLED_ShowChar(x_pos, 32, char_set[current_char], 16, 0,0);
//		//OLED_DrawLine(x_pos, 47, x_pos + 10, 47, 0);
//	}


	// 绘制选择框 (用四条线绘制)
	// DrawBasicRectangle(x_pos-2, 33, x_pos+10, 47, 1);
	OLED_DrawLine(x_pos, 47, x_pos + 10, 47, 1);
	OLED_DrawLine(20, 50, 140, 50, 1);

	// 第三行
	Gui_StrCenter(0, 60, WHITE, BLACK, "HOLD TO", 16, 1,languageMark);

	// 第四行
	//Gui_StrCenter(0, 80, WHITE, BLACK, "DISCARD/SAVE", 16, 1,languageMark);
	OLED_ShowString(15,80, "DISCARD", 16, 1,languageMark);
	OLED_ShowString(80,80, "/", 16, 1,0);
	OLED_ShowString(110,80, "SAVE", 16, 1,languageMark);

	// 第五行 - 细线
	OLED_DrawLine(0, 105, 160, 105, 1);
	
	if (init_tag)
	{
		if (progress_bar < progress_bar_last || progress_bar == 0)
		{
			OLED_DrawLine(0, 104, 160, 104, 0);
			OLED_DrawLine(0, 106, 160, 106, 0);
		}
		else
		{
			OLED_DrawLine(0, 104, progress_bar, 104, 1);
			OLED_DrawLine(0, 106, progress_bar, 106, 1);
		}
	}
	
	OLED_Refresh();
}

void Task_NameOver_Show(void) // 命名成功页面
{
	uint8_t size_val;
	
	switch(languageMark){
		case 0:
					size_val = 24;
					break;
		case 1:
					size_val = 16;
					break;
		default:
			break;
	}
	
	OLED_ClearArea();

	// 第一行 - 打勾图标
	OLED_DrawDiagonalLine(60, 10, 72, 20, 1);
	OLED_DrawDiagonalLine(60, 11, 72, 21, 1);
	OLED_DrawDiagonalLine(60, 12, 72, 22, 1);
	OLED_DrawDiagonalLine(72, 20, 92, 5, 1);
	OLED_DrawDiagonalLine(72, 21, 92, 6, 1);
	OLED_DrawDiagonalLine(72, 22, 92, 7, 1);
	// 第二行 - 保存的名称
	Gui_StrCenter(0, 40, WHITE, BLACK, save_name, 16, 1,0);
	// Gui_StrCenter(0, 40, WHITE, BLACK, save_name, 16, 1);
	//  第三行
	Gui_StrCenter(0, 70, WHITE, BLACK, "SAVED!", size_val, 1,languageMark);

	OLED_Refresh();
}

void Task_ExecuteWay_Show(uint8_t selected_option) //存储后执行选择界面
{
	uint8_t size_val;
	
	switch(languageMark){
		case 0:
					size_val = 24;
					break;
		case 1:
					size_val = 16;
					break;
		default:
			break;
	}
	
	OLED_ClearArea();
	// 显示标题
//	Gui_StrCenter(0, 5, WHITE, BLACK, "DO YOU WANT TO", 16, 1);
//	Gui_StrCenter(0, 20, WHITE, BLACK, "EXECUTE NOW?", 16, 1);
		Gui_StrCenter(0, 10, WHITE, BLACK, "BREW NOW?", 16, 1,languageMark);

	if (selected_option == 1)
	{
		// 画选中框 YES
			// 第三行 - YES
		Gui_StrCenter(0, 60, WHITE, BLACK, "NOW", size_val, 1,languageMark);
		OLED_DrawRoundedRect(10, 55, 140, 30, 3, 1);
			// 从 (60,10) 到 (40,30) 画一条右上到左下的斜线
//		OLED_DrawSolidDiagonalLine(125, 70, 130, 65, 1);
//		OLED_DrawSolidDiagonalLine(124, 70, 129, 65, 1);
//		OLED_DrawSolidDiagonalLine(125, 70, 130, 75, 1);
//		OLED_DrawSolidDiagonalLine(124, 70, 129, 75, 1);
//		OLED_DrawLine(124, 70, 145, 70, 1); // 横线中间
	}
	else{
		Gui_StrCenter(0, 60, WHITE, BLACK, "NOW", 16, 1,languageMark);
	}


	if (selected_option == 0)
	{
			// 第四行 - NO
		Gui_StrCenter(0, 90, WHITE, BLACK, "LATER", size_val, 1,languageMark);
		
		// 画选中框 NO
		OLED_DrawRoundedRect(10, 85, 140, 30, 3, 1);
//		OLED_DrawSolidDiagonalLine(125, 100, 130, 95, 1);
//		OLED_DrawSolidDiagonalLine(124, 100, 129, 95, 1);
//		OLED_DrawSolidDiagonalLine(125, 100, 130, 105, 1);
//		OLED_DrawSolidDiagonalLine(124, 100, 129, 105, 1);
//		OLED_DrawLine(124, 100, 145, 100, 1); // 横线中间
	}
	else{
		Gui_StrCenter(0, 90, WHITE, BLACK, "LATER", 16, 1,languageMark);
	}

	OLED_Refresh();
}

extern uint8_t Progress_bar;

void Task_0x2FFF_Show(uint8_t init_tag, uint8_t progress_bar)		//准备开始页面
{
	if(init_tag)
	{
		OLED_DrawLine(0,100,progress_bar, 100, 1);
		OLED_DrawLine(0,102,progress_bar, 102, 1);
	}
	else
	{
		OLED_ClearArea();
		
		Gui_StrCenter(0,35,WHITE, BLACK, "IT'S",20, 1,0);//80		
		Gui_StrCenter(0,60,WHITE, BLACK, "COLD BREW TIME",20, 1,0);//80
		OLED_DrawLine(0,101,160, 101, 1);
	}
	
	OLED_Refresh();
}

extern unsigned char bmpReserva[];

void Task_0x3FFF_Show(uint8_t init_tag, uint32_t Reservation_Time)	//延时页面reserve
{
	uint8_t size = 24;
	uint8_t size_half = size/2;
		
	uint8_t x = 22;
	uint8_t y = 64;
	
	if(init_tag)
	{
		uint32_t Reservation_Time_Last;		
		if(Reservation_Time != Reservation_Time_Last)
		{
			uint32_t minute_t, hour_t;
		
			hour_t	 = Reservation_Time / 60;
			minute_t = Reservation_Time % 60;
			
//			if(hour_t2>10){
//				hour_t2 = hour_t/10;
//			}
//			else{
//				hour_t2 = 0;
//			}
			
//			if(hour_t > 9)hour_t=9;
			
//			OLED_ShowNum(x, 			y, 0,		1, size, 1);
			//OLED_ShowNum(x, 			y, hour_t / 10,		1, size, 1);
//			OLED_ShowNum(x+size_half, 	y, hour_t,			1, size, 1);
//			OLED_ShowNum(x+size_half*3,	y, minute_t / 10,	1, size, 1);
//			OLED_ShowNum(x+size_half*4,	y, minute_t,		1, size, 1);
//			OLED_ShowNum(x+size_half*6,	y, 0,				1, size, 1);
//			OLED_ShowNum(x+size_half*7,	y, 0,				1, size, 1);	
			OLED_ShowPicture(x-5,y,24,24,bmpReserva,1);

			// 有效范围检查
//			hour_t = (hour_t > 24) ? 24 : hour_t;  // 限制最大23小时
			
			if (hour_t >= 10) {
					// 两位数显示（12:30）
					OLED_ShowNum(x+size_half*2, y, hour_t / 10, 1, size, 1);
			} else {
					// 一位数显示（1:30）
					OLED_ShowNum(x+size_half*2, y, 0, 1, size, 1);  // 十位补零
			}
			OLED_ShowNum(x+size_half*3, y, hour_t % 10, 1, size, 1);

//			OLED_ShowNum(x+size_half*3,	y, hour_t / 10,	1, size, 1);
//			OLED_ShowNum(x+size_half*4,	y, hour_t,		1, size, 1);
			OLED_ShowNum(x+size_half*6,	y, minute_t / 10,				1, size, 1);
			OLED_ShowNum(x+size_half*7,	y, minute_t,				1, size, 1);	
			
			
		}
		else
		{
		
		}
		Reservation_Time_Last = Reservation_Time;
	}
	else
	{
		OLED_ClearArea();
									
		Gui_StrCenter(0,0,WHITE, BLACK, "BREW LATER",16, 1,languageMark);//80
		
		//OLED_ShowString(x+size_half*2,y, ":", size, 1,0);
		OLED_ShowString(x+size_half*4,y, "h", size, 1,0);
		OLED_ShowString(x+size_half*8,y, "m", size, 1,0);
		
		Encoder = Reservation_Time/15;
	}
	
	OLED_Refresh();
}

void Task_0xEFFF_Show(uint8_t init_tag, uint8_t progress_bar)		//取消页面 cancel
{
	uint8_t size_0 = 16;
	uint8_t size_1 = 20;
	uint8_t y_0 = 64- size_1;
	uint8_t y_1 = 64- size_1/2;
	
	static uint8_t progress_bar_last;
	
	if(init_tag)
	{
		if(progress_bar < progress_bar_last || progress_bar == 0)
		{
			OLED_DrawLine(0,100,160, 100, 0);
			OLED_DrawLine(0,102,160, 102, 0);
		}
		else
		{
			OLED_DrawLine(0,100,progress_bar, 100, 1);
			OLED_DrawLine(0,102,progress_bar, 102, 1);
		}
	}
	else
	{
		OLED_ClearArea();
		
		Gui_StrCenter(0,10,WHITE, BLACK, "CANCEL BREWING?",size_0, 1,languageMark);//80
		OLED_ShowString(5,	y_0, "HOLD TO", size_0, 1,languageMark);
		OLED_ShowString(5,	64, "CANCEL", size_0, 1,languageMark);
		OLED_ShowString(160 - size_1*5/2,	y_1, "RESUME", size_0, 1,languageMark);
		OLED_DrawLine(0,101,160, 101, 1);
	}
	
	progress_bar_last = progress_bar;
	
	OLED_Refresh();
}

void Task_0xFFFF_Show(uint16_t state, Coffee_mode mode_now)			//执行页面
{
	uint8_t size_0 = 16;
	uint8_t size_1 = 20;
	
	uint8_t y_0 = 10;
	uint8_t y_1 = 128 - size_1; //
	
	uint16_t fc = WHITE;
	uint16_t bc = BLACK;
	
		// 创建临时缓冲区（确保足够大）
	char combined_str[64];  // 假设最大长度不超过64字节
	
	OLED_ClearArea();
	
	switch(state)
	{
		case 0:
			Gui_StrCenter(0,y_0,WHITE, BLACK, "WAIT TO START", size_0, 1,languageMark);
			
			break;
		case 1:
			Gui_StrCenter(0,y_0,fc, bc, "HEATING", size_0, 1,languageMark);
			break;	

		case 2:
			Gui_StrCenter(0,y_0,fc, bc, "BLOOMING", size_0, 1,languageMark);
	
			if(UnitMark)
			{
				gui_circle(5+size_1+6+13, 					y_1+size_1-size_0, 1, 2, 0);
				OLED_ShowString(4+size_1+5+6+14,				y_1+size_1-size_0, "F",  size_0, 1,0);
			}
			else
			{
				gui_circle(5+size_1+6, 					y_1+size_1-size_0, 1, 2, 0);
				OLED_ShowString(4+size_1+5+6,				y_1+size_1-size_0, "C",  size_0, 1,0);
			}
			OLED_ShowString(160- 6 * size_1 / 2-4-6, 	y_1, ".", size_1, 1,0);
			OLED_ShowNum(160- 5 * size_1 / 2-4-6,		y_1, mode_now.Heating_spd, 2, size_1, 1);				
			OLED_ShowString(160- 3 * size_1 / 2-4-6, 	y_1, "ml", size_1, 1,0);
			OLED_ShowString(160- 2 * size_1 / 2-6, 	y_1, "/s", size_1, 1,0);
			break;
		case 3:
			Gui_StrCenter(0,y_0,fc, bc, "COOLING", size_0, 1,languageMark);			
			break;
		case 4:
			Gui_StrCenter(0,y_0,fc, bc, "COLD BREWING", size_0, 1,languageMark);

			if(UnitMark)
			{
				gui_circle(5+size_1+6+13, 						y_1+size_1-size_0, 1, 2, 0);
				OLED_ShowString(4+size_1+5+6+14, 				y_1+size_1-size_0, "F",  size_0, 1,0);
			}
			else
			{
				gui_circle(5+size_1+6, 						y_1+size_1-size_0, 1, 2, 0);
				OLED_ShowString(4+size_1+5+6, 				y_1+size_1-size_0, "C",  size_0, 1,0);
			}

			OLED_ShowString(160- 6 * size_1 / 2-4-6, 	y_1, ".", size_1, 1,0);
			OLED_ShowNum(160- 5 * size_1 / 2-4-6,		y_1, mode_now.Cooling_spd, 2, size_1, 1);				
			OLED_ShowString(160- 3 * size_1 / 2-4-6, 	y_1, "ml", size_1, 1,0);
			OLED_ShowString(160- 2 * size_1 / 2-6, 		y_1, "/s", size_1, 1,0);
			break;
		
		case 5:
		case 6:
			Task_Finish_Show();
			break;
		
		default:
			break;
	}
		
	OLED_Refresh();
}

void Task_Finish_Show(void)											//完成页面
{
	uint8_t sizeVal = 16;
	if(languageMark)sizeVal=16;
	else sizeVal = 24;
	OLED_ClearArea();
	//Gui_StrCenter(0,40, WHITE, BLACK, "WHEN DRIPS STOP,", 20, 1,languageMark);
	Gui_StrCenter(0,52, WHITE, BLACK, "ENJOY!",sizeVal , 1,languageMark);
	OLED_Refresh();							
}

void Task_test_Show(void)											//测试页面
{
	static uint16_t y 		= 64; 
	static uint8_t size 	= 24;
	static uint16_t fc 		= WHITE;
	static uint16_t bc 		= BLACK;
	static uint16_t mode 	= 1;
	
	OLED_ClearArea();
	
	OLED_ColorTurn(1);//0正常显示，1 反色显示
	
	OLED_Refresh();
	
	delay_ms(50);
	
	OLED_ClearArea();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
	Gui_StrCenter(0,y,fc, bc, "TEST", size, mode,0);			//159
	OLED_Refresh();
	delay_ms(1000);
}

void ERROR_Show(uint8_t error)										//错误页面
{
	OLED_ClearArea();
	
	switch(languageMark){
		case 0:
					if(Limit_Switch)
					{
						Gui_StrCenter(0,40, WHITE, BLACK, "LOW", 24, 1,0);
						Gui_StrCenter(0,64, WHITE, BLACK, "WATER LEVEL", 24, 1,0);
						Gui_StrCenter(0,90, WHITE, BLACK, "PLEASE REFILL", 16, 1,0);
					}
					else
					{
						OLED_ShowString(25,40, "ERROR", 24, 1,0);
						OLED_ShowNum(95,	40, Error_Code, 3, 24, 1);
						Gui_StrCenter(0,80, WHITE, BLACK, "CONTACT", 16, 1,0);
						Gui_StrCenter(0,96, WHITE, BLACK, "CUSTOMER SERVICE", 16, 1,0);
					}
					break;
		case 1:
					if(Limit_Switch)
					{
						Gui_StrCenter(0,30, WHITE, BLACK, "WARN!", 16, 1,1);
						Gui_StrCenter(0,52, WHITE, BLACK, "WATER LOW", 16, 1,1);
						Gui_StrCenter(0,80, WHITE, BLACK, "PLEASE REFILL", 16, 1,1);
					}
					else
					{
						OLED_ShowString(25,40, "ERROR!", 16, 1,1);
						OLED_ShowNum(95,	40, Error_Code, 3, 16, 1);
						Gui_StrCenter(0,80, WHITE, BLACK, "CONTACT CUSTOMER SERVICE", 16, 1,1);
						//Gui_StrCenter(0,96, WHITE, BLACK, "CUSTOMER SERVICE", 16, 1,1);
					}
					break;
		default:
					break;
	}
	
	OLED_Refresh();
}

//void Task_Factory_test_Show(uint8_t check_done)
//{	
//	uint8_t size_0 = 20;
//	uint8_t size_1 = 16;
//	uint8_t x_0 = 92;
//	uint8_t y_0 = 32;
//	uint8_t y_1 = 52;
//	uint8_t y_2 = 72;
//	uint8_t y_3 = 92;
//	
//	if(check_done == 1)
//	{
//		OLED_ClearArea();
//		Gui_StrCenter(0,10,WHITE, BLACK, "FACTORY TEST", size_1, 1,0);
//		
//		{
//			Gui_StrCenter(0, y_0,WHITE, BLACK, "PUMP   ", size_1, 1,0);

//			if(Pump_Error)
//				OLED_ShowString(x_0, y_0, "NG", size_0, 1,0);
//			else
//				OLED_ShowString(x_0, y_0, "OK", size_1, 1,0);
//		}

//		{							
//			Gui_StrCenter(0, y_1,WHITE, BLACK, "NTC1   ", size_1, 1,0);
//			if(NTC_1_Error)
//				OLED_ShowString(x_0, y_1, "NG", size_0, 1,0);
//			else
//				OLED_ShowString(x_0, y_1, "OK", size_1, 1,0);
//		}

//		{
//			Gui_StrCenter(0, y_2,WHITE, BLACK, "NTC2   ", size_1, 1,0);
//			if(NTC_2_Error)
//				OLED_ShowString(x_0,y_2, "NG", size_0, 1,0);
//			else
//				OLED_ShowString(x_0,y_2, "OK", 16, 1,0);
//		}

//		{
//			Gui_StrCenter(0,y_3,WHITE, BLACK, "TANK   ", size_1, 1,0);
//			if(Limit_Switch)
//				OLED_ShowString(x_0,y_3, "NG", size_0, 1,0);
//			else
//				OLED_ShowString(x_0,y_3, "OK", size_1, 1,0);
//		}
//		
//		OLED_ShowString(122,48, "PUMP", size_1, 1,0);
//		OLED_ShowString(122,64, "Cal.", size_1, 1,0);

//		OLED_Refresh();
//	}
//	else
//	{
//		Temp_Show(Temp_0, 1);
//	}
//	
//	OLED_Refresh();
//}


