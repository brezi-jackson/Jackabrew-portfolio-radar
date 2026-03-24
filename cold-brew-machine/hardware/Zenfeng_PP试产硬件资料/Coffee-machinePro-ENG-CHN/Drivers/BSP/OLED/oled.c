#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"

u8 OLED_GRAM[144][20];

//反显函数
void OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
		}
}


void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else
	  OLED_DC_Clr();
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{
		OLED_SCL_Clr();
		if(dat&0x80)
		   OLED_SDA_Set();
		else 
		   OLED_SDA_Clr();
		OLED_SCL_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
}

//开启OLED显示 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
}

//关闭OLED显示 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
	OLED_WR_Byte(0xAE,OLED_CMD);//关闭屏幕
}

//更新显存到OLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<20;i++)
	{
		OLED_WR_Byte(0xb0,OLED_CMD); //设置行起始地址
		OLED_WR_Byte(i,OLED_CMD);
		OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
		OLED_WR_Byte(0x11,OLED_CMD);   //设置高列起始地址
		for(n=0;n<128;n++)
		OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}
//清屏函数
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<20;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//清除所有数据
			}
  }
	OLED_Refresh();//更新显示
}

//清屏函数
void OLED_ClearArea(void)
{
	u8 i,n;
	for(i=0;i<20;i++)
	{
		for(n=0;n<128;n++)
		{
			OLED_GRAM[n][i]=0;//清除所有数据
		}
	}	
}

//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空	
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 i,m,n;
	u8 x0=x,y0=y;
	if(USE_HORIZONTAL==2)
	{
		x=y0;
		y=x0;
	}
	else if(USE_HORIZONTAL==3)
	{
		x=y0;
		y=x0;
	}
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_GRAM[x][i]|=n;}
	else
	{
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
		OLED_GRAM[x][i]|=n;
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	}
}
//画线
//x1,y1:起点坐标
//x2,y2:结束坐标
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		OLED_DrawPoint(uRow,uCol,mode);//画点
		xerr+=delta_x;
		yerr+=delta_y;
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
//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);
 
        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size1:选择字体 6x8/6x12/8x16/12x24
//mode:0,反色显示;1,正常显示
//void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
//{
//	u8 i,m,temp,size2,chr1;
//	u8 x0=x,y0=y;
//	if(size1==8)size2=6;
//	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
//	chr1=chr-' ';  //计算偏移后的值
//	for(i=0;i<size2;i++)
//	{
//		if(size1==12)
//			{temp=asc2_1206[chr1][i];} //调用1206字体
//		else if(size1==16)
//			{temp=asc2_1608[chr1][i];} //调用1608字体
////		else if(size1==18)
////			{temp=asc2_1809[chr1][i];} //调用1809字体
//		else if(size1==20)
//			{temp=asc2_2010[chr1][i];} //调用2010字体
////		else if(size1==22)
////			{temp=asc2_2211[chr1][i];} //调用2211字体
//		else if(size1==24)
//			{temp=asc2_2412[chr1][i];} //调用2412字体
////		else if(size1==26)
////			{temp=asc2_2613[chr1][i];} //调用2814字体
//		else if(size1==28)
//			{temp=asc2_2814[chr1][i];} //调用2814字体
////		else if(size1==30)
////			{temp=asc2_3015[chr1][i];} //调用2814字体
//		else if(size1==32)
//			{temp=asc2_3216[chr1][i];} //调用3216字体
//		else return;
//		for(m=0;m<8;m++)
//		{
//			if(temp&0x01)OLED_DrawPoint(x,y,mode);
//			else OLED_DrawPoint(x,y,!mode);
//			temp>>=1;
//			y++;
//		}
//		x++;
//		if((size1!=8)&&((x-x0)==size1/2))
//		{x=x0;y0=y0+8;}
//		y=y0;
//  }
//}

// 英文-中文翻译映射表
typedef struct {
    const char *english;
    const char *chinese;
} ChineseMapping;



ChineseMapping chineseMap[] = {
    {"COFFEE", "\xE5\x92\x96\xE5\x95\xA1"},//咖啡
    {"MODE", "\xE6\xA8\xA1\xE5\xBC\x8F"},//模式
    {"REGULAR", "\xE7\xBB\x8F\xE5\x85\xB8"},//经典
    {"CONCENTRATE", "\xE6\xB5\x93\xE7\xBC\xA9"},//浓缩
    {"SLOW", "\xE5\x86\xB0\xE6\xBB\xA4"},//冰滴
    {"ROAST LEVEL", "\xE7\x83\x98\xE5\x9F\xB9\xE5\xBA\xA6"},//烘培度
    {"DARK", "\xE6\xB7\xB1\xE7\x83\x98"},//深烘
    {"MEDIUM", "\xE4\xB8\xAD\xE7\x83\x98"},//中烘
    {"LIGHT", "\xE6\xB5\x85\xE7\x83\x98"},//浅烘
    {"COFFEE ADDED", "\xE5\x92\x96\xE5\x95\xA1\xE7\xB2\x89\xE9\x87\x8F"},//咖啡粉量
    {"READY TO BREW?", "\xE5\xBC\x80\xE5\xA7\x8B\xE5\x86\xB2\xE6\xB3\xA1\xEF\xBC\x9F"},//开始冲泡？
    {"TEA TYPE", "\xE8\x8C\xB6\xE7\xB1\xBB"},//茶类
    {"GREEN TEA", "\xE7\xBB\xBF\xE8\x8C\xB6"},//绿茶
    {"WHITE TEA", "\xE7\x99\xBD\xE8\x8C\xB6"},//白茶
    {"OOLONG", "\xE4\xB9\x9C\xE9\xBE\x99\xE8\x8C\xB6"},//乌龙茶
    {"BLACK TEA", "\xE7\xBA\xA2\xE8\x8C\xB6"},//红茶
    {"HERBAL", "\xE8\x8A\xB1\xE8\x8C\xB6"},//花茶
    {"TEA ADDED", "\xE8\x8C\xB6\xE9\x87\x8F"},//茶量
    {"WAIT TO START", "\xE5\x87\x86\xE5\xA4\x87\xE5\xBC\x80\xE5\xA7\x8B"},//准备开始
    {"HEATING", "\xE9\xA2\x84\xE7\x83\xAD\xE4\xB8\xAD"},//预热中
    {"BLOOMING", "\xE9\x97\xB7\xE8\x92\xB8\xE4\xB8\xAD"},//闷蒸中
    {"COOLING", "\xE5\x86\xB7\xE5\x8D\xB4\xE4\xB8\xAD"},//冷却中
    {"COLD BREWING", "\xE5\x86\xB7\xE8\x90\x83\xE4\xB8\xAD"},//冷萃中
    {"CANCEL BREWING?", "\xE7\xA1\xAE\xE8\xAE\xA4\xE5\x8F\x96\xE6\xB6\x88\xEF\xBC\x9F"},//确认取消？
    {"HOLD TO", "\xE9\x95\xBF\xE6\x8C\x89"},//长按
    {"CANCEL", "\xE5\x8F\x96\xE6\xB6\x88"},//取消
    {"RESUME", "\xE7\xBB\xAD\xE7\xBB\xAD"},//继续
    {"CANCELLING", "\xE5\x8F\x96\xE6\xB6\x88\xE4\xB8\xAD"},//取消中
    {"CUSTOM", "\xE8\x87\xAA\xE5\xAE\x9A\xE4\xB9\x89"},//自定义
    {"COFFEE:WATER", "\xE7\xB2\x89\xE6\xB0\xB4"},//粉水
    {"BLOOM RATIO", "\xE9\x97\xB7\xE8\x92\xB8\xE6\xAF\x94\xE4\xBE\x8B"},//闷蒸比例
    {"BLOOM TEMP", "\xE9\x97\xB7\xE8\x92\xB8\xE6\xB8\xA9\xE5\xBA\xA6"},//闷蒸温度
    {"BLOOM FLOW", "\xE9\x97\xB7\xE8\x92\xB8\xE6\xBB\xB4\xE9\x80\x9F"},//闷蒸滴速
    {"COLD BREW TEMP", "\xE5\x86\xB7\xE8\x90\x83\xE6\xB8\xA9\xE5\xBA\xA6"},//冷萃温度
    {"COLD BREW FLOW", "\xE5\x86\xB7\xE8\x90\x83\xE6\xBB\xB4\xE9\x80\x9F"},//冷萃滴速
    {"TOTAL RATIO", "\xE6\x80\xBB\xE6\xAF\x94\xE4\xBE\x8B"},//总比例
    {"SUMMARY", "\xE6\x80\xBB\xE7\xBB\x93"},//总结
    {"BLOOM", "\xE9\x97\xB7\xE8\x92\xB8"},//闷蒸
    {"BREW", "\xE5\x86\xB7\xE8\x90\x83"},//冷萃
    {"TOTAL", "\xE7\xB2\x89\xE6\xB0\xB4"},//粉水
    {"SAVE PROFILE?", "\xE6\x98\xAF\xE5\x90\xA6\xE5\xAD\x98\xE5\x82\xA8\xEF\xBC\x9F"},//是否存储？
    {"YES", "\xE6\x98\xAF"},//是
    {"NO", "\xE5\x90\xA6"},//否
    {"LATER", "\xE7\xA8\x8D\xE5\x90\x8E"},//稍后
    {"NAME", "\xE5\x90\x8D\xE7\xA7\xB0"},//名称
    {"DISCARD", "\xE5\x8F\x96\xE6\xB6\x88"},//取消
    {"SAVE", "\xE4\xBF\x9D\xE5\xAD\x98"},//保存
    {"SAVED!", "\xE4\xBF\x9D\xE5\xAD\x98\xE6\x88\x90\xE5\x8A\x9F\xEF\xBC\x81"},//保存成功！
    {"SAVED", "\xE5\xB7\xB2\xE5\xAD\x98\xE6\xA1\xA3"},//已存档
    {"SAVED PROFILE", "\xE5\xB7\xB2\xE5\xAD\x98\xE6\xA1\xA3"},//已存档
    {"EDIT", "\xE7\xBC\x96\xE8\xBE\x91"},//编辑
    {"DELETE", "\xE5\x88\xA0\xE9\x99\xA4"},//删除
    {"RENAMED", "\xE9\x87\x8D\xE5\x91\xBD\xE5\x90\x8D"},//重命名
    {"DELETE!", "\xE5\x88\xA0\xE9\x99\xA4\xE6\x88\x90\xE5\x8A\x9F\xEF\xBC\x81"},//删除成功！
    {"MEMORY FULL,REPLACE", "\xE5\x86\x85\xE5\xAD\x98\xE5\xB7\xB2\xE6\xBB\xA1\xEF\xBC\x8C\xE6\x9B\xBF\xE6\x8D\xA2"},//内存已满，替换
    {"REPLACE", "\xE6\x9B\xBF\xE6\x8D\xA2"},//替换
    {"SETTING", "\xE8\xAE\xBE\xE7\xBD\xAE"},//设置
    {"LANGUAGE", "\xE8\xAF\xAD\xE8\xA8\x80"},//语言
    {"UNIT", "\xE5\x8D\x95\xE4\xBD\x8D"},//单位
    {"SOUND", "\xE5\xA3\xB0\xE9\x9F\xB3"},//声音
    {"DESCALE", "\xE9\x99\xA4\xE5\x9E\xA2"},//除垢
    {"SYSTEM CHECK", "\xE8\x87\xAA\xE6\xA3\x80"},//自检
    {"RESET ALL", "\xE7\xB3\xBB\xE7\xBB\x9F\xE9\x87\x8D\xE7\xBD\xAE"},//系统重置
    {"ENGLISH", "\xE8\x8B\xB1\xE8\xAF\xAD"},//英语
    {"CHINESE", "\xE4\xB8\xAD\xE6\x96\x87"},//中文
    {"METRIC", "\xE5\x85\xAC\xE5\x88\xB6"},//公制
    {"IMPERIAL", "\xE8\x8B\xB1\xE5\x88\xB6"},//英制
    {"ON", "\xE5\xBC\x80"},//开
    {"OFF", "\xE5\x85\xB3"},//关
		{"TEA", "\xE8\x8C\xB6"},//茶
		{"FINISHED", "\xE5\xB7\xB2\xE5\xAE\x8C\xE6\x88\x90"},//已完成
		{"TEA:WATER", "\xE8\x8C\xB6\xE6\xB0\xB4"},//茶水
		{"BREW LATER", "\xE5\xAE\x9A\xE6\x97\xB6\xE5\x86\xB2\xE6\xB3\xA1"},//定时冲泡
		{"ENJOY!", "\xE8\xAF\xB7\xE4\xBA\xAB\xE7\x94\xA8\xEF\xBC\x81"},//请享用
		{"PRESS TO START", "\xE5\x8F\xB3\xE9\x94\xAE\xE5\xBC\x80\xE5\xA7\x8B"},//右键开始
		{"RESET ALL?",         "\xE9\x87\x8D\xE7\xBD\xAE\xE6\x89\x80\xE6\x9C\x89\xE8\xAE\xBE\xE7\xBD\xAE\xEF\xBC\x9F"},  // 重置所有设置？
		{"THIS WILL ERASE",    "\xE6\xAD\xA4\xE6\x93\x8D\xE4\xBD\x9C\xE5\xB0\x86\xE5\x88\xA0\xE9\x99\xA4"},          // 此操作将删除
		{"ALL SETTINGS",       "\xE6\x89\x80\xE6\x9C\x89\xE8\xAE\xBE\xE7\xBD\xAE"},      // 所有设置
		{"HOLD TO CONFIRM",    "\xE9\x95\xBF\xE6\x8C\x89\xE7\xA1\xAE\xE8\xAE\xA4"},      // 长按确认
		{"PREPARING", "\xE5\x87\x86\xE5\xA4\x87\xE4\xB8\xAD"},//准备中
		{"WARN!", "\xE8\xAD\xA6\xE5\x91\x8A\xEF\xBC\x81"},//警告！
		{"WATER LOW", "\xE6\xB0\xB4\xE9\x87\x8F\xE8\xBF\x87\xE4\xBD\x8E"},//水量过低
		{"PLEASE REFILL", "\xE8\xAF\xB7\xE5\x86\x8D\xE6\xAC\xA1\xE6\xB3\xA8\xE6\xB0\xB4"},//请再次注水
		{"ERROR!", "\xE9\x94\x99\xE8\xAF\xAF\xEF\xBC\x81"},//错误！
		{"CONTACT CUSTOMER SERVICE", "\xE8\xAF\xB7\xE8\x81\x94\xE7\xB3\xBB\xE5\xAE\xA2\xE6\x9C\x8D"},//请联系客服
		
};

// 英文-日文翻译映射表
typedef struct {
    const char *english;
    const char *japanese;  // UTF-8编码的日文
} JapaneseMapping;



ChineseMapping japaneseMap[] = {
    {"COFFEE", "\xE3\x82\xB3\xE3\x83\xBC\xE3\x83\x92\xE3\x83\xBC"},//咖啡  コーヒー
    {"MODE", "\xE3\x83\x91\xE3\x82\xBF\xE3\x83\xBC\xE3\x83\xB3\xEF\xBC\x83\xE3\x83\x91\xE3\x82\xBF\xE3\x83\xBC\xE3\x83\xB3\xEF\xBC\x83"},//模式  パターン＃パターン＃
    {"REGULAR", "\xE3\x82\xAF\xE3\x83\xA9\xE3\x82\xB7\xE3\x83\x83\xE3\x82\xAF"},//经典  クラシック
    {"CONCENTRATE", "\xE3\x81\xAE\xE3\x81\x86\xE3\x81\x97\xE3\x82\x85\xE3\x81\x8F"},//浓缩  のうしゅく
    {"SLOW", "\xE6\xB0\xB7\xE3\x81\xAE\xE3\x81\x97\xE3\x81\x9A\xE3\x81\x8F"},//冰滴  氷のしずく
    {"ROAST LEVEL", "\xE3\x83\x99\xE3\x83\xBC\xE3\x82\xAD\xE3\x83\xB3\xE3\x82\xB0\xE3\x81\xA9"},//烘培度  ベーキングど
    {"DARK", "\xE6\xB7\xB1\xE7\x84\xBC\xE3\x81\x8D"},//深烘  深焼き
    {"MEDIUM", "\xE3\x81\xA1\xE3\x82\x85\xE3\x81\x86\xE3\x81\x8B\xE3\x82\x93\xE3\x81\x9D\xE3\x81\x86"},//中烘  ちゅうかんそう
    {"LIGHT", "\xE6\xB5\x85\xE7\x84\xBC\xE3\x81\x8D"},//浅烘  浅焼き
    {"COFFEE ADDED", "\xE3\x82\xB3\xE3\x83\xBC\xE3\x83\x92\xE3\x83\xBC\xE7\xB2\x89\xE9\x87\x8F"},//咖啡粉量  コーヒー粉量
    {"READY TO BREW?", "\xE6\x8A\xBD\xE5\x87\xBA\xE3\x82\x92\xE9\x96\x8B\xE5\xA7\x8B\xE3\x81\x97\xE3\x81\xBE\xE3\x81\x99\xE3\x81\x8B\xEF\xBC\x9F"},//开始冲泡？  抽出を開始しますか？
    {"TEA TYPE", "\xE8\x8C\xB6\xE9\xA1\x9E"},//茶类  茶類
    {"GREEN TEA", "\xE7\xB7\x91\xE8\x8C\xB6"},//绿茶  緑茶
    {"WHITE TEA", "\xE7\x99\xBD\xE8\x8C\xB6"},//白茶  白茶
    {"OOLONG", "\xE3\x82\xA6\xE3\x83\xBC\xE3\x83\xAD\xE3\x83\xB3\xE8\x8C\xB6"},//乌龙茶  ウーロン茶
    {"BLACK TEA", "\xE7\xB4\x85\xE8\x8C\xB6"},//红茶  紅茶
    {"HERBAL", "\xE8\x8A\xB1\xE8\x8C\xB6"},//花茶  花茶
    {"TEA ADDED", "\xE3\x81\xA1\xE3\x82\x83\xE3\x82\x8A\xE3\x82\x87\xE3\x81\x86"},//茶量  ちゃりょう
    {"WAIT TO START", "\xE6\xBA\x96\xE5\x82\x99\xE9\x96\x8B\xE5\xA7\x8B"},//准备开始  準備開始
    {"HEATING", "\xE4\xBA\x88\xE7\x86\xB1\xE4\xB8\xAD"},//预热中  予熱中
    {"BLOOMING", "\xE8\x92\xB8\xE3\x81\x99"},//闷蒸中  蒸す
    {"COOLING", "\xE5\x86\xB7\xE3\x82\x84\xE3\x81\x97\xE3\x81\xA6\xE3\x81\x84\xE3\x81\xBE\xE3\x81\x99"},//冷却中  冷やしています
    {"COLD BREWING", "\xE3\x82\xB3\xE3\x83\xBC\xE3\x83\xAB\xE3\x83\x89\xE3\x81\xA1\xE3\x82\x85\xE3\x81\x86\xE3\x81\x97\xE3\x82\x85\xE3\x81\xA4"},//冷萃中  コールドちゅうしゅつ
    {"CANCEL BREWING?", "\xE3\x82\xAD\xE3\x83\xA3\xE3\x83\xB3\xE3\x82\xBB\xE3\x83\xAB\xE3\x81\xAE\xE7\xA2\xBA\xE8\xAA\x8D\xEF\xBC\x9F"},//确认取消？  キャンセルの確認？
    {"HOLD TO", "\xE9\x95\xB7\xE6\x8A\xBC\xE3\x81\x97"},//长按  長押し
    {"CANCEL", "\xE3\x82\xAD\xE3\x83\xA3\xE3\x83\xB3\xE3\x82\xBB\xE3\x83\xAB"},//取消  キャンセル
    {"RESUME", "\xE7\xB6\x9A\xE8\xA1\x8C"},//继续  続行
    {"CANCELLING", "\xE3\x82\xAD\xE3\x83\xA3\xE3\x83\xB3\xE3\x82\xBB\xE3\x83\xAB\xE4\xB8\xAD"},//取消中  キャンセル中
    {"CUSTOM", "\xE3\x83\xA6\xE3\x83\xBC\xE3\x82\xB6\xE3\x83\xBC\xE8\xA8\xAD\xE5\xAE\x9A"},//自定义  ユーザー設定
    {"COFFEE:WATER", "\xE7\xB2\x89\xE3\x81\xA8\xE6\xB0\xB4\xE3\x81\xAE\xE6\xAF\x94\xE7\x8E\x87"},//粉水  粉と水の比率
    {"BLOOM RATIO", "\xE8\x92\xB8\xE3\x82\x89\xE3\x81\x97\xE6\xAF\x94\xE7\x8E\x87"},//闷蒸比例  蒸らし比率
    {"BLOOM TEMP", "\xE8\x92\xB8\xE3\x82\x89\xE3\x81\x97\xE6\xB8\xA9\xE5\xBA\xA6"},//闷蒸温度  蒸らし温度
    {"BLOOM FLOW", "\xE3\x83\x96\xE3\x83\xAB\xE3\x83\xBC\xE3\x83\xA0\xE6\xBB\xB4\xE9\x80\x9F"},//闷蒸滴速  ブルーム滴速
    {"COLD BREW TEMP", "\xE3\x82\xB3\xE3\x83\xBC\xE3\x83\xAB\xE3\x83\x89\xE3\x83\x96\xE3\x83\xAA\xE3\x83\xA5\xE3\x83\xBC\xE6\xB8\xA9\xE5\xBA\xA6"},//冷萃温度  コールドブリュー温度
    {"COLD BREW FLOW", "\xE3\x82\x8C\xE3\x81\x84\xE3\x81\xA1\xE3\x82\x85\xE3\x81\x86\xE3\x81\x97\xE3\x82\x85\xE3\x81\xA4\xE3\x81\x9D\xE3\x81\x8F\xE3\x81\xA9"},//冷萃滴速  れいちゅうしゅつそくど
    {"TOTAL RATIO", "\xE5\x90\x88\xE8\xA8\x88\xE3\x82\xB9\xE3\x82\xB1\xE3\x83\xBC\xE3\x83\xAB"},//总比例   合計スケール
    {"SUMMARY", "\xE3\x81\xBE\xE3\x81\xA8\xE3\x82\x81"},//总结  まとめ
    {"BLOOM", "\xE8\x92\xB8\xE3\x81\x99"},//闷蒸   蒸す
    {"BREW", "\xE3\x82\x8C\xE3\x81\x84\xE3\x81\xA1\xE3\x82\x85\xE3\x81\x86\xE3\x81\x97\xE3\x82\x85\xE3\x81\xA4"},//冷萃  れいちゅうしゅつ
    {"TOTAL", "\xE3\x81\xB5\xE3\x82\x93\xE3\x81\x99\xE3\x81\x84"},//粉水  ふんすい
    {"SAVE TO PROFILE?", "\xE4\xBF\x9D\xE5\xAD\x98\xE3\x81\x97\xE3\x81\xBE\xE3\x81\x99\xE3\x81\x8B\xEF\xBC\x9F"},//是否存储？  保存しますか？
    {"YES", "\xE3\x81\xAF\xE3\x81\x84"},//是  はい
    {"NO", "\xE3\x81\x84\xE3\x81\x84\xE3\x81\x88"},//否  いいえ
    {"LATER", "\xE5\xBE\x8C\xE3\x81\xA7"},//稍后  後で
    {"NAME", "\xE3\x81\xAE\xE5\x90\x8D\xE5\x89\x8D\xE3\x82\x92\xE3\x81\x82\xE3\x81\x92\xE3\x82\x8B"},//名称  の名前をあげる
    {"DISCARD", "\xE3\x82\xAD\xE3\x83\xA3\xE3\x83\xB3\xE3\x82\xBB\xE3\x83\xAB"},//取消  キャンセル
    {"SAVE", "\xE4\xBF\x9D\xE5\xAD\x98"},//保存 
    {"SAVED!", "\xE4\xBF\x9D\xE5\xAD\x98\xE3\x81\xAB\xE6\x88\x90\xE5\x8A\x9F\xE3\x81\x97\xE3\x81\xBE\xE3\x81\x97\xE3\x81\x9F\xEF\xBC\x81"},//保存成功！  保存に成功しました！
    {"SAVED", "\xE3\x82\xA2\xE3\x83\xBC\xE3\x82\xAB\xE3\x82\xA4\xE3\x83\x96\xE6\xB8\x88\xE3\x81\xBF"},//已存档  アーカイブ済み
    {"SAVED PROFILE", "\xE3\x82\xA2\xE3\x83\xBC\xE3\x82\xAB\xE3\x82\xA4\xE3\x83\x96\xE6\xB8\x88\xE3\x81\xBF"},//已存档  アーカイブ済み
    {"EDIT", "\xE7\xB7\xA8\xE9\x9B\x86"},//编辑  編集
    {"DELETE", "\xE5\x89\x8A\xE9\x99\xA4"},//删除   削除
    {"RENAMED", "\xE5\x90\x8D\xE5\x89\x8D\xE3\x82\x92\xE5\xA4\x89\xE6\x9B\xB4"},//重命名  名前を変更
    {"DELETE!", "\xE5\x89\x8A\xE9\x99\xA4\xE3\x81\xAB\xE6\x88\x90\xE5\x8A\x9F\xE3\x81\x97\xE3\x81\xBE\xE3\x81\x97\xE3\x81\x9F\xEF\xBC\x81"},//删除成功！  削除に成功しました！
    {"MEMORY FULL,REPLACE", "\xE5\x86\x85\xE5\xAD\x98\xE5\xB7\xB2\xE6\xBB\xA1\xEF\xBC\x8C\xE6\x9B\xBF\xE6\x8D\xA2"},//内存已满，替换
    {"REPLACE", "\xE6\x9B\xBF\xE6\x8D\xA2"},//替换
    {"SETTING", "\xE8\xAE\xBE\xE7\xBD\xAE"},//设置
    {"LANGUAGE", "\xE8\xAF\xAD\xE8\xA8\x80"},//语言
    {"UNIT", "\xE5\x8D\x95\xE4\xBD\x8D"},//单位
    {"SOUND", "\xE5\xA3\xB0\xE9\x9F\xB3"},//声音
    {"DESCALE", "\xE9\x99\xA4\xE5\x9E\xA2"},//除垢
    {"SYSTEM CHECK", "\xE8\x87\xAA\xE6\xA3\x80"},//自检
    {"RESET ALL", "\xE7\xB3\xBB\xE7\xBB\x9F\xE9\x87\x8D\xE7\xBD\xAE"},//系统重置
    {"ENGLISH", "\xE8\x8B\xB1\xE8\xAF\xAD"},//英语
    {"CHINESE", "\xE4\xB8\xAD\xE6\x96\x87"},//中文
    {"METRIC", "\xE5\x85\xAC\xE5\x88\xB6"},//公制
    {"IMPERIAL", "\xE8\x8B\xB1\xE5\x88\xB6"},//英制
    {"ON", "\xE5\xBC\x80"},//开
    {"OFF", "\xE5\x85\xB3"},//关
		{"TEA", "\xE8\x8C\xB6"},//茶
		{"FINISHED", "\xE5\xB7\xB2\xE5\xAE\x8C\xE6\x88\x90"},//已完成
		{"TEA:WATER", "\xE8\x8C\xB6\xE6\xB0\xB4"},//茶水
		{"BREW LATER", "\xE5\xAE\x9A\xE6\x97\xB6\xE5\x86\xB2\xE6\xB3\xA1"},//定时冲泡
		{"ENJOY!", "\xE8\xAF\xB7\xE4\xBA\xAB\xE7\x94\xA8\xEF\xBC\x81"},//请享用
		{"PRESS TO START", "\xE5\x8F\xB3\xE9\x94\xAE\xE5\xBC\x80\xE5\xA7\x8B"},//右键开始
		{"RESET ALL?",         "\xE9\x87\x8D\xE7\xBD\xAE\xE6\x89\x80\xE6\x9C\x89\xE8\xAE\xBE\xE7\xBD\xAE\xEF\xBC\x9F"},  // 重置所有设置？
		{"THIS WILL ERASE",    "\xE6\xAD\xA4\xE6\x93\x8D\xE4\xBD\x9C\xE5\xB0\x86\xE5\x88\xA0\xE9\x99\xA4"},          // 此操作将删除
		{"ALL SETTINGS",       "\xE6\x89\x80\xE6\x9C\x89\xE8\xAE\xBE\xE7\xBD\xAE"},      // 所有设置
		{"HOLD TO CONFIRM",    "\xE9\x95\xBF\xE6\x8C\x89\xE7\xA1\xAE\xE8\xAE\xA4"},      // 长按确认
		{"PREPARING", "\xE5\x87\x86\xE5\xA4\x87\xE4\xB8\xAD"},//准备中
		{"WARN!", "\xE8\xAD\xA6\xE5\x91\x8A\xEF\xBC\x81"},//警告！
		{"WATER LOW", "\xE6\xB0\xB4\xE9\x87\x8F\xE8\xBF\x87\xE4\xBD\x8E"},//水量过低
		{"PLEASE REFILL", "\xE8\xAF\xB7\xE5\x86\x8D\xE6\xAC\xA1\xE6\xB3\xA8\xE6\xB0\xB4"},//请再次注水
		{"ERROR!", "\xE9\x94\x99\xE8\xAF\xAF\xEF\xBC\x81"},//错误！
		{"CONTACT CUSTOMER SERVICE", "\xE8\xAF\xB7\xE8\x81\x94\xE7\xB3\xBB\xE5\xAE\xA2\xE6\x9C\x8D"},//请联系客服
		
};

// 获取中文字符在字库中的索引
int Get_Hzk_Index(const char* hanzi) {
    for (int i = 0; i < sizeof(Hzk_Table)/sizeof(Hzk_Table[0]); i++) {
        if (memcmp(hanzi, Hzk_Table[i], 3) == 0) {
            return i;
        }
    }
    return -1;  // 未找到对应字符
}

// 查找英文对应的中文翻译
const char* FindChinese(const char* english) {
    for (int i = 0; chineseMap[i].english != NULL; i++) {
        if (strcmp(english, chineseMap[i].english) == 0) {
            return chineseMap[i].chinese;
        }
    }
    return NULL;  // 没找到对应的中文
}

// 显示字符函数，支持中英文
void OLED_ShowChar(u8 x, u8 y, u16 chr, u8 size1, u8 mode, u8 isChinese) {
    u8 i, m, temp, size2;
    u8 x0 = x, y0 = y;
    
    if (isChinese) {
        // 中文显示处理
        u16 size3 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * size1;
        
        // 目前仅支持16x16中文字体
        if (size1 != 16) return;
        
        // 使用chr作为Hzk1数组的索引
        if (chr >= sizeof(Hzk)/sizeof(Hzk[0])) return;
        
        for (i = 0; i < size3; i++) {
            temp = Hzk[chr][i];
            
            for (m = 0; m < 8; m++) {
                if (temp & 0x01)
                    OLED_DrawPoint(x, y, mode);
                else
                    OLED_DrawPoint(x, y, !mode);
                
                temp >>= 1;
                y++;
            }
            
            x++;
            
            if ((x - x0) == size1) {
                x = x0;
                y0 += 8;
            }
            
            y = y0;
        }
    } else {
        // 英文显示处理
		if(chr == '!' || chr == ':'){
				x = x + 3;
		}
				
        if (size1 == 8) size2 = 6;
        else size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2);
        
        u8 chr1 = chr - ' ';
        
        for (i = 0; i < size2; i++) {
            if (size1 == 16)
                temp = asc2_1608[chr1][i];
            else if (size1 == 20)
                temp = asc2_2010[chr1][i];
            else if (size1 == 24)
                temp = asc2_2412[chr1][i];
            else if (size1 == 28)
                temp = asc2_2814[chr1][i];
            else return;
            
            for (m = 0; m < 8; m++) {
                if (temp & 0x01) OLED_DrawPoint(x, y, mode);
                else OLED_DrawPoint(x, y, !mode);
                temp >>= 1;
                y++;
            }
            
            x++;
            
            if ((size1 != 8) && ((x - x0) == size1 / 2)) {
                x = x0;
                y0 += 8;
            }
            
            y = y0;
        }
    }
}

//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
//mode:0,反色显示;1,正常显示
//void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
//{
//	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
//	{
//		OLED_ShowChar(x,y,*chr,size1,mode);
//		if(size1==8)x+=6;
//		else x+=size1/2;
//		chr++;
//  }
//}

void OLED_ShowString(u8 x, u8 y, const char *str, u8 size, u8 mode, u8 lang) 
{
    const char *displayStr = str;
    
    // 中文模式：尝试翻译
    if (lang == 1) {
        const char* chinese = FindChinese(displayStr);
        if (chinese != NULL) {
            displayStr = chinese;
        }
    }

    // 统一显示处理
    const char *p = displayStr;
    while (*p != '\0') {
        // 中文处理
        if (lang == 1 && (*p & 0xE0) == 0xE0) {
            int index = Get_Hzk_Index(p);
            if (index >= 0) {
                OLED_ShowChar(x, y, index, size, mode, 1);
                x += size;
                p += 3;
                continue;
            }
        }
        
        // 英文处理
        OLED_ShowChar(x, y, *p, size, mode, 0);
        x += (size == 8) ? 6 : (size / 2);
        p++;
    }
}



void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str, u8 size, u8 mode, u8 lang)
{
//
    // 如果需要显示中文且输入是英文，先查找对应的中文字符串
    const u8 *displayStr = str;
    u16 charCount = 0;
    
    if (lang == 1) {
        int i;
        for (i = 0; i < (sizeof(chineseMap)/sizeof(chineseMap[0])); i++) {
            if (strcmp((const char*)str, chineseMap[i].english) == 0) {
                displayStr = (u8*)chineseMap[i].chinese;
                break;
            }
        }
        
        // 计算中文字符数量
        const u8 *p = displayStr;
        while (*p != '\0') {
            if (*p & 0xE0) {
                charCount++;
                p += 3;
            } else {
                p++;
            }
        }
        
        // 根据字符数计算起始位置
        u16 x1;
        switch(charCount) {
            case 1: x1 = 72; break;    // 1字: 72
            case 2: x1 = 64; break;    // 2字: 64,80
            case 3: x1 = 56; break;    // 3字: 56,72,88
            case 4: x1 = 48; break;    // 4字: 48,64,80,96
						case 5: x1 = 40; break;    // 5字: 40,48,64,80,96
						case 6: x1 = 32; break;    // 6字: 32,40,48,64,80,96
						case 7: x1 = 24; break;    
						case 8: x1 = 16; break;    
            default: 
                // 其他情况使用通用计算
                x1 = (160 - (charCount * 16 + (charCount-1)*16)) / 2;
                break;
        }
        
        OLED_ShowString(x1, y, displayStr, size, mode, lang);
        return;
    }
    
    // 英文处理保持不变
    u16 len = 0;
    const u8 *p = displayStr;
    while (*p != '\0') {
        len += (size == 8) ? 6 : (size / 2);
        p++;
    }
    u16 x1 = (160 - len) / 2;
    OLED_ShowString(x1, y, displayStr, size, mode, lang);
	
//	    // 如果需要显示中文且输入是英文，先查找对应的中文字符串
//    const u8 *displayStr = str;
//    if (lang == 1) {
//			int i ;
//        for (i = 0; i < (sizeof(chineseMap)/sizeof(chineseMap[0])); i++) {
//            if (strcmp((const char*)str, chineseMap[i].english) == 0) {
//                displayStr = (u8*)chineseMap[i].chinese;
//                break;
//            }
//        }
//    }

//    // 计算字符串显示长度
//    u16 len = 0;
//    const u8 *p = displayStr;
//    
//    while (*p != '\0') {
//        if (lang == 1 && (*p & 0xE0) == 0xE0) {
//            // 中文字符，占2个英文字符宽度
//            len += size * 2;
//            p += 3;
//        } else {
//            // 英文字符
//            len += (size == 8) ? 6 : (size / 2);
//            p++;
//        }
//    }
//    
//    // 计算居中位置并显示
//    u16 x1 = (160 - len) / 2;
//    OLED_ShowString(x1, y, displayStr, size, mode, lang);
		
		
		
    // 计算字符串显示长度
//    u16 len = 0;
//    const char *p = (const char *)str;
//    
//    while (*p != '\0') {
//        if (lang == 1 && (*p & 0xE0) == 0xE0) {
//            len += size;
//            p += 3;
//        } else {
//            len += (size == 8) ? 6 : (size / 2);
//            p++;
//        }
//    }
//    
//    // 计算居中位置并显示
//    u16 x1 = (160 - len) / 2;
//		if(lang == 1)x1 += 8;
//    OLED_ShowString(x1, y, str, size, mode, lang);
}

//m^n
u32 OLED_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

//显示数字
//x,y :起点坐标
//num :要显示的数字
//len :数字的位数
//size:字体大小
//mode:0,反色显示;1,正常显示
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode)
{
	u8 t,temp,m=0;
	if(size1==8)m=2;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode,0);
      }
			else 
			{
			  OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode,0);
			}
  }
}



////显示汉字
////x,y:起点坐标
////num:汉字对应的序号
////mode:0,反色显示;1,正常显示
//void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
//{
//	u8 m,temp;
//	u8 x0=x,y0=y;
//	u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //得到字体一个字符对应点阵集所占的字节数
//	for(i=0;i<size3;i++)
//	{
////		if(size1==16)
////				{temp=Hzk1[num][i];}//调用16*16字体
////		else if(size1==24)
////				{temp=Hzk2[num][i];}//调用24*24字体
////		else if(size1==32)       
////				{temp=Hzk3[num][i];}//调用32*32字体
////		else if(size1==64)
////				{temp=Hzk4[num][i];}//调用64*64字体
////		else return;
//		for(m=0;m<8;m++)
//		{
//			if(temp&0x01)OLED_DrawPoint(x,y,mode);
//			else OLED_DrawPoint(x,y,!mode);
//			temp>>=1;
//			y++;
//		}
//		x++;
//		if((x-x0)==size1)
//		{x=x0;y0=y0+8;}
//		y=y0;
//	}
//}

// 显示汉字函数
// x, y     : 起始坐标（左上角）
// num      : 汉字在 Hzk1 数组中的索引
// size1    : 字体大小，目前仅支持 16（即16x16）
// mode     : 显示模式，0=反色，1=正常显示

extern const unsigned char Hzk1[][32];

void OLED_ShowChinese(u8 x, u8 y, u8 num, u8 size1, u8 mode)
{
    u8 i, m, temp;
    u8 x0 = x, y0 = y;
    u16 size3 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * size1;  // 计算一个汉字所需的字节数

    // 安全检查：目前仅支持 16x16 字模
    if (size1 != 16) return;

    for (i = 0; i < size3; i++)
    {
        temp = Hzk[num][i];  // 获取当前字节点阵

        for (m = 0; m < 8; m++)  // 每字节8位，对应纵向8个像素
        {
            if (temp & 0x01)
                OLED_DrawPoint(x, y, mode);     // 画点
            else
                OLED_DrawPoint(x, y, !mode);    // 反色模式下清点

            temp >>= 1;
            y++;
        }

        x++;

        // 每绘制完一行 size1 宽度后换页（即16列后开始下一页）
        if ((x - x0) == size1)
        {
            x = x0;
            y0 += 8;
        }

        y = y0;
    }
}


//num 显示汉字的个数
//space 每一遍显示的间隔
//mode:0,反色显示;1,正常显示
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
			OLED_ShowChinese(128,24,t,16,mode); //写入一个汉字保存在OLED_GRAM[][]数组中
			t++;
		}
		if(t==num)
		{
			for(r=0;r<16*space;r++)      //显示间隔
			{
				for(i=1;i<144;i++)
				{
					for(n=0;n<8;n++)
					{
						OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
					}
				}
				OLED_Refresh();
			}
			t=0;
		}
		m++;
		if(m==16){m=0;}
		for(i=1;i<144;i++)   //实现左移
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

//x,y：起点坐标
//sizex,sizey,图片长宽
//BMP[]：要写入的图片数组
//mode:0,反色显示;1,正常显示
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode)
{
	u16 j=0;
	u8 i,n,temp,m;
	u8 x0=x,y0=y;
	sizey=sizey/8+((sizey%8)?1:0);
	for(n=0;n<sizey;n++)
	{
		for(i=0;i<sizex;i++)
		{
			temp=BMP[j];
			j++;
			for(m=0;m<8;m++)
			{
				if(temp&0x01)OLED_DrawPoint(x,y,mode);
				else OLED_DrawPoint(x,y,!mode);
				temp>>=1;
				y++;
			}
			x++;
			if((x-x0)==sizex)
			{
				x=x0;
				y0=y0+8;
			}
			y=y0;
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
void OLED_GPIOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	OLED_RES_GPIO_CLK_ENABLE();
	OLED_DC_GPIO_CLK_ENABLE();
	OLED_CS_GPIO_CLK_ENABLE();
	SPI1_SCK_GPIO_CLK_ENABLE();
	SPI1_MOSI_GPIO_CLK_ENABLE();

	GPIO_InitStructure.Pin		= SPI1_SCK_GPIO_PIN;			//PB5 SDIN
	GPIO_InitStructure.Mode		= GPIO_MODE_OUTPUT_PP;	//推挽输出
	GPIO_InitStructure.Speed	= GPIO_SPEED_HIGH;	//高速
	HAL_GPIO_Init(SPI1_SCK_GPIO_PORT, &GPIO_InitStructure);			//初始化
	OLED_SCL_Set();
	
	GPIO_InitStructure.Pin		= SPI1_MOSI_GPIO_PIN;			//PB5 SDIN
	GPIO_InitStructure.Mode		= GPIO_MODE_OUTPUT_PP;	//推挽输出
	GPIO_InitStructure.Speed	= GPIO_SPEED_HIGH;	//高速
	HAL_GPIO_Init(SPI1_MOSI_GPIO_PORT, &GPIO_InitStructure);			//初始化
	OLED_SDA_Set();
	
	GPIO_InitStructure.Pin		= OLED_CS_GPIO_PIN;			//PB5 SDIN
	GPIO_InitStructure.Mode		= GPIO_MODE_OUTPUT_PP;	//推挽输出
	GPIO_InitStructure.Speed	= GPIO_SPEED_HIGH;	//高速
	HAL_GPIO_Init(OLED_CS_GPIO_PORT, &GPIO_InitStructure);			//初始化
	OLED_CS_Set();
	
	GPIO_InitStructure.Pin		= OLED_RST_GPIO_PIN;			//PB5 SDIN
	GPIO_InitStructure.Mode		= GPIO_MODE_OUTPUT_PP;	//推挽输出
	GPIO_InitStructure.Speed	= GPIO_SPEED_HIGH;	//高速
	HAL_GPIO_Init(OLED_RST_GPIO_PORT, &GPIO_InitStructure);			//初始化
	OLED_RES_Set();
	
	GPIO_InitStructure.Pin		= OLED_DC_GPIO_PIN;			//PB5 SDIN
	GPIO_InitStructure.Mode		= GPIO_MODE_OUTPUT_PP;	//推挽输出
	GPIO_InitStructure.Speed	= GPIO_SPEED_HIGH;	//高速
	HAL_GPIO_Init(OLED_DC_GPIO_PORT, &GPIO_InitStructure);			//初始化
	OLED_DC_Set();
}

//OLED的初始化
void OLED_Init(void)
{
//	SPI1_Init();
	
	OLED_GPIOInit();
		
	OLED_RES_Clr();
	delay_ms(200);
	OLED_RES_Set();
	
	OLED_WR_Byte(0xae,OLED_CMD);//Display OFF(sleep mode) 
	OLED_WR_Byte(0x81,OLED_CMD);//Set Contrast Control //可以调节屏幕亮度
	OLED_WR_Byte(0xd0,OLED_CMD); 
	OLED_WR_Byte(0xa4,OLED_CMD);//Set Entire Display OFF/ON 
	OLED_WR_Byte(0xa6,OLED_CMD);//Set Normal/Reverse Display 
	OLED_WR_Byte(0xa9,OLED_CMD);//Display Resolution Control 
	OLED_WR_Byte(0x02,OLED_CMD); 
	OLED_WR_Byte(0xad,OLED_CMD);//DC-DC Control Mode Set 
	OLED_WR_Byte(0x80,OLED_CMD); 
	if(USE_HORIZONTAL==0)
	 {
		OLED_WR_Byte(0xC0,OLED_CMD);//正向显示
		OLED_WR_Byte(0xA0,OLED_CMD);
	 }
	else if(USE_HORIZONTAL==1)
	{
		OLED_WR_Byte(0xC8,OLED_CMD);//旋转180度
		OLED_WR_Byte(0xA1,OLED_CMD);
	}
	else if(USE_HORIZONTAL==2)
	{
		OLED_WR_Byte(0xC0,OLED_CMD);//旋转270度
		OLED_WR_Byte(0xA1,OLED_CMD);
	}
	else
	{
		OLED_WR_Byte(0xC8,OLED_CMD);//旋转90度
		OLED_WR_Byte(0xA0,OLED_CMD);
	}
	OLED_WR_Byte(0xd5,OLED_CMD);//Set Display Clock Divide Ratio/Oscillator Frequency 
	OLED_WR_Byte(0x40,OLED_CMD); 
	OLED_WR_Byte(0xd9,OLED_CMD);//Dis-charge/Pre-charge Period Mode Set 
	OLED_WR_Byte(0x2f,OLED_CMD); 
	OLED_WR_Byte(0xdb,OLED_CMD);//Set VCOMH Deselect Level 
	OLED_WR_Byte(0x3f,OLED_CMD); 
	OLED_WR_Byte(0x20,OLED_CMD);//Page addressing mode 
	OLED_WR_Byte(0xdc,OLED_CMD);//VSEGM Deselect Level Mode Set 
	OLED_WR_Byte(0x35,OLED_CMD); 
	OLED_WR_Byte(0x30,OLED_CMD);//Set Discharge VSL Level 
	OLED_Clear(); 
	OLED_WR_Byte(0xaf,OLED_CMD);//Set Display ON
}

