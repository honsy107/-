#include <math.h>
#include "stm32f1xx_hal.h"               // Device header

#define __MADC_NUM_ 3
#define __MADC_TIMES_ 5000

float ADC_DT, x;
uint16_t message[__MADC_NUM_] = {0};
int ADC_KK=0, i=0, i1=0;
float g[__MADC_NUM_]={0};


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc == &hadc1)
	{
		ADC_KK = 1;
	}
}


void MY_HAL_ADC_Init(ADC_HandleTypeDef* hadc)
{
HAL_ADCEx_Calibration_Start(hadc);
HAL_ADC_Start_DMA(hadc, (uint32_t*)message, __MADC_NUM_);
}


void MY_HAL_ADC_GET(float *mese)
{
	for(i=0; i<__MADC_NUM_; i++)
	{
		g[i] = 0;
	}
	
	for(i1=0; i1<__MADC_TIMES_; i1++)
	{
		for(;!ADC_KK;);
		for(i=0; i<__MADC_NUM_; i++)
		{
			g[i] += message[i];
		}
		ADC_KK = 0;
	}
	
	ADC_DT = 1.2163 * 4095.0/( g[__MADC_NUM_-1] / __MADC_TIMES_ ) ;
	mese[__MADC_NUM_-1] = ADC_DT;
	
	for(i=0; i<__MADC_NUM_-1; i++)
	{
		mese[i] = ((g[i]/__MADC_TIMES_)/4095.0)*ADC_DT;
	}
}



