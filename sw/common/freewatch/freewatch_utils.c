/*
 * Copyright (C) 2014 Julian Lewis
 * @author Federico Vaga <federico.vaga@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */
 
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "freewatch_utils.h"

/* Counts 1ms timeTicks */
volatile uint32_t msTicks;

/**************************************************************************//**
* @brief SysTick_Handler
* Interrupt Service Routine for system tick counter
 *****************************************************************************/
void SysTick_Handler(void)
{
    msTicks++; /* increment counter necessary in Delay()*/
}

/**************************************************************************//**
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 *****************************************************************************/
void Delay(uint32_t dlyTicks)
{
    uint32_t curTicks;

    curTicks = msTicks;
    while ((msTicks - curTicks) < dlyTicks);
}

/**************************************************************************//**
 * @brief Reset the MCU
 *****************************************************************************/
void ResetEFM32(void)
{
	SCB->AIRCR = 0x05FA0004;
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
void freewatch_backlight_blinky(int delay, int rep)
{
  int i, c = rep * 2;

    /* Setup SysTick Timer for 1 msec interrupts  */
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);
    GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

    /* Infinite blink loop */
    while (c--) {
            GPIO_PinOutToggle(gpioPortE, 11);
	    GPIO_PinOutToggle(gpioPortE, 12);
            Delay(delay);
    }
}


