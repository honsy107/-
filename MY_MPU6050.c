
#include "MY_MPU6050.h"


void MY_MPU6050_SendByte(uint8_t RedAddress, uint8_t Data)
{
	uint8_t temp[2] = {RedAddress, Data};
	HAL_I2C_Master_Transmit(MY_MPU6050_I2C, MY_MPU6050_W_ADDRESS, temp, 2, 1000);
}

void MY_MPU6050_ReadByte(uint8_t RedAddress, uint8_t *Data, int Size)
{
	uint8_t temp[1] = {RedAddress};
	HAL_I2C_Master_Transmit(MY_MPU6050_I2C, MY_MPU6050_W_ADDRESS, temp, 1, 1000);
	HAL_I2C_Master_Receive(MY_MPU6050_I2C, MY_MPU6050_R_ADDRESS, Data, Size, 1000);
}

void MY_MPU6050_Init(void)
{
	MY_MPU6050_SendByte(MY_MPU6050_PWR_MGMT_1, 0x01);
	MY_MPU6050_SendByte(MY_MPU6050_PWR_MGMT_2, 0x00);
	MY_MPU6050_SendByte(MY_MPU6050_SMPLRT_DIV, 0x09);
	MY_MPU6050_SendByte(MY_MPU6050_CONFIG, 0x06);
	MY_MPU6050_SendByte(MY_MPU6050_GYRO_CONFIG, 0x18);
	MY_MPU6050_SendByte(MY_MPU6050_ACCEL_CONFIG, 0x18);
}

uint8_t MY_MPU6050_GetID(void)
{
	uint8_t temp[1] = {0};
	MY_MPU6050_ReadByte(MY_MPU6050_WHO_AM_I, temp, 1);
	return temp[0];
}

void MY_MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	MY_MPU6050_ReadByte(MY_MPU6050_ACCEL_XOUT_H, &DataH, 1);
	MY_MPU6050_ReadByte(MY_MPU6050_ACCEL_XOUT_L, &DataL, 1);
	*AccX = (DataH << 8) | DataL;
	
	MY_MPU6050_ReadByte(MY_MPU6050_ACCEL_YOUT_H, &DataH, 1);
	MY_MPU6050_ReadByte(MY_MPU6050_ACCEL_YOUT_L, &DataL, 1);
	*AccY = (DataH << 8) | DataL;
	
	MY_MPU6050_ReadByte(MY_MPU6050_ACCEL_ZOUT_H, &DataH, 1);
	MY_MPU6050_ReadByte(MY_MPU6050_ACCEL_ZOUT_L, &DataL, 1);
	*AccZ = (DataH << 8) | DataL;
	
	MY_MPU6050_ReadByte(MY_MPU6050_GYRO_XOUT_H, &DataH, 1);
	MY_MPU6050_ReadByte(MY_MPU6050_GYRO_XOUT_L, &DataL, 1);
	*GyroX = (DataH << 8) | DataL;
	
	MY_MPU6050_ReadByte(MY_MPU6050_GYRO_YOUT_H, &DataH, 1);
	MY_MPU6050_ReadByte(MY_MPU6050_GYRO_YOUT_L, &DataL, 1);
	*GyroY = (DataH << 8) | DataL;
	
	MY_MPU6050_ReadByte(MY_MPU6050_GYRO_ZOUT_H, &DataH, 1);
	MY_MPU6050_ReadByte(MY_MPU6050_GYRO_ZOUT_L, &DataL, 1);
	*GyroZ = (DataH << 8) | DataL;
}


