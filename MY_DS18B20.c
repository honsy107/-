#include "MY_DS18B20.h"


/*毫秒级别延迟*/
void MY_Delay_us(int temp)
{
	volatile int delay_us_temp = temp*5;
	while(delay_us_temp--);
}

/*设置通信引脚的输入输出状态*/
void __PORT_SET(int cs)
{
	GPIO_InitTypeDef GPIO_InitStruct_0;
	GPIO_InitStruct_0.Speed = GPIO_SPEED_FREQ_HIGH;
	if(cs == 0)
	{
		GPIO_InitStruct_0.Pin = DS18B20_Pin;
		GPIO_InitStruct_0.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct_0.Pull = GPIO_NOPULL;
		HAL_GPIO_WritePin(DS18B20_Port, DS18B20_Pin, GPIO_PIN_RESET);
	}
	else
	{
		GPIO_InitStruct_0.Pin = DS18B20_Pin;
		GPIO_InitStruct_0.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct_0.Pull = GPIO_NOPULL;
	}
	HAL_GPIO_Init(DS18B20_Port, &GPIO_InitStruct_0);
}


/*对DS18B20的复位信号*/
int MY_DS18B20_RESET(void)
{
	__PORT_SET(0);
	MY_Delay_us(480);
	__PORT_SET(1);
	MY_Delay_us(60);
	if(!HAL_GPIO_ReadPin(DS18B20_Port, DS18B20_Pin))
	{
		MY_Delay_us(470);
		return 1;
	}
	else
		return 0;
}

/*主机对从机通信*/
void MY_DS18B20_SEND(uint8_t data)
{
	__PORT_SET(1);
//	HAL_GPIO_WritePin(DS18B20_Port, DS18B20_Pin, GPIO_PIN_SET);
	for(int b=0; b<8; b++)
	{
		if((data>>b)&0x01)
		{
			__PORT_SET(0);
			MY_Delay_us(2);
			__PORT_SET(1);
//			HAL_GPIO_WritePin(DS18B20_Port, DS18B20_Pin, GPIO_PIN_SET);
			MY_Delay_us(70);
		}
		else
		{
			__PORT_SET(0);
//			HAL_GPIO_WritePin(DS18B20_Port, DS18B20_Pin, GPIO_PIN_RESET);
			MY_Delay_us(72);
		}
		__PORT_SET(1);
//		HAL_GPIO_WritePin(DS18B20_Port, DS18B20_Pin, GPIO_PIN_SET);
	}
	__PORT_SET(1);
}

/*主机接受从机通信*/
uint8_t MY_DS18B20_READ(void)
{
	uint8_t temp = 0;
	__PORT_SET(1);
	for(int b=0; b<8; b++)
	{
		__PORT_SET(0);
		MY_Delay_us(2);
		__PORT_SET(1);
		MY_Delay_us(12);
		if(HAL_GPIO_ReadPin(DS18B20_Port, DS18B20_Pin))
			temp = temp|(0x01<<b);
//		else
//			temp |= 1;
		MY_Delay_us(45);
	}
	return temp;
}

/*获取DS18B20的温度信息*/
float MY_DS18B20_TR(void)
{
	uint8_t H=0, L=0;
	int u=1;
	float temp;
	
	if(!MY_DS18B20_RESET()) return -999;
	MY_DS18B20_SEND(0xCC);
	MY_DS18B20_SEND(0x44);
	while(!HAL_GPIO_ReadPin(DS18B20_Port, DS18B20_Pin));
	if(!MY_DS18B20_RESET()) return -999;
	MY_DS18B20_SEND(0xCC);
	MY_DS18B20_SEND(0xBE);

	L = MY_DS18B20_READ();
	H = MY_DS18B20_READ();
	if((H&0xF0) == 0xF0)
		u = -1;
	
	temp = ((H << 8) | L) * u * 0.0625f;
	
	return temp;
}



