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
//#include "sched.h"
#include "chute.h"
//#include "jobs.h"
#include "l3g4200d.h"
#include "adxl345.h"
#include "hmc5883.h"
#include "bmp085.h"

/* ОСТОРОЖНО! Глобальная структура, должна быть объявлена в cansat-jun-rewrite.ino */
/* ничего умнее не придумал ахахаха */
extern struct telemPacketStruct_t mainTelem;

L3G4200D gyro;

static float baseAlt = 0;

void job_sensors_readIMU(void *);
void job_sensors_readAll(void *);

void sensors_init(sSched_t* sched)
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

void job_sensors_readIMU(void *)
{
    Serial.println(F("Reading IMU"));
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
}

void job_sensors_readAll(void *)
{
    Serial.println(F("Reading ALL"));
    mainTelem.a = sqrt(mainTelem.rawIMU[0]*mainTelem.rawIMU[0] + mainTelem.rawIMU[1]*mainTelem.rawIMU[1] + mainTelem.rawIMU[2]*mainTelem.rawIMU[2]);
    mainTelem.temperature = bmp085_getTemperature(bmp085_readUT());
    mainTelem.pressure = bmp085_getPressure(bmp085_readUP());
    mainTelem.altitude = bmp085_calcAltitude(mainTelem.pressure) - baseAlt;
    mainTelem.vbat = analogRead(SENSOR_BAT); 
    mainTelem.light = analogRead(SENSOR_LIGHT);

    if (mainTelem.a > 12 && !mainTelem.startPoint) {
        mainTelem.startPoint = true;
    }

    if (mainTelem.a > 12 && mainTelem.recoveryPoint) {
        mainTelem.landingPoint = true;
    }

    if (mainTelem.light >= mainTelem.lightInside && !mainTelem.separatePoint) {
        mainTelem.separatePoint = true;
    }

    if (mainTelem.separatePoint && !mainTelem.recoveryPoint) {
        chute_deploy(2000);
    }
}