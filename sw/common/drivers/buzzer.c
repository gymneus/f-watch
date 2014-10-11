/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
 * @author Matthieu Cattin <matthieu.cattin@cern.ch>
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
 * http://www.gnu.org/licenses/gpl-3.0-standalone.html
 * or you may search the http://www.gnu.org website for the version 3 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @brief Buzzer control.
 */

#include "buzzer.h"

#include <em_cmu.h>
#include <em_gpio.h>
#include <em_timer.h>
#include <sleep.h>

void buzzer_init(void)
{
    // Configure buzzer pin as push/pull output
    GPIO_PinModeSet(gpioPortA, 9, gpioModePushPull, 0);

    // Enable clock for TIMER2 module
    CMU_ClockEnable(cmuClock_TIMER2, true);

    // Select CC channel parameters
    TIMER_InitCC_TypeDef timerCCInit =
    {
        .cufoa      = timerOutputActionNone,
        .cofoa      = timerOutputActionToggle,
        .cmoa       = timerOutputActionNone,
        .mode       = timerCCModeCompare,
        .filter     = false,
        .prsInput   = false,
        .coist      = false,
        .outInvert  = false,
    };

    // Configure CC channel 1
    TIMER_InitCC(TIMER2, 1, &timerCCInit);

    // Route CC1 to location 0 (PA9) and enable pin
    TIMER2->ROUTE |= (TIMER_ROUTE_CC1PEN | TIMER_ROUTE_LOCATION_LOC0);

    // Select timer parameter
    TIMER_Init_TypeDef timerInit =
    {
        .enable     = false,
        .debugRun   = true,
        .prescale   = timerPrescale64,
        .clkSel     = timerClkSelHFPerClk,
        .fallAction = timerInputActionNone,
        .riseAction = timerInputActionNone,
        .mode       = timerModeUp,
        .dmaClrAct  = false,
        .quadModeX4 = false,
        .oneShot    = false,
        .sync       = false,
    };

    // Configure timer
    TIMER_Init(TIMER2, &timerInit);

    buzzer_set_freq(5500);
}

void buzzer_enable(void)
{
    SLEEP_SleepBlockBegin(sleepEM2);
    TIMER_Enable(TIMER2, true);
}

void buzzer_disable(void)
{
    TIMER_Enable(TIMER2, false);
    SLEEP_SleepBlockEnd(sleepEM2);
}

void buzzer_set_freq(int frequency)
{
    const int MIN_AUDIBLE = 6;
    const int MAX_AUDIBLE = 5000;

    // For timer, lower TOP values mean higher frequency
    frequency = MAX_AUDIBLE + MIN_AUDIBLE - frequency;

    if(frequency < MIN_AUDIBLE)
        frequency = MIN_AUDIBLE;
    else if(frequency > MAX_AUDIBLE)
        frequency = MAX_AUDIBLE;

    // TODO add a formula to convert frequency to TIMER top value
    TIMER_TopSet(TIMER2, frequency);
}

