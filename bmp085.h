/*
 * bmp085.h
 * 
 * Created: 27.06.2021 10:55:55
 * Author: ThePetrovich
 */


#ifndef _CJUN_BMP085_H
#define _CJUN_BMP085_H

#define BMP085_ADDRESS         0x77    /* Адрес на шине i2c, дефолтный 0x77 */
#define BMP085_CTRL_ENAB       0x00
#define BMP085_CTRL_MODE       0x02
#define BMP085_CTRL_UT         0xF4
#define BMP085_CTRL_UTV        0x2E
#define BMP085_CTRL_UP         0xF4
#define BMP085_CTRL_UPV        0x34

#define BMP085_CAL_REG_AC1     0xAA
#define BMP085_CAL_REG_AC2     0xAC
#define BMP085_CAL_REG_AC3     0xAE
#define BMP085_CAL_REG_AC4     0xB0
#define BMP085_CAL_REG_AC5     0xB2
#define BMP085_CAL_REG_AC6     0xB4
#define BMP085_CAL_REG_B1      0xB6
#define BMP085_CAL_REG_B2      0xB8
#define BMP085_CAL_REG_MB      0xBA
#define BMP085_CAL_REG_MC      0xBC
#define BMP085_CAL_REG_MD      0xBE

#define BMP085_OUT             0xF6

#define BMP085_TIMEOUT_MS      100

#define ASL 101325.0F    /* Давление на уровне моря в Па */

void bmp085_init();

unsigned int bmp085_readUT();
unsigned long bmp085_readUP();

int bmp085_getTemperature(unsigned int ut);
long bmp085_getPressure(unsigned long up);
int bmp085_calcAltitude(long int prs);

#endif /* _CJUN_BMP085_H */
