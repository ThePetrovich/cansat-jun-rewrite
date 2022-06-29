/*
 * imu.h
 * 
 * Created: 27.06.2021 10:55:55
 * Author: ThePetrovich
 */


#ifndef _CJUN_IMU_H
#define _CJUN_IMU_H

void imu_init();

#define HMC5883_ADDRESS                 0x1E
#define HMC5883_CTRL_MODE               0x02
#define HMC5883_CTRL_MODEV              0x00
#define HMC5883_OUT_X                   0x03
#define HMC5883_OUT_Y                   0x05
#define HMC5883_OUT_Z                   0x07

#define HMC5883_TIMEOUT_MS              100

long int hmc5883_readX();
long int hmc5883_readY();
long int hmc5883_readZ();

#define ADXL345_CTRL_FORMAT             0x31
#define ADXL345_CTRL_FORMAT_VALUE       0x03
#define ADXL345_CTRL_2D                 0x2D
#define ADXL345_CTRL_2D_VALUE           0x08
#define ADXL345_OUT_X0                  0x32
#define ADXL345_OUT_X1                  0x33
#define ADXL345_OUT_Y0                  0x34
#define ADXL345_OUT_Y1                  0x35
#define ADXL345_OUT_Z0                  0x36 
#define ADXL345_OUT_Z1                  0x37
#define ADXL345_ADDRESS                 0x53     /* Адрес на шине i2c, дефолтный 0x53 */

#define ADXL345_TIMEOUT_MS              100

long int adxl345_readX();
long int adxl345_readY();
long int adxl345_readZ();

#define L3G4200D_ADDRESS                0xD2
#define L3G4200D_WHO_AM_I               0x0F

#define L3G4200D_CTRL_REG1              0x20
#define L3G4200D_CTRL_REG2              0x21
#define L3G4200D_CTRL_REG3              0x22
#define L3G4200D_CTRL_REG4              0x23
#define L3G4200D_CTRL_REG5              0x24
#define L3G4200D_REFERENCE              0x25
#define L3G4200D_OUT_TEMP               0x26
#define L3G4200D_STATUS_REG             0x27

#define L3G4200D_OUT_X_L                0x28
#define L3G4200D_OUT_X_H                0x29
#define L3G4200D_OUT_Y_L                0x2A
#define L3G4200D_OUT_Y_H                0x2B
#define L3G4200D_OUT_Z_L                0x2C
#define L3G4200D_OUT_Z_H                0x2D

#define L3G4200D_FIFO_CTRL_REG          0x2E
#define L3G4200D_FIFO_SRC_REG           0x2F

#define L3G4200D_INT1_CFG               0x30
#define L3G4200D_INT1_SRC               0x31
#define L3G4200D_INT1_THS_XH            0x32
#define L3G4200D_INT1_THS_XL            0x33
#define L3G4200D_INT1_THS_YH            0x34
#define L3G4200D_INT1_THS_YL            0x35
#define L3G4200D_INT1_THS_ZH            0x36
#define L3G4200D_INT1_THS_ZL            0x37
#define L3G4200D_INT1_DURATION          0x38

long int l3g4200_readX();
long int l3g4200_readY();
long int l3g4200_readZ();

#endif /* _CJUN_IMU_H */
