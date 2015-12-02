/* *******************************************************************************************************
*
*
* Filename      : app_timer.c
* Version       : $Id: $
* Programmer(s) : DUC
********************************************************************************************************* */

#define APP_TIMER_MODULE

// ******************************  INTERFACE (includes)
#include "app_timer.h"


// ******************************  INTERFACE (definition of global/observable variables)
CPU_INT32U    g_ulCountTimerISR  = 0;
CPU_INT32U    g_ulCountTimerTask = 0;
CPU_INT32U    g_ulCountTimerDiff = 0;
CPU_INT32U    g_ulCountDiff = 0;


// ******************************  INTERFACE (definition of variables)
OS_TCB        AppTaskTimerSyncTCB;
CPU_STK       AppTaskTimerSyncStk[APP_TASK_TIMER_SYNC_STK_SIZE];


// ******************************  FUNCTION PROTOTYPES


// ******************************  LOCAL DEFINES


// ******************************  LOCAL DATA TYPES


// ******************************  LOCAL TABLES


// ******************************  LOCAL GLOBAL VARIABLES


// ******************************  LOCAL FUNCTION PROTOTYPES


// ******************************  LOCAL CONFIGURATION ERRORS


// ################################################
void  AppTaskTimerSync (void  *p_arg)
{
    CPU_TS      ts;
    OS_ERR      err;
    CPU_INT32U  ulTemp;
    long        lIntTimerB0Priority;
    volatile  CPU_INT32U  ulCount;


    (void)&p_arg;


    // Timer0: Enable for use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // GPIO: Enable Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // GPIO: Enable PF, Pin 2&3
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);

    // GPIO: Timer0B, output CCP1 for PJ6
    //GPIOPinConfigure(GPIO_PA6_CCP1);
    //GPIOPinTypeTimer(GPIO_PORTA_BASE, GPIO_PIN_6);

    // Timer0B: 16-bit, PWM, ???ms
    TimerConfigure(TIMER0_BASE, TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_PWM);             // PWM Output
    TimerLoadSet(TIMER0_BASE, TIMER_B, SysCtlClockGet() / 10);                 // Timer0B to turnaround time of ....
    TimerMatchSet(TIMER0_BASE, TIMER_B, TimerLoadGet(TIMER0_BASE, TIMER_B) / 3);  // PWM-Out of TimerB = 33%

    // Timer0B: Interrupt
    TimerIntEnable(TIMER0_BASE, TIMER_TIMB_TIMEOUT);                                  // Configure the Timer0B interrupt for timer timeout.
    BSP_IntPrioSet(INT_TIMER0B, 3 );

    TimerEnable(TIMER0_BASE, TIMER_B);                                                // Enable Timer0B.

    //
//    CPU_IntSrcPrioSet(CPU_INT_SYSTICK, 5);
//    BSP_IntPrioSet(INT_UART0, 3);
//    lIntTimerB0Priority = BSP_IntPriorityGet(INT_TIMER0B);
//    BSP_IntPrioritySet(INT_TIMER0B, lIntTimerB0Priority-1 );



    IntEnable(INT_TIMER0B);                                                           // Enable the Timer0B interrupt on the processor (NVIC).
    while (DEF_ON)
     {
        OSTaskSemPend( 10000u,
                      OS_OPT_PEND_BLOCKING,
                      &ts,
                      &err);

        GPIOPinWrite( GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2 );

        g_ulCountTimerTask++;
        ulTemp = (g_ulCountTimerISR-g_ulCountTimerTask);
        if (ulTemp != g_ulCountTimerDiff)
        {
           g_ulCountTimerDiff = ulTemp;
           g_ulCountDiff++;
        }

        for (ulCount=0;ulCount<100;ulCount++)
          {
          ulTemp = (g_ulCountTimerISR-g_ulCountTimerTask);
          if (ulTemp != g_ulCountTimerDiff)
             g_ulCountTimerDiff = ulTemp;
          };

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
     }
}

// #####################################################
void  My_IntHandler_TIMER0B(void)
{
  OS_ERR      err;

#if (APP_TIMER_INT_PROCESSING == 1)
  CPU_SR_ALLOC();
  CPU_CRITICAL_ENTER();
  OSIntEnter();
  CPU_CRITICAL_EXIT();
#endif


  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);

  TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);

  OSTaskSemPost( &AppTaskTimerSyncTCB,
                 OS_OPT_POST_NONE,
                 &err );

  g_ulCountTimerISR++;

  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0 );


#if (APP_TIMER_INT_PROCESSING == 1)
  OSIntExit();
#endif

}


