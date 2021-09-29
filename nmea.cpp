/*
 * nmea.h
 *
 * Created: 11.06.2019 22:43:33
 *  Author: bear1ake & ThePetrovich
 */ 


#include "nmea.h"
#include "pins.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial gps(GPS_RXPIN, GPS_TXPIN);

char rmc_str[128];
unsigned char rmc_index;
unsigned char field_index;
unsigned char packet_type;
struct GPS_t GPS; 

void nmea_init()
{
	gps.begin(9600);
}

unsigned char nmea_getNumberCode(char chr)
{
	if ((chr >= 0x30) && (chr <= 0x39))
		return chr - 0x30;
	else
		return 0xFF;
}

unsigned char nmea_strCompare(char * a, const char * b, unsigned char len)
{
	unsigned char i;
	for (i = 0; i < len; i++)
		if (a[i] != b[i]) 
			return 0;
	return 1;
}

//$GPGSV,t,u,vv,ww,ww,www,ww,xx,xx,xxx,xx,yy,yy, yyy,yy,zz,zz,zzz,zz*hh<CR><LF>
//$GPRMC,hhmmss.ss,A,GGMM.MM,P,gggmm.mm,J,v.v,b.b,ddmmyy,x.x,n,m*hh<CR><LF>
//0      1         2 3       4 5        6 7   8   9      10  11 12
//$GPGGA,hhmmss,GGGG.GGG,P,ggggg.ggg,J,F,SS,D.D,AAA.A,M,aaa.a,M,,*CC<CR><LF>
//0      1      2        3 4         5 6 7  8   9     10 11   12 

static void nmea_process(char data) 
{
	if (data == ',') {
		switch (field_index) {
			case 0:  //NMEA Header
				if (nmea_strCompare(rmc_str+2,"RMC",3))
					packet_type = 1;
				else if (nmea_strCompare(rmc_str+2,"GSV",3))
					packet_type = 2;
				else if (nmea_strCompare(rmc_str+2,"GGA",3))
					packet_type = 3;
				else
					packet_type = 0;
				break;
			case 2: //data validity
				if ((rmc_index > 0) && (packet_type == 1)) {
					if(rmc_str[0] == 'A')
						GPS.valid = 1;
					else
						GPS.valid = 0;
				}
				break;
			case 3: //sats & latitude
				if ((rmc_index > 1) && (packet_type == 2))
					GPS.Sats = nmea_getNumberCode(rmc_str[0]) * 10 + nmea_getNumberCode(rmc_str[1]);
				if ((rmc_index > 5) && (packet_type == 1))
					strncpy(GPS.latitude, rmc_str, 15);
				break;
			case 4: //P
				if ((rmc_index > 0) && (packet_type == 1))
					GPS.P = rmc_str[0];
				break;
			case 5: //longitude
				if ((rmc_index > 5) && (packet_type == 1))
					strncpy(GPS.longitude, rmc_str, 15);
				break;
			case 6: //J
				if ((rmc_index > 0) && (packet_type == 1))
					GPS.J = rmc_str[0];
				break;
			default:
				break;
		}	
		rmc_index = 0;
		rmc_str[rmc_index] = 0;
		field_index++;
	}
	else {
		rmc_str[rmc_index] = data;
		rmc_str[rmc_index+1] = 0;
		rmc_index++;
		if (rmc_index > 127) {
			rmc_index = 0;
			field_index = 0;
		}
		if (data == '$') {
			rmc_index = 0;
			field_index = 0;
			rmc_str[rmc_index] = 0;
		}		
	}
}

void nmea_load()
{
	while (gps.available()) {
		char data = gps.read();
		nmea_process(data);
	}
}

