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
*                                            UART SERVICES
*
*                             LUMINARY MICRO LM3S8962 on the EK-LM3S8962
*
* Filename      : bsp_ser.c
* Version       : V1.02
* Programmer(s) : BAN
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_SER_MODULE
#include <bsp.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  BSP_SER_PORT_NBR_MAX               BSP_SER_ID_UART0

#define  LM3SXXXX_BASE_UART0                    0x4000C000uL
#define  LM3SXXXX_BASE_UART1                    0x4000D000uL
#define  LM3SXXXX_BASE_UART2                    0x4000E000uL

/*
*********************************************************************************************************
*                                        REGISTER BIT DEFINES
*********************************************************************************************************
*/

#define  LM3SXXXX_BIT_UARTDR_FE                  DEF_BIT_08
#define  LM3SXXXX_BIT_UARTDR_PE                  DEF_BIT_09
#define  LM3SXXXX_BIT_UARTDR_BE                  DEF_BIT_10
#define  LM3SXXXX_BIT_UARTDR_OE                  DEF_BIT_11

#define  LM3SXXXX_BIT_UARTRSR_FE                 DEF_BIT_00
#define  LM3SXXXX_BIT_UARTRSR_PE                 DEF_BIT_01
#define  LM3SXXXX_BIT_UARTRSR_BE                 DEF_BIT_02
#define  LM3SXXXX_BIT_UARTRSR_OE                 DEF_BIT_03

#define  LM3SXXXX_BIT_UARTFR_CTS                 DEF_BIT_00
#define  LM3SXXXX_BIT_UARTFR_DSR                 DEF_BIT_01
#define  LM3SXXXX_BIT_UARTFR_DCD                 DEF_BIT_02
#define  LM3SXXXX_BIT_UARTFR_BUSY                DEF_BIT_03
#define  LM3SXXXX_BIT_UARTFR_RXFE                DEF_BIT_04
#define  LM3SXXXX_BIT_UARTFR_TXFF                DEF_BIT_05
#define  LM3SXXXX_BIT_UARTFR_RXFF                DEF_BIT_06
#define  LM3SXXXX_BIT_UARTFR_TXFE                DEF_BIT_07
#define  LM3SXXXX_BIT_UARTFR_RI                  DEF_BIT_08

#define  LM3SXXXX_BIT_UARTLCRH_BRK               DEF_BIT_00
#define  LM3SXXXX_BIT_UARTLCRH_PEN               DEF_BIT_01
#define  LM3SXXXX_BIT_UARTLCRH_EPS               DEF_BIT_02
#define  LM3SXXXX_BIT_UARTLCRH_STP2              DEF_BIT_03
#define  LM3SXXXX_BIT_UARTLCRH_FEN               DEF_BIT_04
#define  LM3SXXXX_BIT_UARTLCRH_WLEN_5            0x00000000
#define  LM3SXXXX_BIT_UARTLCRH_WLEN_6            0x00000020
#define  LM3SXXXX_BIT_UARTLCRH_WLEN_7            0x00000040
#define  LM3SXXXX_BIT_UARTLCRH_WLEN_8            0x00000060

#define  LM3SXXXX_BIT_UARTCTL_UARTEN             DEF_BIT_00
#define  LM3SXXXX_BIT_UARTCTL_SIREN              DEF_BIT_01
#define  LM3SXXXX_BIT_UARTCTL_SIRLP              DEF_BIT_02
#define  LM3SXXXX_BIT_UARTCTL_SMART              DEF_BIT_03
#define  LM3SXXXX_BIT_UARTCTL_EOT                DEF_BIT_04
#define  LM3SXXXX_BIT_UARTCTL_HSE                DEF_BIT_05
#define  LM3SXXXX_BIT_UARTCTL_LIN                DEF_BIT_06
#define  LM3SXXXX_BIT_UARTCTL_LBE                DEF_BIT_07
#define  LM3SXXXX_BIT_UARTCTL_TXE                DEF_BIT_08
#define  LM3SXXXX_BIT_UARTCTL_RXE                DEF_BIT_09
#define  LM3SXXXX_BIT_UARTCTL_DTR                DEF_BIT_10
#define  LM3SXXXX_BIT_UARTCTL_RTS                DEF_BIT_11

#define  LM3SXXXX_BIT_UARTFLS_RX_1_8             0x00000000
#define  LM3SXXXX_BIT_UARTFLS_RX_1_4             0x00000080
#define  LM3SXXXX_BIT_UARTFLS_RX_1_2             0x00000100
#define  LM3SXXXX_BIT_UARTFLS_RX_3_4             0x00000180
#define  LM3SXXXX_BIT_UARTFLS_RX_7_8             0x00000200
#define  LM3SXXXX_BIT_UARTFLS_TX_1_8             0x00000000
#define  LM3SXXXX_BIT_UARTFLS_TX_1_4             0x00000001
#define  LM3SXXXX_BIT_UARTFLS_TX_1_2             0x00000002
#define  LM3SXXXX_BIT_UARTFLS_TX_3_4             0x00000003
#define  LM3SXXXX_BIT_UARTFLS_TX_7_8             0x00000004

#define  LM3SXXXX_BIT_UARTINT_RX                 DEF_BIT_04
#define  LM3SXXXX_BIT_UARTINT_TX                 DEF_BIT_05
#define  LM3SXXXX_BIT_UARTINT_RT                 DEF_BIT_06
#define  LM3SXXXX_BIT_UARTINT_FE                 DEF_BIT_07
#define  LM3SXXXX_BIT_UARTINT_PE                 DEF_BIT_08
#define  LM3SXXXX_BIT_UARTINT_BE                 DEF_BIT_09
#define  LM3SXXXX_BIT_UARTINT_OE                 DEF_BIT_10


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

typedef  struct  lm3sxxxx_struct_uart {
    CPU_REG32  DR;
    CPU_REG32  RSR_ECR;
    CPU_REG32  Dummy1;
    CPU_REG32  Dummy2;
    CPU_REG32  Dummy3;
    CPU_REG32  Dummy4;
    CPU_REG32  FR;
    CPU_REG32  Dummy5;
    CPU_REG32  ILPR;
    CPU_REG32  IBRD;
    CPU_REG32  FBRD;
    CPU_REG32  LCRH;
    CPU_REG32  CTL;
    CPU_REG32  IFLS;
    CPU_REG32  IM;
    CPU_REG32  RIS;
    CPU_REG32  MIS;
    CPU_REG32  ICR;
    CPU_REG32  DMACTL;
} LM3SXXXX_STRUCT_UART;


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


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            BSP_SerInit()
*
* Description : Initialize UART.
*
* Argument(s) : port_id     Port to initailize.
*
*               baud_rate   Baud rate.
*
* Return(s)   : DEF_YES, if UART initialized.
&               DEF_NO,  otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_SerInit (CPU_DATA    port_id,
                          CPU_INT32U  baud_rate)
{
    LM3SXXXX_STRUCT_UART  *uart;
    CPU_INT32U             idiv;
    CPU_INT32U             fdiv;
    CPU_INT32U             clk_freq;



    if (port_id > BSP_SER_PORT_NBR_MAX) {
        return (DEF_NO);
    }


                                                                /* --------------- COMPUTE DIV BAUD RATE -------------- */
    clk_freq = BSP_CPU_ClkFreq();
    idiv     = clk_freq / (16u * baud_rate);
    fdiv     = clk_freq % (16u * baud_rate);
    fdiv     = ((((2u * fdiv * 4u) / baud_rate) + 1u) / 2u);


                                                                /* --------------------- INIT PORT -------------------- */
    switch (port_id) {
        case BSP_SER_ID_UART0:
             SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
             SysCtlPeripheralReset(SYSCTL_PERIPH_UART0);
             SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
             GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
             uart = (LM3SXXXX_STRUCT_UART *)LM3SXXXX_BASE_UART0;
             break;

#if (BSP_SER_PORT_NBR_MAX >= BSP_SER_ID_UART1)
        case BSP_SER_ID_UART1:
             SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
             SysCtlPeripheralReset(SYSCTL_PERIPH_UART1);
             SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
             GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3);
             uart = (LM3SXXXX_STRUCT_UART *)LM3SXXXX_BASE_UART1;
             break;
#endif

#if (BSP_SER_PORT_NBR_MAX >= BSP_SER_ID_UART2)
        case BSP_SER_ID_UART2:
             SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
             SysCtlPeripheralReset(SYSCTL_PERIPH_UART2);
             SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
             GPIOPinTypeUART(GPIO_PORTG_BASE, GPIO_PIN_0 | GPIO_PIN_1);
             uart = (LM3SXXXX_STRUCT_UART *)LM3SXXXX_BASE_UART2;
             break;
#endif

        default:
             return (DEF_NO);
    }

    uart->IBRD = idiv;
    uart->FBRD = fdiv;
    uart->LCRH = LM3SXXXX_BIT_UARTLCRH_WLEN_8
               | LM3SXXXX_BIT_UARTLCRH_FEN;
    uart->FR   = 0x00000000;
    uart->CTL  = LM3SXXXX_BIT_UARTCTL_RXE | LM3SXXXX_BIT_UARTCTL_TXE | LM3SXXXX_BIT_UARTCTL_UARTEN;
    uart->IFLS = 0x00000000;
    uart->ICR  = LM3SXXXX_BIT_UARTINT_OE  | LM3SXXXX_BIT_UARTINT_BE  | LM3SXXXX_BIT_UARTINT_PE |
                 LM3SXXXX_BIT_UARTINT_FE  | LM3SXXXX_BIT_UARTINT_RT  | LM3SXXXX_BIT_UARTINT_TX | LM3SXXXX_BIT_UARTINT_RX;

    return (DEF_YES);
}


/*
*********************************************************************************************************
*                                             BSP_SerRd()
*
* Description : Read octet(s) from UART.
*
* Argument(s) : port_id     Port to read from.
*
*               p_dest      Pointer to destination buffer.
*
*               size        Number of octets to read.
*
* Return(s)   : The number of octets read.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_SIZE_T  BSP_SerRd (CPU_DATA     port_id,
                       void        *p_dest,
                       CPU_SIZE_T   size)
{
    LM3SXXXX_STRUCT_UART  *uart;
    CPU_INT08U            *p_dest_08;
    CPU_SIZE_T             cnt;
    CPU_INT08U             datum;
    CPU_INT32U             fr;


    if (port_id > BSP_SER_PORT_NBR_MAX) {
        return (DEF_NO);
    }

    switch (port_id) {
        case BSP_SER_ID_UART0:
             uart = (LM3SXXXX_STRUCT_UART *)LM3SXXXX_BASE_UART0;
             break;

#if (BSP_SER_PORT_NBR_MAX >= BSP_SER_ID_UART1)
        case BSP_SER_ID_UART1:
             uart = (LM3SXXXX_STRUCT_UART *)LM3SXXXX_BASE_UART1;
             break;
#endif

#if (BSP_SER_PORT_NBR_MAX >= BSP_SER_ID_UART2)
        case BSP_SER_ID_UART2:
             uart = (LM3SXXXX_STRUCT_UART *)LM3SXXXX_BASE_UART2;
             break;
#endif

        default:
             return (0u);
    }


    p_dest_08 = (CPU_INT08U *)p_dest;
    cnt       =  0u;
    while (cnt < size) {
        fr = uart->FR;
        while (DEF_BIT_IS_SET(fr, LM3SXXXX_BIT_UARTFR_RXFE) == DEF_YES) {
            BSP_OS_Dly_ms(1u);
            fr = uart->FR;
        }
        datum     = (CPU_INT08U)uart->DR;
       *p_dest_08 =  datum;
        p_dest_08++;
        cnt++;
    }
    return (cnt);
}


/*
*********************************************************************************************************
*                                           BSP_SerRdLine()
*
* Description : Read line from UART.
*
* Argument(s) : port_id     Port to read from.
*
*               p_dest      Pointer to destination buffer.
*
*               size        Maximum number of octets to read.
*
* Return(s)   : The number of octets read.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_SIZE_T  BSP_SerRdLine(CPU_DATA     port_id,
                          void        *p_dest,
                          CPU_SIZE_T   size)
{
    LM3SXXXX_STRUCT_UART  *uart;
    CPU_INT08U            *p_dest_08;
    CPU_SIZE_T             cnt;
    CPU_INT08U             datum;
    CPU_INT32U             fr;
    CPU_BOOLEAN            print;


    if (port_id > BSP_SER_PORT_NBR_MAX) {
        return (DEF_NO);
    }

    switch (port_id) {
        case BSP_SER_ID_UART0:
             uart = (LM3SXXXX_STRUCT_UART *)LM3SXXXX_BASE_UART0;
             break;

#if (BSP_SER_PORT_NBR_MAX >= BSP_SER_ID_UART1)
        case BSP_SER_ID_UART1:
             uart = (LM3SXXXX_STRUCT_UART *)LM3SXXXX_BASE_UART1;
             break;
#endif

#if (BSP_SER_PORT_NBR_MAX >= BSP_SER_ID_UART2)
        case BSP_SER_ID_UART2:
             uart = (LM3SXXXX_STRUCT_UART *)LM3SXXXX_BASE_UART2;
             break;
#endif

        default:
             return (0u);
    }


    p_dest_08 = (CPU_INT08U *)p_dest;
    cnt       =  0u;
    while (cnt < size - 1u) {
        fr = uart->FR;
        while (DEF_BIT_IS_SET(fr, LM3SXXXX_BIT_UARTFR_RXFE) == DEF_YES) {
            BSP_OS_Dly_ms(1u);
            fr = uart->FR;
        }
        datum = (CPU_INT08U)uart->DR;

        print =  ASCII_IS_PRINT(datum);
        if (print == DEF_YES) {
           *p_dest_08 =  datum;
            p_dest_08++;
            BSP_SerWr(port_id, (void *)&datum, 1);
        } else {
            if (datum == ASCII_CHAR_BACKSPACE) {
                if (cnt > 0u) {
                    BSP_SerWr(port_id, (void *)"\b \b", 3);
                    cnt--;
                }
            } else if (datum == ASCII_CHAR_CARRIAGE_RETURN) {
               *p_dest_08 = (CPU_CHAR)ASCII_CHAR_NULL;
                return (cnt);
            }
        }
        cnt++;
    }
   *p_dest_08 = (CPU_CHAR)ASCII_CHAR_NULL;
    return (cnt);
}


/*
*********************************************************************************************************
*                                             BSP_SerWr()
*
* Description : Write octet(s) to UART.
*
* Argument(s) : port_id     Port to write to.
*
*               p_src       Pointer to source buffer.
*
*               size        Number of octets to write.
*
* Return(s)   : The number of octets written.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_SIZE_T  BSP_SerWr (CPU_DATA     port_id,
                       void        *p_src,
                       CPU_SIZE_T   size)
{
    LM3SXXXX_STRUCT_UART  *uart;
    CPU_INT08U            *p_src_08;
    CPU_SIZE_T             cnt;
    CPU_INT08U             datum;
    CPU_INT32U             fr;


    if (port_id > BSP_SER_PORT_NBR_MAX) {
        return (DEF_NO);
    }

    switch (port_id) {
        case BSP_SER_ID_UART0:
             uart = (LM3SXXXX_STRUCT_UART *)LM3SXXXX_BASE_UART0;
             break;

#if (BSP_SER_PORT_NBR_MAX >= BSP_SER_ID_UART1)
        case BSP_SER_ID_UART1:
             uart = (LM3SXXXX_STRUCT_UART *)LM3SXXXX_BASE_UART1;
             break;
#endif

#if (BSP_SER_PORT_NBR_MAX >= BSP_SER_ID_UART2)
        case BSP_SER_ID_UART2:
             uart = (LM3SXXXX_STRUCT_UART *)LM3SXXXX_BASE_UART2;
             break;
#endif

        default:
             return (0u);
    }


    p_src_08 = (CPU_INT08U *)p_src;
    cnt      =  0u;
    while (cnt < size) {
        fr = uart->FR;
        while (DEF_BIT_IS_SET(fr, LM3SXXXX_BIT_UARTFR_TXFF) == DEF_YES) {
            BSP_OS_Dly_ms(1u);
            fr = uart->FR;
        }
        datum     = *p_src_08;
        uart->DR  =  datum;
        p_src_08++;
        cnt++;
    }
    return (cnt);
}


/*
*********************************************************************************************************
*                                           BSP_SerPrint()
*
* Description : Print to UART.
*
* Argument(s) : port_id     Port to write to.
*
*               p_src       Pointer to source buffer.
*
*               size        Number of octets to write.
*
* Return(s)   : The number of octets written.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_SIZE_T  BSP_SerPrint (CPU_DATA     port_id,
                          CPU_CHAR    *fmt,
                       /* ARGS */      ...)
{
    CPU_CHAR    buf[128u + 1u];
    CPU_SIZE_T  len;
    CPU_SIZE_T  len_wr;
    va_list     vArgs;


    buf[0] = (CPU_CHAR)ASCII_CHAR_NULL;

    va_start(vArgs, format);
    vsnprintf((char *)buf, sizeof(buf) - 1, (char const *)fmt, vArgs);
    va_end(vArgs);

    len    = Str_Len(buf);
    len_wr = BSP_SerWr (        port_id,
                        (void *)buf,
                                len);

    return (len_wr);
}


/*
*********************************************************************************************************
*                                           BSP_SerPrint()
*
* Description : Print to debug UART.
*
* Argument(s) : p_src       Pointer to source buffer.
*
*               size        Number of octets to write.
*
* Return(s)   : The number of octets written.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

int  BSP_Print (const  char  *fmt,
               /* ARGS */     ...)
{
    CPU_CHAR    buf[128u + 1u];
    CPU_SIZE_T  len;
    CPU_SIZE_T  len_wr;
    va_list     vArgs;


    buf[0] = (CPU_CHAR)ASCII_CHAR_NULL;

    va_start(vArgs, format);
    vsnprintf((char *)buf, sizeof(buf) - 1, (char const *)fmt, vArgs);
    va_end(vArgs);

    len    = Str_Len(buf);
    len_wr = BSP_SerWr (        BSP_SER_ID_UART0,
                        (void *)buf,
                                len);

    return ((int)len_wr);
}
