#include "MY_DS1302.h"
#include "My_OLED.h"


void __GPIO_SET(int temp)
{
	GPIO_InitTypeDef GPIO_InitStruct_0 = {0};
	if(temp)
	{
		GPIO_InitStruct_0.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct_0.Pin = DS1302_DAT_PIN;
		GPIO_InitStruct_0.Pull = GPIO_NOPULL;
	}
	else
	{
		GPIO_InitStruct_0.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct_0.Pin = DS1302_DAT_PIN;
		GPIO_InitStruct_0.Pull = GPIO_NOPULL;
		GPIO_InitStruct_0.Speed = GPIO_SPEED_HIGH;
	}
	HAL_GPIO_Init(DS1302_DAT_PORT, &GPIO_InitStruct_0);
}


void MY_DS1302_START(void)
{
	HAL_GPIO_WritePin(DS1302_CLK_PORT, DS1302_CLK_PIN, GPIO_PIN_RESET);
	MY_Delay_us(5);
	HAL_GPIO_WritePin(DS1302_RST_PORT, DS1302_RST_PIN, GPIO_PIN_SET);
	MY_Delay_us(5);
}


void MY_DS1302_END(void)
{
	HAL_GPIO_WritePin(DS1302_RST_PORT, DS1302_RST_PIN, GPIO_PIN_RESET);
}


void MY_DS1302_SendBit(uint8_t temp)
{
	__GPIO_SET(0);

	if(temp)
		HAL_GPIO_WritePin(DS1302_DAT_PORT, DS1302_DAT_PIN, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DS1302_DAT_PORT, DS1302_DAT_PIN, GPIO_PIN_RESET);
	
	MY_Delay_us(2);
	
	HAL_GPIO_WritePin(DS1302_CLK_PORT, DS1302_CLK_PIN, GPIO_PIN_SET);
	MY_Delay_us(2);
	HAL_GPIO_WritePin(DS1302_DAT_PORT, DS1302_DAT_PIN, GPIO_PIN_RESET);
	MY_Delay_us(1);
	
	HAL_GPIO_WritePin(DS1302_CLK_PORT, DS1302_CLK_PIN, GPIO_PIN_RESET);
	MY_Delay_us(2);
}


uint8_t MY_DS1302_ReadBit(void)
{
	uint8_t temp;
	__GPIO_SET(1);
	
	temp = HAL_GPIO_ReadPin(DS1302_DAT_PORT, DS1302_DAT_PIN);
	HAL_GPIO_WritePin(DS1302_CLK_PORT, DS1302_CLK_PIN, GPIO_PIN_SET);
	MY_Delay_us(2);
	HAL_GPIO_WritePin(DS1302_CLK_PORT, DS1302_CLK_PIN, GPIO_PIN_RESET);
	MY_Delay_us(2);
	return temp;
}


//Send or receive data in little-endian order
void MY_DS1302_SendByte(uint8_t temp)
{
	
	for(int a=0; a<8; a++)
	{
		MY_DS1302_SendBit(temp&0x01);
		temp = temp >> 1;
	}
	MY_Delay_us(1);
}


uint8_t MY_DS1302_ReadByte(void)
{
	uint8_t temp=0;
	for(int a=0; a<8; a++)
	{
		temp |= MY_DS1302_ReadBit() << a;
	}
	return temp;
}


void MY_DS1302_WriteSecond(uint8_t data)
{
	uint8_t temp = 0;
	temp = ((((data/10)%10)&0x07)<<4) | (data%10);
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_SECOND_ADDRESS);
	MY_DS1302_SendByte(temp);
	
	MY_DS1302_END();
}


uint8_t MY_DS1302_ReadSecond(void)
{
	uint8_t temp=0;
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_SECOND_ADDRESS | 0x01);
	temp = MY_DS1302_ReadByte();
	
	MY_DS1302_END();
	
	return (((temp>>4)&0x07)*10)+(temp&0x0F);
}


void MY_DS1302_WriteMinute(uint8_t data)
{
	uint8_t temp = 0;
	temp = ((((data/10)%10)&0x07)<<4) | (data%10);
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_MINUTE_ADDRESS);
	MY_DS1302_SendByte(temp);
	
	MY_DS1302_END();
}


uint8_t MY_DS1302_ReadMinute(void)
{
	uint8_t temp=0;
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_MINUTE_ADDRESS | 0x01);
	temp = MY_DS1302_ReadByte();
	
	MY_DS1302_END();
	
	return (((temp>>4)&0x07)*10)+(temp&0x0F);
}


void MY_DS1302_WriteHour24(uint8_t data)
{
	uint8_t temp = 0;
	temp = (((data/10)%10)<<4) | (data%10);
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_HOUR_ADDRESS);
	MY_DS1302_SendByte(temp);
	
	MY_DS1302_END();
}


uint8_t MY_DS1302_ReadHour24(void)
{
	uint8_t temp=0;
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_HOUR_ADDRESS | 0x01);
	temp = MY_DS1302_ReadByte();
	
	MY_DS1302_END();
	
	return (((temp>>4)&0x03)*10)+(temp&0x0F);
}


void MY_DS1302_WriteYear(uint8_t data)
{
	uint8_t temp = 0;
	temp = (((data/10)%10)<<4) | (data%10);
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_YEAR_ADDRESS);
	MY_DS1302_SendByte(temp);
	
	MY_DS1302_END();
}


uint8_t MY_DS1302_ReadYear(void)
{
	uint8_t temp=0;
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_YEAR_ADDRESS | 0x01);
	temp = MY_DS1302_ReadByte();
	
	MY_DS1302_END();
	
	return (((temp>>4)&0x03)*10)+(temp&0x0F);
}


void MY_DS1302_WriteMonth(uint8_t data)
{
	uint8_t temp = 0;
	temp = ((((data/10)%10)&0x1)<<4) | (data%10);
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_MONTH_ADDRESS);
	MY_DS1302_SendByte(temp);
	
	MY_DS1302_END();
}


uint8_t MY_DS1302_ReadMonth(void)
{
	uint8_t temp=0;
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_MONTH_ADDRESS | 0x01);
	temp = MY_DS1302_ReadByte();
	
	MY_DS1302_END();
	
	return (((temp>>4)&0x01)*10)+(temp&0x0F);
}


void MY_DS1302_WriteDay(uint8_t data)
{
	uint8_t temp = 0;
	temp = ((((data/10)%10)&0x3)<<4) | (data%10);
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_DAY_ADDRESS);
	MY_DS1302_SendByte(temp);
	
	MY_DS1302_END();
}


uint8_t MY_DS1302_ReadDay(void)
{
	uint8_t temp=0;
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_DAY_ADDRESS | 0x01);
	temp = MY_DS1302_ReadByte();
	
	MY_DS1302_END();
	
	return (((temp>>4)&0x03)*10)+(temp&0x0F);
}


void MY_DS1302_WriteWeek(uint8_t data)
{
	uint8_t temp = 0;
	temp = (data%10);
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_WEEK_ADDRESS);
	MY_DS1302_SendByte(temp);
	
	MY_DS1302_END();
}


uint8_t MY_DS1302_ReadWeek(void)
{
	uint8_t temp=0;
	
	MY_DS1302_START();
	
	MY_DS1302_SendByte(DS1302_WEEK_ADDRESS | 0x01);
	temp = MY_DS1302_ReadByte();
	
	MY_DS1302_END();
	
	return temp;
}


void MY_DS1302_Init(MY_DS1302_Params temp)
{
	HAL_GPIO_WritePin(DS1302_RST_PORT, DS1302_RST_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DS1302_CLK_PORT, DS1302_CLK_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DS1302_DAT_PORT, DS1302_DAT_PIN, GPIO_PIN_RESET);
	
	MY_DS1302_WriteYear(temp.Year);
	MY_DS1302_WriteMonth(temp.Month);
	MY_DS1302_WriteDay(temp.Day);
	MY_DS1302_WriteWeek(temp.Week);
	MY_DS1302_WriteHour24(temp.Hour);
	MY_DS1302_WriteMinute(temp.Minute);
	MY_DS1302_WriteSecond(temp.Second);
}

