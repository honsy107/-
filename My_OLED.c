
//基于STM32F103 HAL库运行自行打包的用于驱动OLED的一些函数。
#include "My_OLED.h"
//#include "stm32f1xx_hal_rtc.h"

#define OLED_address 0x78
HAL_StatusTypeDef Response__1;


//对OLED发送单个命令。
//uint8_t Command: 发送的十六进制命令。
void MY_OLED_Command(uint8_t Command)
{
	uint8_t Temp[2] = {0x00, Command};
	HAL_I2C_Master_Transmit(&hi2c1, OLED_address, Temp, 2, 1000);
}


//对OLED发送多个命令，请不要搞错数据类型。
//uint8_t Command[]: 发送的十六进制命令的数组。
//int Num: 数组长度。
void MY_OLED_CommandM(uint8_t Command[], int Num)
{
	uint8_t Temp[Num+1];
	Temp[0] = 0x00;
	for (int ii=0; ii<Num; ii++)
	{
		Temp[ii+1] = Command[ii];
	}
	HAL_I2C_Master_Transmit(&hi2c1, OLED_address, Temp, Num+1, 1000);
}


//对OLED发送单个数据，请注意是数据而不是命令。
//uint8_t Data: 发送的十六进制数据。
void MY_OLED_Data(uint8_t Data)
{
	uint8_t Temp[2] = {0x40, Data};
	HAL_I2C_Master_Transmit(&hi2c1, OLED_address, Temp, 2, 1000);
}


//对OLED发送多个数据。
//uint8_t Data[]: 发送的十六进制数据数组。
//int i: 数组长度。
void MY_OLED_DataM(uint8_t Data[], int Num)
{
	uint8_t Temp[Num+1];
	Temp[0] = 0x40;
	for (int ii=0; ii<Num; ii++)
	{
		Temp[ii+1] = Data[ii];
	}
	HAL_I2C_Master_Transmit(&hi2c1, OLED_address, Temp, Num+1, 1000);
}


//设置OLED的指针，跟标准库的原理一致。
//uint8_t x: 指针的X轴参数，取值 0 ~ 128 。
//uint8_t y: 指针的Y轴参数，取值 0 ~ 8 。
void MY_OLED_SetCursor(uint8_t x, uint8_t y)
{
	uint8_t Temp[3] = {0xB0|y, 0x10|((x&0xF0)>>4), 0x00|(x&0x0F)};
	MY_OLED_CommandM(Temp, 3);
}


//用单个数据填充整个屏幕，一般用于清屏或显示白屏。
//uint8_t data: 十六进制数据，如果想清屏取值 0x00 ,白屏取值 0xFF 。
void MY_OLED_Full(uint8_t data)
{
	uint8_t i, Temp[128];
	for(i=0; i<128; i++)
	{
		Temp[i] = data;
	}
	for(i=0; i<8; i++)
	{
		MY_OLED_SetCursor(0, i);
		MY_OLED_DataM(Temp, 128);
	}
}


//int i1: 字高(像素/8);
//int i2: 字宽;
//int temp1: OLED_ShowFout的行位置;
//int X: 显示在OLED屏上的横向坐标;
//int Y: 显示在OLED屏上的竖向坐标;
void MY_OLED_ShowD1(int i1, int i2, int temp1, int X, int Y)
{
	if(i2<16)
	{
		for(int i=0; i<i1; i++)
		{
			MY_OLED_SetCursor(X, Y+i);
			for (int j=0; j<i2; j++)
			{
				MY_OLED_Data(OLED_ShowFout[temp1][i*i2+j]);
			}
		}
	}
	else
	{
		for(int i=0; i<i1; i++)
		{
			MY_OLED_SetCursor(X, Y+i);
			for (int j=0; j<i2; j++)
			{
				MY_OLED_Data(OLED_ShowFout[temp1+i][j]);
			}
		}
	}
}


////int i1: 图片高度(像素/8);
////int i2: 图片长度;
////int temp1: OLED_ShowImg的位置;
////int X: 显示在OLED屏上的横向坐标;
////int Y: 显示在OLED屏上的竖向坐标;
//void MY_OLED_ShowDP(int i1, int i2, int temp1, int X, int Y)
//{
//	for(int i=0; i<i1; i++)
//	{
//		MY_OLED_SetCursor(X, Y+i);
//		for (int j=0; j<i2; j++)
//		{
//			MY_OLED_Data(OLED_ShowImg[i*i2+j+temp1]);
//		}
//	}
//}



//就是清屏，好玩封装一下。
void MY_OLED_Clear(void)
{  
	MY_OLED_Full(0x00);
}


//OLED初始化，请在使用其他函数前先使用此函数初始化OLED。
void MY_OLED_Init(void)
{
	uint8_t DA[23] = {0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0xA1, 0xC8, 0xDA, 0x12, 0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x30, 0xA4, 0xA6, 0x8D, 0x14, 0xAF};
	MY_OLED_CommandM(DA, 23);
	MY_OLED_Full(0);
}


//OLED显示英文。
//char StrE；要显示的字符串
//int X：显示到哪一列(1~16)。
//int Y：显示到哪一行(1~8)。
void MY_OLED_ShowSE(char StrE[], int X, int Y)
{
	for (int ii=0; StrE[ii]!='\0'; ii++)
	{
		MY_OLED_SetCursor((X+ii)*8, Y*2);
		for (int i1=0; i1<8; i1++)
		{
			MY_OLED_Data(OLED_ShowFout[StrE[ii] - ' '][i1]);
		}
		MY_OLED_SetCursor((X+ii)*8, Y*2+1);
		for (int i1=8; i1<16; i1++)
		{
			MY_OLED_Data(OLED_ShowFout[StrE[ii] - ' '][i1]);
		}
	}
}



//显示浮点数
//float num：要显示的浮点数;
//int size；显示的整数的位数，如果整数部分长度小于size会补'0'或' '，如果大于则正常显示浮点数;
//int sn：显示的小数部分的位数;
//int x：在OLED上的纵向定位;
//int y：在OLED上的横向定位;
//int o；选择整数部分填补何物，如果是0就补'0'，其他（1）则为' ';

void MY_OLED_ShowFS(float num, int size, int sn, int x, int y, int o)
{
	int num1=0, temp=0, tt=0, oo;
	int num_str[20]={0};
	
	if(o==0) oo=16;
	else oo=0;
	
	if(num>=0)
	{
		MY_OLED_ShowD1(2, 8, 11, x*8, y);
	}
	else
	{
		MY_OLED_ShowD1(2, 8, 13, x*8, y);
		num*=-1;
	}
	x+=1;
	num1 = num;
	
	for(int i=0; num1; i++)
	{
		num_str[i] = num1 % 10;
		num1/=10;
		temp = i+1;
	}

	if(size>temp)
	{
		tt = size;
		if(temp)
		{
			for(int i=0; i<tt-temp; i++)
			{
				MY_OLED_ShowD1(2, 8, oo, 8*(i+x), y);
			}
			for(int i=0; i<temp; i++)
			{
				MY_OLED_ShowD1(2, 8, num_str[temp-1-i]+16, (x+tt-temp+i)*8, y);
			}
		}
		else
		{
			for(int i=0; i<tt-temp-1; i++)
			{
				MY_OLED_ShowD1(2, 8, oo, 8*(i+x), y);
			}
			MY_OLED_ShowD1(2, 8, 16, (tt+x-1)*8, y);
		}
	}
	
	else
	{
		tt = temp;
		for(int i=0; i<tt; i++)
		{
			MY_OLED_ShowD1(2, 8, num_str[tt-1-i]+16, (x+i)*8, y);
		}
	}
	
	if(sn)
	{
		MY_OLED_ShowD1(2, 8, 14, (x+tt)*8, y);
		
		temp = 0;
		for(int i=0,k=10; i<sn; i++,k*=10)
		{
			num1 = num*k;
			num_str[i] = num1%10;
		}
		for(int i=0; i<sn; i++)
		{
			MY_OLED_ShowD1(2, 8, num_str[i]+16, (i+tt+1+x)*8, y);
		}
	}
	else
	{
		return;
	}
}


const uint8_t OLED_ShowFout[][16]=
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//  0
	
	0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,//! 1
	
	0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//" 2
	
	0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,//# 3
	
	0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,//$ 4
	
	0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,//% 5
	
	0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,//& 6
	
	0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//' 7
	
	0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,//( 8
	
	0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,//) 9
	
	0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,//* 10
	
	0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,//+ 11
	
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,//, 12
	
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//- 13
	
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,//. 14
	
	0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,/// 15
	
	0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,//0 16
	
	0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//1 17
	
	0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//2 18
	
	0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,//3 19
	
	0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,//4 20
	
	0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,//5 21
	
	0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,//6 22
	
	0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,//7 23
	
	0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,//8 24
	
	0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,//9 25
	
	0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,//: 26
	
	0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,//; 27
	
	0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,//< 28
	
	0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,//= 29
	
	0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,//> 30
	
	0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,//? 31
	
	0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,//@ 32
	
	0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,//A 33
	
	0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,//B 34
	
	0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,//C 35
	
	0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,//D 36
	
	0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,//E 37
	
	0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,//F 38
	
	0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//G 39
	
	0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,//H 40
	
	0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//I 41
	
	0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,//J 42
	
	0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,//K 43
	
	0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,//L 44
	
	0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,//M 45
	
	0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,//N 46
	
	0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,//O 47
	
	0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,//P 48
	
	0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,//Q 49
	
	0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,//R 50
	
	0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,//S 51
	
	0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//T 52
	
	0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//U 53
	
	0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,//V 54
	
	0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,//W 55
	
	0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,//X 56
	
	0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//Y 57
	
	0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,//Z 58
	
	0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,//[ 59
	
	0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,//\ 60
	
	0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,//] 61
	
	0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//^ 62
	
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//_ 63
	
	0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//` 64
	
	0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,//a 65
	
	0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,//b 66
	
	0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,//c 67
	
	0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,//d 68
	
	0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,//e 69
	
	0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//f 70
	
	0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//g 71
	
	0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//h 72
	
	0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//i 73
	
	0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,//j 74
	
	0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,//k 75
	
	0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//l 76
	
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,//m 77
	
	0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//n 78
	
	0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//o 79
	
	0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,//p 80
	
	0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,//q 81
	
	0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,//r 82
	
	0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,//s 83
	
	0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,//t 84
	
	0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//u 85
	
	0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,//v 86
	
	0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,//w 87
	
	0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,//x 88
	
	0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,//y 89
	
	0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,//z 90
	
	0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,//{ 91
	
	0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,//| 92
	
	0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,//} 93
	
	0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//~ 94
};

