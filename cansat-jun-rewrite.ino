/*
 * cansat-jun-rewrite.ino
 * Version: 0.0.7
 * 
 * Created: 28.06.2021 12:12:39
 * Author: ThePetrovich
 * Rewriting cansat-jun from scratch because fuck my life.
 */


#define VERSION "CanSatJun v0.0.7 built " __TIMESTAMP__

#include "chute.h"
#include "telemetry.h"
#include "sensors.h"
#include "indication.h"
#include "nmea.h"

unsigned long int lastImu = 0;
unsigned long int lastAll = 0;
unsigned long int lastGPS = 0;

void setup()
{
    Serial.begin(9600);

    sensors_init();
    telem_init();

    indicators_init();

    telem_sendMessage("Init OK");
    telem_sendMessage(VERSION);

    sensors_selfTest();

    indicators_showCharge();
    delay(3000);

    chute_lock();

    nmea_init();
}

void loop()
{
    if (millis() - lastImu >= 250) {
        telem_sendVerbose();
        indicators_showStatus();

        lastImu = millis();
    }

    if (millis() - lastAll >= 500) {
        telem_sendBasic();

        lastAll = millis();
    }

    sensors_read();

    if (millis() - lastGPS >= 5000) {
        nmea_load();
        telem_sendGPS();
        lastGPS = millis();
    }
}

