/*
 * telemetry.cpp
 * 
 * Created: 28.06.2021 01:12:02
 * Author: ThePetrovich
 */


#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <stdio.h>
#include "telemetry.h"
#include "pins.h"
#include "nmea.h"

struct telemPacketStruct_t mainTelem;
struct telemStatusStruct_t mainStatus;

File dataFile;

void telem_init()
{   
    Serial1.begin(9600);
    
    pinMode(TELEM_SDCSPIN, OUTPUT);
    if (!SD.begin(TELEM_SDCSPIN)) {
        Serial1.println(F("SD init fail"));
    }
}

void telem_sendMessage(String msg)
{
    Serial1.println(msg);
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

    Serial1.print(data);
}

void telem_sendGPS()
{
    char data[150] = "";
    snprintf(data, 148, "%s;%ld;GPS;%d;%s;%s \n", TEAM_ID, 
                                        millis(), 
                                        GPS.Sats,
                                        GPS.latitude,
                                        GPS.longitude);

    Serial1.print(data);
}

void telem_sendVerbose()
{
    char data[200] = "";

    dataFile = SD.open("cosmo14.log", FILE_WRITE);
    
    if (dataFile) {
        snprintf(data, 198, "%s;%ld;%d;%ld;%ld;%ld;%ld;%ld;%ld;%ld;%ld;%ld;%ld;%d;%d;%d;%d;%d \n",TEAM_ID, 
                                                                                millis(), 
                                                                                mainTelem.altitude, 
                                                                                mainTelem.aX, 
                                                                                mainTelem.aY,
                                                                                mainTelem.aZ,
                                                                                mainTelem.mX, 
                                                                                mainTelem.mY,
                                                                                mainTelem.mZ,
                                                                                mainTelem.gX, 
                                                                                mainTelem.gY,
                                                                                mainTelem.gZ,
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
