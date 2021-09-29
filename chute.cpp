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
#include "pins.h"

Servo chuteServo;

void chute_lock()
{
    /* Разблокируем механизм на старте */
    chuteServo.attach(CHUTE_SERVOPIN);
	chuteServo.write(5);
    delay(2000);
	chuteServo.detach();

    /* Смотрим на свет внутри ракеты/чехла */
    /* Определяем минимальный уровень для открытия */
	mainTelem.lightInside = analogRead(SENSOR_LIGHT);

    /* Ждем, пока аппарат не будет установлен на ракету */
	while (analogRead(SENSOR_LIGHT) + 50 >= mainTelem.lightInside) {
		telem_sendMessage("Waiting for installation");
		delay(500);
	}

    /* Запираем механизм, чуть ждем и отключаемся */
	chuteServo.attach(CHUTE_SERVOPIN);
	chuteServo.write(90);

	delay(2000);

	chuteServo.detach();

    telem_sendMessage("Chute locked");
}

unsigned long int deployStart = 0;

void chute_deploy(unsigned int time)
{
    /* Если сигнал на раскрытие получен в первый раз... */
    if (deployStart == 0) {
        /* ...запоминаем время получения сигнала и отписываемся на землю */
        deployStart = millis();

        telem_sendMessage("------ Deploy at ");
        telem_sendMessage(String(millis()));
        telem_sendMessage(" ------");
    }

    /* Если прошло регламентное время задержки раскрытия (5 сек.), вскрываемся */
    if ((millis() - deployStart) >= time) {
        chuteServo.attach(CHUTE_SERVOPIN);
        chuteServo.write(5);
        delay(2000);
        chuteServo.detach();
        mainTelem.recoveryPoint = 1; 
    }

    /* Даем время сервоприводу на срабатывание и выставляем флаг recoveryPoint */
    //if ((millis() - deployStart) >= time + 2000) {  
    //}
}
