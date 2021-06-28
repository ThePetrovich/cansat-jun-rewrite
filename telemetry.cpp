/*
 * telemetry.cpp
 * 
 * Created: 28.06.2021 01:12:02
 * Author: ThePetrovich
 */


#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <stdio.h>
#include "telemetry.h"
#include "sched.h"

/* ОСТОРОЖНО! Глобальная структура, должна быть объявлена в cansat-jun-rewrite.ino */
/* ничего умнее не придумал ахахаха */
extern struct telemPacketStruct_t mainTelem;

SoftwareSerial radio(TELEM_RXPIN, TELEM_TXPIN);

void job_telem_sendBasic(void *);
void job_telem_sendVerbose(void *);

void telem_init(sSched_t* sched)
{   
    radio.begin(9600);
    SD.begin(TELEM_SDCSPIN);
}

void telem_sendMessage(char* msg)
{
    Serial.println(msg);
    radio.println(msg);
}

/*
struct telemPacketStruct_t 
{
    char teamId[3];
    unsigned long int time;
    int altitude;
    float a;
    float vbat;
    int light;
    int lightInside;
    bool startPoint;
    bool separatePoint;
    bool recoveryPoint;
    bool landingPoint;
    long int pressure;
    int temperature;
    float rawIMU[9];
};*/

/* TeamID;Time;Altitude;A;Start point;Separate point;Recovery point;Landing point \n  */
void job_telem_sendBasic(void *)
{
    Serial.println("Sending data");
    char data[100] = "";
    sprintf(data, "%s;%ld;%d;%d;%d;%d;%d;%d ",  "CS", 
                                                millis(), 
                                                mainTelem.altitude, 
                                                mainTelem.a, 
                                                mainTelem.startPoint, 
                                                mainTelem.separatePoint, 
                                                mainTelem.recoveryPoint, 
                                                mainTelem.landingPoint);

    Serial.println(data);
    radio.println(data);
}

void job_telem_sendVerbose(void *)
{
    Serial.println("Sending data");
    char data[200] = "";

    File dataFile = SD.open("cosmo14.log", FILE_WRITE);

    sprintf(data, "%s;%ld;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;","CS", 
                                                                            millis(), 
                                                                            mainTelem.altitude, 
                                                                            mainTelem.rawIMU[0], 
                                                                            mainTelem.rawIMU[1],
                                                                            mainTelem.rawIMU[2],
                                                                            mainTelem.rawIMU[6], 
                                                                            mainTelem.rawIMU[7],
                                                                            mainTelem.rawIMU[8],
                                                                            mainTelem.rawIMU[3], 
                                                                            mainTelem.rawIMU[4],
                                                                            mainTelem.rawIMU[5],
                                                                            mainTelem.pressure,
                                                                            mainTelem.temperature,
                                                                            mainTelem.startPoint, 
                                                                            mainTelem.separatePoint, 
                                                                            mainTelem.recoveryPoint, 
                                                                            mainTelem.landingPoint);

    Serial.println(data);
    
    if (dataFile) {
        dataFile.println(data);
        dataFile.close();
    }
}
