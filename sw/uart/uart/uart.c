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

#include <string.h>

#include "uart.h"

#include "em_cmu.h"
#include "em_int.h"
#include "em_gpio.h"

#if defined(UART_USE_USART)
#include "em_usart.h"
#else
#include "em_leuart.h"
#endif

#define RXBUFSIZE 32
static volatile uint8_t rxbuf[RXBUFSIZE];

static volatile int widx  = 0;
static volatile int ridx  = 0;
static volatile int rxcnt = 0;
static volatile int rxovf = 0;

/*
 *==============================================================================
 * UART IRQ for data reception
 *
 *  params:
 *     * none
 *
 *  returns:
 *     * none
 *
 *  comments:
 *     Receives data and stores it in a buffer for later readout
 *
 *==============================================================================
 */
void UART_IRQ_NAME()
{
#if defined(UART_USE_USART)
        if (UART_PORT->STATUS & USART_STATUS_RXDATAV)
#else
        if (UART_PORT->IF & LEUART_IF_RXDATAV)
#endif
        {
                if (rxovf == 0)
                {
                        rxbuf[widx] = UART_RX(UART_PORT);
                        widx++;
                        rxcnt++;
                        if (widx == RXBUFSIZE)
                        {
                                widx = 0;
                        }
                        if (rxcnt > RXBUFSIZE)
                        {
                                rxovf = 1;
                        }
                }
        }
}

/*
 *==============================================================================
 * UART initialization
 *
 *  params:
 *     * baud           -- integer baud rate
 *     * databits       -- integer number of data bits
 *                         USART:  4-16
 *                         LEUART: 8 or 9
 *     * parity         -- "n" == none
 *                         "e" == even
 *                         "o" == odd
 *     * stopbits       -- "0.5"/"1"/"1.5"/"2"
 *
 *  returns:
 *     * none
 *
 *  comments:
 *     Initializes UART library for selected UART
 *
 *==============================================================================
 */
void uart_init(int baud, int databits, char *parity, char *stopbits)
{
/* First, adjust the init struct according to params */
#if defined(UART_USE_USART)
        USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
        init.baudrate = baud;
        init.databits = databits - 3;

        if (strcmp(parity, "n") == 0)
                init.parity = usartNoParity;
        else if (strcmp(parity, "e") == 0)
                init.parity = usartEvenParity;
        else if (strcmp(parity, "o") == 0)
                 init.parity = usartOddParity;

        if (strcmp(stopbits, "0.5") == 0)
                init.stopbits = usartStopbits0p5;
        else if (strcmp(stopbits, "1") == 0)
                init.stopbits = usartStopbits1;
        else if (strcmp(stopbits, "1.5") == 0)
                init.stopbits = usartStopbits1p5;
        else if (strcmp(stopbits, "2") == 0)
                init.stopbits = usartStopbits2;

#else
        LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;
        //
        //
        //
        //
        //
        // SAME AS ABOVE FOR LEUART
        //
        //
        //
        //
        //
#endif

/* Now, the rest of the init sequence */
#if defined(UART_USE_USART)
        /* Configure GPIO pins */
        CMU_ClockEnable(cmuClock_GPIO, true);

        /* To avoid false start, configure output as high */
        GPIO_PinModeSet(UART_TXPORT, UART_TXPIN, gpioModePushPull, 1);
        GPIO_PinModeSet(UART_RXPORT, UART_RXPIN, gpioModeInput, 0);

        /* Enable peripheral clocks */
        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(UART_CLK, true);

        /* Configure USART for basic async operation */
        init.enable = usartDisable;
        USART_InitAsync(UART_PORT, &init);

        /* Enable pins at UART1 location #2 */
        UART_PORT->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | UART_LOC;

        /* Clear previous RX interrupts */
        USART_IntClear(UART_PORT, USART_IF_RXDATAV);
        NVIC_ClearPendingIRQ(UART_IRQN);

        /* Enable RX interrupts */
        USART_IntEnable(UART_PORT, USART_IF_RXDATAV);
        NVIC_EnableIRQ(UART_IRQN);

        /* Finally enable it */
        USART_Enable(UART_PORT, usartEnable);
#else
        /* Enable CORE LE clock in order to access LE modules */
        CMU_ClockEnable(cmuClock_CORELE, true);

        /* Select LFXO for LEUARTs (and wait for it to stabilize) */
        CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);

        CMU_ClockEnable(UART_CLK, true);

        /* Do not prescale clock */
        CMU_ClockDivSet(UART_CLK, cmuClkDiv_1);

        /* Configure LEUART */
        init.enable = leuartDisable;
        LEUART_Init(UART_PORT, &init);

        /* Enable pins at default location */
        UART_PORT->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | UART_LOC;

        /* Clear previous RX interrupts */
        LEUART_IntClear(UART_PORT, LEUART_IF_RXDATAV);
        NVIC_ClearPendingIRQ(UART_IRQN);

        /* Enable RX interrupts */
        LEUART_IntEnable(UART_PORT, LEUART_IF_RXDATAV);
        NVIC_EnableIRQ(UART_IRQN);

        /* Finally enable it */
        LEUART_Enable(UART_PORT, leuartEnable);
#endif
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
        UART_TX(UART_PORT, c);
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
        for ( ; *s != '\0'; *s++)
        {
                uart_putc(*s);
                nc++;
        }
        return nc;
}

/*
 *==============================================================================
 * Receive character
 *
 *  params:
 *     * none
 *
 *  returns:
 *     * character in buffer
 *
 *  comments:
 *     Retrieves data from the receive buffer, which is filled in the ISR above
 *
 *==============================================================================
 */
char uart_getc()
{
        char c = '\0';

        INT_Disable();
        if (rxcnt > 0)
        {
                c = rxbuf[ridx++];
                if (ridx == RXBUFSIZE)
                {
                        ridx = 0;
                }
                rxcnt--;
                rxovf = 0;
        }
        INT_Enable();

        return c;
}

/*
 *==============================================================================
 * Read a set of characters from UART buffer
 *
 *  params:
 *     * buffer to write to
 *
 *  returns:
 *     * number of characters read
 *
 *  comments:
 *     Replica of stdlib gets. Returns characters from the UART buffers until
 *     the null character or a line break character is encountered. The latter,
 *     if encountered, is replaced with a null character.
 *
 *==============================================================================
 */
int uart_gets(char *s)
{
        uint32_t nc = 0;
        do {
                *s = uart_getc();
                if (*s == '\n')
                        *s = '\0';
                if (*s == '\0')
                        break;
                nc++;
                *s++;
        } while (1);

        return nc;
}

