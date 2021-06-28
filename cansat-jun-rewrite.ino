/*
 * cansat-jun-rewrite.ino
 * 
 * Created: 28.06.2021 12:12:39
 * Author: ThePetrovich
 * Rewriting cansat-jun from scratch because fuck my life.
 */

#include "sched.h"
#include "chute.h"
#include "telemetry.h"
#include "sensors.h"

struct telemPacketStruct_t mainTelem;

sSched_t mainScheduler;


void job_blink(void *);

void setup()
{
    Serial.begin(9600);

    sensors_init(&mainScheduler);
    telem_init(&mainScheduler);

    chute_lock();

    Serial.println("Init OK");
}

void loop()
{
    if (millis() % 250 == 0) {
        job_sensors_readIMU(NULL);
        job_telem_sendVerbose(NULL);
    }

    if (millis() % 500 == 0) {
        job_sensors_readAll(NULL);
        job_telem_sendBasic(NULL);
    }
}

