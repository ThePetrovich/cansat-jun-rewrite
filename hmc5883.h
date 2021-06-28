/*
 * hmc5883.h
 * 
 * Created: 27.06.2021 10:55:55
 * Author: ThePetrovich
 */


#ifndef _CJUN_HMC5883_H
#define _CJUN_HMC5883_H

#define HMC5883_ADDRESS        0x1E
#define HMC5883_CTRL_MODE      0x02
#define HMC5883_CTRL_MODEV     0x00
#define HMC5883_OUT_X          0x03
#define HMC5883_OUT_Y          0x05
#define HMC5883_OUT_Z          0x07

#define HMC5883_TIMEOUT_MS     100

void hmc5883_init();

int hmc5883_readX();
int hmc5883_readY();
int hmc5883_readZ();

#endif /* _CJUN_HMC5883_H */
