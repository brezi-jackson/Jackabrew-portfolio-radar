#ifndef __OLED_H
#define __OLED_H 

#include "sys.h"
#include "stdlib.h"	
#include "spi.h"

#define USE_HORIZONTAL 3 //0：正向显示 1：180度显示 2：270度显示 3：90度显示

//-----------------OLED端口定义---------------- 
#define SPI1_SCK_GPIO_PORT              GPIOA
#define SPI1_SCK_GPIO_PIN               GPIO_PIN_5
#define SPI1_SCK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

#define SPI1_MOSI_GPIO_PORT             GPIOA
#define SPI1_MOSI_GPIO_PIN              GPIO_PIN_7
#define SPI1_MOSI_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

#define OLED_SCL_Clr() HAL_GPIO_WritePin(SPI1_SCK_GPIO_PORT, SPI1_SCK_GPIO_PIN, GPIO_PIN_RESET)//SCL
#define OLED_SCL_Set() HAL_GPIO_WritePin(SPI1_SCK_GPIO_PORT, SPI1_SCK_GPIO_PIN, GPIO_PIN_SET)

#define OLED_SDA_Clr() HAL_GPIO_WritePin(SPI1_MOSI_GPIO_PORT, SPI1_MOSI_GPIO_PIN, GPIO_PIN_RESET)//SDA
#define OLED_SDA_Set() HAL_GPIO_WritePin(SPI1_MOSI_GPIO_PORT, SPI1_MOSI_GPIO_PIN, GPIO_PIN_SET)

#define OLED_RST_GPIO_PORT	GPIOB
#define OLED_RST_GPIO_PIN	GPIO_PIN_1
#define OLED_RES_Clr() HAL_GPIO_WritePin(OLED_RST_GPIO_PORT, OLED_RST_GPIO_PIN,	GPIO_PIN_RESET)//RES
#define OLED_RES_Set() HAL_GPIO_WritePin(OLED_RST_GPIO_PORT, OLED_RST_GPIO_PIN, GPIO_PIN_SET)
#define OLED_RES_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PC口时钟使能 */

#define OLED_CS_GPIO_PORT	GPIOB
#define OLED_CS_GPIO_PIN	GPIO_PIN_10
#define OLED_DC_Clr()  HAL_GPIO_WritePin(OLED_DC_GPIO_PORT, OLED_DC_GPIO_PIN, GPIO_PIN_RESET)//DC
#define OLED_DC_Set()  HAL_GPIO_WritePin(OLED_DC_GPIO_PORT, OLED_DC_GPIO_PIN, GPIO_PIN_SET)
#define OLED_DC_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PC口时钟使能 */

#define OLED_DC_GPIO_PORT	GPIOB
#define OLED_DC_GPIO_PIN	GPIO_PIN_0
#define OLED_CS_Clr()  HAL_GPIO_WritePin(OLED_CS_GPIO_PORT, OLED_CS_GPIO_PIN, GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(OLED_CS_GPIO_PORT, OLED_CS_GPIO_PIN, GPIO_PIN_SET)
#define OLED_CS_GPIO_CLK_ENABLE()	do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PC口时钟使能 */

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_ClearArea(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x, u8 y, u16 chr, u8 size1, u8 mode, u8 isChinese);
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowString(u8 x, u8 y, const char *str, u8 size, u8 mode, u8 lang) ;

void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode, u8 lang);

void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_Init(void);

#endif

