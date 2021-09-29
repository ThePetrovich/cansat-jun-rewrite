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
#include "pins.h"
#include "nmea.h"

struct telemPacketStruct_t mainTelem;
struct telemStatusStruct_t mainStatus;

File dataFile;

SoftwareSerial radio(TELEM_RXPIN, TELEM_TXPIN);

void telem_init()
{   
    radio.begin(9600);
    pinMode(TELEM_SDCSPIN, OUTPUT);
    SD.begin(TELEM_SDCSPIN);
}

void telem_sendMessage(String msg)
{
    Serial.println(msg);
    radio.println(msg);
}

/* TeamID;Time;Altitude;A;Start point;Separate point;Recovery point;Landing point \n  */
void telem_sendBasic()
{
    char data[150] = "";
    snprintf(data, 148, "%s;%ld;%d;%d;%d;%d;%d;%d \n", TEAM_ID, 
                                                millis(), 
                                                mainTelem.altitude, 
                                                mainTelem.a, 
                                                mainTelem.startPoint, 
                                                mainTelem.separatePoint, 
                                                mainTelem.recoveryPoint, 
                                                mainTelem.landingPoint);

    Serial.print(data);
    radio.print(data);
}

void telem_sendGPS()
{
    char data[150] = "";
    snprintf(data, 148, "%s;%ld;GPS;%d;%s;%s \n", TEAM_ID, 
                                        millis(), 
                                        GPS.Sats,
                                        GPS.latitude,
                                        GPS.longitude);

    Serial.print(data);
    radio.print(data);
}

void telem_sendVerbose()
{
    char data[250] = "";

    dataFile = SD.open("cosmo14.log", FILE_WRITE);
    
    if (dataFile) {
        /* 0,1,2 = aX,aY,aZ; 3,4,5 = mX,mY,mZ; 6,7,8 = hX,hY,hZ */
        snprintf(data, 248, "%s;%ld;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%ld;%d;%d;%d;%d;%d \n",TEAM_ID, 
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
        dataFile.print(data);
        dataFile.close();
    }
}
