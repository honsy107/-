#include "MY_MAX30102.h"
#include "My_OLED.h"


uint32_t red_data[200] = {0};
uint32_t ir_data[200] = {0};


void MY_MAX30102_SendByte(uint8_t address, uint8_t data)
{
	uint8_t temp[2];
	temp[0] = address;
	temp[1] = data;
	HAL_I2C_Master_Transmit(MY_MAX30102_HI2C, MY_MAX30102_W_ADDRESS, temp, 2, 1000);
}

void MY_MAX30102_ReadByte(uint8_t address, uint8_t *data, int size)
{
	uint8_t temp[1] = {address};
	HAL_I2C_Master_Transmit(MY_MAX30102_HI2C, MY_MAX30102_W_ADDRESS, temp, 1, 10000);
	HAL_I2C_Master_Receive(MY_MAX30102_HI2C, MY_MAX30102_R_ADDRESS, data, size, 1000);
}

/**/
void MY_MAX30102_Init(void)
{
	MY_MAX30102_SendByte(MY_MAX30102_IE1_ADDRESS, 0xF0);//C0
	MY_MAX30102_SendByte(MY_MAX30102_IE2_ADDRESS, 0x00);
	
	MY_MAX30102_SendByte(MY_MAX30102_FIFO_WP_ADDRESS, 0x00);
	MY_MAX30102_SendByte(MY_MAX30102_OFC_ADDRESS, 0x00);
	MY_MAX30102_SendByte(MY_MAX30102_FIFO_RP_ADDRESS, 0x00);
	
	MY_MAX30102_SendByte(MY_MAX30102_FIFO_C_ADDRESS, 0xE0);//0f
	MY_MAX30102_SendByte(MY_MAX30102_MC_ADDRESS, 0x03);//03
	MY_MAX30102_SendByte(MY_MAX30102_SPO2C_ADDRESS, 0x75);//27
	
	MY_MAX30102_SendByte(MY_MAX30102_LED1_PA_ADDRESS, 0x7F);//24
	MY_MAX30102_SendByte(MY_MAX30102_LED2_PA_ADDRESS, 0x7F);//24
	
	MY_MAX30102_SendByte(MY_MAX30102_LED_MCR21_ADDRESS, 0x00);//00
	MY_MAX30102_SendByte(MY_MAX30102_LED_MCR43_ADDRESS, 0x00);//00
	
	MY_MAX30102_SendByte(MY_MAX30102_DTC_ADDRESS, 0x00);
}


void MY_MAX30102_Init1(void)
{
	MY_MAX30102_SendByte(MY_MAX30102_IE1_ADDRESS, 0xC0);
	MY_MAX30102_SendByte(MY_MAX30102_IE2_ADDRESS, 0x00);
	
	MY_MAX30102_SendByte(MY_MAX30102_FIFO_WP_ADDRESS, 0x00);
	MY_MAX30102_SendByte(MY_MAX30102_OFC_ADDRESS, 0x00);
	MY_MAX30102_SendByte(MY_MAX30102_FIFO_RP_ADDRESS, 0x00);
	
	MY_MAX30102_SendByte(MY_MAX30102_FIFO_C_ADDRESS, 0x0F);
	MY_MAX30102_SendByte(MY_MAX30102_MC_ADDRESS, 0x03);
	MY_MAX30102_SendByte(MY_MAX30102_SPO2C_ADDRESS, 0x27);
	
	MY_MAX30102_SendByte(MY_MAX30102_LED1_PA_ADDRESS, 0x24);
	MY_MAX30102_SendByte(MY_MAX30102_LED2_PA_ADDRESS, 0x24);
	
	MY_MAX30102_SendByte(MY_MAX30102_LED_MCR21_ADDRESS, 0x00);
	MY_MAX30102_SendByte(MY_MAX30102_LED_MCR43_ADDRESS, 0x00);
	
	MY_MAX30102_SendByte(MY_MAX30102_DTC_ADDRESS, 0x00);
}


void MY_MAX30102_ReadFIFO(uint32_t *red, uint32_t *ir)
{
	uint8_t temp[6] = {0};
	MY_MAX30102_ReadByte(MY_MAX30102_IS1_ADDRESS, temp, 1);
	MY_MAX30102_SendByte(MY_MAX30102_FIFO_RP_ADDRESS, 0x00);
	
	MY_MAX30102_ReadByte(MY_MAX30102_FIFO_DR_ADDRESS, temp, 6);
	
	MY_MAX30102_SendByte(MY_MAX30102_FIFO_WP_ADDRESS, 0x00);
	MY_MAX30102_SendByte(MY_MAX30102_OFC_ADDRESS, 0x00);
//	MY_OLED_ShowFS(temp[0], 3, 0, 5, 2, 0);
//	MY_OLED_ShowFS(temp[1], 3, 0, 5, 4, 0);
//	MY_OLED_ShowFS(temp[2], 3, 0, 5, 6, 0);
//	MY_OLED_ShowFS(temp[3], 3, 0, 10, 2, 0);
//	MY_OLED_ShowFS(temp[4], 3, 0, 10, 4, 0);
//	MY_OLED_ShowFS(temp[5], 3, 0, 10, 6, 0);
	
	*red = 0;
	*ir = 0;
	*red=temp[0];	*red<<=8;	*red|=temp[1];	*red<<=8;	*red|=temp[2];	*red&=0x03FFFF;
	*ir=temp[3];	*ir<<=8;	*ir|=temp[4];	*ir<<=8;	*ir|=temp[5];	*ir&=0x03FFFF;
	MY_OLED_ShowFS(*ir, 6, 0, 0, 4, 0);
	MY_OLED_ShowFS(*red, 6, 0, 0, 6, 0);
}


void MY_MAX30102_FIFOToData(void)
{
	for(int a=100; a<200; a++)
	{
		uint8_t temp[1] = {0};
		while((temp[0]&0x40)!=0x40)
			MY_MAX30102_ReadByte(MY_MAX30102_IS1_ADDRESS, temp, 1);
		MY_MAX30102_ReadFIFO(&red_data[a], &ir_data[a]);
	}
}


void MY_MAX30102_DataToggle(void)
{
	for(int a=0; a<100; a++)
	{
		red_data[a] = red_data[a+100];
		ir_data[a] = ir_data[a+100];
		red_data[a+100] = 0;
		ir_data[a+100] = 0;
	}
}


void MY_MAX30102_Data_DCAC(uint32_t *DC, uint32_t *AC_min, uint32_t *AC_max)
{
	*AC_min=0x03FFFF; *AC_max=0;
	for(int a=0; a<100; a++)
	{
		if(*AC_min>ir_data[a])
			*AC_min = ir_data[a];
		if(*AC_max<ir_data[a])
			*AC_max = ir_data[a];
	}
}


uint32_t MY_MAX30102_DataAverage(void)
{
	int temp=0;
	for(int a=0; a<100; a++)
	{
		temp+=ir_data[a];
	}
	temp/=100;
	return temp;
}


uint32_t MY_MAX30102_HeartCalculate(uint32_t AC_min, uint32_t AC_max, uint32_t average)
{
	uint32_t MiddleLineM = (AC_max+AC_min)*2/3;
	uint32_t MiddleLineL = (AC_max+AC_min)*1/3;
//	uint32_t MiddleLine = (AC_max+AC_min)/2;
	uint32_t MiddleLine = average;
	uint32_t data_temp[500] = {0};
	uint32_t temp0 = 0;
	volatile uint32_t num = 0;
	
	for(int a=0; a<200; a+=5)
	{
		if(temp0==0)
		{
//			if(ir_data[a]*5>ir_data[a+1]+ir_data[a+2]+ir_data[a+3]+ir_data[a+4]+ir_data[a+5])
			if(ir_data[a]>MiddleLineM)
			{
				temp0 = 1;
				num++;
			}
		}
		else
		{
//			if(ir_data[a]*5<ir_data[a+1]+ir_data[a+2]+ir_data[a+3]+ir_data[a+4]+ir_data[a+5])
			if(ir_data[a]<MiddleLineL)
			{
				temp0 = 0;
			}
		}
	}
//	MY_OLED_ShowFS(MiddleLineM, 6, 0, 8, 4, 0);
//	MY_OLED_ShowFS(MiddleLineL, 6, 0, 8, 6, 0);
	MY_OLED_ShowFS(MiddleLine, 6, 0, 8, 6, 0);
	return num;
}


void MY_MAX30102_ReadIR(uint32_t *HR)
{
	uint32_t DC, temp;
	uint32_t AC_min;
	uint32_t AC_max;
	
	MY_MAX30102_FIFOToData();
	MY_MAX30102_DataToggle();
	MY_MAX30102_FIFOToData();
	MY_OLED_ShowSE("OK", 0, 0);
	temp = MY_MAX30102_DataAverage();
	MY_MAX30102_Data_DCAC(&DC, &AC_min, &AC_max);
	*HR = MY_MAX30102_HeartCalculate(AC_min, AC_max, temp);
}
