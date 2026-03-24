/*************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#ifndef __LCD_H
#define __LCD_H

#include "sys.h"	 
#include "stdlib.h"

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				  //LCD ID
	u8  dir;			  //横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	 wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令	
}_lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////	 
#define USE_HORIZONTAL_LCD	1 //定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#define LCD_W 240
#define LCD_H 320

//TFTLCD部分外要调用的函数		   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

////////////////////////////////////////////////////////////////////

//-----------------LCD端口定义----------------
#define LCD_RST_GPIO_PORT	GPIOC
#define LCD_RST_GPIO_PIN	GPIO_PIN_4
#define LCD_CS_GPIO_PORT	GPIOB
#define LCD_CS_GPIO_PIN		GPIO_PIN_0
#define LCD_DC_GPIO_PORT	GPIOC
#define LCD_DC_GPIO_PIN		GPIO_PIN_5
#define LCD_BL_GPIO_PORT	GPIOB
#define LCD_BL_GPIO_PIN		GPIO_PIN_1

/******************************* BL ***********************************/

#define BL_TIM					TIM3
#define BL_TIM_CLK_ENABLE()		do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)   /* TEC时钟使能 */

#define BL_CCR					CCR4
#define BL_TIM_CHANNEL			TIM_CHANNEL_4
#define BL_GPIO_PORT			LCD_BL_GPIO_PORT
#define BL_GPIO_PIN				LCD_BL_GPIO_PIN
#define BL_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PC口时钟使能 */

#define BL_ENABLE()				do{HAL_GPIO_WritePin(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN, GPIO_PIN_SET);  }while(0)
#define BL_DISABLE()			do{HAL_GPIO_WritePin(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN, GPIO_PIN_RESET);}while(0)

//QDtech全系列模块采用了三极管控制背光亮灭，用户也可以接PWM调节背光亮度
#define	LCD_LED PBout(1) //LCD背光    		 PB1

//如果使用官方库函数定义下列底层，速度将会下降到14帧每秒，建议采用我司推荐方法
//以下IO定义直接操作寄存器，快速IO操作，刷屏速率可以达到28帧每秒！ 

//GPIO置位（拉高）
#define	LCD_BL_SET	HAL_GPIO_WritePin(LCD_BL_GPIO_PORT,	LCD_BL_GPIO_PIN,	GPIO_PIN_SET)	
#define	LCD_CS_SET	HAL_GPIO_WritePin(LCD_CS_GPIO_PORT,	LCD_CS_GPIO_PIN,	GPIO_PIN_SET)	
#define	LCD_DC_SET	HAL_GPIO_WritePin(LCD_DC_GPIO_PORT,	LCD_DC_GPIO_PIN,	GPIO_PIN_SET)
#define	LCD_RST_SET	HAL_GPIO_WritePin(LCD_RST_GPIO_PORT,LCD_RST_GPIO_PIN,	GPIO_PIN_SET)

//GPIO复位（拉低）
#define	LCD_BL_CLR	HAL_GPIO_WritePin(LCD_BL_GPIO_PORT,	LCD_BL_GPIO_PIN,	GPIO_PIN_RESET)	
#define	LCD_CS_CLR	HAL_GPIO_WritePin(LCD_CS_GPIO_PORT,	LCD_CS_GPIO_PIN,	GPIO_PIN_RESET)
#define	LCD_DC_CLR	HAL_GPIO_WritePin(LCD_DC_GPIO_PORT,	LCD_DC_GPIO_PIN,	GPIO_PIN_RESET)
#define	LCD_RST_CLR	HAL_GPIO_WritePin(LCD_RST_GPIO_PORT,LCD_RST_GPIO_PIN,	GPIO_PIN_RESET)

//画笔颜色
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED		0XFFE0
#define GBLUE		0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN		0x0000 // 0X72A3 // 0XBC40	//棕色
#define BRRED		0XFC07	//棕红色
#define GRAY		0X8430	//灰色
#define GRAY0       0xEF7D 
#define GRAY1       0x8410	//灰色1      00000 000000 00000
#define GRAY2       0x4208 
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	0X841F //浅绿色
#define LIGHTGRAY     0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 		0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2B12 //浅棕蓝色(选择条目的反色)

void LCD_GPIOInit(uint16_t arr);

void BL_Set(u32 compare);

void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(u16 Color);	 
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x,u16 y);//画点
u16  LCD_ReadPoint(u16 x,u16 y); //读点
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);

u16 LCD_RD_DATA(void);//读取LCD数据									    
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WR_DATA(u8 data);
u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
u16 LCD_ReadRAM(void);		   
u16 LCD_BGR2RGB(u16 c);
void LCD_SetParam(void);
void Lcd_WriteData_16Bit(u16 Data);
void LCD_direction(u8 direction );

void LCD_WR_REG(u8 data);

void LCD_Partial_Init(void);
void LCD_Partial_Area_Set(uint16_t Area_Start, uint16_t Area_End);

void LCD_Set_Scroll_Start_Address(uint16_t vsp);
int LCD_Set_Scroll_Area(uint16_t TFA, uint16_t VSA, uint16_t BFA);
					  		 
#endif  
	 
	 



