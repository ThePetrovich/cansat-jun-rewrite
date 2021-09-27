#include <Arduino.h>
#include "indication.h"
#include "telemetry.h"
#include "pins.h"

void indicators_init()
{
    pinMode(S_DATA, OUTPUT);
    pinMode(S_CLK, OUTPUT);
    pinMode(S_SCLK, OUTPUT);
    pinMode(S_MR, OUTPUT);

    pinMode(TEST_BTN, INPUT);
}

void indicators_showCharge()
{	
	const byte a[9] = {B00000000, B00000001, B00000011, B00000111, B00001111, B00011111, B00111111, B01111111, B11111111};
	digitalWrite(S_SCLK, 0); 

	int n = (mainTelem.vbat + 10) / 10;

	if (n > 8) n = 8;
	shiftOut(S_DATA, S_CLK, LSBFIRST, a[n]);

	digitalWrite(S_SCLK, 1); 
}

void indicators_showStatus()
{
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
        out |= (mainStatus.adxl << IND_LED_ADXL)
            | (mainStatus.l3g << IND_LED_L3G)
            | (mainStatus.hmc << IND_LED_HMC)
            | (mainStatus.bmp << IND_LED_BMP);
    }

    digitalWrite(S_SCLK, 0);

    shiftOut(S_DATA, S_CLK, LSBFIRST, out);

    digitalWrite(S_SCLK, 1); 
}