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

    _delay_ms(5);

    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(ADXL345_CTRL_FORMAT);
    Wire.write(ADXL345_CTRL_FORMAT_VALUE);
    Wire.endTransmission();
}

static int adxl345_readAcceleration(byte r1, byte r2)
{
    int data = 0;

    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(r1);
    Wire.write(r2);
    Wire.endTransmission();

    Wire.requestFrom(ADXL345_ADDRESS, 2); 

    _delay_ms(5);

    if (Wire.available() >= 2) {
        data = Wire.read();
        data |= ((int)Wire.read()) << 8; 
    }

    return ((float)data / 32.0) * 100;
}

int adxl345_readX()
{
    return adxl345_readAcceleration(ADXL345_OUT_X0, ADXL345_OUT_X1);
}

int adxl345_readY()
{
    return adxl345_readAcceleration(ADXL345_OUT_Y0, ADXL345_OUT_Y1);
}

int adxl345_readZ()
{
    return adxl345_readAcceleration(ADXL345_OUT_Z0, ADXL345_OUT_Z1);
}
