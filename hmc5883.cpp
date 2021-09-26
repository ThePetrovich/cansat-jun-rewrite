/*
 * hmc5883.cpp
 * 
 * Created: 27.06.2021 10:55:55
 * Author: ThePetrovich
 */


#include "hmc5883.h"
#include <Arduino.h>
#include <Wire.h>

void hmc5883_init()
{
    Wire.beginTransmission(HMC5883_ADDRESS); 
    Wire.write(HMC5883_CTRL_MODE);
    Wire.write(HMC5883_CTRL_MODEV);
    Wire.endTransmission();
}

static int hmc5883_readData(byte r1)
{
    int data = 0;

    Wire.beginTransmission(HMC5883_ADDRESS);
    Wire.write(r1);
    Wire.endTransmission();
    Wire.requestFrom(HMC5883_ADDRESS, 2);

    _delay_ms(5);

    if (Wire.available() >= 2) {
        data = ((int)Wire.read()) << 8;
        data |= ((int)Wire.read());
    }

    return data;

}

int hmc5883_readX()
{
    return hmc5883_readData(HMC5883_OUT_X);
}

int hmc5883_readY()
{
    return hmc5883_readData(HMC5883_OUT_Y);
}

int hmc5883_readZ()
{
    return hmc5883_readData(HMC5883_OUT_Z);
}
