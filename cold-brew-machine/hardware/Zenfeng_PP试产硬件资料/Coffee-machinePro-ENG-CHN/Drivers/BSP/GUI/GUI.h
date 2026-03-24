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
#ifndef __GUI_H__
#define __GUI_H__

#define OLED_Width 160
#define OLED_Hight 128

#include "sys.h"

#define Size_Weight 24

extern uint16_t tag_x;

void GUI_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void Draw_Circle(u16 x0,u16 y0,u16 fc,u8 r);
void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len,u8 size,u8 mode);
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode);
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void Gui_Drawbmp16(u16 x,u16 y,u16 w, u16 h, const unsigned char *p); //œ‘ æ40*40 QQÕº∆¨
void gui_circle(int xc, int yc,u16 c,int r, int fill);
//void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
//void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode, u8 lang);

void Gui_StrCenter_AutoRetract(u16 len, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2);

void Gui_StrCenterRange(u16 x1, u16 x2, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode);
void Gui_Str(uint8_t x, uint8_t y, uint8_t color, uint8_t bgcolor, const char *str, uint8_t font_size, uint8_t is_ascii);
void DrawBasicRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void OLED_DrawDiagonalLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode);
void OLED_DrawRoundedRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t mode);
void OLED_DrawSolidDiagonalLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode);
#endif

