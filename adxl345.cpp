/*
 * adxl345.cpp
 * 
 * Created: 27.06.2021 10:55:55
 * Author: ThePetrovich
 */


#include "adxl345.h"
#include <Arduino.h>
#include <Wire.h>

void adxl345_init()
{
    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(ADXL345_CTRL_2D);
    Wire.write(ADXL345_CTRL_2D_VALUE);     
    Wire.endTransmission();

    delay(50);

    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(ADXL345_CTRL_FORMAT);
    Wire.write(ADXL345_CTRL_FORMAT_VALUE);
    Wire.endTransmission();
}

double adxl345_readAcceleration(byte r1, byte r2)
{
    int a0 = 0;
    int a1 = 0;
    int aOut = 0;
    double aG = 0;

    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(r1);
    Wire.write(r2);
    Wire.endTransmission();

    delay(2);

    if (Wire.available() >= 2) {
        a0 = Wire.read();
        a1 = Wire.read(); 
        a1 = a1 << 8;
        aOut = a0 + a1;   
    }

    aG = (aOut / 32.0) * 100.0;

    return aG;
}

int adxl345_readX()
{
    return (int)adxl345_readAcceleration(ADXL345_OUT_X0, ADXL345_OUT_X1);
}

int adxl345_readY()
{
    return (int)adxl345_readAcceleration(ADXL345_OUT_Y0, ADXL345_OUT_Y1);
}

int adxl345_readZ()
{
    return (int)adxl345_readAcceleration(ADXL345_OUT_Z0, ADXL345_OUT_Z1);
}
