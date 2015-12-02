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
    long        lIntTimerA0Priority;
    volatile  CPU_INT32U  ulCount;


    (void)&p_arg;

    // Prepare Port F for DIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);								// GPIO: Enable Port F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);			// GPIO: Enable PF, Pin 2&3


    // Prepare Timer0 and output
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); 								// The Timer peripheral must be enabled for use
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);							    // Enable Port D for CCP0 on PD4
    GPIOPinConfigure(GPIO_PD4_CCP0);											// Configure the GPIO pin muxing for the Timer/CCP function
    GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_4);								// Configure the CCP settings for CCP pin

    TimerConfigure(TIMER0_BASE, TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_PWM);       // Configure Timer0A as a 16-bit periodic timer.
    TimerLoadSet(TIMER0_BASE, TIMER_A, 10000 );					                // Rate based on 50MHz: 10000 means 200µs
    TimerMatchSet(TIMER0_BASE, TIMER_A, TimerLoadGet(TIMER0_BASE, TIMER_A) /3); // Set the Timer match value: %

    // Timer0B: Interrupt
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);                            // Configure the Timer0A interrupt for timer timeout.
    BSP_IntPrioSet(INT_TIMER0A, 3 );

    TimerEnable(TIMER0_BASE, TIMER_A);                                          // Enable Timer0A.

//
    CPU_IntSrcPrioSet(CPU_INT_SYSTICK, 5);
//    BSP_IntPrioSet(INT_UART0, 3);
//    lIntTimerA0Priority = CPU_IntSrcPrioGet(INT_TIMER0A);
//    CPU_IntSrcPrioSet(INT_TIMER0A, lIntTimerA0Priority-1 );
    CPU_IntSrcPrioSet(INT_TIMER0A, 4 );



    IntEnable(INT_TIMER0A);                                                     // Enable the Timer0B interrupt on the processor (NVIC).
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
void  My_IntHandler_TIMER0A(void)
{
  OS_ERR      err;

#if (APP_TIMER_INT_PROCESSING == 1)
  CPU_SR_ALLOC();
  CPU_CRITICAL_ENTER();
  OSIntEnter();
  CPU_CRITICAL_EXIT();
#endif


  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);

  TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

  OSTaskSemPost( &AppTaskTimerSyncTCB,
                 OS_OPT_POST_NONE,
                 &err );

  g_ulCountTimerISR++;

  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0 );


#if (APP_TIMER_INT_PROCESSING == 1)
  OSIntExit();
#endif

}


