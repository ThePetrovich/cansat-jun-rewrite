/*
 * telemetry.h
 * 
 * Created: 28.06.2021 01:12:13
 * Author: ThePetrovich
 */


#ifndef _CJUN_TELEM_H
#define _CJUN_TELEM_H

#include "sched.h"

#define TELEM_TXPIN 5
#define TELEM_RXPIN 6
#define TELEM_SDCSPIN 4

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
    float rawIMU[9]; /* 0,1,2 = aX,aY,aZ; 3,4,5 = mX,mY,mZ; 6,7,8 = hX,hY,hZ */
};

void telem_init(sSched_t* sched);
void telem_sendMessage(char* msg);

void job_telem_sendBasic(void *);
void job_telem_sendVerbose(void *);

#endif /* _CJUN_TELEM_H */