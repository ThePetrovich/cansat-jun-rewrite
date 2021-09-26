/*
 * sensors.h
 * 
 * Created: 28.06.2021 01:28:41
 * Author: ThePetrovich
 */


#ifndef _CJUN_SENSOR_H
#define _CJUN_SENSOR_H

#define SENSOR_VREF 5.0F
#define SENSOR_VDIV 2
#define SENSOR_LIGHT_THRESHOLD 450

void sensors_init();

void sensors_read();

void sensors_selfTest();

#endif /* _CJUN_SENSOR_H */