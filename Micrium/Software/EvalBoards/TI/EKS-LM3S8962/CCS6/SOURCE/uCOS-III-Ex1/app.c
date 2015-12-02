/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2009-2010; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : JJL
*                 EHS
*                 KWN
*                 EMO
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*********************************************************************************************************
*/

static  OS_TCB       AppTaskStartTCB;
static  OS_TCB       AppTaskDisplay_TCB;

static  CPU_STK      AppTaskStartStk[APP_TASK_START_STK_SIZE];
static  CPU_STK      AppTaskDisplay_Stk[APP_TASK_DISPLAY_STK_SIZE];

static  CPU_BOOLEAN  bLED[2] = {DEF_TRUE, DEF_FALSE};           /* Variable used by uC/Probe for monitoring             */

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart        (void  *p_arg);
static  void  AppTaskDisplay(void  *p_arg);
static  void  AppDisplayTaskCreate(void);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none.
*
* Returns     : none.
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;


    BSP_IntDisAll();                                            /* Disable all interrupts.                              */

    OSInit(&err);                                               /* Init uC/OS-III.                                      */

    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10u,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none.
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void  *p_arg)
{
    CPU_INT32U  clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;


   (void)&p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */

    clk_freq = BSP_CPU_ClkFreq();                                /* Determine SysTick reference freq.                    */
    cnts     = clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* Determine nbr SysTick increments                     */
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */
    CPU_TS_TmrFreqSet(clk_freq);

#if (OS_CFG_STAT_TASK_EN > 0u)
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

    CPU_IntDisMeasMaxCurReset();

    App_ProbeInit();

    BSP_LED_On(1);
    BSP_LED_Off(2);

    AppDisplayTaskCreate();

    while (DEF_ON) {                                            /* Task body, always written as an infinite loop.       */
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);

        BSP_LED_Toggle(0);                                      /* Toggle both LEDs every second.                       */

        bLED[0] ^= DEF_TRUE;
        bLED[1] ^= DEF_TRUE;
    }
}


/*
*********************************************************************************************************
*                                          DISPLAY TASK
*
* Description : This is a task to display messages in the display.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskDisplay()' by 'OSTaskCreate()'.
*
* Returns     : none.
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskDisplay(void  *p_arg)
{
    CPU_INT08U  state;
    CPU_INT16U  sec;
    OS_ERR      err;


   (void)&p_arg;

    sec   = 1u;
    state = 0u;

    while (DEF_ON) {
        OSTimeDlyHMSM(0u, 0u, sec, 300u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);

        switch(state) {
            case 0:
            	 RIT128x96x4StringDraw("TEXAS", 29u, 0u, 15);
            	 RIT128x96x4StringDraw("INSTRUMENTS", 11u, 9u, 15);
                 sec   = 5u;
                 state = 1u;
                 break;


            case 1:
            	 RIT128x96x4Clear();
                 sec   = 1u;
                 state = 2u;
                 break;


            case 2:
            	 RIT128x96x4StringDraw("EKS-LM3S8962", 21u, 0u, 15);
            	 RIT128x96x4StringDraw("CCS 5.3 - EX1", 18u, 9u, 15);
            	 RIT128x96x4StringDraw("$Rev:: 16   $", 18u, 18u, 15);
                 sec   = 5u;
                 state = 3u;
                 break;


            case 3:
            	RIT128x96x4Clear();
                 sec   = 1u;
                 state = 4u;
                 break;


            case 4:
            	RIT128x96x4StringDraw( "MICRIUM",   27u, 0u, 15);
            	RIT128x96x4StringDraw( "uC/OS-III", 21u, 9u, 15);
                 sec   = 5u;
                 state = 5u;
                 break;


            case 5:
            	RIT128x96x4Clear();
                 sec   = 1u;
                 state = 0u;
                 break;


            default:
                 sec   = 1u;
                 state = 0u;
                 break;
        }
    }
}


static  void  AppDisplayTaskCreate (void)
{
    OS_ERR  err;


                                                                /* Create Display Task                                  */
    OSTaskCreate((OS_TCB     *)&AppTaskDisplay_TCB,
                 (CPU_CHAR   *)"Display Task",
                 (OS_TASK_PTR ) AppTaskDisplay,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_DISPLAY_PRIO,
                 (CPU_STK    *)&AppTaskDisplay_Stk[0],
                 (CPU_STK_SIZE) APP_TASK_DISPLAY_STK_SIZE / 10u,
                 (CPU_STK_SIZE) APP_TASK_DISPLAY_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err); 
}

