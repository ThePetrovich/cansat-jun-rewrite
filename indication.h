#ifndef _CJUN_INDICATION_H
#define _CJUN_INDICATION_H

#define IND_LED_BAT 0
#define IND_LED_RXEN 1
#define IND_LED_RDY 2
#define IND_LED_TEST 3
#define IND_LED_LAUNCH 4
#define IND_LED_SEP 5
#define IND_LED_DEPLOY 6
#define IND_LED_LAND 7

#define IND_LED_ADXL 0
#define IND_LED_L3G 1
#define IND_LED_HMC 2
#define IND_LED_BMP 3

void indicators_init();
void indicators_showCharge();
void indicators_showStatus();

#endif