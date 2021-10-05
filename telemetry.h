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
    char teamId[3]; /* ID команды по регламенту */
    unsigned long int time; /* Время от старта в мс */
    int altitude; /* Высота от старта в метрах */
    int a; /* Модуль ускорения, м/с^2 * 10 !!!! */
    int vbat; /* Напряжение в В * 100 */
    int light; /* Освещенность датчика в попугаях */
    int lightInside; /* Освещенность внутри ракеты */
    short int ready; /* Флаг готовности систем */
    short int test; /* Флаг тестового режима */
    short int startPoint; /* Флаг старта ракеты */
    short int separatePoint; /* Флаг отделения от ракеты */
    short int recoveryPoint; /* Флаг раскрытия парашюта */
    short int landingPoint; /* Флаг приземления */
    long int pressure; /* Давление в Па */
    int temperature; /* Температура в град. C * 10 */
    long int rawIMU[9]; /* 0,1,2 = aX,aY,aZ; 3,4,5 = mX,mY,mZ; 6,7,8 = hX,hY,hZ */
    /* !!! a, aX, aY, aZ, temperature УМНОЖЕНЫ НА 10, vbat УМНОЖЕН НА 100 !!! */
    /* остальное всё как есть */
};

struct telemStatusStruct_t
{
    short int adxl; /* Флаг работы акселерометра (ADXL345) */
    short int l3g; /* Флаг работы гироскопа (L3G4200D) */
    short int hmc; /* Флаг работы магнитометра (HMC5883) */
    short int bmp; /* Флаг работы барометра (BMP085) */
    short int init; /* Флаг работы всех датчиков */
};

/* ОСТОРОЖНО! ГИГАНТСКИЙ КОСТЫЛЬ! Глобальная структура, должна быть объявлена в telemetry.cpp */
/* ничего умнее не придумал ахахаха */
extern struct telemPacketStruct_t mainTelem;
extern struct telemStatusStruct_t mainStatus;

void telem_init();
void telem_sendMessage(String msg);

void telem_sendBasic();
void telem_sendVerbose();
void telem_sendGPS();

#endif /* _CJUN_TELEM_H */
