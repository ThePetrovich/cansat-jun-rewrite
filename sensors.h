/*
 * sensors.h
 * 
 * Created: 28.06.2021 01:28:41
 * Author: ThePetrovich
 */


#ifndef _CJUN_SENSOR_H
#define _CJUN_SENSOR_H

#include "sched.h"

#define SENSOR_BAT A0
#define SENSOR_LIGHT A2

#define SENSOR_VREF 4.5F
#define SENSOR_VDIV 2
#define SENSOR_LIGHT_THRESHOLD 450

void sensors_init(sSched_t* sched);

void job_sensors_readIMU(void *);
void job_sensors_readAll(void *);

void job_sensors_statusStart(void *);
void job_sensors_statusDepl(void *);
void job_sensors_statusLand(void *);

#endif /* _CJUN_SENSOR_H */