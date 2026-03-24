#include "Init.h"
#include "bmp.h"

lpf_1rd_t lpf1rd_0,lpf1rd_1;

PidObject Temp_pid;

//Coffee_mode Default_modes[] = {
//    {
//        .name = "REGULAR",
//        .title_1 = "DARK ROAST",
//        .title_2 = "",
//        .Heating_Temp = 50,
//        .Cooling_Temp = 5,
//        .Heating_spd = 40,
//        .Cooling_spd = 30,
//        .Heating_CWratio = 15,
//        .Cooling_CWratio = 110,
//        .mode_tag = 0
//    },
//    {
//        .name = "REGULAR",
//        .title_1 = "MEDIUM ROAST",
//        .title_2 = "",
//        .Heating_Temp = 55,
//        .Cooling_Temp = 5,
//        .Heating_spd = 45,
//        .Cooling_spd = 30,
//        .Heating_CWratio = 15,
//        .Cooling_CWratio = 110,
//        .mode_tag = 1
//    },
//    {
//        .name = "REGULAR",
//        .title_1 = "LIGHT ROAST",
//        .title_2 = "",
//        .Heating_Temp = 60,
//        .Cooling_Temp = 5,
//        .Heating_spd = 45,
//        .Cooling_spd = 30,
//        .Heating_CWratio = 15,
//        .Cooling_CWratio = 110,
//        .mode_tag = 2
//    },
//    {
//        .name = "CONCENTRATE",
//        .title_1 = "DARK ROAST",
//        .title_2 = "",
//        .Heating_Temp = 50,
//        .Cooling_Temp = 5,
//        .Heating_spd = 45,
//        .Cooling_spd = 30,
//        .Heating_CWratio = 15,
//        .Cooling_CWratio = 40,
//        .mode_tag = 3
//    },
//    {
//        .name = "CONCENTRATE",
//        .title_1 = "MEDIUM ROAST",
//        .title_2 = "",
//        .Heating_Temp = 55,
//        .Cooling_Temp = 5,
//        .Heating_spd = 45,
//        .Cooling_spd = 30,
//        .Heating_CWratio = 15,
//        .Cooling_CWratio = 40,
//        .mode_tag = 4
//    },
//    {
//        .name = "CONCENTRATE",
//        .title_1 = "LIGHT ROAST",
//        .title_2 = "",
//        .Heating_Temp = 60,
//        .Cooling_Temp = 5,
//        .Heating_spd = 45,
//        .Cooling_spd = 30,
//        .Heating_CWratio = 15,
//        .Cooling_CWratio = 40,
//        .mode_tag = 5
//    },
//    {
//        .name = "SLOW",
//        .title_1 = "DRIP",
//        .title_2 = " ",
//        .Heating_Temp = 40,
//        .Cooling_Temp = 5,
//        .Heating_spd = 45,
//        .Cooling_spd = 5,
//        .Heating_CWratio = 15,
//        .Cooling_CWratio = 105,
//        .mode_tag = 6
//    },
//    // 初始化 Cus_0 到 Cus_9
//	{
//        .name = "CUSTOM",
//        .title_1 = "BREW",
//        .title_2 = "",
//        .Heating_Temp = 55,
//        .Cooling_Temp = 5,
//        .Heating_spd = 55,
//        .Cooling_spd = 35,
//        .Heating_CWratio = 20,
//        .Cooling_CWratio = 100,
//        .mode_tag = 7
//    },
//	//Setting
//    {
//        .name = "SETTING",
//        .title_1 = "",
//        .title_2 = "",
//        .Heating_Temp = 50,
//        .Cooling_Temp = 5,
//        .Heating_spd = 55,
//        .Cooling_spd = 35,
//        .Heating_CWratio = 15,
//        .Cooling_CWratio = 110,
//        .mode_tag = 8
//    },
//						//Choose
//    {
//        .name = "SAVED",
//        .title_1 = "PROFILE",
//        .title_2 = "",
//        .Heating_Temp = 55,
//        .Cooling_Temp = 5,
//        .Heating_spd = 55,
//        .Cooling_spd = 35,
//        .Heating_CWratio = 20,
//        .Cooling_CWratio = 100,
//        .mode_tag = 9
//    },
//};

Coffee_mode Default_modes[] = {
    {
        .name = "REGULAR",
        .title_1 = "",
        .title_2 = "",
        .Heating_Temp = 50,
        .Cooling_Temp = 5,
        .Heating_spd = 40,
        .Cooling_spd = 30,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 110,
        .mode_tag = 0
    },
    {
        .name = "CONCENTRATE",
        .title_1 = "",
        .title_2 = "",
        .Heating_Temp = 50,
        .Cooling_Temp = 5,
        .Heating_spd = 45,
        .Cooling_spd = 30,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 40,
        .mode_tag = 1
    },
    {
        .name = "SLOW",
        .title_1 = "DRIP",
        .title_2 = " ",
        .Heating_Temp = 40,
        .Cooling_Temp = 5,
        .Heating_spd = 45,
        .Cooling_spd = 5,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 105,
        .mode_tag = 2
    },
		{
        .name = "TEA",
        .title_1 = "",
        .title_2 = "",
        .Heating_Temp = 70,
        .Cooling_Temp = 5,
        .Heating_spd = 45,
        .Cooling_spd = 30,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 110,
        .mode_tag = 3
    },
    // 初始化 Cus_0 到 Cus_9
	{
        .name = "CUSTOM",
        .title_1 = "BREW",
        .title_2 = "",
        .Heating_Temp = 55,
        .Cooling_Temp = 5,
        .Heating_spd = 40,
        .Cooling_spd = 30,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 105,
        .mode_tag = 4
    },
		//Choose
    {
        .name = "SAVED",
        .title_1 = "PROFILE",
        .title_2 = "",
        .Heating_Temp = 55,
        .Cooling_Temp = 5,
        .Heating_spd = 55,
        .Cooling_spd = 35,
        .Heating_CWratio = 20,
        .Cooling_CWratio = 100,
        .mode_tag = 5
    },
	//Setting
    {
        .name = "SETTING",
        .title_1 = "",
        .title_2 = "",
        .Heating_Temp = 50,
        .Cooling_Temp = 5,
        .Heating_spd = 55,
        .Cooling_spd = 35,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 110,
        .mode_tag = 6
    },
};

Coffee_mode Default_modes_Regular[] = {	
    {
        .name = "DARK",
        .title_1 = "ROAST",
        .title_2 = "",
        .Heating_Temp = 50,
        .Cooling_Temp = 5,
        .Heating_spd = 40,
        .Cooling_spd = 30,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 110,
        .mode_tag = 0
    },
    {
        .name = "MEDIUM",
        .title_1 = "ROAST",
        .title_2 = "",
        .Heating_Temp = 55,
        .Cooling_Temp = 5,
        .Heating_spd = 40,
        .Cooling_spd = 30,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 110,
        .mode_tag = 1
    },
    {
        .name = "LIGHT",
        .title_1 = "ROAST",
        .title_2 = "",
        .Heating_Temp = 65,
        .Cooling_Temp = 5,
        .Heating_spd = 40,
        .Cooling_spd = 30,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 110,
        .mode_tag = 2
    },
};

Coffee_mode Default_modes_Concentrate[] = {	
    {
        .name = "DARK",
        .title_1 = "ROAST",
        .title_2 = "",
        .Heating_Temp = 50,
        .Cooling_Temp = 5,
        .Heating_spd = 40,
        .Cooling_spd = 30,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 40,
        .mode_tag = 0
    },
    {
        .name = "MEDIUM",
        .title_1 = "ROAST",
        .title_2 = "",
        .Heating_Temp = 55,
        .Cooling_Temp = 5,
        .Heating_spd = 35,
        .Cooling_spd = 25,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 40,
        .mode_tag = 1
    },
    {
        .name = "LIGHT",
        .title_1 = "ROAST",
        .title_2 = "",
        .Heating_Temp = 65,
        .Cooling_Temp = 5,
        .Heating_spd = 35,
        .Cooling_spd = 25,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 40,
        .mode_tag = 2
    },
};

Coffee_mode Default_modes_SlowDrip[] = {	
    {
        .name = "DARK",
        .title_1 = "ROAST",
        .title_2 = "",
        .Heating_Temp = 30,
        .Cooling_Temp = 5,
        .Heating_spd = 40,
        .Cooling_spd = 5,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 85,
        .mode_tag = 0
    },
    {
        .name = "MEDIUM",
        .title_1 = "ROAST",
        .title_2 = "",
        .Heating_Temp = 35,
        .Cooling_Temp = 5,
        .Heating_spd = 40,
        .Cooling_spd = 5,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 85,
        .mode_tag = 1
    },
    {
        .name = "LIGHT",
        .title_1 = "ROAST",
        .title_2 = "",
        .Heating_Temp = 40,
        .Cooling_Temp = 5,
        .Heating_spd = 40,
        .Cooling_spd = 5,
        .Heating_CWratio = 15,
        .Cooling_CWratio = 85,
        .mode_tag = 2
    },
};

Coffee_mode Tea_Default_modes[] =
{
    {
        .name = "GREEN TEA",
        .title_1 = "",
        .title_2 = "",
        .Heating_Temp = 70,
        .Cooling_Temp = 5,
        .Heating_spd = 30,
        .Cooling_spd = 30,
        .Heating_CWratio = 60,
        .Cooling_CWratio = 800,
        .mode_tag = 3
    },
    {
        .name = "WHITE TEA",
        .title_1 = "",
        .title_2 = "",
        .Heating_Temp = 65,
        .Cooling_Temp = 5,
        .Heating_spd = 30,
        .Cooling_spd = 30,
        .Heating_CWratio = 60,
        .Cooling_CWratio = 800,
        .mode_tag = 3
    },
    {
        .name = "OOLONG",
        .title_1 = "",
        .title_2 = "",
        .Heating_Temp = 70,
        .Cooling_Temp = 5,
        .Heating_spd = 30,
        .Cooling_spd = 30,
        .Heating_CWratio = 80,
        .Cooling_CWratio = 800,
        .mode_tag = 3
    },
    {
        .name = "BLACK TEA",
        .title_1 = "",
        .title_2 = "",
        .Heating_Temp = 70,
        .Cooling_Temp = 5,
        .Heating_spd = 30,
        .Cooling_spd = 30,
        .Heating_CWratio = 90,
        .Cooling_CWratio = 800,
        .mode_tag = 3
    },
    {
        .name = "HERBAL",
        .title_1 = "",
        .title_2 = "",
        .Heating_Temp = 70,
        .Cooling_Temp = 5,
        .Heating_spd = 30,
        .Cooling_spd = 30,
        .Heating_CWratio = 100,
        .Cooling_CWratio = 800,
        .mode_tag = 3
    },
};

Data_set Custom_Setting[] = {
	{
		.data_default = 20,
        .max = 40,// BloomRatio
        .min = 0,
        .Resolution = 50 //放大10倍
    },
	{
		.data_default = 55,
        .max = 70,// Heating_temp_custom
        .min = 34,
        .Resolution = 10
	},
	{
		.data_default = 40,
		.max = 70,// Heating_spd_custom
		.min = 5,
		.Resolution = 50
    },
    {
		.data_default = 5,
        .max = 16,// Cooling_temp_custom
        .min = 5,
        .Resolution = 10
	},
    {
		.data_default = 25,
        .max = 40,// Cooling_spd_custom
        .min = 5,
        .Resolution = 50
	},
	{
		.data_default = 120,
		.max = 900,// CWratio_custom
		.min = 50,
		.Resolution = 50
	}
};

void Drivers_Init(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
	delay_init(72);                         /* 延时初始化 */
	
	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
	
	OLED_ShowPicture(30,52,100,24,BMP1,1);
	OLED_Refresh();

	delay_ms(4000);
	
	TIM4_Init(72 - 1, 1000 - 1);
		
	usart_init(115200);
		
	Touch_Init();
			
	BUZZER_Init(100 - 1);
	
	VIBRAT_MOTOR_Init();	
	VIBRAT_MOTOR_Set(0);

	ENCODER_Init();	
	
	lpf1rd_0.tc = 0.5;
}

float lpf_1rd_calc(lpf_1rd_t *lpf1rd, int32_t new_sample)
{
    lpf1rd->in =  new_sample;
	lpf1rd->out = lpf1rd->z1 + lpf1rd->tc * (lpf1rd->in - lpf1rd->z1);
    lpf1rd->z1 =  lpf1rd->out;

    return lpf1rd->out;
}


















