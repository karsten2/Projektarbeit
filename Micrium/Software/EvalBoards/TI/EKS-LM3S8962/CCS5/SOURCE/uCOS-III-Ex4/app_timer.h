/* ******************************************************************************************************
*
*
* Filename      : app_timer.h
* Version       : $Id: $
* Programmer(s) : DUC
********************************************************************************************************* */

#ifndef _APP_TIMER_H
#define _APP_TIMER_H

// ******************************  INTERFACE (includes)
#include  <app_cfg.h>
#include  <cpu.h>
#include  <bsp.h>
#include  <hw_ints.h>
#include  <os.h>
#include  "timer.h"


// ******************************  INTERFACE (avoid circular dependencies)
#ifdef   APP_TIMER_MODULE
#define  APP_TIMER_MODULE_EXT
#else
#define  APP_TIMER_MODULE_EXT  extern
#endif


// ******************************  INTERFACE (declaration/definition of variables)
APP_TIMER_MODULE_EXT OS_TCB        AppTaskTimerSyncTCB;
APP_TIMER_MODULE_EXT CPU_STK       AppTaskTimerSyncStk[APP_TASK_TIMER_SYNC_STK_SIZE];


// ******************************  INTERFACE (declaration/definition of function prototypes)
APP_TIMER_MODULE_EXT void My_IntHandler_TIMER0B (void);
APP_TIMER_MODULE_EXT void AppTaskTimerSync (void *p_arg);


// ******************************  LOCAL DEFINES
#define  APP_TIMER_INT_PROCESSING 1 // (0: uCOSIII-Book --> Chap.14, OSTaskSemPost; 1: uCOSIII-Book --> Chap.3, ISR )

// ******************************  LOCAL DATA TYPES


// ******************************  LOCAL TABLES


// ******************************  LOCAL GLOBAL VARIABLES


// ******************************  LOCAL FUNCTION PROTOTYPES


// ******************************  LOCAL CONFIGURATION ERRORS

#endif
