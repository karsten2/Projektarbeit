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
*
*                             LUMINARY MICRO LM3S8962 on the EK-LM3S8962
*
* Filename      : bsp.c
* Version       : V1.02
* Programmer(s) : BAN
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_MODULE
#include "bsp.h"


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

                                                                /* -------------------- GPIOA PINS -------------------- */
#define  BSP_GPIOA_U0RX                         DEF_BIT_00      /* UART0 Receive.                                       */
#define  BSP_GPIOA_U0TX                         DEF_BIT_01      /* UART1 Transmit.                                      */
#define  BSP_GPIOA_SSI0CLK                      DEF_BIT_02      /* MicroSD Card Slot & OLED.                            */
#define  BSP_GPIOA_OLEDCSN                      DEF_BIT_03      /* OLED Chip Select.                                    */
#define  BSP_GPIOA_SSI0RX                       DEF_BIT_04      /* MicroSD Card Slot & OLED.                            */
#define  BSP_GPIOA_SSI0TX                       DEF_BIT_05      /* MicroSD Card Slot.                                   */

                                                                /* -------------------- GPIOC PINS -------------------- */
#define  BSP_GPIOC_OLEDDC                       DEF_BIT_07      /* OLED.                                                */

                                                                /* -------------------- GPIOD PINS -------------------- */
#define  BSP_GPIOD_CARDCSN                      DEF_BIT_00      /* MicroSD Card Slot Chip Select.                       */

                                                                /* -------------------- GPIOE PINS -------------------- */
#define  BSP_GPIOE_SW_UP                        DEF_BIT_00      /* User Button: Up.                                     */
#define  BSP_GPIOE_SW_DOWN                      DEF_BIT_01      /* User Button: Down.                                   */
#define  BSP_GPIOE_SW_LEFT                      DEF_BIT_02      /* User Button: Left.                                   */
#define  BSP_GPIOE_SW_RIGHT                     DEF_BIT_03      /* User Button: Right.                                  */

                                                                /* -------------------- GPIOF PINS -------------------- */
#define  BSP_GPIOF_PWM0                         DEF_BIT_00      /* User LED.                                            */
#define  BSP_GPIOF_SW_SELECT                    DEF_BIT_01      /* User Button: Select.                                 */

                                                                /* -------------------- GPIOG PINS -------------------- */
#define  BSP_GPIOG_PWM1                         DEF_BIT_01      /* Sound.                                               */


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  BSP_LED_Init(void);

static  void  BSP_PB_Init (void);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    //
    // Set the clocking to run directly from the crystal.
    //
    //SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
    //SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

    BSP_IntInit();

    //
    // Initialize the OLED display and write status.
    //
    //RIT128x96x4Init(1000000);

    BSP_LED_Init();                                             /* Initialize the I/Os for the LED controls.            */
    BSP_PB_Init();                                              /* Initialize the I/Os for the PB  controls.            */
}


/*
*********************************************************************************************************
*                                          BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    CPU_INT32U  freq;


    freq = SysCtlClockGet();
    return (freq);
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LED FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           BSP_LED_Init()
*
* Description : Initialize the I/O for the LEDs
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_LED_Init (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIODirModeSet(GPIO_PORTF_BASE, BSP_GPIOF_PWM0, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTF_BASE, BSP_GPIOF_PWM0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    BSP_LED_Off(0);        /* Turn OFF all the LEDs.                               */
}


/*
*********************************************************************************************************
*                                            BSP_LED_On()
*
* Description : Turn ON any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turn ON all LEDs on the board
*                       1    turn ON LED
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_On (CPU_INT08U led)
{
    switch (led) {
        case 0:
        case 1:
             GPIOPinWrite(GPIO_PORTF_BASE, BSP_GPIOF_PWM0, 0);
             break;

        default:
             break;
    }
}


/*
*********************************************************************************************************
*                                            BSP_LED_Off()
*
* Description : Turn OFF any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turn OFF all LEDs on the board
*                       1    turn OFF LED
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Off (CPU_INT08U led)
{
    switch (led) {
        case 0:
        case 1:
             GPIOPinWrite(GPIO_PORTF_BASE, BSP_GPIOF_PWM0, BSP_GPIOF_PWM0);
             break;

        default:
             break;
    }
}


/*
*********************************************************************************************************
*                                          BSP_LED_Toggle()
*
* Description : TOGGLE any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    TOGGLE all LEDs on the board
*                       1    TOGGLE LED
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Toggle (CPU_INT08U led)
{
    CPU_INT32U  pins;


    switch (led) {
        case 0:
        case 1:
             pins = GPIOPinRead(GPIO_PORTF_BASE, BSP_GPIOF_PWM0);
             if (DEF_BIT_IS_CLR(pins, BSP_GPIOF_PWM0) == DEF_YES) {
                 GPIOPinWrite(GPIO_PORTF_BASE, BSP_GPIOF_PWM0, BSP_GPIOF_PWM0);
             } else {
                 GPIOPinWrite(GPIO_PORTF_BASE, BSP_GPIOF_PWM0, 0);
             }
             break;

        default:
             break;
    }
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            PB FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            BSP_PB_Init()
*
* Description : Initialize the board's PB.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_PB_Init (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIODirModeSet(  GPIO_PORTE_BASE, BSP_GPIOE_SW_UP | BSP_GPIOE_SW_DOWN | BSP_GPIOE_SW_LEFT | BSP_GPIOE_SW_RIGHT, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTE_BASE, BSP_GPIOE_SW_UP | BSP_GPIOE_SW_DOWN | BSP_GPIOE_SW_LEFT | BSP_GPIOE_SW_RIGHT, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(  GPIO_PORTF_BASE, BSP_GPIOF_SW_SELECT, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, BSP_GPIOF_SW_SELECT, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


/*
*********************************************************************************************************
*                                         BSP_PB_GetStatus()
*
* Description : Get the status of a push button on the board.
*
* Argument(s) : pb      The ID of the push button to probe
*
*                       1    probe the SELECT push button
*                       2    probe the UP     push button
*                       3    probe the DOWN   push button
*                       4    probe the LEFT   push button
*                       5    probe the RIGHT  push button
*
* Return(s)   : DEF_FALSE   if the push button is pressed.
*               DEF_TRUE    if the push button is not pressed.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_PB_GetStatus (CPU_INT08U pb)
{
    CPU_BOOLEAN  status;
    CPU_INT32U   pins;


    status = DEF_FALSE;

    switch (pb) {
        case 1:
             pins = GPIOPinRead(GPIO_PORTF_BASE, BSP_GPIOF_SW_SELECT);
             if (pins == 0) {
                 status = DEF_TRUE;
             }
             break;

        case 2:
             pins = GPIOPinRead(GPIO_PORTE_BASE, BSP_GPIOE_SW_UP);
             if (pins == 0) {
                 status = DEF_TRUE;
             }
             break;

        case 3:
             pins = GPIOPinRead(GPIO_PORTE_BASE, BSP_GPIOE_SW_DOWN);
             if (pins == 0) {
                 status = DEF_TRUE;
             }
             break;

        case 4:
             pins = GPIOPinRead(GPIO_PORTE_BASE, BSP_GPIOE_SW_LEFT);
             if (pins == 0) {
                 status = DEF_TRUE;
             }
             break;

        case 5:
             pins = GPIOPinRead(GPIO_PORTE_BASE, BSP_GPIOE_SW_RIGHT);
             if (pins == 0) {
                 status = DEF_TRUE;
             }
             break;

        default:
             break;
    }

    return (status);
}
