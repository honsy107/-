#ifndef __MY_MAX30102_H__
#define __MY_MAX30102_H__


#include "i2c.h"
#include "main.h"


#define MY_MAX30102_HI2C			&hi2c2
#define MY_MAX30102_INT_PORT	MAX30102_INT_GPIO_Port
#define MY_MAX30102_INT_PIN		MAX30102_INT_Pin

#define MY_MAX30102_R_ADDRESS					0xAF	//0b1010111
#define MY_MAX30102_W_ADDRESS					0xAE

#define MY_MAX30102_IS1_ADDRESS				0x00
#define MY_MAX30102_IS2_ADDRESS				0x01

#define MY_MAX30102_IE1_ADDRESS				0x02
#define MY_MAX30102_IE2_ADDRESS				0x03

#define MY_MAX30102_FIFO_WP_ADDRESS		0x04
#define MY_MAX30102_OFC_ADDRESS				0x05
#define MY_MAX30102_FIFO_RP_ADDRESS		0x06
#define MY_MAX30102_FIFO_DR_ADDRESS 	0x07

#define MY_MAX30102_FIFO_C_ADDRESS		0x08
#define MY_MAX30102_MC_ADDRESS				0x09
#define MY_MAX30102_SPO2C_ADDRESS			0x0A

#define MY_MAX30102_LED1_PA_ADDRESS		0x0C
#define MY_MAX30102_LED2_PA_ADDRESS		0x0D
#define MY_MAX30102_LED_MCR21_ADDRESS	0x11
#define MY_MAX30102_LED_MCR43_ADDRESS	0x12

#define MY_MAX30102_DTI_ADDRESS				0x1F
#define MY_MAX30102_DTF_ADDRESS				0x20
#define MY_MAX30102_DTC_ADDRESS				0x21

#define MY_MAX30102_RID_ADDRESS				0xFE
#define MY_MAX30102_PID_ADDRESS				0xFF


void MY_MAX30102_ReadByte(uint8_t address, uint8_t *data, int size);
void MY_MAX30102_SendByte(uint8_t address, uint8_t data);
void MY_MAX30102_Init(void);
void MY_MAX30102_Init1(void);
void MY_MAX30102_ReadFIFO(uint32_t *red, uint32_t *ir);
void MY_MAX30102_FIFOToData(void);
void MY_MAX30102_DataToggle(void);
uint32_t MY_MAX30102_DataAverage(void);
void MY_MAX30102_Data_DCAC(uint32_t *DC, uint32_t *AC_min, uint32_t *AC_max);
uint32_t MY_MAX30102_HeartCalculate(uint32_t AC_min, uint32_t AC_max, uint32_t average);
void MY_MAX30102_ReadIR(uint32_t *HR);

#endif
