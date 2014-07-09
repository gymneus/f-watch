/*
 *==============================================================================
 * CERN (BE-CO-HT)
 * Source file for Freewatch UART library
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

#include "uart.h"

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

/*
 *==============================================================================
 * UART initialization
 *
 *  params:
 *     * none
 *
 *  returns:
 *     * none
 *
 *  comments:
 *     Initializes UART library for selected UART
 *
 *==============================================================================
 */
void uart_init()
{
        /* Configure GPIO pins */
        CMU_ClockEnable(cmuClock_GPIO, true);

        /* To avoid false start, configure output as high */
        GPIO_PinModeSet(UART_TXPORT, UART_TXPIN, gpioModePushPull, 1);
        GPIO_PinModeSet(UART_RXPORT, UART_RXPIN, gpioModeInput, 0);

        USART_TypeDef           *usart = UART_PORT;
        USART_InitAsync_TypeDef init   = USART_INITASYNC_DEFAULT;

        /* Enable peripheral clocks */
        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(UART_CLK, true);

        /* Configure USART for basic async operation */
        init.enable = usartDisable;
        USART_InitAsync(usart, &init);

        /* Enable pins at UART1 location #2 */
        usart->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | UART_LOC;

        /* Clear previous RX interrupts */
        USART_IntClear(UART_PORT, USART_IF_RXDATAV);
        NVIC_ClearPendingIRQ(UART_IRQN);

        /* Enable RX interrupts */
        USART_IntEnable(UART_PORT, USART_IF_RXDATAV);
        NVIC_EnableIRQ(UART_IRQN);

        /* Finally enable it */
        USART_Enable(usart, usartEnable);
}

/*
 *==============================================================================
 * Send character
 *
 *  params:
 *     * char to send
 *
 *  returns:
 *     * none
 *
 *  comments:
 *     Calls the EMLIB function for sending a character over the selected
 *     UART peripheral.
 *
 *==============================================================================
 */
void uart_putc(char c)
{
        USART_Tx(UART_PORT, c);
}

/*
 *==============================================================================
 * Send string
 *
 *  params:
 *     * string to send
 *
 *  returns:
 *     * number of chars sent
 *
 *  comments:
 *      Sends the characters in a string over the selected UART peripheral until
 *      the null character is encountered and returns the number of characters
 *      sent.
 *
 *==============================================================================
 */
int uart_puts(char *s)
{
        int nc = 0;
        for (; *s == '\0'; *s++)
        {
                uart_putc(*s);
                nc++;
        }
        return nc;
}

