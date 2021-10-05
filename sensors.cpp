/*
 * sensors.cpp
 * 
 * Created: 28.06.2021 01:28:45
 * Author: ThePetrovich
 */


#include <Arduino.h>
#include <Wire.h>
#include "sensors.h"
#include "telemetry.h"
#include "chute.h"
#include "l3g4200d.h"
#include "imu.h"
#include "bmp085.h"
#include "pins.h"

L3G4200D gyro;

static float baseAlt = 0;

void job_sensors_readIMU();
void job_sensors_readAll();

void sensors_init()
{
    Wire.begin();
	
	gyro.enableDefault();
	
    
	bmp085_init();
	float temp = 0;
	bmp085_getTemperature(bmp085_readUT()); 
	temp = bmp085_getPressure(bmp085_readUP());
	baseAlt = bmp085_calcAltitude(temp);
	
	hmc5883_init();

	adxl345_init();
}

void sensors_read()
{
    gyro.read();

    /* 0,1,2 = aX,aY,aZ; 3,4,5 = mX,mY,mZ; 6,7,8 = hX,hY,hZ */
    mainTelem.rawIMU[0] = adxl345_readX();
    mainTelem.rawIMU[1] = adxl345_readY();
    mainTelem.rawIMU[2] = adxl345_readZ();

    mainTelem.rawIMU[3] = hmc5883_readX();
    mainTelem.rawIMU[4] = hmc5883_readY();
    mainTelem.rawIMU[5] = hmc5883_readZ();

    mainTelem.rawIMU[6] = gyro.g.x;
    mainTelem.rawIMU[7] = gyro.g.y;
    mainTelem.rawIMU[8] = gyro.g.z;

    mainTelem.a = sqrt(mainTelem.rawIMU[0]*mainTelem.rawIMU[0] + mainTelem.rawIMU[1]*mainTelem.rawIMU[1] + mainTelem.rawIMU[2]*mainTelem.rawIMU[2]);
    mainTelem.temperature = bmp085_getTemperature(bmp085_readUT());
    mainTelem.pressure = bmp085_getPressure(bmp085_readUP());
    mainTelem.altitude = bmp085_calcAltitude(mainTelem.pressure) - baseAlt;
    mainTelem.vbat = ((float)analogRead(SENSOR_BAT) / 1024.0) * SENSOR_VREF * SENSOR_VDIV * 100; 
    mainTelem.light = analogRead(SENSOR_LIGHT);

    if (mainTelem.a > 160 && !mainTelem.startPoint) {
        mainTelem.startPoint = 1;
    }

    if (mainTelem.a > 160 && mainTelem.recoveryPoint) {
        mainTelem.landingPoint = 1;
    }

    if (mainTelem.light >= mainTelem.lightInside && !mainTelem.separatePoint && mainTelem.startPoint) {
        mainTelem.separatePoint = 1;
    }

    if (mainTelem.separatePoint && !mainTelem.recoveryPoint) {
        chute_deploy(SENSORS_CHUTE_DELAY);
    }

    if (mainTelem.recoveryPoint) {
        analogWrite(SPEAKER, 127);
    }
}

void sensors_selfTest()
{
    telem_sendMessage("Running POST");

    sensors_read();

    if (mainTelem.rawIMU[0] + mainTelem.rawIMU[1] + mainTelem.rawIMU[2] != 0) {
        mainStatus.adxl = 1;
    }

    if (mainTelem.rawIMU[3] + mainTelem.rawIMU[4] + mainTelem.rawIMU[5] != 0) {
        mainStatus.hmc = 1;
    }

    if (mainTelem.rawIMU[6] + mainTelem.rawIMU[7] + mainTelem.rawIMU[8] != 0) {
        mainStatus.l3g = 1;
    }

    if (mainTelem.pressure != 0) {
        mainStatus.bmp = 1;
    }

    if (mainStatus.adxl + mainStatus.hmc + mainStatus.l3g + mainStatus.bmp == 4) {
        mainStatus.init = 1;
        mainTelem.ready = 1;
        telem_sendMessage("POST OK");
    }
}