/*
 * hmc5883.cpp
 * 
 * Created: 27.06.2021 10:55:55
 * Author: ThePetrovich
 */


#include "imu.h"
#include <Arduino.h>
#include <Wire.h>

void i2c_write2(byte addr, byte b1, byte b2)
{
    Wire.beginTransmission(addr); 
    Wire.write(b1);
    Wire.write(b2);
    Wire.endTransmission();
}

void hmc5883_init()
{
    i2c_write2(HMC5883_ADDRESS, HMC5883_CTRL_MODE, HMC5883_CTRL_MODEV);
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

void adxl345_init()
{
    i2c_write2(ADXL345_ADDRESS, ADXL345_CTRL_2D, ADXL345_CTRL_2D_VALUE);

    _delay_ms(5);

    i2c_write2(ADXL345_ADDRESS, ADXL345_CTRL_FORMAT, ADXL345_CTRL_FORMAT_VALUE);
}

static int adxl345_readAcceleration(byte r1, byte r2)
{
    int data = 0;

    i2c_write2(ADXL345_ADDRESS, r1, r2);

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
