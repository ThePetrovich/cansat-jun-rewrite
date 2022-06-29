/*
 * bmp085.cpp
 * 
 * Created: 27.06.2021 10:55:55
 * Author: ThePetrovich
 */


#include <Arduino.h>
#include "microWire.h"
#include "bmp085.h"

/* Калибровочные значения из даташита, не трогать */
static int ac1;
static int ac2;
static int ac3;
static int b1;
static int b2;
static int mb;
static long b5;
static unsigned int ac4;
static const unsigned char OSS = 0;

static int bmp085_readInt(unsigned char address)
{
    unsigned char msb, lsb;

    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(BMP085_ADDRESS, 2);

    while(Wire.available() < 2);

    msb = Wire.read();
    lsb = Wire.read();

    return ((int)((int)msb << 8) | (int)lsb);
}

void bmp085_init()
{
    /* Читаем калибровочные данные */
    ac1 = bmp085_readInt(BMP085_CAL_REG_AC1);
    ac2 = bmp085_readInt(BMP085_CAL_REG_AC2);
    ac3 = bmp085_readInt(BMP085_CAL_REG_AC3);
    ac4 = bmp085_readInt(BMP085_CAL_REG_AC4);
    b1 = bmp085_readInt(BMP085_CAL_REG_B1);
    b2 = bmp085_readInt(BMP085_CAL_REG_B2);
    mb = bmp085_readInt(BMP085_CAL_REG_MB);    
}

int bmp085_getTemperature(unsigned int ut)
{
    /* Какая-то страшная хрень из даташита */
    long x1 = 0;
    long x2 = 0;

    int mc = bmp085_readInt(BMP085_CAL_REG_MC);
    int md = bmp085_readInt(BMP085_CAL_REG_MD);

    unsigned int ac5 = bmp085_readInt(BMP085_CAL_REG_AC5);
    unsigned int ac6 = bmp085_readInt(BMP085_CAL_REG_AC6);

    x1 = (((long)ut - (long)ac6) * (long)ac5) >> 15;
    x2 = ((long)mc << 11) / (x1 + md);

    b5 = x1 + x2;

    return ((b5 + 8) >> 4);    
}

long bmp085_getPressure(unsigned long up)
{
    /* Какая-то страшная хрень из даташита */
    long x1 = 0;
    long x2 = 0;
    long x3 = 0;
    long b3 = 0;
    long b6 = 0;
    long p = 0;
    unsigned long b4 = 0;
    unsigned long b7 = 0;

    /* Что-то очень проклятое */
    b6 = b5 - 4000;
    x1 = (b2 * (b6 * b6) >> 12) >> 11;
    x2 = (ac2 * b6) >> 11;
    x3 = x1 + x2;
    b3 = (((((long)ac1) * 4 + x3) << OSS) + 2) >> 2;

    /* Сенсор от бош - мозги еб*шь */
    x1 = (ac3 * b6) >> 13;
    x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    b4 = (ac4 * (unsigned long)(x3 + 32768)) >> 15;
    b7 = ((unsigned long)(up - b3) * (50000 >> OSS));

    if (b7 < 0x80000000) {
        p = (b7 << 1) / b4;
    }
    else {
        p = (b7 / b4) << 1; 
    }

    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    p += (x1 + x2 + 3791) >> 4;

    return p;
}

unsigned int bmp085_readUT()
{
    unsigned int ut = 0;

    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(BMP085_CTRL_UT);
    Wire.write(BMP085_CTRL_UTV);
    Wire.endTransmission();

    delay(5);

    ut = bmp085_readInt(BMP085_OUT);

    return ut;
}

unsigned long bmp085_readUP()
{
    unsigned char msb = 0;
    unsigned char lsb = 0;
    unsigned char xlsb = 0;
    unsigned long up = 0;

    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(BMP085_CTRL_UP);
    Wire.write(BMP085_CTRL_UPV + (OSS << 6));
    Wire.endTransmission();

    /* а че так можно было чтоли */
    delay(2 + (3 << OSS));

    Wire.beginTransmission(BMP085_ADDRESS);
    Wire.write(BMP085_OUT);
    Wire.endTransmission();
    Wire.requestFrom(BMP085_ADDRESS, 3);

    delay(5);

    msb = Wire.read();
    lsb = Wire.read();
    xlsb = Wire.read();

    up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS); 

    return up;
}

int bmp085_calcAltitude(long int prs)
{
    if (prs == 0) return -1;
    return (int)((44330.0 * (1.0 - pow((float) prs / ASL, 0.1903))));
}
