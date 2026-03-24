#include "sys.h"
#include "usart.h"
#include "delay.h"

#include "led.h"
#include "lcd.h"

#include "key.h"

#include "myiic.h"
#include "motor.h"
#include "scale.h"
#include "oled.h"
//#include "bmp.h"

#include "timer.h"
#include "spi.h"

#include "test.h"
#include "gui.h"
#include "pic.h"

#include "Init.h"
#include "task.h"

uint16_t Task = 0, Task_Last = 1;

extern uint8_t sleep_flag;

uint32_t flash_id;         
extern uint8_t SaveMark;  
extern Coffee_mode custom_Kind[10];     
extern Coffee_mode custom_Kind2[10];
extern uint8_t resetMark; 

extern uint8_t BMP1[];   //logo

extern uint8_t languageMark;
extern uint8_t UnitMark;
extern uint8_t SoundMark;

extern SystemFlags flagsToSave;

uint8_t Factory_test_start, Factory_test_start_last;

int main(void)
{
		//初始化
		Drivers_Init();
		W25Q32_Init();
		
		// W25Q32 ID
		flash_id = W25Q32_ReadID();

		uint8_t r_name[10][11];
		uint8_t r_title1[10][11];
		uint8_t r_title2[10][11];

		for (int i = 0; i < 10; i++) 
		{
			CoffeeFlash_Read(i, &custom_Kind[i], r_name[i], r_title1[i], r_title2[i]);
			if (custom_Kind[i].name != NULL &&
				custom_Kind[i].name[0] != '\0' &&
				strcmp((char *)custom_Kind[i].name, "None") != 0)
				//
			{
				SaveMark++;
			}
		}

		LoadSystemFlags(&flagsToSave);
		languageMark = flagsToSave.languageMark;
		UnitMark = flagsToSave.UnitMark;
		SoundMark = flagsToSave.SoundMark;
		
	while(1)
	{
		if(resetMark){
			OLED_ClearArea();
			// logo 显示
			OLED_ShowPicture(30,52,100,24,BMP1,1);
			OLED_Refresh();
			HAL_Delay(3000); // 延时3秒配合驱动板预留的烧录时间
			resetMark = 0;
		}
		
		LED_Set(sleep_flag);
		

		if((Task & 0xFFF0) == 0) 					// 一级页面
		{
			if(Factory_test_start)
				Task_Factory_test_work();		// 出厂设置 自检+流量标定
			else
				Task_Mode_Set_work();			// 模式选择
		}
		else
		{
			if((Task & 0xFF00) == 0) 				// 二级页面
			{				
				switch(mode_Now.mode_tag){
					case 0:
					case 1:
					case 2:
						Task_CoffeeType_work();   // 咖啡默认
						break;
					case 3:
						Task_Tea_Default_work();  // 茶默认
						break;
					case 4:
						Task_Custom_work();   	//	自定义页面
						break;
					case 5:
						Task_Choose_work();		
						break;
					case 6:
						Task_Setting_work();  	//	Setting 页面
						break;
					default:
						if(Task != Task_Last)
						{						
							if(Task < Task_Last)
								Task &= 0x000F;
							else
								Task += 0x0100;
						}
						break;
				}	
			}
			else
			{
				if((Task & 0xF000) == 0)			//三级页面
				{
					if(mode_Now.mode_tag == 3)
					{
						Task_Tea_Add_work();      // 茶叶克重
					}
					else
					{
						Task_Bean_Add_work();     // 咖啡豆克重
					}
				}
				else								//四级页面
				{
					switch(Task & 0xF000)
					{
						case 0x1000: 		// 参数总结
							Task_Summary_work();
							break;
						case 0x2000: 		// 自定义参数存储
							Task_Saved_work();
							break;
						case 0x3000:		// 冲泡前准备
							Task_Ready_work();
							break;
						case 0x4000:		// 延时冲泡
							Task_Reservation_work();
							break;
						case 0x5000:		// 自定义参数命名
							Task_Named_work();
							break;
						case 0x6000:		// 自定义参数命名完成
							Task_NameOver_work();
							break;
						case 0x7000:		// 品种替换
							Task_NameReplace_work();
							break;
						case 0x8000:		// 确认瓶中替换
							Task_AckReplace_work();
							break;
						case 0x9000:		// 编辑或删除
							Task_EditOrDelete_work();
							break;
						case 0xA000:		// 删除
							Task_NameDelete_work();
							break;
						case 0xB000:		// 自定义参数存储后执行
							Task_ExecuteWay_work();
							break;
						case 0xE000:		// 取消
							Task_Cancel_work();
							break;
						case 0xF000:		// 冲泡执行
							Task_Execute_work();
							break;
						default:
							break;
					}
				}			
			}
		}
	}
}

