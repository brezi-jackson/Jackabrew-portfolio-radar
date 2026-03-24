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
#include "lcd.h"
#include "sys.h"
#include "stdlib.h"
#include "delay.h"	 
#include "spi.h"

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
//u16 POINT_COLOR = 0xFFFF,BACK_COLOR = 0X72A3; 
u16 POINT_COLOR = 0xFFFF,BACK_COLOR = 0X0000;
u16 DeviceCode;	 


/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(u8 data) 
{	
	LCD_CS_CLR;
	SPI_WriteByte(SPI1,data);
	LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_WR_REG(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u8 data)
{ 
	LCD_DC_CLR;
	LCD_Writ_Bus(data);
	LCD_DC_SET;	
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u8 data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u8 data)
{
	LCD_CS_CLR;
	LCD_DC_SET;
	SPI_WriteByte(SPI1,data);
	LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void Lcd_WriteData_16Bit(u16 Data)
{	
	LCD_CS_CLR;
	LCD_DC_SET;  
	SPI_WriteByte(SPI1,Data>>8);
	SPI_WriteByte(SPI1,Data);
	LCD_CS_SET;
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	Lcd_WriteData_16Bit(POINT_COLOR); 
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
	unsigned int i,m;
	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);   

	for(i=0;i<lcddev.height;i++)
	{
		for(m=0;m<lcddev.width;m++)
		{	
			Lcd_WriteData_16Bit(Color);
		}
	}
} 

/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(uint16_t arr)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	TIM_HandleTypeDef 	TIM_BL_Handler;      	//定时器句柄 
	TIM_OC_InitTypeDef 	TIM_BL_CH4Handler;	//定时器3通道1句柄
	
	TIM_BL_Handler.Instance				= BL_TIM;         			//定时器1
    TIM_BL_Handler.Init.Prescaler		= 23;       				//定时器分频
    TIM_BL_Handler.Init.CounterMode		= TIM_COUNTERMODE_UP;		//向上计数模式
    TIM_BL_Handler.Init.Period			= arr;          			//自动重装载值
    TIM_BL_Handler.Init.ClockDivision	= TIM_CLOCKDIVISION_DIV1;	//分频因子
    HAL_TIM_PWM_Init(&TIM_BL_Handler);       						//初始化PWM
    
    TIM_BL_CH4Handler.OCMode		= TIM_OCMODE_PWM1; 		//模式选择PWM1
    TIM_BL_CH4Handler.Pulse			= arr;            		//设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM_BL_CH4Handler.OCPolarity	= TIM_OCPOLARITY_HIGH; 	//输出比较极性为低 
    
	HAL_TIM_PWM_ConfigChannel(&TIM_BL_Handler, &TIM_BL_CH4Handler, BL_TIM_CHANNEL);//配置TIM3通道1
    
	HAL_TIM_PWM_Start(&TIM_BL_Handler, BL_TIM_CHANNEL);//开启PWM通道1	    
	
	__HAL_RCC_GPIOB_CLK_ENABLE();           			//开启GPIOB时钟
	__HAL_RCC_GPIOC_CLK_ENABLE();           			//开启GPIOB时钟

	GPIO_InitStructure.Pin = LCD_CS_GPIO_PIN;			//PB5 SDIN
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;	//推挽输出
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	//高速
	HAL_GPIO_Init(LCD_CS_GPIO_PORT, &GPIO_InitStructure);			//初始化
	HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN, GPIO_PIN_SET);
	
	GPIO_InitStructure.Pin = LCD_RST_GPIO_PIN;			//PB5 SDIN
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;	//推挽输出
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	//高速
	HAL_GPIO_Init(LCD_RST_GPIO_PORT, &GPIO_InitStructure);			//初始化
	HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_GPIO_PIN, GPIO_PIN_SET);
	
	GPIO_InitStructure.Pin = LCD_DC_GPIO_PIN;			//PB5 SDIN
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;	//推挽输出
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	//高速
	HAL_GPIO_Init(LCD_DC_GPIO_PORT, &GPIO_InitStructure);			//初始化
	HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_GPIO_PIN, GPIO_PIN_SET);
}

void BL_Set(u32 compare)
{
	BL_TIM->BL_CCR = compare; 
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(20);	
	LCD_RST_SET;
	delay_ms(20);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{	
	LCD_GPIOInit(100-1);//初始化GPIO
		
	SPI1_Init();
	
	LCD_RST_SET;
	delay_ms(10);	
	LCD_RST_CLR;//复位
	delay_ms(10);
	LCD_RST_SET;
	delay_ms(120);
	LCD_BL_SET;//打开背光

	LCD_WR_REG(0x11);     
	delay_ms(120);                

	LCD_direction(USE_HORIZONTAL_LCD);

	LCD_WR_REG(0x3A);     
	LCD_WR_DATA( 0x05);   //16BIT

	LCD_WR_REG(0xB2);     
	LCD_WR_DATA( 0x05);   
	LCD_WR_DATA( 0x05);   
	LCD_WR_DATA( 0x00);   
	LCD_WR_DATA( 0x33);   
	LCD_WR_DATA( 0x33);   

	LCD_WR_REG(0xB7);     
	LCD_WR_DATA( 0x35);   


	LCD_WR_REG(0xBB);     
	LCD_WR_DATA( 0x21);   

	LCD_WR_REG(0xC0);     
	LCD_WR_DATA( 0x2C);   

	LCD_WR_REG(0xC2);     
	LCD_WR_DATA( 0x01);   

	LCD_WR_REG(0xC3);     
	LCD_WR_DATA( 0x0B);   

	LCD_WR_REG(0xC4);     
	LCD_WR_DATA( 0x20);   

	LCD_WR_REG(0xC6);     
	LCD_WR_DATA( 0x0F);   //60HZ dot inversion

	LCD_WR_REG(0xD0);     
	LCD_WR_DATA( 0xA7);   
	LCD_WR_DATA( 0xA1); 

	LCD_WR_REG(0xD0);     
	LCD_WR_DATA( 0xA4);   
	LCD_WR_DATA( 0xA1);   


	LCD_WR_REG(0xD6);     
	LCD_WR_DATA( 0xA1);   

	LCD_WR_REG(0xE0);     
	LCD_WR_DATA( 0xD0);   
	LCD_WR_DATA( 0x04);   
	LCD_WR_DATA( 0x08);   
	LCD_WR_DATA( 0x0A);   
	LCD_WR_DATA( 0x09);   
	LCD_WR_DATA( 0x05);   
	LCD_WR_DATA( 0x2D);   
	LCD_WR_DATA( 0x43);   
	LCD_WR_DATA( 0x49);   
	LCD_WR_DATA( 0x09);   
	LCD_WR_DATA( 0x16);   
	LCD_WR_DATA( 0x15);   
	LCD_WR_DATA( 0x26);   
	LCD_WR_DATA( 0x2B);   

	LCD_WR_REG(0xE1);     
	LCD_WR_DATA( 0xD0);   
	LCD_WR_DATA( 0x03);   
	LCD_WR_DATA( 0x09);   
	LCD_WR_DATA( 0x0A);   
	LCD_WR_DATA( 0x0A);   
	LCD_WR_DATA( 0x06);   
	LCD_WR_DATA( 0x2E);   
	LCD_WR_DATA( 0x44);   
	LCD_WR_DATA( 0x40);   
	LCD_WR_DATA( 0x3A);   
	LCD_WR_DATA( 0x15);   
	LCD_WR_DATA( 0x15);   
	LCD_WR_DATA( 0x26);   
	LCD_WR_DATA( 0x2A);

	LCD_WR_REG(0x21);

	LCD_WR_REG(0x28);

	delay_ms(10);
	
	LCD_Clear(BROWN);
	LCD_BL_SET;
	BL_Set(10);
}

/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(yEnd);

	LCD_WriteRAM_Prepare();	//开始写入GRAM	

//	LCD_WR_REG(0x2a);//列地址设置
//	LCD_WR_DATA(xStar);
//	LCD_WR_DATA(xEnd);
//	LCD_WR_REG(0x2b);//行地址设置
//	LCD_WR_DATA(yStar);
//	LCD_WR_DATA(yEnd);
//	LCD_WR_REG(0x2c);//储存器写	
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
	
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x36,0x00);//BGR==1,MY==0,MX==0,MV==0
			break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,0x70);//BGR==1,MY==1,MX==0,MV==1
			break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;			
			LCD_WriteReg(0x36,0xC0);//BGR==1,MY==0,MX==0,MV==0
			break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,0xA0);//BGR==1,MY==1,MX==0,MV==1
			break;	
		default:
			break;
	}		
}	 


int LCD_Set_Scroll_Area(uint16_t TFA, uint16_t VSA, uint16_t BFA)
{
    uint8_t data;    
    if (TFA + VSA + BFA != 320) {
        return -1;
    }    
    LCD_WR_REG(0x33);    
    data = TFA >> 8;    
    LCD_WR_DATA(data);   //TFA高8位
    data = TFA;
    LCD_WR_DATA(data);   //TFA低8位
    data = VSA >> 8;
    LCD_WR_DATA(data);   //VSA高8位
    data = VSA;
    LCD_WR_DATA(data);   //VSA低8位
    data = BFA >> 8;
    LCD_WR_DATA(data);   //BFA高8位
    data = BFA;
    LCD_WR_DATA(data);   //BFA低8位
    return 0;
}


/*
      函数说明：设置滚动显示起始地址
      入口数据：TFA：顶部固定显示区域
                VSA：滚动区域，这部分指定后会滚动显示；
                BFA：底部固定显示区域
      返回值：  无
      说明：在LCD初始化代码的最后添加设置滚动显示起始地址的代码
                正滚
                for(uint8_t i=120;i>0;i--)
                {
                  LCD_Set_Scroll_Start_Address(i);
                  HAL_Delay(5); 
                }
                 反滚
                for(uint8_t i=0;i<120;i++)
                {
                  LCD_Set_Scroll_Start_Address(i);
                  HAL_Delay(5); 
                }
 
*/
void LCD_Set_Scroll_Start_Address(uint16_t vsp)
{
    uint8_t data;
    LCD_WR_REG(0x37);   //设置滚动显示起始地址的指令为0x37
    data=vsp>>8;
    LCD_WR_DATA(data);  //VSP高8位
    data=vsp;
    LCD_WR_DATA(data);  //VSP低8位
}

void LCD_Partial_Init(void)
{
	LCD_WR_REG(0x12);
}

void LCD_Partial_Area_Set(uint16_t Area_Start, uint16_t Area_End)
{
	uint8_t data;
	
	LCD_WR_REG(0x30);
	data=Area_Start>>8;
	LCD_WR_DATA(data);  //VSP高8位
    data=Area_Start;
    LCD_WR_DATA(data);  //VSP低8位
	data=Area_End>>8;
	LCD_WR_DATA(data);  //VSP高8位
    data=Area_End;
    LCD_WR_DATA(data);  //VSP低8位
}
