/*
 * sensors.cpp
 * 
 * Created: 28.06.2021 01:28:45
 * Author: ThePetrovich
 */


#include <Arduino.h>
#include "microWire.h"
#include "sensors.h"
#include "telemetry.h"
#include "chute.h"
#include "imu.h"
#include "bmp085.h"
#include "pins.h"

static float baseAlt = 0;

void job_sensors_readIMU();
void job_sensors_readAll();

void sensors_init()
{
    Wire.begin();
	
	bmp085_init();

	float temp = 0;
	bmp085_getTemperature(bmp085_readUT()); 
	temp = bmp085_getPressure(bmp085_readUP());
	baseAlt = bmp085_calcAltitude(temp);
	
	imu_init();
}

void sensors_read()
{
    mainTelem.aX = adxl345_readX();
    mainTelem.aY = adxl345_readY();
    mainTelem.aZ = adxl345_readZ();

    mainTelem.mX = hmc5883_readX();
    mainTelem.mY = hmc5883_readY();
    mainTelem.mZ = hmc5883_readZ();

    mainTelem.gX = l3g4200_readX();
    mainTelem.gY = l3g4200_readY();
    mainTelem.gZ = l3g4200_readZ();

    mainTelem.a = sqrt(mainTelem.aX*mainTelem.aX + mainTelem.aY*mainTelem.aY + mainTelem.aZ*mainTelem.aZ);
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

    if (mainTelem.a > 80) {
        mainStatus.adxl = 1;
    }

    if (mainTelem.mX + mainTelem.mY + mainTelem.mZ != 0) {
        mainStatus.hmc = 1;
    }

    if (mainTelem.gX + mainTelem.gY + mainTelem.gZ != 0) {
        mainStatus.l3g = 1;
    }

    if (mainTelem.pressure > 80000) {
        mainStatus.bmp = 1;
    }

    if (mainStatus.adxl + mainStatus.hmc + mainStatus.l3g + mainStatus.bmp == 4) {
        mainStatus.init = 1;
        mainTelem.ready = 1;
        telem_sendMessage("POST OK");
    }
}