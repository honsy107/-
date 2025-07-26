#ifndef __MY_DS1302_H__
#define __MY_DS1302_H__

#include "main.h"
#include "MY_UT.h"

#define DS1302_RST_PORT  GPIOA
#define DS1302_RST_PIN   GPIO_PIN_8
#define DS1302_CLK_PORT  GPIOB
#define DS1302_CLK_PIN   GPIO_PIN_10
#define DS1302_DAT_PORT  GPIOB
#define DS1302_DAT_PIN   GPIO_PIN_11

#define DS1302_SECOND_ADDRESS		0x80
#define DS1302_MINUTE_ADDRESS		0x82
#define DS1302_HOUR_ADDRESS			0x84
#define DS1302_DAY_ADDRESS			0x86
#define DS1302_MONTH_ADDRESS		0x88
#define DS1302_WEEK_ADDRESS			0x8A
#define DS1302_YEAR_ADDRESS			0x8C

typedef struct {
    uint8_t Year;
    uint8_t Month;
    uint8_t Day;
    uint8_t Week;
    uint8_t Hour;
    uint8_t Minute;
    uint8_t Second;
} MY_DS1302_Params;

//void __GPIO_SET(int temp);
//void MY_DS1302_SendBit(uint8_t temp);
//uint8_t MY_DS1302_ReadBit(void);
//void MY_DS1302_SendByte(uint8_t temp);
//uint8_t MY_DS1302_ReadByte(void);

void MY_DS1302_WriteSecond(uint8_t data);
uint8_t MY_DS1302_ReadSecond(void);
void MY_DS1302_WriteMinute(uint8_t data);
uint8_t MY_DS1302_ReadMinute(void);
void MY_DS1302_WriteHour24(uint8_t data);
uint8_t MY_DS1302_ReadHour24(void);
void MY_DS1302_WriteYear(uint8_t data);
uint8_t MY_DS1302_ReadYear(void);
void MY_DS1302_WriteMonth(uint8_t data);
uint8_t MY_DS1302_ReadMonth(void);
void MY_DS1302_WriteDay(uint8_t data);
uint8_t MY_DS1302_ReadDay(void);
void MY_DS1302_WriteWeek(uint8_t data);
uint8_t MY_DS1302_ReadWeek(void);


#endif
