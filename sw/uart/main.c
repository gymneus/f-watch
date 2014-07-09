/*
 *==============================================================================
 * CERN (BE-CO-HT)
 * Source file for Main file for UART example
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

#include "em_device.h"
#include "em_cmu.h"

uint32_t msticks = 0;

void SysTick_Handler()
{
        msticks++;
}

void delay(uint32_t n)
{
        uint32_t curticks;
        curticks = msticks;
        while ((msticks - curticks) < n)
                ;
}

int main()
{
        uart_init();

        /* Setup SysTick Timer for 1 msec interrupts  */
        if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

        CMU_ClockEnable(cmuClock_HFPER, true);

        while (1)
        {
                uart_puts("supercalifrageristic\n");
                delay(500);
        }
}
