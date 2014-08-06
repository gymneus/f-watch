/*
 *==============================================================================
 * CERN (BE-CO-HT)
 * Header file for Freewatch UART library
 *==============================================================================
 *
 * author: Theodor Stana (t.stana@cern.ch)
 *
 * date of creation: 2014-07-07
 *
 * version: 1.0
 *
 * description:
 *
 * dependencies:
 *
 * references:
 *
 *==============================================================================
 * GNU LESSER GENERAL PUBLIC LICENSE
 *==============================================================================
 * This source file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version. This source is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details. You should have
 * received a copy of the GNU Lesser General Public License along with this
 * source; if not, download it from http://www.gnu.org/licenses/lgpl-2.1.html
 *==============================================================================
 * last changes:
 *    2014-07-07   Theodor Stana     t.stana@cern.ch     File created
 *==============================================================================
 * TODO: -
 *==============================================================================
 */

#ifndef __UART_H_
#define __UART_H_

#include "em_device.h"

#include "em_usart.h"

/*============================================================================*/
/* Defines                                                                    */
/*============================================================================*/
/* Use USART/LEUART definition; no UART for EFM32GG330, thus not supported    */
/* Should be mutually exclusive, else compilation error                       */
#define UART_USE_USART
//#define UART_USE_LEUART

/* Change the definition files below according to the port you use */
#if defined(UART_USE_USART)
        #define UART_PORT       USART0
        #define UART_IRQ_NAME   USART0_RX_IRQHandler
        #define UART_CLK        cmuClock_USART0
        #define UART_IRQN       USART0_RX_IRQn
        #define UART_TX         USART_Tx
        #define UART_RX         USART_Rx
        #define UART_TXPORT     gpioPortE
        #define UART_TXPIN      10
        #define UART_RXPORT     gpioPortE
        #define UART_RXPIN      11
        #define UART_LOC        USART_ROUTE_LOCATION_LOC0
#endif

#if defined(UART_USE_LEUART)
        #define UART_PORT       LEUART1
        #define UART_IRQ_NAME   LEUART1_IRQHandler
        #define UART_CLK        cmuClock_LEUART1
        #define UART_IRQN       LEUART1_IRQn
        #define UART_UART       LEUART1
        #define UART_TX         LEUART_Tx
        #define UART_RX         LEUART_Rx
        #define UART_TXPORT     gpioPortC
        #define UART_TXPIN      6
        #define UART_RXPORT     gpioPortC
        #define UART_RXPIN      7
        #define UART_LOC        LEUART_ROUTE_LOCATION_LOC0
#endif

/*============================================================================*/
/* Function prototypes                                                        */
/*============================================================================*/
void    uart_init(int baud, int databits, char *parity, char *stopbits);
void    uart_putc(char c);
int     uart_puts(char *s);
char    uart_getc();
int     uart_gets(char *s);

#endif // __UART_H_
