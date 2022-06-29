/*
 * hmc5883.cpp
 * 
 * Created: 27.06.2021 10:55:55
 * Author: ThePetrovich
 */


#include "imu.h"
#include <Arduino.h>
#include "microWire.h"

#define GYR_ADDRESS (0xD2 >> 1)

void i2c_write2(byte addr, byte b1, byte b2)
{
    Wire.beginTransmission(addr); 
    Wire.write(b1);
    Wire.write(b2);
    Wire.endTransmission();
}

void imu_init()
{
    i2c_write2(HMC5883_ADDRESS, HMC5883_CTRL_MODE, HMC5883_CTRL_MODEV);

    i2c_write2(ADXL345_ADDRESS, ADXL345_CTRL_2D, ADXL345_CTRL_2D_VALUE);

    _delay_ms(5);

    i2c_write2(ADXL345_ADDRESS, ADXL345_CTRL_FORMAT, ADXL345_CTRL_FORMAT_VALUE);

    i2c_write2(L3G4200D_ADDRESS >> 1, L3G4200D_CTRL_REG1, 0x0F);
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

long int hmc5883_readX()
{
    return (long int)hmc5883_readData(HMC5883_OUT_X);
}

long int hmc5883_readY()
{
    return (long int)hmc5883_readData(HMC5883_OUT_Y);
}

long int hmc5883_readZ()
{
    return (long int)hmc5883_readData(HMC5883_OUT_Z);
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

long int adxl345_readX()
{
    return (long int)adxl345_readAcceleration(ADXL345_OUT_X0, ADXL345_OUT_X1);
}

long int adxl345_readY()
{
    return (long int)adxl345_readAcceleration(ADXL345_OUT_Y0, ADXL345_OUT_Y1);
}

long int adxl345_readZ()
{
    return (long int)adxl345_readAcceleration(ADXL345_OUT_Z0, ADXL345_OUT_Z1);
}

static int l3g4200_readAngularVelocity(byte r1)
{
    int data = 0;

    Wire.beginTransmission(L3G4200D_ADDRESS >> 1);
    Wire.write(r1 | (1 << 7)); 
    Wire.endTransmission();

    Wire.requestFrom(L3G4200D_ADDRESS >> 1, 2);

    _delay_ms(5);

    if (Wire.available() >= 2) {
        data = Wire.read();
        data |= ((int)Wire.read()) << 8; 
    }

    return data;
}

long int l3g4200_readX()
{
    return (long int)l3g4200_readAngularVelocity(L3G4200D_OUT_X_L);
}

long int l3g4200_readY()
{
    return (long int)l3g4200_readAngularVelocity(L3G4200D_OUT_Y_L);
}

long int l3g4200_readZ()
{
    return (long int)l3g4200_readAngularVelocity(L3G4200D_OUT_Z_L);
}