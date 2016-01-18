/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                              (c) Copyright 2009; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*                                         INTERRUPT SERVICES
*
*                             LUMINARY MICRO LM3S8962 on the EK-LM3S8962
*
* Filename      : bsp_int.h
* Version       : V1.02
* Programmer(s) : BAN
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_INT_PRESENT
#define  BSP_INT_PRESENT


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_INT_MODULE
#define  BSP_INT_EXT
#else
#define  BSP_INT_EXT  extern
#endif


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <bsp.h>


/*
*********************************************************************************************************
*                                                 DEFINES
*********************************************************************************************************
*/

#define  BSP_INT_ID_GPIOA                                  0u   /* GPIO Port A                                          */
#define  BSP_INT_ID_GPIOB                                  1u   /* GPIO Port B                                          */
#define  BSP_INT_ID_GPIOC                                  2u   /* GPIO Port C                                          */
#define  BSP_INT_ID_GPIOD                                  3u   /* GPIO Port D                                          */
#define  BSP_INT_ID_GPIOE                                  4u   /* GPIO Port E                                          */
#define  BSP_INT_ID_UART0                                  5u   /* UART0 Rx and Tx                                      */
#define  BSP_INT_ID_UART1                                  6u   /* UART1 Rx and Tx                                      */
#define  BSP_INT_ID_SSI0                                   7u   /* SSI0 Rx and Tx                                       */
#define  BSP_INT_ID_I2C0                                   8u   /* I2C0 Master and Slave                                */
#define  BSP_INT_ID_PWM_FAULT                              9u   /* PWM Fault                                            */
#define  BSP_INT_ID_PWM_GEN0                              10u   /* PWM Generator 0                                      */
#define  BSP_INT_ID_PWM_GEN1                              11u   /* PWM Generator 1                                      */
#define  BSP_INT_ID_PWM_GEN2                              12u   /* PWM Generator 2                                      */
#define  BSP_INT_ID_QEI0                                  13u   /* QEI0                                                 */
#define  BSP_INT_ID_ADC0                                  14u   /* ADC Sequence 0                                       */
#define  BSP_INT_ID_ADC1                                  15u   /* ADC Sequence 1                                       */

#define  BSP_INT_ID_ADC2                                  16u   /* ADC Sequence 2                                       */
#define  BSP_INT_ID_ADC3                                  17u   /* ADC Sequence 3                                       */
#define  BSP_INT_ID_WATCHDOG                              18u   /* Watchdog timer                                       */
#define  BSP_INT_ID_TIMER0A                               19u   /* Timer 0 subtimer A                                   */
#define  BSP_INT_ID_TIMER0B                               20u   /* Timer 0 subtimer B                                   */
#define  BSP_INT_ID_TIMER1A                               21u   /* Timer 1 subtimer A                                   */
#define  BSP_INT_ID_TIMER1B                               22u   /* Timer 1 subtimer B                                   */
#define  BSP_INT_ID_TIMER2A                               23u   /* Timer 2 subtimer A                                   */
#define  BSP_INT_ID_TIMER2B                               24u   /* Timer 2 subtimer B                                   */
#define  BSP_INT_ID_COMP0                                 25u   /* Analog Comparator 0                                  */
#define  BSP_INT_ID_SYSCTL                                28u   /* System Control (PLL, OSC, BO)                        */
#define  BSP_INT_ID_FLASH                                 29u   /* FLASH Control                                        */
#define  BSP_INT_ID_GPIOF                                 30u   /* GPIO Port F                                          */
#define  BSP_INT_ID_GPIOG                                 31u   /* GPIO Port G                                          */

#define  BSP_INT_ID_TIMER3A                               35u   /* Timer 3 subtimer A                                   */
#define  BSP_INT_ID_TIMER3B                               36u   /* Timer 3 subtimer B                                   */
#define  BSP_INT_ID_QEI1                                  38u   /* QEI1                                                 */
#define  BSP_INT_ID_CAN0                                  39u   /* CAN0                                                 */
#define  BSP_INT_ID_ETH                                   42u   /* Ethernet Controller                                  */
#define  BSP_INT_ID_HIBERNATE                             43u   /* Hibernation module                                   */


/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  BSP_IntInit            (void);

void  BSP_IntDisAll          (void);

void  BSP_IntEn              (CPU_DATA       int_id);

void  BSP_IntDis             (CPU_DATA       int_id);

void  BSP_IntClr             (CPU_DATA       int_id);

void  BSP_IntVectSet         (CPU_DATA       int_id,
                              CPU_FNCT_VOID  isr);

void  BSP_IntPrioSet         (CPU_DATA       int_id,
                              CPU_INT08U     prio);

void  BSP_IntHandlerGPIOA    (void);
void  BSP_IntHandlerGPIOB    (void);
void  BSP_IntHandlerGPIOC    (void);
void  BSP_IntHandlerGPIOD    (void);
void  BSP_IntHandlerGPIOE    (void);
void  BSP_IntHandlerUART0    (void);
void  BSP_IntHandlerUART1    (void);
void  BSP_IntHandlerSSI0     (void);
void  BSP_IntHandlerI2C0     (void);
void  BSP_IntHandlerPWM_FAULT(void);
void  BSP_IntHandlerPWM_GEN0 (void);
void  BSP_IntHandlerPWM_GEN1 (void);
void  BSP_IntHandlerPWM_GEN2 (void);
void  BSP_IntHandlerQEI0     (void);
void  BSP_IntHandlerADC0     (void);
void  BSP_IntHandlerADC1     (void);

void  BSP_IntHandlerADC2     (void);
void  BSP_IntHandlerADC3     (void);
void  BSP_IntHandlerWATCHDOG (void);
void  BSP_IntHandlerTIMER0A  (void);
void  BSP_IntHandlerTIMER0B  (void);
void  BSP_IntHandlerTIMER1A  (void);
void  BSP_IntHandlerTIMER1B  (void);
void  BSP_IntHandlerTIMER2A  (void);
void  BSP_IntHandlerTIMER2B  (void);
void  BSP_IntHandlerCOMP0    (void);
void  BSP_IntHandlerSYSCTL   (void);
void  BSP_IntHandlerFLASH    (void);
void  BSP_IntHandlerGPIOF    (void);
void  BSP_IntHandlerGPIOG    (void);

void  BSP_IntHandlerTIMER3A  (void);
void  BSP_IntHandlerTIMER3B  (void);
void  BSP_IntHandlerQEI1     (void);
void  BSP_IntHandlerCAN0     (void);
void  BSP_IntHandlerETH      (void);
void  BSP_IntHandlerHIBERNATE(void);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.                               */
