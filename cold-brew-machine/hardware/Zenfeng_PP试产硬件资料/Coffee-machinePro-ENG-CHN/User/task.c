#include "task.h"
#include "GUI.h"
#include "stmflash.h"

#define calibrate_addr 0x08010000

int16_t temp_set = 0;

uint8_t time_start_tag, Progress_bar_tag, Progress_bar;

Coffee_mode mode_Now;
uint8_t pump_reset;

//Coffee_mode mode_Last;

uint8_t languageMark = 0;     //语言选择标志位，0：英   1：中
uint8_t UnitMark = 0;         //单位选择标志位，0：公制 1：英制
uint8_t SoundMark = 1;        //声音开启标志位，1：关   0：开

uint8_t resetMark = 0;        //系统重置标志位，1：开启重置

extern int Touch_flag_R;
extern int Touch_flag_L;

//结构体变量，存储语言、单位和声音标志位，用于写入外部Flash，W25Q32
//里面还有一个变量 uint8_t isValid;  // 有效性标志 (0xA5表示有效)
//有效标志在函数处理，不用赋值
SystemFlags flagsToSave = {
		.languageMark = 0,
		.UnitMark = 0,
		.SoundMark = 1
};
//custom_Kind 品种数据结构体  用于存储
Coffee_mode custom_Kind[10] = 
{
	{ .name = "None", .title_1 = "", .title_2 = "" }, // 第0项
	{ .name = "None", .title_1 = "", .title_2 = "" }, // 第1项
	{ .name = "None", .title_1 = "", .title_2 = "" }, // 第2项
	{ .name = "None", .title_1 = "", .title_2 = "" }, // 第3项
	{ .name = "None", .title_1 = "", .title_2 = "" }, // 第4项
	{ .name = "None", .title_1 = "", .title_2 = "" }, // 第5项
	{ .name = "None", .title_1 = "", .title_2 = "" }, // 第6项
	{ .name = "None", .title_1 = "", .title_2 = "" }, // 第7项
	{ .name = "None", .title_1 = "", .title_2 = "" }, // 第8项
	{ .name = "None", .title_1 = "", .title_2 = "" }, // 第9项
};

//custom_Kind2 测试结构体，用于赋值测试等
Coffee_mode custom_Kind2[10] = 
{
	{ .name = "1", .title_1 = "", .title_2 = "" }, // 第0项
	{ .name = "2", .title_1 = "", .title_2 = "" }, // 第1项
	{ .name = "3", .title_1 = "", .title_2 = "" }, // 第2项
	{ .name = "4", .title_1 = "", .title_2 = "" }, // 第3项
	{ .name = "5", .title_1 = "", .title_2 = "" }, // 第4项
	{ .name = "6", .title_1 = "", .title_2 = "" }, // 第5项
	{ .name = "7", .title_1 = "", .title_2 = "" }, // 第6项
	{ .name = "8", .title_1 = "", .title_2 = "" }, // 第7项
	{ .name = "9", .title_1 = "", .title_2 = "" }, // 第8项
	{ .name = "10", .title_1 = "", .title_2 = "" }, // 第9项
};

extern uint8_t Factory_test_start, Factory_test_start_last;

extern uint8_t Error_rx;
uint8_t cancelling_led_tag;			
void Task_Mode_Set_work(void) 		//一级界面
{
	static int8_t Status_Encoder = 0;
	static int8_t Task_start_tag = 0;
	
	if((Task_Last & 0xFFF0))
	{
		Task = Encoder = Status_Encoder;
	}

	if((Task != Task_Last) && ((Task_Last & 0xF000) == 0xE000))
	{
		OLED_ClearArea();
				
		switch(languageMark)
		{
			case 0:
				Gui_StrCenter(0, 52,	WHITE, BLACK, "CANCELLING",	24, 1,0);
				break;
			case 1:
				Gui_StrCenter(0, 52,	WHITE, BLACK, "CANCELLING",	16, 1,languageMark);
				break;
			default:
				break;
		}
		
		OLED_Refresh();
		
		pump_reset = 1;
		
		Task_Last = 1;
		
		Touch_Reset();
	}
	else
	{
		cancelling_led_tag = 0;
		
		if(pump_reset)
		{
			if(Count_Down_unShow(8))
			{
				pump_set = 0;
				
				time_start_tag = 0;
				Time_Reset();
				
				pump_reset = 0;
				
				Encoder = Status_Encoder;
			}
			else
			{
				time_start_tag = 1;
				
				pump_flag = 2;
				pump_set = 80;
			}
			
			Touch_Reset();
		}
		else
		{
			Status_Encoder = Encoder;
			
			if(Status_Encoder > mode_num)
				Status_Encoder = Encoder = 0;
			else if(Status_Encoder < 0)
				Status_Encoder = Encoder = mode_num;
			
			Task = Status_Encoder;
			
			if((Task != Task_Last) || (Task == 0 && Task_Last == 0 && Task_start_tag == 0)) // 初始化
			{	
				if(Task == 0 && Task_Last == 0)
					Task_start_tag = 1;
				else
					Task_start_tag = 0;
				mode_Now = Default_modes[Status_Encoder];
			}
			
			//强制修复：coffer模式中的CONCENTRATE执行后取消返回上一级显示错误
			if(mode_Now.mode_tag == 0 || mode_Now.mode_tag == 1 || mode_Now.mode_tag == 2){
				mode_Now = Default_modes[Status_Encoder];
			}
			
			Task_0xFFF0_Show(mode_Now); // UI display
			
			Task_Last = Task;
			
			Factory_test_start_last = Factory_test_start;
			
			if(Touch_s_R == 1)
			{
				Task += 0x0010;
				Touch_Reset();
			}
			else
			{
				static uint8_t touch_l_tag;
				
				if(Touch_l_R == 1 && Touch_l_L == 1)
					touch_l_tag++;
				else
					touch_l_tag = 0;
				
				if(touch_l_tag >= 30)
				{
					Factory_test_start = 1;
					Touch_Reset();
				}
			}
		}
	}
}

uint16_t pump_cal_0, pump_cal_1;
float pump_var_a, pump_var_b;
uint8_t variable_tx;
extern uint8_t Pump_Cal_Done;
extern float TEC_Current;
extern uint8_t  Main_Version_0;
extern uint8_t  Main_Version_1;
extern uint8_t  Main_Version_2;

#define pump_cal_time 50
#define FT_1_TEST_time 55

uint16_t cal_last_0, cal_last_1;
uint8_t flash_tag = 1;
void Task_Factory_test_work(void)	// 出厂测试
{
	static uint8_t check_done;
	static uint8_t Pump_cal;
	static uint8_t TEC_Current_Error = 1;
	static uint8_t TEC_Cooling_Error = 1;
	static int16_t encoder_last;
	
	static float temp_base;
	static uint8_t FT_heating_tag, FT_colding_tag;
	
	uint8_t size_0 = 20;
	uint8_t size_1 = 16;
	uint8_t x_0 = 92;
	uint8_t y_0 = 32;
	uint8_t y_1 = 52;
	uint8_t y_2 = 72;
	uint8_t y_3 = 92;
	uint8_t y_4 = 112;
	
	if(Factory_test_start != Factory_test_start_last)
	{
		OLED_ClearArea();
	
		OLED_ColorTurn(1);//0正常显示，1 反色显示
		
		Gui_StrCenter(0,10,WHITE, BLACK, "FACTORY TEST", 16, 1,0);
		
		temp_base = Temp_0;
		
		stmflash_read(calibrate_addr,   &cal_last_0, 2);
		stmflash_read(calibrate_addr+4, &cal_last_1, 2);
		
		if(cal_last_0 == 0xFFFF || cal_last_1 == 0xFFFF)
		{
			OLED_ShowString(0, 50, "No", 16, 1,0);
			OLED_ShowString(0, 66, "Cal", 16, 1,0);
		}
		else
		{
			OLED_ShowNum(0, 50, cal_last_0/10, 2, 16, 1);
			OLED_ShowString(16, 50, ".", 16, 1,0);
			OLED_ShowNum(24, 50, cal_last_0, 1, 16, 1);

			OLED_ShowNum(0, 66, cal_last_1/10, 2, 16, 1);
			OLED_ShowString(16, 66, ".", 16, 1,0);
			OLED_ShowNum(24, 66, cal_last_1, 1, 16, 1);
		}
		OLED_ShowString(152, 58, "s", 16, 1,0);
		
		temp_switch = 2;
		temp_set = 70;
		FT_heating_tag = 0;
		FT_colding_tag = 0;
		
		pump_set = 45;
	}
	
	Task_Last = Task;
	
	if(check_done == 0)
	{
		if(Error_rx)
		{
			ERROR_Show(Error_rx);
										
			temp_switch = 0;
			pump_set = 0;
				
			time_start_tag = 0;
			Time_Reset();
			
			BUZZER_Set(0);
		}
		else
		{
			if(Second < 1)
				BUZZER_Set(100);
			else
				BUZZER_Set(0);
			
			if(Temp_0 >= 70 && FT_heating_tag == 0)
			{
				FT_heating_tag = 1;
				
				Time_Reset();
				
				pump_set = 0;
			}
			else if(FT_heating_tag == 1 && Second >= 10)
			{
				temp_switch = 1;
				temp_set = 5;
				
				if(FT_colding_tag ==0)
				{
					if(Temp_0 <= 5)
					{
						FT_colding_tag = 1;
						pump_set = 80;
						
						if(Second <= 45)
						{
							TEC_Cooling_Error = 0;
							Second = FT_1_TEST_time - 8;
						}						
					}
					else
					{
						pump_set = 0;
						OLED_ShowNum(130, 58, Second - 10, 2, 16, 1);
					}
				}
			}
			
			if(TEC_Current > 9)
				TEC_Current_Error = 0;
			
			if(Count_Down_unShow(FT_1_TEST_time))
			{
				pump_set = 0;
				temp_switch = 0;					
				
				time_start_tag = 0;
				
				{
					OLED_ClearArea();
					Gui_StrCenter(0,10,WHITE, BLACK, "FACTORY TEST", size_1, 1,0);
					
					{
						Gui_StrCenter(0, y_0,WHITE, BLACK, "PUMP   ", size_1, 1,0);

						if(Pump_Error)
							OLED_ShowString(x_0, y_0, "NG", size_0, 1,0);
						else
							OLED_ShowString(x_0, y_0, "OK", size_1, 1,0);
					}

					{							
						Gui_StrCenter(0, y_1,WHITE, BLACK, "NTC1   ", size_1, 1,0);
						if(NTC_1_Error)
							OLED_ShowString(x_0, y_1, "NG", size_0, 1,0);
						else
							OLED_ShowString(x_0, y_1, "OK", size_1, 1,0);
					}

					{
						Gui_StrCenter(0, y_2,WHITE, BLACK, "NTC2   ", size_1, 1,0);
						if(NTC_2_Error)
							OLED_ShowString(x_0,y_2, "NG", size_0, 1,0);
						else
							OLED_ShowString(x_0,y_2, "OK", 16, 1,0);
					}

					{
						Gui_StrCenter(0,y_3,WHITE, BLACK, "TANK   ", size_1, 1,0);
						if(Limit_Switch)
							OLED_ShowString(x_0,y_3, "NG", size_0, 1,0);
						else
							OLED_ShowString(x_0,y_3, "OK", size_1, 1,0);
					}
					
					{
						Gui_StrCenter(0,y_4,WHITE, BLACK, "TEC    ", size_1, 1,0);
						if(TEC_Current_Error || TEC_Cooling_Error)
							OLED_ShowString(x_0,y_4, "NG", size_0, 1,0);
						else
							OLED_ShowString(x_0,y_4, "OK", size_1, 1,0);
					}
				
					OLED_ShowString(122,48, "PUMP", size_1, 1,0);
					OLED_ShowString(122,64, "Cal.", size_1, 1,0);

					OLED_Refresh();
				}
				
				if(Touch_s_R == 1)
				{
					Time_Reset();
					
					Pump_cal = 1;
					Touch_Reset();
					check_done = 1;
					OLED_ClearArea();
					Gui_StrCenter(0,10,WHITE, BLACK, "PUMP CALIBRATE 1", size_1, 2,0);
				}
			}
			else
			{
				time_start_tag = 1;
				
				OLED_ShowNum(70, 100, (uint16_t)TEC_Current, 2, 16, 1);
				OLED_ShowString(86,100, ".", 16, 1,0);
				OLED_ShowNum(92, 100, (uint16_t)(TEC_Current * 10), 1, 16, 1);
				OLED_ShowString(108,100, "A", 16, 1,0);
				
				OLED_ShowString(20, 26, "SCREEN", size_1, 1,0);
				OLED_ShowNum(70, 26, SCREEN_VERSION_0, 2, 16, 1);
				OLED_ShowString(86,26, ".", 16, 1,0);
				OLED_ShowNum(92, 26, SCREEN_VERSION_1, 2, 16, 1);
				OLED_ShowString(108,26, ".", 16, 1,0);
				OLED_ShowNum(114, 26, SCREEN_VERSION_2, 2, 16, 1);
				
				OLED_ShowString(20, 82, "MAIN"	, size_1, 1,0);
				OLED_ShowNum(70, 82, Main_Version_0, 2, 16, 1);
				OLED_ShowString(86,82, ".", 16, 1,0);
				OLED_ShowNum(92, 82, Main_Version_1, 2, 16, 1);
				OLED_ShowString(108,82, ".", 16, 1,0);
				OLED_ShowNum(114, 82, Main_Version_2, 2, 16, 1);
				
				Temp_Show(Temp_0, 1);
				
				if(Touch_s_R == 1)
					Touch_Reset();
			}
		}
	}
	else
	{
		if(Error_rx)
		{
			ERROR_Show(Error_rx);
										
			temp_switch = 0;
			pump_set = 0;
				
			time_start_tag = 0;
		}
		else
		{			
			if(Pump_cal == 1)
			{
				if(Count_Down_Show(pump_cal_time))
				{
					pump_set = 0;

					time_start_tag = 0;
					
					if(Touch_s_R == 1)
					{
						Time_Reset();
						Pump_cal = 2;
						Touch_Reset();	
						OLED_ClearArea();
						Gui_StrCenter(0,10,WHITE, BLACK, "PUMP CALIBRATE 2", size_1, 1,0);
						OLED_Refresh();
					}
					else
					{
						pump_cal_0 = 250 + Encoder;
						
						if(pump_cal_0 > 300)
						{
							pump_cal_0 = 300;
							Encoder = encoder_last;
						}
						else if(pump_cal_0 < 150)
						{
							pump_cal_0 = 150;
							Encoder = encoder_last;
						}

						encoder_last = Encoder;
						
						OLED_ClearArea();
					
						Gui_StrCenter(0,10,WHITE, BLACK, "PUMP CALIBRATE 1", size_1, 1,0);
						
						OLED_ShowNum(50, 50, pump_cal_0 / 10, 2, 28, 1);
						OLED_ShowString(78, 62, ".", size_0, 1,0);
						OLED_ShowNum(88, 50, pump_cal_0, 1, 28, 1);
						OLED_ShowString(114, 62, "g", size_0, 1,0);

						OLED_Refresh();
					}
				}
				else
				{
					time_start_tag = 1;
					Encoder = 0;
					pump_set = 50;
					pump_cal_0 = 250;
				}
			}
			else if(Pump_cal == 2)
			{
				if(Count_Down_Show(pump_cal_time))
				{
					time_start_tag = 0;
					
					if(Touch_s_R > 1 && (Pump_Cal_Done == 0))
					{
						Factory_test_start = 0;
						Touch_Reset();
						check_done = 0;
						Pump_cal = 0;
						Time_Reset();
						
						pump_flag = 0;
						pump_set = 0;

						temp_switch = 0;
						variable_tx = 0;
						
						OLED_ClearArea();
						OLED_ColorTurn(0);//0正常显示，1 反色显示
					}
					else if(Touch_s_R >= 1)
					{						
						OLED_ClearArea();
						
						Gui_StrCenter(0,10,WHITE, BLACK, "PUMP CALIBRATE 2", size_1, 1,0);
						
						pump_var_a = 150.f / (float)(pump_cal_1 - pump_cal_0 - pump_cal_0);
						pump_var_b = 50.f - (float)(pump_cal_0 * pump_var_a * 0.2f);
											
						if(Pump_Cal_Done == 0)
						{
							Gui_StrCenter(0,48,WHITE, BLACK, "DONE", 28, 1,0);
							variable_tx = 0;
							
							pump_set = 0;
						}
						else
						{
							Gui_StrCenter(0,48,WHITE, BLACK, "WAIT", 28, 1,0);
							
							variable_tx = 1;
							
							if(flash_tag)
							{
								flash_tag = 0;
								stmflash_write(calibrate_addr,&pump_cal_0, 1);
								stmflash_write(calibrate_addr + 4, &pump_cal_1, 1);
							}
							
							pump_flag = 2;
							pump_set = 80;
						}
					}
					else if(Touch_s_R == 0)
					{
						pump_set = 0;
						
						pump_cal_1 = 650 + Encoder;
						
						if(pump_cal_1 > 750)
						{
							pump_cal_1 = 750;
							Encoder = encoder_last;
						}
						else if(pump_cal_1 < 600)
						{
							pump_cal_1 = 600;
							Encoder = encoder_last;
						}

						encoder_last = Encoder;
					
						OLED_ClearArea();
						
						Gui_StrCenter(0,10,WHITE, BLACK, "PUMP CALIBRATE 2", size_1, 1,0);
						
						OLED_ShowNum(50, 50, pump_cal_1 / 10, 2, 28, 1);
						OLED_ShowString(78, 62, ".", size_0, 1,0);
						OLED_ShowNum(88, 50, pump_cal_1, 1, 28, 1);
						OLED_ShowString(114, 62, "g", size_0, 1,0);

						OLED_Refresh();
						Pump_Cal_Done = 1;
					}
				}
				else
				{
					Encoder = 0;
					time_start_tag = 1;
					pump_set = 80;
					Touch_Reset();
					pump_cal_1 = 650;
				}
			}
		}
	}
	
	Factory_test_start_last = Factory_test_start;
	
	if(Touch_s_L == 1)
	{
		BUZZER_Set(0);
		
		Factory_test_start = 0;
		
		check_done = 0;
		
		Pump_cal = 0;
		
		Time_Reset();
		time_start_tag = 0;
		
		pump_set = 0;
		temp_switch = 0;
		variable_tx = 0;
		
		OLED_ClearArea();
		OLED_ColorTurn(0);//0正常显示，1 反色显示
		
		Touch_Reset();
	}
}

//咖啡任务：REGULAR、CONCENTRATE、SLOW
//每个模式有三种烘烤度供选择
void Task_CoffeeType_work()
{
	static int8_t Status_Encoder = 0;
	static int8_t Task_start_tag = 0;

	static Coffee_mode* Default_modes_Temp;

	if((Task != Task_Last) || (Task == 0 && Task_Last == 0 && Task_start_tag == 0)) // 初始化
	{	
		if(Task == 0 && Task_Last == 0)
			Task_start_tag = 1;
		else
			Task_start_tag = 0;
		
		//根据名字判断咖啡的模式
		if (strcmp((const char*)mode_Now.name, "REGULAR") == 0) {	
			//将REGULAR的三种烘培度参数赋值到中间结构体变量
			Default_modes_Temp = Default_modes_Regular;		
			Status_Encoder = Encoder = 0;
		} 
		else if(strcmp((const char*)mode_Now.name, "CONCENTRATE") == 0){
			Default_modes_Temp = Default_modes_Concentrate;
			Status_Encoder = Encoder = 0;
		}
		else if(strcmp((const char*)mode_Now.name, "SLOW") == 0){
			Default_modes_Temp = Default_modes_SlowDrip;
			Status_Encoder = Encoder = 0;
		}
	}

	Status_Encoder = Encoder;

	if(Status_Encoder > 2)
		Status_Encoder = Encoder = 0;
	else if(Status_Encoder < 0)
		Status_Encoder = Encoder = 2;
		
	//显示烘培度选择界面
	Task_CoffeeType_Show(mode_Now); // UI display		
	Task_Last = Task;

	
	if(Touch_s_L == 1){
	
		// 左键触摸返回
		Task &= 0x000F;
		Touch_Reset();
	}
	else if(Touch_s_R == 1){
		//在烘培度选择界面，编码器上下选择，右键确认选择
		//将烘培度结构体参数  赋值到 要执行的结构体mode_Now中
		mode_Now = Default_modes_Temp[Status_Encoder];
		//切换到 加咖啡豆任务
		Task += 0x0100;
		Touch_Reset();
	}
}

//int8_t Tea_Status_Encoder = 0;

//茶模式 任务  选择茶种类
void Task_Tea_Default_work(){

		static int8_t Tea_Status_Encoder = 0;
	
		if(Task != Task_Last)
		{
			Tea_Status_Encoder = Encoder = 0;
		}
		
		Tea_Status_Encoder = Encoder;

		if(Tea_Status_Encoder > tea_mode_num)
			Tea_Status_Encoder = Encoder = tea_mode_num;
		else if(Tea_Status_Encoder < 0)
			Tea_Status_Encoder = Encoder = 0;
		
		//茶 种类 选择界面
		Task_TeaType_Show() ;// UI display
		
		Task_Last = Task;
		
		if(Touch_s_R == 1)
		{
			//将对应茶的结构体参数  赋值到 要执行的结构体mode_Now中
			mode_Now = Tea_Default_modes[Tea_Status_Encoder];
			Task += Tea_Status_Encoder << 4;
			//切换到 加茶叶任务，在main有mode_Now.tag的判断
			//mode_Now.tag = 3  加茶，反之加咖啡豆
			Task += 0x0100;  
			Touch_Reset();
		}
		else if(Touch_s_L == 1)
		{
			//Task &= 0x0FFF;
			Task &= 0x000F;
			Touch_Reset();
		}

}

uint8_t Save_flag = 0;        //自定义品种保存标志位，1：要保存
uint8_t SaveMark = 0;         //品种保存下标，0-10
int8_t uesr_EditMark = 0;     //用户编辑标志位，在品种选择界面进入，1：要编辑

void Task_Custom_work(void)			//自定义界面
{
	static uint8_t off_tag;
					
	static uint8_t custom_state_now;
	static uint8_t custom_state_last;
						
	static uint8_t touch_tag;
						
	static uint32_t Data_Custom;
	
	//从自定义界面进入后，直接令品种保存标志位为1，在后续 保存任务Task_Saved_work 会选择清零与否
	// custom_state_now == 0  表示还未进入，还在一级选择界面
	if(custom_state_now == 0){
			Save_flag = 0;
	}
	else{
			Save_flag = 1;
	}
	
	//第一次刚进入时
	if(Task != Task_Last)
	{
		OLED_ClearArea();
		//Task != Task_Last  第一次刚进入时
		//将自定义模式的默认结构体参数  赋值到 要执行的结构体mode_Now中
		mode_Now = Default_modes[4];
		
		//将参数进行换算处理，得到展示给用户的数据
		Data_Custom = Custom_Data_Process(custom_state_now, 1);
		//将自定义模式的子界面显示，一共有6个，并显示对应处理好的参数
		Task_0xFF16_Show(custom_state_now, Custom_Setting[custom_state_now].data_default);
	}
	
	Data_Custom = Custom_Data_Process(custom_state_now, custom_state_last);
	
	Task_0xFF16_Show(custom_state_now, Data_Custom);
							
	custom_state_last = custom_state_now;
	
	Task_Last = Task;
/************************ Touch_set **************************/	
	{
		if(Touch_s_L == 1)
		{
			//左键返回到自定义首页后，将用户编辑标志位清零
			if(custom_state_now == 0){
					uesr_EditMark = 0;
			}
			if(custom_state_now == 3 && off_tag == 1)
			{
				custom_state_now -= 3;
				touch_tag -= 3;
			}
			else
			{
				if(touch_tag > 0)
				{
					custom_state_now -- ;
					touch_tag --;
				}
				else
				{
					Task &= 0x000F;
					custom_state_now = 0;
					touch_tag = 0;
					off_tag = 0;
					Default_modes[4] = mode_Now;
				}
			}
			Touch_Reset();
		}
		else if(Touch_s_R == 1)
		{
			if(custom_state_now == 0 && Data_Custom == 0)
			{
				custom_state_now += 3;
				touch_tag += 3;
				off_tag = 1;
			}
			else
			{
				if(touch_tag < 5)
				{
					custom_state_now ++;
					touch_tag++;
				}
				else
				{
					//Task += 0x0100;
					Task += 0x1000;
					custom_state_now = 0;
					touch_tag = 0;
					off_tag = 0;
					Default_modes[4] = mode_Now;
				}
			}
			Touch_Reset();
		}
		else if(Touch_l_L)
		{
			if(touch_tag > 0)
			{
				custom_state_now -- ;
				touch_tag --;
			}
			else
			{
				Task &= 0x000F;
				Touch_Reset();
				custom_state_now = 0;
				touch_tag = 0;
				off_tag = 0;
				Default_modes[4] = mode_Now;
			}
		}
	}
/*************************************************************/
}

extern uint8_t Descale_Done;        //除垢完成标志位
extern uint8_t System_Check_Done;   //自检完成标志位
extern uint8_t Descale_Finish;      //除垢结束界面 标志位


uint8_t is_setting_first_enter; // 新增首次进入标志

uint8_t last_language_index = 0;  // 保存上次选择的语言索引
uint8_t last_unit_index = 0;      // 新增：保存单位选择
uint8_t last_sound_index = 1;     // 新增：保存声音设置

extern uint8_t Pump_cal;

extern uint8_t setting_led_close;   //左右键 灯  标志位
//extern uint8_t BMP1[];

int8_t Setting_mode_tag, Setting_mode_tag_last;
int8_t Setting_data_tag;

//设置 任务
void Task_Setting_work(void)
{
	if (Task != Task_Last) 
	{
		OLED_ClearArea();
		Encoder = Setting_mode_tag = 0;
		Setting_data_tag = 0;
		is_setting_first_enter = 1; 
    }

    if (Setting_data_tag == 0) 
	{
        if (Encoder < 0) Encoder = 0;
        if (Encoder > 4) Encoder = 4;

		Setting_mode_tag = Encoder;
    }

    if (Setting_mode_tag_last != Setting_mode_tag) 
	{
        OLED_ClearArea();
        Setting_mode_tag_last = Setting_mode_tag;
    }
	
	//除垢结束后，显示结束画面，两分钟无操作，自动返回并清零标志位
	if(Descale_Finish)
	{
		Descale_Finish = 0;
		
		OLED_ClearArea();
		
		Descale_Done = 0;
		setting_led_close = 0;
		
		Time_Reset();
		time_start_tag = 0;
	
		Setting_data_tag = 0;
		Setting_mode_tag = 3;
		Encoder= 3;
	}
	
	//在系统重置界面 右键长按 触发重置
	if(Progress_bar >= 159)
	{
		languageMark = 0;
		UnitMark = 0;
		SoundMark = 1;
		mode_Now.mode_tag = 0;
		flagsToSave.languageMark = languageMark;
		flagsToSave.UnitMark = UnitMark;
		flagsToSave.SoundMark = SoundMark;
		SaveSystemFlags(&flagsToSave);
		Encoder = 0;
		Progress_bar_tag = 0;
		Setting_data_tag = 0;
		Setting_mode_tag = 0;
		//W25Q32_EraseSector(0x000000);
		
		//存储结构体custom_Kind清空
		for (int i = 0; i < SaveMark; i++) 
		{
			free(custom_Kind[i].name);  // 释放内存
			custom_Kind[i].name = NULL;
			CoffeeFlash_DeleteAndCompact(i); 
			//HAL_Delay(50);

			custom_Kind[i].name = NULL;
			// 指针设为NULL
			custom_Kind[i].title_1 = NULL;       // 字符串指针
			custom_Kind[i].title_2 = NULL;       // 字符串指针

			custom_Kind[i].Heating_Temp = 0;     // 温度值
			custom_Kind[i].Cooling_Temp = 0;     // 温度值
			custom_Kind[i].Heating_spd = 0;      // 速度值
			custom_Kind[i].Cooling_spd = 0;      // 速度值

			custom_Kind[i].Heating_Time = 0;     // 时间值
			custom_Kind[i].Cooling_Time = 0;    // 时间值

			custom_Kind[i].Heating_CWratio = 0;  // 比例值
			custom_Kind[i].Cooling_CWratio = 0;  // 比例值

			custom_Kind[i].Total_Time = 0;       // 总时间
			custom_Kind[i].Total_ml = 0;         // 总量
			custom_Kind[i].mode_tag = 0;         // 模式标记
		}
		//品种保存标志位归零
		SaveMark = 0;
		//重置标志位置1
		resetMark = 1;
		//返回开机界面
		Task = 0x0000;  
		Touch_Reset();
		Time_Reset();
	}
		
	Task_SETTING_Show(Setting_mode_tag, Setting_data_tag);
		
    Task_Last = Task;

  if (Touch_s_L == 1) 
	{
		//设置选择列表界面，左键返回一级界面
		if(is_setting_first_enter && Setting_data_tag == 0)
		{
			Task &= 0x000F; // 返回主界面
			is_setting_first_enter = 0;
		}
		else
		{
			if(Setting_data_tag == 1)
			{
				BUZZER_Set(0);
				
				Setting_data_tag = 0;
				Encoder = Setting_mode_tag;

				if(Setting_mode_tag == 4 && System_Check_Done == 0)
				{
					temp_switch = 0;
					pump_set = 0;
					time_start_tag = 0;
					Time_Reset();
				}
			}
			else if(Setting_data_tag == 2)
			{
				Setting_data_tag--;
				
				if(Setting_mode_tag == 3 && Descale_Done == 0)
				{
					temp_switch = 0;
					pump_set = 0;
					time_start_tag = 0;
					Time_Reset();
				}
			}
		}

		OLED_ClearArea();
		Touch_Reset();
    } 
	//右键短按
	else if (Touch_s_R == 1) 
	{		
		if(Setting_data_tag != 0)
		{
			switch(Setting_mode_tag)
			{
				//确认语言选项，并写入外部Flash，返回上一级界面
				case 0:
					languageMark = Encoder;
					flagsToSave.languageMark = languageMark;
					//将标志位结构体写入外部存储器
					SaveSystemFlags(&flagsToSave);
					last_language_index = Encoder;  // 保存选择
					Encoder = 0;
					Setting_data_tag= 0;
					break;
				
				case 1:
					UnitMark = Encoder;
					flagsToSave.UnitMark = UnitMark;
					SaveSystemFlags(&flagsToSave);
					last_unit_index = Encoder;
					Encoder = 1;
					Setting_data_tag= 0;
					break;
				
				case 2:
					SoundMark = Encoder;
					flagsToSave.SoundMark = SoundMark;
					SaveSystemFlags(&flagsToSave);
					//开启声音后，响300ms，再返回上一级界面
					if(!SoundMark){
						BUZZER_Set(100);
						HAL_Delay(300);
						BUZZER_Set(0);
						HAL_Delay(50);
					}
					last_sound_index = Encoder;
					Encoder = 2;
					Setting_data_tag= 0;
					break;
				
				case 3:
					if(Setting_data_tag == 1)
						Setting_data_tag++;
					else if(Setting_data_tag == 2 && Descale_Done)
					{						
						Descale_Finish = 1;
						Descale_Done = 0;
						
						Setting_data_tag= 0;
						Encoder = 3;
					}
					break;
				
				case 4:
//					if(System_Check_Done)
//					{
//						setting_led_close = 1;
//						Setting_data_tag= 0;
//						Encoder = 4;
//					}
//					break;
//				
//				case 5:
					break;
				
				default:
					break;		
			}
		}
		else
		{
			Setting_data_tag++;
			
			switch(Setting_mode_tag)
			{
				case 0:
					/* 修复1：首次进入时同步编码器值 */
					Encoder = languageMark; // 强制同步物理编码器值
					break;
				
				case 1:
					/* 修复1：首次进入时同步编码器值 */
					Encoder = UnitMark; // 强制同步物理编码器值
					break;
				
				case 2:
					/* 修复1：首次进入时同步编码器值 */
					Encoder = SoundMark; // 强制同步物理编码器值
					break;
				
				case 3:
					break;
				
				case 4:
//					break;
//				
//				case 5:
					break;
				
				default:
					break;		
			}
		}
				
		if(System_Check_Done)
		{
			Setting_data_tag = 0;
			System_Check_Done = 0;
			Time_Reset();
		}
				
        OLED_ClearArea();
        Touch_Reset();
    }

	if(Touch_l_R)
	{
		if(Touch_flag_R == 1 && Progress_bar < 159)
		{
			if(Setting_mode_tag == 4)
			{
				Progress_bar_tag = 1;
			}
		}
		else
		{
			Progress_bar_tag = 0;

			Touch_Reset();
		}
	}
}

int16_t selected_idx = 0;       //品种选择界面，当前选中选项的标志位
int8_t uesr_chooseMark = 0;			//用户选中后确认标志位
//uint8_t need_refresh = 1;       //显示界面刷新标志位 

//品种选择 任务
void Task_Choose_work(void) {
	
	
    	if(Task != Task_Last)
			{
				//从特殊界面进入，选中框强制赋值
				if(Task_Last == 0xB000)selected_idx = Encoder;
				else if((Task_Last & 0xF000) == 0x6000)selected_idx = SaveMark - 1;
				//else Encoder = selected_idx = 0;
				//品种选择界面
				Task_Choose_Show(0, 0);
			}
	
//			Encoder = selected_idx = 0;
			
			//品种选择界面
			Task_Choose_Show(0, 0);
			
			Task_Last = Task;
	
    // 触摸按键处理
    if(Touch_s_L == 1) {
				//uesr_EditMark = 0;
				//uesr_chooseMark = 0;
        Task = 0x0004;
        Touch_Reset();
    }
    else if(Touch_s_R == 1) {
			
			if (custom_Kind[selected_idx].name != NULL && custom_Kind[selected_idx].name[0] != '\0' && strcmp((char*)custom_Kind[selected_idx].name, "None") != 0) {
							//右键选择，选中标志位触发
							uesr_chooseMark = 2;
							//mode_Now = custom_Kind[selected_idx];
							Task = (Task & 0x0FFF) | 0x1000;  // 保留低12位，强制高4位=0x1

			}
        Touch_Reset();
    }
		else if(Touch_l_R == 1)
		{
			//右键长按 进入 编辑/删除品种界面 0x9000
			if(custom_Kind[selected_idx].name != NULL && custom_Kind[selected_idx].name[0] != '\0' && strcmp((char*)custom_Kind[selected_idx].name, "None") != 0){
					Task = (Task & 0x0FFF) | 0x9000;  
			}
			Touch_Reset();
		}       
    
}

int16_t selected_idx_replace; //替换品种下标
uint8_t replace_index = 0xFF; // 初始化为无效值

// 品种替换 任务
void Task_NameReplace_work(void) {
	
		//替换界面
    Task_NameReplace_Show(); 
    
    // 触摸按键处理
    if(Touch_s_L == 1) {
        // 左键触摸返回
        Task = (Task & 0x0FFF) | 0x2000;  // 保留低12位，强制高4位=0x2
        Touch_Reset();
    }
    else if(Touch_s_R == 1) {
        // 右键触摸确认替换选中的选项，并判断名字是否为有效值
        if (custom_Kind[selected_idx_replace].name != NULL && custom_Kind[selected_idx_replace].name != '\0' && strcmp((char*)custom_Kind[selected_idx_replace].name, "None") != 0) {
            // 标记当前选中项为待替换项
            replace_index = selected_idx_replace;
            
            // 返回上级菜单进行替换操作
//            Task = (Task & 0x0FFF) | 0x5000;  // 返回上级菜单
						Task = (Task & 0x0FFF) | 0x8000;  // 返回上级菜单
        }
        Touch_Reset();
    }
}

// 确认替换 任务
void Task_AckReplace_work(void) {

	//确认选择界面
	Task_AckReplace_Show();
							
	Task_Last = Task;

	
	if(Touch_s_L == 1){
	
		// 左键触摸返回
		Task = (Task & 0x0FFF) | 0x7000;  // 保留低12位，强制高4位=0x7
		Touch_Reset();
	
	}
	else if(Touch_s_R == 1){
	
		Task = (Task & 0x0FFF) | 0x5000;  // 返回上级菜单  品种命名
		Touch_Reset();
	
	}
}

static uint8_t selected_option_Edit = 0;    //用户编辑标志位
//编辑删除选择界面
void Task_EditOrDelete_work(void){
	static int16_t accumulated_encoder = 0; // 编码器累计值

	// 初始化检查
	if(Task != Task_Last) {
			accumulated_encoder = 0;
			Encoder = 0;
			selected_option_Edit = 1;
	}

	// 编码器处理（循环式选择）
	if(Encoder != 0) {
			accumulated_encoder += Encoder; // 累计编码器变化量
			Encoder = 0; // 清除增量
			// 每2个脉冲切换一个字符（可根据编码器灵敏度调整）
			if(abs(accumulated_encoder) >= 1) {
					selected_option_Edit = !selected_option_Edit;
					accumulated_encoder = 0; 
			}
	}
	Task_EditOrDelete_Show(selected_option_Edit);
							
	Task_Last = Task;

	if(Touch_s_L == 1)
	{							
		mode_Now.mode_tag = 5;
		Task = 0x0015;
		Touch_Reset();
	}	
	else if(Touch_s_R == 1)
	{	
		uesr_chooseMark = 0;
		if(selected_option_Edit == 0){
				//切换到删除任务
					Task = (Task & 0x0FFF) | 0xA000;  // 保留低12位，强制高4位=0x7
					Touch_Reset();
		}
		else if(selected_option_Edit == 1){
					uesr_EditMark = 1;
				//切换到编辑任务,用户自定义界面
					mode_Now.mode_tag = 4;
					Task = 0x0014;
					Touch_Reset();
		}
	}
}


void Task_NameDelete_work(void) {
	
	static int16_t accumulated_encoder = 0; // 编码器累计值

	Task_NameDelete_Show();
							
	Task_Last = Task;

	
	if(Touch_s_L == 1){
	
		// 左键触摸返回
		mode_Now.mode_tag = 5;
		Task = 0x0015;  // 返回上级菜单
		Touch_Reset();
	
	}
	else if(Touch_s_R == 1){
				Task_NameDeleteOver_Show();
        // 右键触摸删除选中的选项
        if (custom_Kind[selected_idx].name != NULL && custom_Kind[selected_idx].name[0] != '\0' && strcmp((char*)custom_Kind[selected_idx].name, "None") != 0) {
            // 删除选中的项目
            free(custom_Kind[selected_idx].name);  // 释放内存
            custom_Kind[selected_idx].name = NULL;
            CoffeeFlash_DeleteAndCompact(selected_idx);
					
            // 重新排序数组，将后面的项目前移
            for (uint8_t i = selected_idx; i < 9; i++) {
                custom_Kind[i].name = custom_Kind[i+1].name;
                // 如果有其他字段也需要复制
                // custom_Kind[i].other_field = custom_Kind[i+1].other_field;
								custom_Kind[i].title_1 = custom_Kind[i+1].title_1;       // 字符串指针
								custom_Kind[i].title_2 = custom_Kind[i+1].title_2;       // 字符串指针

								custom_Kind[i].Heating_Temp = custom_Kind[i+1].Heating_Temp;     // 温度值
								custom_Kind[i].Cooling_Temp = custom_Kind[i+1].Cooling_Temp;     // 温度值
								custom_Kind[i].Heating_spd = custom_Kind[i+1].Heating_spd;      // 速度值
								custom_Kind[i].Cooling_spd = custom_Kind[i+1].Cooling_spd;      // 速度值

								custom_Kind[i].Heating_Time = custom_Kind[i+1].Heating_Time;     // 时间值
								custom_Kind[i].Cooling_Time = custom_Kind[i+1].Cooling_Time;    // 时间值

								custom_Kind[i].Heating_CWratio = custom_Kind[i+1].Heating_CWratio;  // 比例值
								custom_Kind[i].Cooling_CWratio = custom_Kind[i+1].Cooling_CWratio;  // 比例值

								custom_Kind[i].Total_Time = custom_Kind[i+1].Total_Time;       // 总时间
								custom_Kind[i].Total_ml = custom_Kind[i+1].Total_ml;         // 总量
								custom_Kind[i].mode_tag = custom_Kind[i+1].mode_tag;         // 模式标记
            }
						
						
            // 清空最后一个元素
						SaveMark--;
            custom_Kind[SaveMark].name = NULL;
						// 指针设为NULL
						custom_Kind[SaveMark].title_1 = NULL;       // 字符串指针
						custom_Kind[SaveMark].title_2 = NULL;       // 字符串指针

						custom_Kind[SaveMark].Heating_Temp = 0;     // 温度值
						custom_Kind[SaveMark].Cooling_Temp = 0;     // 温度值
						custom_Kind[SaveMark].Heating_spd = 0;      // 速度值
						custom_Kind[SaveMark].Cooling_spd = 0;      // 速度值

						custom_Kind[SaveMark].Heating_Time = 0;     // 时间值
						custom_Kind[SaveMark].Cooling_Time = 0;    // 时间值

						custom_Kind[SaveMark].Heating_CWratio = 0;  // 比例值
						custom_Kind[SaveMark].Cooling_CWratio = 0;  // 比例值

						custom_Kind[SaveMark].Total_Time = 0;       // 总时间
						custom_Kind[SaveMark].Total_ml = 0;         // 总量
						custom_Kind[SaveMark].mode_tag = 0;         // 模式标记
						
					
            
            // 调整选中索引，防止越界
            if (selected_idx > 0) {
                selected_idx--;
                Encoder--;
            }
        }
		
		HAL_Delay(2000);
		mode_Now.mode_tag = 5;
		Task = 0x0015;
		Touch_Reset();
	
	}
}

uint8_t Coffee_Added = 16;

uint8_t Added_Mark = 0;

void Task_Bean_Add_work(void)		//加豆
{
	static uint8_t Coffee_Added_last;
					
	if(Task != Task_Last)
	{
		Encoder = Coffee_Added;
								
		Task_0xF1FF_Show(Coffee_Added); // 豆量
	}
	
	Coffee_Added = Encoder;
				
	if(Coffee_Added > 60)
		Coffee_Added = Encoder = 60;
	else if(Coffee_Added < 2)
		Coffee_Added = Encoder = 2;
	
	if(Coffee_Added != Coffee_Added_last)
		Task_0xF1FF_Show(Coffee_Added);
	
	Task_Last = Task;

	if(Touch_s_L == 1)
	{
		if(uesr_chooseMark == 1 || uesr_chooseMark == 2){
			uesr_chooseMark = 2;
			Task = 0x1000;  // 保留低12位，强制高4位=0x1
			Touch_Reset();
		}
		else{
			Task &= 0x00FF;
			Touch_Reset();
		}
	}	
	else if(Touch_s_R == 1)
	{
		Added_Mark = 1;
		Task += 0x1000;
		Touch_Reset();
	}
	
	Coffee_Added_last = Coffee_Added;
}
		
uint8_t Tea_pods_Added = 1;
uint8_t Tea_Added = 5;

void Task_Tea_Pods_Add_work(void)	//加茶包
{
	static uint8_t Tea_pods_Added_last;
				
	if(Task != Task_Last)
	{
		Encoder = Tea_pods_Added;
		Task_Tea_Add_Show(Tea_pods_Added, 0); // 豆量
	}
	
	Tea_pods_Added = Encoder;
				
	if(Tea_pods_Added > 25)
		Tea_pods_Added = Encoder = 25;
	else if(Tea_pods_Added < 1)
		Tea_pods_Added = Encoder = 1;
	
	if(Tea_pods_Added != Tea_pods_Added_last)
		Task_Tea_Add_Show(Tea_pods_Added, 0);
	
	Task_Last = Task;

	if(Touch_s_L == 1)
	{
		Task &= 0x00FF;
		Touch_Reset();
	}	
	else if(Touch_s_R == 1)
	{
		Added_Mark = 1;
		Task += 0x1000;
		Touch_Reset();
		Tea_Added = Tea_pods_Added * 2;
		Coffee_Added = Tea_Added ;
	}
	
	Tea_pods_Added_last = Tea_pods_Added;
}

void Task_Tea_Add_work(void)		//加茶叶
{
	static uint8_t Tea_Added_last;
				
	if(Task != Task_Last)
	{
		Encoder = Tea_Added;
		Task_Tea_Add_Show(Tea_Added, 1);
	}
	
	Tea_Added = Encoder;
				
	if(Tea_Added > 50)
		Tea_Added = Encoder = 50;
	else if(Tea_Added < 2)
		Tea_Added = Encoder = 2;
	
	if(Tea_Added != Tea_Added_last)
		Task_Tea_Add_Show(Tea_Added, 1);
	
	Task_Last = Task;

	if(Touch_s_L == 1)
	{
		Task &= 0x00FF;
		Touch_Reset();
	}	
	else if(Touch_s_R == 1)
	{
		Added_Mark = 1;
		Task += 0x1000;
		Touch_Reset();
	}
	
	Tea_Added_last = Tea_Added;
	Coffee_Added = Tea_Added ;
}

uint32_t Reservation_Time = 20;
uint8_t Reservation_flag = 0;

void Task_Summary_work(void)
{
	if(Task != Task_Last)
	{
		if(uesr_chooseMark){
			mode_Now = custom_Kind[selected_idx];
		}
		
		if(mode_Now.Heating_spd)
			mode_Now.Heating_Time = Coffee_Added * mode_Now.Heating_CWratio * 10 / mode_Now.Heating_spd;
		else
			mode_Now.Heating_Time = 0;
		if(mode_Now.Cooling_spd)
			mode_Now.Cooling_Time = Coffee_Added * mode_Now.Cooling_CWratio * 10 / mode_Now.Cooling_spd;
		else
			mode_Now.Cooling_Time = 0;
		
		mode_Now.Total_ml = Coffee_Added * (mode_Now.Heating_CWratio + mode_Now.Cooling_CWratio - 20) / 10;
		mode_Now.Total_Time = mode_Now.Heating_Time + mode_Now.Cooling_Time + 60;
		
		Task_0x1FFF_Show(mode_Now);
		Added_Mark = 0;
		
	}
				
	Task_Last = Task;

	if(Touch_s_L == 1)
	{
		if(uesr_chooseMark == 1)
		{
			uesr_chooseMark = 2;
			Task &= 0x0FFF;
		}
		else if(uesr_chooseMark == 2)
		{
			uesr_chooseMark = 0;
			mode_Now.mode_tag = 5;
			Task = 0x0015;
		}
		else
		{ 
			Task &= 0x0FFF;
		}
		Touch_Reset();
	}	
	else if(Touch_s_R == 1)
	{	
		//判断是否要保存自定义品种
		if(Save_flag == 1){
			Task += 0x1000;
		}
		else if(uesr_chooseMark == 2)
		{	
			uesr_chooseMark = 1;
			if(((mode_Now.Heating_CWratio + mode_Now.Cooling_CWratio) / 10) > 25)
			{
				mode_Now.mode_tag = 3;
				Task = 0x0113;  
			}
			else
			{
				Task = 0x0117; 
			} 
		}
		else
		{
			if(uesr_chooseMark == 1)
				uesr_chooseMark = 0;
			
//			Task += 0x2000;
			Task &= 0x0FFF;
			Task |= 0x3000;			
		}	
		Touch_Reset();
	}
	else if(Touch_l_R == 1)
	{	
		if(uesr_chooseMark == 2){
		Task = (Task & 0x0FFF) |  0x9000;
		}
		else{
		Task = (Task & 0x0FFF) | 0x4000;  // 保留低12位，强制高4位=0x5
		}
		Touch_Reset();
		Reservation_flag = 1;
	}
}

static uint8_t selected_option = 0;

//品种保存界面
void Task_Saved_work(void)
{
	static int16_t accumulated_encoder = 0; // 编码器累计值

	// 初始化检查
	if(Task != Task_Last) {
			accumulated_encoder = 0;
			Encoder = 0;
			selected_option = 1;
	}

	// 编码器处理（循环式选择）
	if(Encoder != 0) {
			accumulated_encoder += Encoder; // 累计编码器变化量
			Encoder = 0; // 清除增量
			// 每2个脉冲切换一个字符（可根据编码器灵敏度调整）
			if(abs(accumulated_encoder) >= 1) {
					selected_option = !selected_option;
					accumulated_encoder = 0; 
			}
	}
//	OLED_ColorTurn(1);
	Task_Saved_Show(selected_option);
							
	Task_Last = Task;

	if(Touch_s_L == 1)
	{							
		Task &= 0x0FFF;
		Touch_Reset();
	}	
	else if(Touch_s_R == 1)
	{	
		/*******4-20***************/
		if(selected_option == 0){
				//品种保存标志位清零
				Save_flag = 0;
				//品种数值清零
				//memset(custom_Kind[SaveMark], 0, sizeof(custom_Kind[SaveMark]));  // 全部清零
				// 清零操作
				//clear_coffee_mode(&custom_Kind[SaveMark]);
				if(uesr_EditMark == 1){
						mode_Now.name = custom_Kind[selected_idx].name;
						custom_Kind[selected_idx] = mode_Now;
						CoffeeFlash_ReplaceSafely(selected_idx, &custom_Kind[selected_idx]);
						uesr_EditMark = 0;
				}
				else if(SaveMark < 10){
							custom_Kind[SaveMark].name = malloc(11); 
							strcpy(custom_Kind[SaveMark].name, "None"); 
							//custom_Kind[SaveMark].name = NULL;
							// 指针设为NULL
							custom_Kind[SaveMark].title_1 = NULL;       // 字符串指针
							custom_Kind[SaveMark].title_2 = NULL;       // 字符串指针

							custom_Kind[SaveMark].Heating_Temp = 0;     // 温度值
							custom_Kind[SaveMark].Cooling_Temp = 0;     // 温度值
							custom_Kind[SaveMark].Heating_spd = 0;      // 速度值
							custom_Kind[SaveMark].Cooling_spd = 0;      // 速度值

							custom_Kind[SaveMark].Heating_Time = 0;     // 时间值
							custom_Kind[SaveMark].Cooling_Time = 0;    // 时间值

							custom_Kind[SaveMark].Heating_CWratio = 0;  // 比例值
							custom_Kind[SaveMark].Cooling_CWratio = 0;  // 比例值

							custom_Kind[SaveMark].Total_Time = 0;       // 总时间
							custom_Kind[SaveMark].Total_ml = 0;         // 总量
							custom_Kind[SaveMark].mode_tag = 0;         // 模式标记
				}
				 if(((mode_Now.Heating_CWratio + mode_Now.Cooling_CWratio) / 10) > 25){
							mode_Now.mode_tag = 3;
							Task = 0x0113;  
					}
					else{
							Task = 0x0117; 
					}
				//Task = (Task & 0x0FFF) | 0x3000;  // 保留低12位，强制高4位=0x3
				Touch_Reset();
		}
		else if(selected_option == 1){
//				if(SaveMark < 10){
//							//种类保存
//					custom_Kind[SaveMark] = mode_Now;
//					Task = (Task & 0x0FFF) | 0x5000;  // 保留低12位，强制高4位=0x5
//					Touch_Reset();
//				}
//				//切换到删除任务
//				else{
//					if(uesr_EditMark == 1){
//						Task = (Task & 0x0FFF) | 0x5000;  // 保留低12位，强制高4位=0x5
//					}
//					else{
//						Task = (Task & 0x0FFF) | 0x7000;  // 保留低12位，强制高4位=0x7
//					}
//					Touch_Reset();
//				}
					if(uesr_EditMark == 1){
							Task = (Task & 0x0FFF) | 0x5000;  // 保留低12位，强制高4位=0x5
					}
					else if(SaveMark < 10){
							//种类保存
							custom_Kind[SaveMark] = mode_Now;
							Task = (Task & 0x0FFF) | 0x5000;  // 保留低12位，强制高4位=0x5
							Touch_Reset();
					}
					else{
							Task = (Task & 0x0FFF) | 0x7000;  // 保留低12位，强制高4位=0x7
					}
					Touch_Reset();
		}
		/*******4-20***************/
	}
}

//单个品种名数组
char save_name[11] = {0};
int current_char = 0;
uint8_t name_length = 0;

void Task_Named_work(void)        // 品种命名界面
{
		const char char_set[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>+-/!?.:_=";
    const uint8_t char_set_size = sizeof(char_set) - 1; // 字符集长度（去掉结尾的\0）
    static int16_t accumulated_encoder = 0; // 编码器累计值
	
    // 初始化检查
    if(Task != Task_Last) {
//				OLED_ClearArea();
        current_char = 0; // 重置当前字符索引
        accumulated_encoder = 0;
        Task_Named_Show(0,0); // 立即刷新显示
    }
		Task_Named_Show(Progress_bar_tag,Progress_bar); // 立即刷新显示
		

		
		if(Progress_bar >= 159)
		{
			Task = (Task & 0x0FFF) | 0x6000;  // 保留低12位，强制高4位=0x6
			Progress_bar_tag = 0;
			Touch_Reset();
			Time_Reset();
		}
		
		
    if(Encoder != 0) {
        accumulated_encoder += Encoder; // 累计编码器变化量
        Encoder = 0; // 清除增量
        
        // 每1个脉冲切换一个字符（可根据编码器灵敏度调整）
        if(abs(accumulated_encoder) >= 1) {
						//last_printed_step = current_char;
            current_char += (accumulated_encoder > 0) ? 1 : -1;
            accumulated_encoder = 0;
					
            // 循环处理
            if(current_char >= char_set_size) current_char = 0;
					//bug:从A到末尾时current_char从0直接变成55并且定住，A也不动
           else if(current_char < 0) current_char = char_set_size - 1;
					   
            Task_Named_Show(0,0); // 立即刷新显示
        }
    }

    // 触摸按键处理
    if(Touch_s_L == 1) { 
				// 删除一个字符
				if(name_length > 0) {
						save_name[--name_length] = '\0';
						Task_Named_Show(0,0); // 立即刷新显示
				}			
        Touch_Reset();
    }  
    else if(Touch_s_R == 1) {    
        // 确认选择当前字符
        if(name_length < 10) {
            save_name[name_length++] = char_set[current_char];
            save_name[name_length] = '\0';
						current_char = 0;
            Task_Named_Show(0,0); // 立即刷新显示
        }
        Touch_Reset();
				
    }
//    else if(Touch_l_R == 1) {    
//				Task = (Task & 0x0FFF) | 0x6000;  // 保留低12位，强制高4位=0x6
//        //Task = 0x6000; // 进入完成界面
//        Touch_Reset();
//    }
		else if(Touch_l_L == 1){
					//memset(save_name, 0, sizeof(save_name)); // 清空存储的名字
						name_length = 0;
						Task = (Task & 0x0FFF) | 0x2000;  // 保留低12位，强制高4位=0x2
						//Task = 0x2000; // 返回上级菜单
						Touch_Reset();
		}
		else{
			if(Touch_flag_R == 1 && Progress_bar < 159){
				if(Touch_l_R == 1 && name_length != 0)Progress_bar_tag = 1;
				
			}
			else
			{
				Progress_bar_tag = 0;
			
				Touch_Reset();
			}	
		
		}
				Task_Last = Task;
}


void Task_NameOver_work(void)		//命名成功界面
{
	
	//品种保存标志位清零
	Save_flag = 0;
	Task_NameOver_Show();
	// 初始化函数，检查Flash中已有多少数据
	//种类保存
	//如果替换标志位触发
	if(replace_index >=0 && replace_index <=9){	
			custom_Kind[replace_index] = mode_Now;
			custom_Kind[replace_index].name = malloc(11); 
			strcpy(custom_Kind[replace_index].name, save_name); 
			//CoffeeMode_Copy(&custom_Kind[replace_index], &mode_Now);
			CoffeeFlash_ReplaceSafely(replace_index, &custom_Kind[replace_index]);
			
			Encoder = selected_idx = replace_index;
		
			replace_index = 0xFF;
	}
//	else{
//				//种类保存
//		if(SaveMark>=0 && SaveMark <=9){
//				custom_Kind[SaveMark] = mode_Now;
//				custom_Kind[SaveMark].name = malloc(11); 
//				strcpy(custom_Kind[SaveMark].name, save_name); 
//				//CoffeeMode_Copy(&custom_Kind[SaveMark], &mode_Now);
//				CoffeeFlash_Append(&custom_Kind[SaveMark]);
//				//Flash替换
//				
//				//Encoder = selected_idx = SaveMark;
//			
//				SaveMark++;
//		}
//	}
	
	else if(uesr_EditMark == 1){
		mode_Now.name = malloc(11); 
		strcpy(mode_Now.name, save_name); 
		custom_Kind[selected_idx] = mode_Now;
		CoffeeFlash_ReplaceSafely(selected_idx, &custom_Kind[selected_idx]);
		uesr_EditMark = 0;
	}
	else if(SaveMark>=0 && SaveMark <=9){
				custom_Kind[SaveMark] = mode_Now;
				custom_Kind[SaveMark].name = malloc(11); 
				strcpy(custom_Kind[SaveMark].name, save_name); 
				//CoffeeMode_Copy(&custom_Kind[SaveMark], &mode_Now);
				CoffeeFlash_Append(&custom_Kind[SaveMark]);
				//Flash替换
				
				Encoder = selected_idx = SaveMark;
			
				SaveMark++;
	}
	memset(save_name, 0, sizeof(save_name));  // 全部清零
	name_length = 0;
	current_char = 0;

	Task_Last = Task;						
	
	HAL_Delay(2000); // 延迟2000毫秒(2秒)
	Touch_Reset();
	//Task_ExecuteWay_work();
	mode_Now.mode_tag = 5;
	Task = 0x0015;
}

static uint8_t Status_Encoder = 0;

void Task_ExecuteWay_work(void)		//存储后执行选择界面
{

	static int16_t accumulated_encoder = 0; // 编码器累计值

	// 初始化检查
	if(Task != Task_Last) {
			accumulated_encoder = 0;
			Encoder = 0;
			Status_Encoder = 1;
	}

	// 编码器处理（循环式选择）
	if(Encoder != 0) {
			accumulated_encoder += Encoder; // 累计编码器变化量
			Encoder = 0; // 清除增量
			// 每2个脉冲切换一个字符（可根据编码器灵敏度调整）
			if(abs(accumulated_encoder) >= 1) {
					Status_Encoder = !Status_Encoder;
					accumulated_encoder = 0; 
			}
	}
	
//	if(Task != Task_Last) {
//			Status_Encoder = Encoder = 1;
//	}
//	
//	Status_Encoder = Encoder;
//	
//	if(Status_Encoder > 1)
//		Status_Encoder = Encoder = 0;
//	else if(Status_Encoder < 0)
//		Status_Encoder = Encoder = 1;
	
	
	Task_ExecuteWay_Show(Status_Encoder);
							
	Task_Last = Task;

	if(Touch_s_L == 1)
	{							
		mode_Now.mode_tag = 4;
		Task = 0x0014;
		Touch_Reset();
	}	
	else if(Touch_s_R == 1)
	{	
		if(Status_Encoder == 0){
				//切换到选择任务
					mode_Now.mode_tag = 5;
					Task = 0x0015;
					//Task = (Task & 0x0FFF) | 0xA000;  // 保留低12位，强制高4位=0x7
					Touch_Reset();
		}
		else if(Status_Encoder == 1){
//					uesr_EditMark = 1;
				//切换到执行
					Task = (Task & 0x0FFF) | 0x3000;  // 保留低12位，强制高4位=0x7
					Touch_Reset();
		}
	}
}

Note hint_tone[] = {
    {1000, 150},
    {0, 100},
    {1200, 150}
};

// 播放旋律序列
void BUZZER_Play_Melody(Note* melody, uint8_t count);

void Task_Ready_work(void)
{
	static uint8_t hasTriggered = 0;
	if(Error_rx)
	{
		ERROR_Show(Error_rx);
		if(!SoundMark)
		{
			if(hasTriggered < 3)  // 只在第一次进入时执行
			{
				BUZZER_Play_Melody(hint_tone, sizeof(hint_tone) / sizeof(Note));
				hasTriggered++;  // 标记为已触发
			}
		}	
	}
	else
	{
		hasTriggered = 0;
		if(Task != Task_Last)
		{
			Progress_bar_tag = 1;
			Task_0x2FFF_Show(0, 0);
		}
		
		Task_Last = Task;
		
		Progress_bar_tag = 1;
		if(Progress_bar > 159)
		{
			Task |= 0xFFFF;
			Progress_bar_tag = 0;
		}		
		
		Task_0x2FFF_Show(1, Progress_bar);
	}
					
	if(Touch_s_L == 1)
	{							
		Task &= 0x0FFF;
		Touch_Reset();
		Progress_bar_tag = 0;
		
		temp_switch = 0;
		hasTriggered = 0;
	}	
	else if(Touch_s_R == 1)
	{
	}
}

void Task_Reservation_work(void)
{
	if(Task != Task_Last)
	{
		Encoder = 20;
		Reservation_Time = 300;
		Task_0x3FFF_Show(0, Reservation_Time);
	}
	Reservation_Time = Encoder * 15;
	
	Task_0x3FFF_Show(1, Reservation_Time);
	
	Task_Last = Task;
	
	if(Touch_s_L == 1)
	{
		Reservation_flag = 0;
		
		Task &= 0x0FFF;
		Touch_Reset();
	}	
	else if(Touch_s_R == 1)
	{
		Task -= 0x1000;
		Touch_Reset();
	}
}

extern int Touch_flag_L;

uint8_t State_Now = 0, State_Last = 0;

void Task_Cancel_work(void)
{
	if(Task != Task_Last)
		Task_0xEFFF_Show(0, 0);

	Task_0xEFFF_Show(1, Progress_bar);

	Task_Last = Task;

	if(Progress_bar >= 159)
	{
		Task &= 0x000F;
		Progress_bar_tag = 0;
		Touch_Reset();
		State_Now = State_Last = 0;
		Reservation_flag = 0;
		Time_Reset();
	}	

	if(Touch_s_R == 1)
	{
		Task += 0x1000;
		Touch_Reset();
	}
	else
	{
		if(Touch_flag_L == 1 && Progress_bar < 159)
			Progress_bar_tag = 1;
		else
		{
			Progress_bar_tag = 0;
		
			Touch_Reset();
		}
	}
}

uint8_t Pause, finish_tag;
uint8_t beep_Run;

uint8_t error_First_Enter;

Note melody[] = {
    {520, 208},
    {650, 185},
    {780, 220},
    {990, 185},
    {1080, 11},
};

// 自动生成的旋律数据（高保真提取）
//Note melody[] = {
//    {520, 203},
//    {780, 185},
//    {1550, 208},
//    {1380, 238},
//    {1030, 946},
//    {1280, 226},
//    {1120, 190},
//    {1380, 263},
//    {1030, 173},
//    {880, 203},
//    {990, 195},
//    {890, 208},
//    {830, 203},
//    {780, 208},
//    {650, 215},
//    {520, 238}
//};

//Note hint_tone[] = {
//    {1000, 150},
//    {0, 100},
//    {1200, 150}
//};

// 播放旋律序列
void BUZZER_Play_Melody(Note* melody, uint8_t count)
{
    for (uint8_t i = 0; i < count; i++) {
        BUZZER_Play_Tone(melody[i].freq, melody[i].duration_ms);
    }
		beep_Run = 0;
}

uint8_t TEC_Working_tag, TEC_Working_time;//TEC_Error_tag, TEC_Speed_Error_tag;

void Task_Execute_work(void)
{
	// 使用静态变量记录是否已经触发过
	static uint8_t hasTriggered = 0;
	
	uint8_t size_val = (languageMark == 1) ? 16 : 24;
	if(Error_rx)
	{
		ERROR_Show(Error_rx);
	
		if(!SoundMark)
		{
			if(hasTriggered < 3)  // 只在第一次进入时执行
			{
				BUZZER_Play_Melody(hint_tone, sizeof(hint_tone) / sizeof(Note));
				hasTriggered++;  // 标记为已触发
			}
		}
		
		temp_switch = 0;
		pump_set = 0;
			
		time_start_tag = 0;
		
		Pause = 1;
		TEC_Working_tag = 0;
	}
	else
	{
		hasTriggered = 0;  // 重置触发状态
		static uint32_t Reservation_Second;
		static uint8_t Pause_last;

		if(Task != Task_Last)
		{
			Reservation_Second = Reservation_Time * 60;
			
			if(State_Last)
			{
				State_Now = State_Last;
			}
			else
			{
				if(Reservation_flag)
					State_Now = 0;
				else
					State_Now  = 1;
			}
			
			Task_0xFFFF_Show(State_Now, mode_Now);
		}
		
		if(Pause)
		{
			if(Pause != Pause_last)
				Task_0xFFFF_Show(State_Now, mode_Now);
			if(State_Now == 2 || State_Now == 4)
			{
				OLED_ClearArea();
				Gui_StrCenter(0, 52,	WHITE, BLACK, "PREPARING",	size_val, 1,languageMark);
				Temp_Show(Temp_0, 0);
			}
		}
		else
		{	
			if(State_Now == 1 || State_Now == 3)
				Temp_Show(Temp_0, 1);
			else if(State_Now == 2 || State_Now == 4)
				Temp_Show(Temp_0, 0);
			
			if(Pause_last != Pause)
				Task_0xFFFF_Show(State_Now, mode_Now);
		}
		
		Pause_last = Pause;
		
		Task_Last = Task;
		
		State_Now = Task_0xFFFF_Work(State_Now,Reservation_Second, mode_Now);
										
		if(State_Now == 5)
		{
			if(finish_tag)
			{
				if(Count_Down_unShow(6))
				{
					pump_set = 0;
					time_start_tag = 0;
					//Second = 0;
					beep_Run = 1;
					State_Now = 6;
				}
				else
				{
					pump_flag = 2;
					pump_set = 50;
				}
			}
			else
			{
				finish_tag = 1;
				time_start_tag = 1;
			}	
		
			if(Touch_s_R == 1)
			{
				finish_tag = 0;
				pump_set = 0;
				time_start_tag = 0;
				Time_Reset();
				
				Task &= 0x000F;
				
				State_Now = State_Last = 0;
					
				Touch_Reset();
				
				Reservation_flag = 0; 
			}
		}
		
		if(State_Now == 6)
		{
			if(Count_Down_unShow(114))
			{
				finish_tag = 0;
				pump_set = 0;
				time_start_tag = 0;
				Time_Reset();
				
				Task &= 0x000F;
				
				State_Now = State_Last = 0;
					
				Touch_Reset();
				
				Reservation_flag = 0; 
			}
			else
			{
				time_start_tag = 1;
			}				
			
			if(!SoundMark)
			{
				if(beep_Run)
				{
					BUZZER_Play_Melody(melody, sizeof(melody) / sizeof(Note));
				}
			}
			
			if(Touch_s_R == 1)
			{
				finish_tag = 0;
				pump_set = 0;
				time_start_tag = 0;
				Time_Reset();
				
				Task &= 0x000F;
				
				State_Now = State_Last = 0;
					
				Touch_Reset();
				
				Reservation_flag = 0; 
			}
		}
		
		if(State_Now != State_Last)
			Task_0xFFFF_Show(State_Now, mode_Now);
		
		State_Last = State_Now;
	}
							
	if(Touch_s_L == 1)
	{
		if(State_Now == 5 || State_Now == 6)
		{
			Task &= 0x000F;
			
			Touch_Reset();

			State_Now = State_Last = 0;
			Reservation_flag = 0;

			temp_switch = 0;
			pump_set = 0;
			finish_tag = 0;
		}
		else
		{
			if(Error_rx)
			{
				Task &= 0x0FFF;
				Touch_Reset();
				State_Now = State_Last = 0;
				Reservation_flag = 0;
				hasTriggered = 0;
			}
			else
			{
				temp_switch = 0;
				pump_set = 0;
				
				time_start_tag = 0;
				
				Task -= 0x1000;
				Touch_Reset();
				
				Pause = 1;
				
				TEC_Working_tag = 0;
			}
		}
	}
}

extern uint8_t Pause;

uint8_t TEC_Error;
uint8_t Temp_delay = 100;

uint8_t Task_0xFFFF_Work(uint8_t Work_state, uint32_t Reservation_Second, Coffee_mode mode_now) //执行界面
{
	switch(Work_state)
	{
		case 0:
			if(Count_Down_Show(Reservation_Second))// 延时-倒计时
			{
				Work_state++;
				time_start_tag = 0;
				Time_Reset();
				TEC_Working_tag = 0;
			}
			else
			{
				time_start_tag = 1;
			}
			break;
			
		case 1:
			if(Pause)
			{
				Pause = 0;
				TEC_Working_tag = 0;
//				TEC_Error_tag = 0;
//				TEC_Speed_Error_tag = 0;
			}
		
			if(mode_now.Heating_Temp > Custom_Setting[1].min && mode_now.Heating_CWratio)
			{
				temp_set = mode_now.Heating_Temp;	// Heating
			
				temp_switch = 2;
				
				if(Temp_0 >= mode_now.Heating_Temp || TEC_Working_time > Temp_delay)
				{
					Work_state++;
					time_start_tag = 0;
					Time_Reset();
					
					TEC_Working_tag = 0;
				}
				else
				{
					if(Count_Down_unShow(8))
					{
						pump_set = 0;
						time_start_tag = 0;
					}
					else
					{
						pump_set = 80;
						time_start_tag = 1;
					}
					
					TEC_Working_tag = 1;
					
//					if(TEC_Error_tag > 30 || TEC_Speed_Error_tag > 5)
//						TEC_Error = 1;
				}
			}
			else
			{
				if(mode_now.Heating_CWratio)
					Work_state ++;
				else
					Work_state += 2;
				
				temp_switch = 0;				
			}
							
			break;
		case 2:
			if(Pause)
			{	
				if(mode_now.Heating_Temp > Custom_Setting[1].min)
				{
					temp_switch = 2;
					temp_set = mode_now.Heating_Temp;
					TEC_Working_tag = 1;					
				}
				
				if(Temp_0 >= mode_now.Heating_Temp || TEC_Working_time > Temp_delay)
				{
					Pause = 0;
					TEC_Working_tag = 0;
				}
			}
			else
			{
				pump_set = mode_now.Heating_spd;
				
				if(Count_Down_Show(mode_now.Heating_Time))		//blooming
				{
					pump_set = 0;
					temp_switch = 0;
					
					Work_state++;
					time_start_tag = 0;
					Time_Reset();
				}
				else
				{
					time_start_tag = 1;
				}
			}
		
			break;
		case 3:
			if(Pause)
			{
				Pause = 0;
				TEC_Working_tag = 0;
//				TEC_Error_tag = 0;
//				TEC_Speed_Error_tag = 0;
			}
			
			if(mode_now.Cooling_Temp < Custom_Setting[3].max)
			{
				temp_set = mode_now.Cooling_Temp;
				temp_switch = 1;
				
				if(Temp_0 <= mode_now.Cooling_Temp || TEC_Working_time > Temp_delay) // Cooling
				{
					Work_state++;
					
					TEC_Working_tag = 0;
				}
				else
				{
					TEC_Working_tag = 1;
					
//					if(TEC_Error_tag > 60 || TEC_Speed_Error_tag > 5)
//						TEC_Error = 1;
				}
			}
			else
			{
				Work_state ++;
				temp_switch = 0;
			}
			break;
		case 4:
			if(Pause)
			{
				if(mode_now.Cooling_Temp < Custom_Setting[3].max)
				{
					temp_switch = 1;
					temp_set = mode_now.Cooling_Temp;
					TEC_Working_tag = 1;
				}
				
				if(Temp_0 <= mode_now.Cooling_Temp || TEC_Working_time > Temp_delay)
				{
					Pause = 0;
					TEC_Working_tag = 0;
				}
			}
			else
			{
				pump_set = mode_now.Cooling_spd;
				
				if(Count_Down_Show(mode_now.Cooling_Time))//Cooling Working
				{
					pump_set = 0;
					temp_switch = 0;
					
					Work_state++;
					time_start_tag = 0;
					Time_Reset();
					
					TEC_Working_tag = 0;
				}
				else
				{
					time_start_tag = 1;
				}
			}
			
			break;
		case 5:
			if(Pause)
				Pause = 0;
			temp_switch = 0;
			break;	
		
		default:
			break;
	}
	
	return Work_state;
}
void Custom_Data_Reset(void)	//自定义数据重置
{
	Custom_Setting[0].data_default = mode_Now.Heating_CWratio;
	Custom_Setting[1].data_default = mode_Now.Heating_Temp;
	Custom_Setting[2].data_default = mode_Now.Heating_spd;
	Custom_Setting[3].data_default = mode_Now.Cooling_Temp;
	Custom_Setting[4].data_default = mode_Now.Cooling_spd;
	Custom_Setting[5].data_default = mode_Now.Heating_CWratio + mode_Now.Cooling_CWratio;
}


uint16_t data_custom;
uint16_t Custom_Data_Process(uint8_t state_now, uint8_t state_last) //自定义数据处理
{
	
	
	if(state_last != state_now)
	{
		Custom_Data_Reset();
		Encoder = Custom_Setting[state_now].data_default * 10 / Custom_Setting[state_now].Resolution;
	}
	
	if(Encoder<0)
		Encoder = 0;
	
		data_custom = Encoder * Custom_Setting[state_now].Resolution / 10;
	
	if((Encoder * Custom_Setting[state_now].Resolution / 10) > 250){
		data_custom = 50* Custom_Setting[state_now].Resolution / 10 + (Encoder-50 )* Custom_Setting[state_now].Resolution;
	}
	else{
		data_custom = Encoder * Custom_Setting[state_now].Resolution / 10;
	}
	
//		if(data_custom > 250){
//			
//			data_custom = 50* Custom_Setting[state_now].Resolution / 10 + (Encoder-50 )* Custom_Setting[state_now].Resolution;
//		}
//		else if(data_custom < 250){
//			Custom_Setting[5].Resolution = 50;
//		}
	
	if(data_custom > Custom_Setting[state_now].max)
	{
		data_custom = Custom_Setting[state_now].max;
		if(state_now == 5)
				// 反向计算 Encoder
				Encoder = (data_custom / Custom_Setting[state_now].Resolution) + 45;
		else
				Encoder = Custom_Setting[state_now].max * 10 / Custom_Setting[state_now].Resolution;
	}
	else if(data_custom > 250 && data_custom < Custom_Setting[state_now].max){
		// 反向计算 Encoder
    Encoder = (data_custom / Custom_Setting[state_now].Resolution) + 45;
	}
	else if(data_custom < Custom_Setting[state_now].min)
	{
		data_custom = Custom_Setting[state_now].min;
		Encoder = Custom_Setting[state_now].min * 10 / Custom_Setting[state_now].Resolution;
	}	
	
	
	return data_custom;
}



void LED_GPIO_Set(uint8_t LED_L, uint8_t LED_R)
{
	if(LED_L)	LED_T1_ENABLE;
	else		LED_T1_DISABLE;
	
	if(LED_R)	LED_T2_ENABLE;
	else		LED_T2_DISABLE;
}


extern uint8_t setting_led_close;
void LED_Set(uint8_t sleep)
{
	if(sleep)
	{
		LED_GPIO_Set(0, 0);
	}
	else
	{
		
		if((Task & 0xFFF0) == 0)
		{
			if(pump_reset)
				LED_GPIO_Set(0, 0);
			else
				LED_GPIO_Set(0, 1);

		}
		else if((Task & 0xFF00) == 0)
		{
			if(setting_led_close)
			{
				LED_GPIO_Set(1, 0);
			}
			else
			{
				LED_GPIO_Set(1, 1);
			}
		}
		else if((Task & 0xF000) == 0)
		{
			LED_GPIO_Set(1, 1);
		}
		else
		{
			if((Task & 0xF000) == 0x2000)
			{
				LED_GPIO_Set(1, 1);
			}
			else if((Task & 0xF000) == 0x3000)
			{
				LED_GPIO_Set(1, 0);
			}
			else if((Task & 0xF000) == 0xE000)
			{
				LED_GPIO_Set(1, 1);
			}
			else if((Task & 0xF000) == 0xF000)
			{
				if(finish_tag)
				{
					LED_GPIO_Set(0, 1);
				}
				else
				{
					LED_GPIO_Set(1, 0);
				}
			}
			else
			{
				LED_GPIO_Set(1, 1);
			}
		}
	}
}

