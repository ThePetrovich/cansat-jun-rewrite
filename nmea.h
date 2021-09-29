/*
 * nmea.h
 *
 * Created: 11.06.2019 22:43:33
 *  Author: bear1ake & ThePetrovich
 */ 

#include <stdlib.h>


 // $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
 
struct GPS_t
{
	  unsigned char valid;
	  char latitude[16];
	  unsigned char P;
	  char longitude[16];
	  unsigned char J;
	  unsigned char Sats;
};

extern struct GPS_t GPS; 

void nmea_init();
unsigned char nmea_getNumberCode(char chr);
unsigned char nmea_strCompare(char * a, char * b,unsigned char  len);
void nmea_load();