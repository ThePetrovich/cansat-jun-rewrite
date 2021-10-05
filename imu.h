/*
 * imu.h
 * 
 * Created: 27.06.2021 10:55:55
 * Author: ThePetrovich
 */


#ifndef _CJUN_IMU_H
#define _CJUN_IMU_H

#define HMC5883_ADDRESS        0x1E
#define HMC5883_CTRL_MODE      0x02
#define HMC5883_CTRL_MODEV     0x00
#define HMC5883_OUT_X          0x03
#define HMC5883_OUT_Y          0x05
#define HMC5883_OUT_Z          0x07

#define HMC5883_TIMEOUT_MS     100

void hmc5883_init();

int hmc5883_readX();
int hmc5883_readY();
int hmc5883_readZ();

#define ADXL345_CTRL_FORMAT            0x31
#define ADXL345_CTRL_FORMAT_VALUE      0x03
#define ADXL345_CTRL_2D                0x2D
#define ADXL345_CTRL_2D_VALUE          0x08
#define ADXL345_OUT_X0                 0x32
#define ADXL345_OUT_X1                 0x33
#define ADXL345_OUT_Y0                 0x34
#define ADXL345_OUT_Y1                 0x35
#define ADXL345_OUT_Z0                 0x36 
#define ADXL345_OUT_Z1                 0x37
#define ADXL345_ADDRESS                0x53     /* Адрес на шине i2c, дефолтный 0x53 */

#define ADXL345_TIMEOUT_MS             100

void adxl345_init();

int adxl345_readX();
int adxl345_readY();
int adxl345_readZ();

#endif /* _CJUN_IMU_H */
