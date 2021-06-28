/*
 * sched.h
 * 
 * Created: 14.02.2021 09:20:53
 * Author: ThePetrovich
 */


#ifndef SCHED_SCHED_H_
#define SCHED_SCHED_H_

#define LIBSCHED_VERSION "1.1.0"

#define CFG_NUMBER_OF_PRIORITIES 8

//#include "lists.h"
//#include "jobs.h"

#define sched_enterCriticalSection()    asm volatile ("lds __tmp_reg__, __SREG__ \n\t"\
                                        "cli                       \n\t"\
                                        "push __tmp_reg__"            ::)
#define sched_exitCriticalSection()     asm volatile ("pop __tmp_reg__           \n\t"\
                                        "sei                       \n\t"\
                                        "sts __SREG__, __tmp_reg__"   ::)

typedef struct sJobStruct_t sJob_t;
typedef struct sLinkedListStruct_t sLinkedList_t;

typedef struct sSchedStruct_t 
{
    //sLinkedList_t activeJobList[CFG_NUMBER_OF_PRIORITIES];
    //sLinkedList_t sleepingJobList;
    //sLinkedList_t suspendedJobList;

    int jobCount;
} sSched_t;

#ifdef __cplusplus
extern "C" {
#endif

void sched_initScheduler(sSched_t* scheduler);

void sched_createJob(sSched_t* sched, sJob_t* job, void (*function)(void*), void* args, int delay, int period, int priority, const char* name);
void sched_restartJob(sSched_t* scheduler, sJob_t* job);
void sched_stopJob(sJob_t* job);

void sched_suspendJob(sJob_t* job);
void sched_activateJob(sJob_t* job);
void sched_changeJobPriority(sJob_t* job, int priority);

void sched_run(sSched_t* scheduler);
void sched_tick(sSched_t* scheduler);

int sched_getJobCount(sSched_t* scheduler);

#ifdef __cplusplus
}
#endif

#endif