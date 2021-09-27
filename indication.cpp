/*
 * indication.cpp
 * 
 * Created: Some time in the past
 * Author: ThePetrovich
 */


#include <Arduino.h>
#include "indication.h"
#include "telemetry.h"
#include "pins.h"

void indicators_init()
{
    pinMode(S_DATA, OUTPUT);
    pinMode(S_CLK, OUTPUT);
    pinMode(S_LATCH, OUTPUT);
    pinMode(S_MR, OUTPUT);

    pinMode(TEST_BTN, INPUT);
}

void indicators_showCharge()
{	
    /* Массив показаний панели индикаторов, 1 бит = 1 лампочка */
	const byte a[9] = {B00000000, B00000001, B00000011, B00000111, B00001111, B00011111, B00111111, B01111111, B11111111};

    /* Втыкаем защелку на сдвиговом регистре */
	digitalWrite(S_LATCH, 0); 

    /* Дытаемся угадать напряжение на батарее */
    /* Диапазон от 9 до 7 В, 7 В = 0 лампочек */
	int n = (mainTelem.vbat - 60) / 2;

    /* Не выходим за границы массива */
	if (n > 8) n = 8;
    if (n < 0) n = 0;

    /* Пишем в регистр */
	shiftOut(S_DATA, S_CLK, LSBFIRST, a[n]);

    /* Защелку выключаем */
	digitalWrite(S_LATCH, 1); 
}

void indicators_showStatus()
{   
    /* Хитрая битовая арифметрика для включения нужных лампочек */
    /* Выставляем биты в переменной через побитовый сдвиг, погуглите что это */
    byte out = (mainTelem.startPoint << IND_LED_LAUNCH)
            | (mainTelem.separatePoint << IND_LED_SEP)
            | (mainTelem.recoveryPoint << IND_LED_DEPLOY)
            | (mainTelem.landingPoint << IND_LED_LAND);

    if (!digitalRead(TEST_BTN)) {
        out |= (((mainTelem.vbat > 76) ? 1 : 0) << IND_LED_BAT) 
            | (1 << IND_LED_RXEN) 
            | (mainTelem.ready << IND_LED_RDY)
            | (mainTelem.test << IND_LED_TEST);
    }
    else {
        /* Если в тестовом режиме, ставим другие биты */
        out |= (mainStatus.adxl << IND_LED_ADXL)
            | (mainStatus.l3g << IND_LED_L3G)
            | (mainStatus.hmc << IND_LED_HMC)
            | (mainStatus.bmp << IND_LED_BMP);
    }
    
    /* Пишем в регистр */
    digitalWrite(S_LATCH, 0);

    shiftOut(S_DATA, S_CLK, LSBFIRST, out);

    digitalWrite(S_LATCH, 1); 
}