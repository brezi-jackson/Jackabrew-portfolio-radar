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
#include "string.h"
#include "font.h" 
#include "delay.h"
#include "gui.h"
#include "oled.h"

/*******************************************************************
 * @name       :void GUI_DrawPoint(u16 x,u16 y,u16 color)
 * @date       :2018-08-09 
 * @function   :draw a point in LCD screen
 * @parameters :x:the x coordinate of the point
                y:the y coordinate of the point
								color:the color value of the point
 * @retvalue   :None
********************************************************************/
void GUI_DrawPoint(u16 x,u16 y,u16 color)
{
//	LCD_SetCursor(x,y);//设置光标位置 
//	Lcd_WriteData_16Bit(color); 
	OLED_DrawPoint(x, y ,1);
}

/*******************************************************************
 * @name       :void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
 * @date       :2018-08-09 
 * @function   :fill the specified area
 * @parameters :sx:the bebinning x coordinate of the specified area
                sy:the bebinning y coordinate of the specified area
								ex:the ending x coordinate of the specified area
								ey:the ending y coordinate of the specified area
								color:the filled color value
 * @retvalue   :None
********************************************************************/
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{  	
	u16 i,j;			
	u16 width=ex-sx+1; 		//得到填充的宽度
	u16 height=ey-sy+1;		//高度
	LCD_SetWindows(sx,sy,ex,ey);//设置显示窗口
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		Lcd_WriteData_16Bit(color);	//写入数据 	 
	}
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口设置为全屏
}

/*******************************************************************
 * @name       :void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Draw a line between two points
 * @parameters :x1:the bebinning x coordinate of the line
                y1:the bebinning y coordinate of the line
								x2:the ending x coordinate of the line
								y2:the ending y coordinate of the line
 * @retvalue   :None
********************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

/*****************************************************************************
 * @name       :void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Draw a rectangle
 * @parameters :x1:the bebinning x coordinate of the rectangle
                y1:the bebinning y coordinate of the rectangle
								x2:the ending x coordinate of the rectangle
								y2:the ending y coordinate of the rectangle
 * @retvalue   :None
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}  

/*****************************************************************************
 * @name       :void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Filled a rectangle
 * @parameters :x1:the bebinning x coordinate of the filled rectangle
                y1:the bebinning y coordinate of the filled rectangle
								x2:the ending x coordinate of the filled rectangle
								y2:the ending y coordinate of the filled rectangle
 * @retvalue   :None
******************************************************************************/  
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_Fill(x1,y1,x2,y2,POINT_COLOR);
}
 
/*****************************************************************************
 * @name       :void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
 * @date       :2018-08-09 
 * @function   :8 symmetry circle drawing algorithm (internal call)
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
								x:the x coordinate relative to the Circular center 
								y:the y coordinate relative to the Circular center 
								c:the color value of the circle
 * @retvalue   :None
******************************************************************************/  
void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
{
	GUI_DrawPoint(xc + x, yc + y, c);

	GUI_DrawPoint(xc - x, yc + y, c);

	GUI_DrawPoint(xc + x, yc - y, c);

	GUI_DrawPoint(xc - x, yc - y, c);

	GUI_DrawPoint(xc + y, yc + x, c);

	GUI_DrawPoint(xc - y, yc + x, c);

	GUI_DrawPoint(xc + y, yc - x, c);

	GUI_DrawPoint(xc - y, yc - x, c);
}

/*****************************************************************************
 * @name       :void gui_circle(int xc, int yc,u16 c,int r, int fill)
 * @date       :2018-08-09 
 * @function   :Draw a circle of specified size at a specified location
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
								r:Circular radius
								fill:1-filling,0-no filling
 * @retvalue   :None
******************************************************************************/  
void gui_circle(int xc, int yc,u16 c,int r, int fill)
{
	int x = 0, y = r, yi, d;

	d = 3 - 2 * r;


	if (fill) 
	{
		// 如果填充（画实心圆）
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				_draw_circle_8(xc, yc, x, yi, c);

			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	} else 
	{
		// 如果不填充（画空心圆）
		while (x <= y) {
			_draw_circle_8(xc, yc, x, y, c);
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}

/*****************************************************************************
 * @name       :void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
 * @date       :2018-08-09 
 * @function   :Draw a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge 
                y0:the bebinning y coordinate of the triangular edge 
								x1:the vertex x coordinate of the triangular
								y1:the vertex y coordinate of the triangular
								x2:the ending x coordinate of the triangular edge 
								y2:the ending y coordinate of the triangular edge 
 * @retvalue   :None
******************************************************************************/ 
void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
{
	LCD_DrawLine(x0,y0,x1,y1);
	LCD_DrawLine(x1,y1,x2,y2);
	LCD_DrawLine(x2,y2,x0,y0);
}

static void _swap(u16 *a, u16 *b)
{
	u16 tmp;
  tmp = *a;
	*a = *b;
	*b = tmp;
}

/*****************************************************************************
 * @name       :void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
 * @date       :2018-08-09 
 * @function   :filling a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge 
                y0:the bebinning y coordinate of the triangular edge 
								x1:the vertex x coordinate of the triangular
								y1:the vertex y coordinate of the triangular
								x2:the ending x coordinate of the triangular edge 
								y2:the ending y coordinate of the triangular edge 
 * @retvalue   :None
******************************************************************************/ 
void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
{
	u16 a, b, y, last;
	int dx01, dy01, dx02, dy02, dx12, dy12;
	long sa = 0;
	long sb = 0;
 	if (y0 > y1) 
	{
    _swap(&y0,&y1); 
		_swap(&x0,&x1);
 	}
 	if (y1 > y2) 
	{
    _swap(&y2,&y1); 
		_swap(&x2,&x1);
 	}
  if (y0 > y1) 
	{
    _swap(&y0,&y1); 
		_swap(&x0,&x1);
  }
	if(y0 == y2) 
	{ 
		a = b = x0;
		if(x1 < a)
    {
			a = x1;
    }
    else if(x1 > b)
    {
			b = x1;
    }
    if(x2 < a)
    {
			a = x2;
    }
		else if(x2 > b)
    {
			b = x2;
    }
		LCD_Fill(a,y0,b,y0,POINT_COLOR);
    return;
	}
	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;
	
	if(y1 == y2)
	{
		last = y1; 
	}
  else
	{
		last = y1-1; 
	}
	for(y=y0; y<=last; y++) 
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
    sb += dx02;
    if(a > b)
    {
			_swap(&a,&b);
		}
		LCD_Fill(a,y,b,y,POINT_COLOR);
	}
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) 
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if(a > b)
		{
			_swap(&a,&b);
		}
		LCD_Fill(a,y,b,y,POINT_COLOR);
	}
} 


/*****************************************************************************
 * @name       :void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
 * @date       :2018-08-09 
 * @function   :Centered display of English and Chinese strings
 * @parameters :x:the bebinning x coordinate of the Chinese and English strings
                y:the bebinning y coordinate of the Chinese and English strings
								fc:the color value of Chinese and English strings
								bc:the background color of Chinese and English strings
								str:the start address of the Chinese and English strings
								size:the size of Chinese and English strings
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
//void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
//{
//	u16 len=strlen((const char *)str);
//	
//	uint16_t x1;

//	x1=(OLED_Width-len*size/2)/2;
//	
//	OLED_ShowString(x1,y,str,size,mode);
//} 



/*****************************************************************************
 * @name       :void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
 * @date       :2018-08-09 
 * @function   :Centered display of English and Chinese strings
 * @parameters :x:the bebinning x coordinate of the Chinese and English strings
                y:the bebinning y coordinate of the Chinese and English strings
								fc:the color value of Chinese and English strings
								bc:the background color of Chinese and English strings
								str:the start address of the Chinese and English strings
								size:the size of Chinese and English strings
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
//void Gui_StrCenter_AutoRetract(u16 len, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
//{
//	u16 x1=(OLED_Width-len)/2;
//	OLED_ShowString(x1,y,str,size,mode,0);
//} 

/*****************************************************************************
 * @name       :void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p)
 * @date       :2018-08-09 
 * @function   :Display a 16-bit BMP image
 * @parameters :x:the bebinning x coordinate of the BMP image
                y:the bebinning y coordinate of the BMP image
								p:the start address of image array
 * @retvalue   :None
******************************************************************************/ 
void Gui_Drawbmp16(u16 x,u16 y,u16 w, u16 h, const unsigned char *p) //显示40*40 QQ图片
{
  	int i; 
	unsigned char picH,picL; 
	LCD_SetWindows(x,y,x+w-1,y+h-1);//窗口设置
    for(i=0;i<w*h;i++)
	{	
	 	picL=*(p+i*2);	//数据低位在前
		picH=*(p+i*2+1);				
		Lcd_WriteData_16Bit(picH<<8|picL);  						
	}	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复显示窗口为全屏	
}

/**
 * @brief 在指定范围内居中显示字符串
 * @param x1 起始x坐标
 * @param x2 终止x坐标
 * @param y y坐标
 * @param fc 前景色
 * @param bc 背景色
 * @param str 要显示的字符串
 * @param size 字体大小
 * @param mode 显示模式
 */
void Gui_StrCenterRange(u16 x1, u16 x2, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode)
{
    // 计算字符串长度
    u16 len = strlen((const char *)str);
    
    // 计算字符串显示宽度（假设每个字符宽度为size/2像素）
    u16 str_width = len * size / 2;
    
    // 计算可用显示范围宽度
    u16 range_width = x2 - x1;
    
    // 如果字符串宽度超过范围宽度，则从x1开始显示（避免溢出）
    if(str_width > range_width) {
        OLED_ShowString(x1, y, str, size, mode,0);
        return;
    }
    
    // 计算居中位置（在x1-x2范围内居中）
    u16 x_pos = x1 + (range_width - str_width) / 2;
    
    // 显示字符串
    OLED_ShowString(x_pos, y, str, size, mode,0);
}

/**
 * @brief 在指定位置显示字符串（基于现有OLED驱动）
 * @param x 起始X坐标
 * @param y 起始Y坐标
 * @param color 文字颜色（1=白色/点亮，0=黑色/熄灭）
 * @param bgcolor 背景颜色（1=白色/点亮，0=黑色/熄灭）
 * @param str 要显示的字符串
 * @param font_size 字体大小（8,12,16,20,24,28,32等）
 * @param is_ascii 是否为ASCII字符（1=是，0=否，保留参数，实际使用OLED_ShowChinese显示中文）
 */
void Gui_Str(uint8_t x, uint8_t y, uint8_t color, uint8_t bgcolor, const char *str, uint8_t font_size, uint8_t is_ascii)
{
    uint8_t mode;
    
    // 根据颜色和背景色设置显示模式
    if(color == 1 && bgcolor == 0) {
        mode = 1; // 正常显示（白字黑底）
    } else if(color == 0 && bgcolor == 1) {
        mode = 0; // 反色显示（黑字白底）
    } else if(color == 1 && bgcolor == 1) {
        mode = 1; // 白字白底（实际效果为全白）
    } else {
        mode = 0; // 黑字黑底（实际效果为全黑）
    }
    
    // 遍历字符串中的每个字符
    while(*str != '\0') {
        // ASCII字符处理
        if((*str >= ' ' && *str <= '~') || *str == '\n' || *str == '\r') {
            // 换行处理
            if(*str == '\n') {
                y += font_size; // 下移一行
                x = 0;         // 回到行首
                str++;
                continue;
            }
            // 回车处理（忽略）
            if(*str == '\r') {
                str++;
                continue;
            }
            
            // 显示ASCII字符
            OLED_ShowChar(x, y, *str, font_size, mode,0);
            
            // 计算下一个字符的位置
            if(font_size == 8) {
                x += 6; // 8x8字体每个字符占6像素宽
            } else {
                x += font_size/2; // 其他字体每个字符占字体大小一半的宽度
            }
        } 
        // 中文字符处理（需要扩展）
        else {
            // 这里可以添加中文显示逻辑，调用OLED_ShowChinese
            // 需要先实现中文字库和对应的显示函数
        }
        
        str++;
        
        // 如果超出屏幕宽度，换行
        if(x >= 128) {
            x = 0;
            y += font_size;
        }
    }
}


// 辅助函数
void DrawBasicRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
	// 绘制上边线
	OLED_DrawLine(x1, y1, x2, y1, color);
	// 绘制下边线
	OLED_DrawLine(x1, y2, x2, y2, color);
	// 绘制左边线
	OLED_DrawLine(x1, y1, x1, y2, color);
	// 绘制右边线
	OLED_DrawLine(x2, y1, x2, y2, color);
}

 /**
  * @brief  在OLED上绘制一条斜线
  * @param  x1,y1: 起点坐标
  * @param  x2,y2: 终点坐标
  * @param  mode: 绘制模式(1:绘制,0:擦除)
  * @retval 无
  */
void OLED_DrawDiagonalLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int e2;
    
    while(1) {
        OLED_DrawPoint(x1, y1, mode);
        
        if (x1 == x2 && y1 == y2) break;
        
        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

/**
 * @brief 在OLED上绘制一个带圆角的矩形
 * @param x 左上角X坐标
 * @param y 左上角Y坐标
 * @param w 矩形宽度
 * @param h 矩形高度
 * @param r 圆角半径，不能大于w/2或h/2
 * @param mode 绘制模式(1:绘制,0:擦除)
 */
void OLED_DrawRoundedRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t mode)
{
    if (r > w / 2) r = w / 2;
    if (r > h / 2) r = h / 2;

    // 水平边线
    OLED_DrawLine(x + r, y, x + w - r - 1, y, mode);                 // 顶边
    OLED_DrawLine(x + r, y + h - 1, x + w - r - 1, y + h - 1, mode); // 底边

    // 垂直边线
    OLED_DrawLine(x, y + r, x, y + h - r - 1, mode);                 // 左边
    OLED_DrawLine(x + w - 1, y + r, x + w - 1, y + h - r - 1, mode); // 右边

    // 四个角 (1/4 圆弧，整圆对称点原理)
    int a = 0, b = r;
    int d = 3 - 2 * r;
    while (a <= b)
    {
        // 左上角
        OLED_DrawPoint(x + r - b, y + r - a, mode);
        OLED_DrawPoint(x + r - a, y + r - b, mode);

        // 右上角
        OLED_DrawPoint(x + w - r + b - 1, y + r - a, mode);
        OLED_DrawPoint(x + w - r + a - 1, y + r - b, mode);

        // 左下角
        OLED_DrawPoint(x + r - b, y + h - r + a - 1, mode);
        OLED_DrawPoint(x + r - a, y + h - r + b - 1, mode);

        // 右下角
        OLED_DrawPoint(x + w - r + b - 1, y + h - r + a - 1, mode);
        OLED_DrawPoint(x + w - r + a - 1, y + h - r + b - 1, mode);

        if (d < 0)
        {
            d += 4 * a + 6;
        }
        else
        {
            d += 4 * (a - b) + 10;
            b--;
        }
        a++;
    }
}

/**
 * @brief 在OLED上绘制一条更实的斜线（更平滑视觉效果）
 * @param x1 起点X坐标
 * @param y1 起点Y坐标
 * @param x2 终点X坐标
 * @param y2 终点Y坐标
 * @param mode 显示模式（1=绘制，0=清除）
 */
void OLED_DrawSolidDiagonalLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);  // 取更长方向作为迭代次数

    float x_inc = dx / (float)steps;
    float y_inc = dy / (float)steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++) {
        OLED_DrawPoint((uint8_t)(x + 0.5f), (uint8_t)(y + 0.5f), mode);  // 四舍五入保证坐标正确
        x += x_inc;
        y += y_inc;
    }
}

