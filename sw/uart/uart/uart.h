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
#define UART_IRQ_NAME       USART0_RX_IRQHandler
#define UART_CLK            cmuClock_USART1
#define UART_IRQN           USART0_RX_IRQn
#define UART_PORT           USART0
#define UART_TX             USART_Tx
#define UART_RX             USART_Rx
#define UART_LOC            USART_ROUTE_LOCATION_LOC0
#define UART_TXPORT         gpioPortE
#define UART_TXPIN          10
#define UART_RXPORT         gpioPortE
#define UART_RXPIN          11

/*============================================================================*/
/* Function prototypes                                                        */
/*============================================================================*/
void    uart_init();
void    uart_putc(char c);
int     uart_puts(char *s);

#endif // __FRW_UART_H_
