/*
 * adxl345.h
 * 
 * Created: 27.06.2021 10:55:55
 * Author: ThePetrovich
 */


#ifndef _CJUN_ADXL345_H
#define _CJUN_ADXL345_H

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

#endif /* _CJUN_ADXL345_H */
