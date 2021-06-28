/*
 * chute.h
 * 
 * Created: 28.06.2021 01:12:52
 * Author: ThePetrovich
 */


#ifndef _CJUN_CHUTE_H
#define _CJUN_CHUTE_H

#include "sched.h"

#define CHUTE_SERVOPIN 12

void chute_init(sSched_t* sched);

void chute_lock();
void chute_deploy(int delay);

void job_chute_deployStart(void *);
void job_chute_deployEnd(void *);

#endif /* _CJUN_CHUTE_H */