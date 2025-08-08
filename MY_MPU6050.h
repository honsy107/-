
#ifndef __MY_MPU6050_H__
#define __MY_MPU6050_H__

#include "main.h"
#include "i2c.h"

#define MY_MPU6050_I2C	&hi2c2

#define MY_MPU6050_W_ADDRESS 			0xD0
#define MY_MPU6050_R_ADDRESS 			0xD0

#define	MY_MPU6050_SMPLRT_DIV			0x19
#define	MY_MPU6050_CONFIG					0x1A
#define	MY_MPU6050_GYRO_CONFIG		0x1B
#define	MY_MPU6050_ACCEL_CONFIG		0x1C

#define	MY_MPU6050_ACCEL_XOUT_H		0x3B
#define	MY_MPU6050_ACCEL_XOUT_L		0x3C
#define	MY_MPU6050_ACCEL_YOUT_H		0x3D
#define	MY_MPU6050_ACCEL_YOUT_L		0x3E
#define	MY_MPU6050_ACCEL_ZOUT_H		0x3F
#define	MY_MPU6050_ACCEL_ZOUT_L		0x40
#define	MY_MPU6050_TEMP_OUT_H			0x41
#define	MY_MPU6050_TEMP_OUT_L			0x42
#define	MY_MPU6050_GYRO_XOUT_H		0x43
#define	MY_MPU6050_GYRO_XOUT_L		0x44
#define	MY_MPU6050_GYRO_YOUT_H		0x45
#define	MY_MPU6050_GYRO_YOUT_L		0x46
#define	MY_MPU6050_GYRO_ZOUT_H		0x47
#define	MY_MPU6050_GYRO_ZOUT_L		0x48

#define	MY_MPU6050_PWR_MGMT_1			0x6B
#define	MY_MPU6050_PWR_MGMT_2			0x6C
#define	MY_MPU6050_WHO_AM_I				0x75

void MY_MPU6050_SendByte(uint8_t RedAddress, uint8_t Data);
void MY_MPU6050_ReadByte(uint8_t RedAddress, uint8_t *Data, int Size);
void MY_MPU6050_Init(void);
uint8_t MY_MPU6050_GetID(void);
void MY_MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);


#endif
