/*
*********************************************************************************************************
*                                              taskHandler
*
* Description:	Call initTasks() to init the multithreading and start both AIs.
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                Includes
*********************************************************************************************************
*/

#include "includes.h"
#include "globals.h"


/*
*********************************************************************************************************
*                                             local Variables
*********************************************************************************************************
*/

static  OS_TCB       AppTaskStarterTCB;
static  OS_TCB       AppTask_Ai_1_TCB;
static  OS_TCB       AppTask_Ai_2_TCB;

static  CPU_STK      AppTaskStarterStk[APP_TASK_START_STK_SIZE];
static  CPU_STK      AppTask_Ai_1_Stk[APP_TASK_AI_1_STK_SIZE];
static  CPU_STK      AppTask_Ai_2_Stk[APP_TASK_AI_2_STK_SIZE];


static void AppTask_Ai_2 (void *p_arg) {
	OS_ERR      err;

	while (DEF_ON) {
		OSTimeDlyHMSM(0u, 0u, 1u, 0u,
				OS_OPT_TIME_HMSM_STRICT,
				&err);

		// Do something
		//BSP_LED_Toggle(0);
	}
}


static void App_Ai_2_TaskCreate (void) {
	OS_ERR  err;


	                                                                /* Create Display Task                                  */
	OSTaskCreate((OS_TCB     *)&AppTask_Ai_2_TCB,
	             (CPU_CHAR   *)"Ai_2",
	             (OS_TASK_PTR ) AppTask_Ai_2,
	             (void       *) 0,
	             (OS_PRIO     ) APP_TASK_AI_2_PRIO,
	             (CPU_STK    *)&AppTask_Ai_2_Stk[0],
	             (CPU_STK_SIZE) APP_TASK_AI_2_STK_SIZE / 10u,
	             (CPU_STK_SIZE) APP_TASK_AI_2_STK_SIZE,
	             (OS_MSG_QTY  ) 0u,
	             (OS_TICK     ) 0u,
	             (void       *) 0,
	             (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
	             (OS_ERR     *)&err);
}


static void AppTask_Ai_1 (void *p_arg) {
	OS_ERR      err;


	while (DEF_ON) {
		OSTimeDlyHMSM(0u, 0u, 1u, 0u,
				OS_OPT_TIME_HMSM_STRICT,
				&err);

		// Do something
		BSP_LED_Toggle(0);
	}
}


static void App_Ai_1_TaskCreate (void) {
	OS_ERR  err;


	                                                                /* Create Display Task                                  */
	OSTaskCreate((OS_TCB     *)&AppTask_Ai_1_TCB,
	             (CPU_CHAR   *)"Ai_1",
	             (OS_TASK_PTR ) AppTask_Ai_1,
	             (void       *) 0,
	             (OS_PRIO     ) APP_TASK_AI_1_PRIO,
	             (CPU_STK    *)&AppTask_Ai_1_Stk[0],
	             (CPU_STK_SIZE) APP_TASK_AI_1_STK_SIZE / 10u,
	             (CPU_STK_SIZE) APP_TASK_AI_1_STK_SIZE,
	             (OS_MSG_QTY  ) 0u,
	             (OS_TICK     ) 0u,
	             (void       *) 0,
	             (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
	             (OS_ERR     *)&err);
}





/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : Task to initialize both AIs and handle the output
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none.
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStarter (void  *p_arg)
{
    CPU_INT32U  clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;


   (void)&p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */

    clk_freq = BSP_CPU_ClkFreq();                               /* Determine SysTick reference freq.                    */
    cnts     = clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* Determine nbr SysTick increments                     */
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */
    CPU_TS_TmrFreqSet(clk_freq);

    CPU_IntDisMeasMaxCurReset();
    App_ProbeInit();

    // Create Task for AI 1
    App_Ai_1_TaskCreate();

    // Create Task for AI 2
    App_Ai_2_TaskCreate();


    CPU_INT08U  state;
    CPU_CHAR    ucText[20];
    CPU_INT08U  ucYOffset;

    state = 0u;

    while (DEF_ON) {                                            /* Task body, always written as an infinite loop.       */
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);

        //BSP_LED_Toggle(0);                                      /* Toggle LED                       */

        // Handle the ai moves on the board
        // Toggle Task priority


        switch(state) {
                	case 0:
                		RIT128x96x4Clear();
                        state = 1u;
                        break;

                    case 1:
                    	 ucYOffset = 0;
                    	 sprintf( ucText, "Tsks: %3u  CPU: %3u%%", OSTaskQty, OSStatTaskCPUUsage );
                    	 RIT128x96x4StringDraw( ucText, 0u, 0u, 8);

                    	 sprintf( ucText, "Lckd: %3u  Cur: %3u", OSSchedLockTimeMax, OSSchedLockTimeMaxCur );
                    	 RIT128x96x4StringDraw( ucText, 0u, 9u, 8);

                    	 sprintf( ucText, "CtxS: %12u", OSTaskCtxSwCtr );
                    	 RIT128x96x4StringDraw( ucText, 0u, 18u, 8);

                    	 sprintf( ucText, "StTC: %12u", OSStatTaskCtr );
                    	 RIT128x96x4StringDraw( ucText, 0u, 27u, 8);

                         state = 1u;
                         break;

                    default:
                         state = 0u;
                         break;
        }
    }
}


void initTasks(void) {
	OS_ERR err;

	CPU_IntDis();							// Disable all Interrupts
	OSInit(&err); 							// Init uC/OS-III.

	OSTaskCreate(
			(OS_TCB *) &AppTaskStarterTCB, /* Create the start task                                */
			(CPU_CHAR *) "Calling AppTaskStarter",
			(OS_TASK_PTR) AppTaskStarter, (void *) 0,
			(OS_PRIO) APP_TASK_STARTER_PRIO,
			(CPU_STK *) &AppTaskStarterStk[0],
			(CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10u,
			(CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
			(OS_MSG_QTY) 0u,
			(OS_TICK) 0u, (void *) 0,
			(OS_OPT) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
			(OS_ERR *) &err);

	OSStart(&err); /* Start multitasking (i.e. give control to uC/OS-III). */

}
