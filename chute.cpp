/*
 * chute.cpp
 * 
 * Created: 28.06.2021 01:12:56
 * Author: ThePetrovich
 */


#include <Arduino.h>
#include <Servo.h>
#include "sensors.h"
#include "chute.h"
#include "telemetry.h"
//#include "sched.h"
//#include "jobs.h"

/* ОСТОРОЖНО! Глобальная структура, должна быть объявлена в cansat-jun-rewrite.ino */
/* ничего умнее не придумал ахахаха */
extern struct telemPacketStruct_t mainTelem;

Servo chuteServo;

void chute_lock()
{
    /* Разблокируем механизм на старте */
    chuteServo.attach(CHUTE_SERVOPIN);
	chuteServo.write(5);
	chuteServo.detach();
	delay(250);

    /* Смотрим на свет внутри ракеты/чехла */
    /* Т.е. определяем минимальный уровень для открытия */
	mainTelem.lightInside = analogRead(SENSOR_LIGHT);

    /* Ждем, пока аппарат не будет установлен на ракету */
	while (analogRead(SENSOR_LIGHT) + 50 >= mainTelem.lightInside) {
		Serial.println(F("Waiting for installation"));
		delay(500);
	}

    /* Запираем механизм, чуть ждем и отключаемся */
	chuteServo.attach(CHUTE_SERVOPIN);
	chuteServo.write(90);

	delay(2000);

	chuteServo.detach();

    Serial.println(F("Chute locked"));
}

unsigned long int deployStart = 0;

void chute_deploy(int delay)
{
    if (deployStart == 0) {
        deployStart = millis();

        Serial.print(F("------ Deploy at "));
        Serial.print(String(millis()));
        Serial.println(F(" ------"));

        chuteServo.attach(CHUTE_SERVOPIN);
        chuteServo.write(5);
    }

    if ((millis() - deployStart) >= delay) {
        chuteServo.detach();
        mainTelem.recoveryPoint = true; 
    }
}
