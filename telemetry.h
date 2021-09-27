/*
 * telemetry.h
 * 
 * Created: 28.06.2021 01:12:13
 * Author: ThePetrovich
 */


#ifndef _CJUN_TELEM_H
#define _CJUN_TELEM_H

#define TEAM_ID "CS"

struct telemPacketStruct_t 
{
    char teamId[3];
    unsigned long int time;
    int altitude;
    int a;
    int vbat;
    int light;
    int lightInside;
    short int ready;
    short int test;
    short int startPoint;
    short int separatePoint;
    short int recoveryPoint;
    short int landingPoint;
    short int indication;
    long int pressure;
    int temperature;
    int rawIMU[9]; /* 0,1,2 = aX,aY,aZ; 3,4,5 = mX,mY,mZ; 6,7,8 = hX,hY,hZ */
};

struct telemStatusStruct_t
{
    short int adxl;
    short int l3g;
    short int hmc;
    short int bmp;
    short int init;
};

/* ОСТОРОЖНО! ГИГАНТСКИЙ КОСТЫЛЬ! Глобальная структура, должна быть объявлена в telemetry.cpp */
/* ничего умнее не придумал ахахаха */
extern struct telemPacketStruct_t mainTelem;
extern struct telemStatusStruct_t mainStatus;

void telem_init();
void telem_sendMessage(String msg);

void telem_sendBasic();
void telem_sendVerbose();

#endif /* _CJUN_TELEM_H */