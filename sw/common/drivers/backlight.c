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
 * @brief Backlight control.
 */

#include "backlight.h"

#include <em_cmu.h>
#include <em_gpio.h>
#include <em_timer.h>

#include "sleep.h"

static int backlight_level;

void backlight_init(void)
{
    CMU_ClockEnable(cmuClock_TIMER1, true);

    // Configure backlight LED pins as outputs
    GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

    // Select CC channel parameters
    TIMER_InitCC_TypeDef timerCCInit =
    {
        .eventCtrl  = timerEventEveryEdge,
        .edge       = timerEdgeBoth,
        .prsSel     = timerPRSSELCh1,
        .cufoa      = timerOutputActionNone,
        .cofoa      = timerOutputActionNone,
        .cmoa       = timerOutputActionToggle,
        .mode       = timerCCModePWM,
        .filter     = false,
        .prsInput   = false,
        .coist      = false,
        .outInvert  = false,
    };

    // Configure CC channel 1 & 2
    TIMER_InitCC(TIMER1, 1, &timerCCInit);
    TIMER_InitCC(TIMER1, 2, &timerCCInit);

    // Route CC1 to location 1 (PE11) and enable pin
    TIMER1->ROUTE |= (TIMER_ROUTE_CC1PEN | TIMER_ROUTE_LOCATION_LOC1);

    // Route CC2 to location 1 (PE12) and enable pin
    TIMER1->ROUTE |= (TIMER_ROUTE_CC2PEN | TIMER_ROUTE_LOCATION_LOC1);

    // Select timer parameters
    TIMER_Init_TypeDef timerInit =
    {
        .enable     = true,
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

    // Set Top Value (determines PWM frequency)
    // TODO the top value may need adjustments if HFPER changes
    // TIMER_TopSet(TIMER1, CMU_ClockFreqGet(cmuClock_HFPER) / 100000));
    TIMER_TopSet(TIMER1, 100);

    backlight_set_level(0);

    // Configure timer
    TIMER_Init(TIMER1, &timerInit);
}

void backlight_set_level(int level)
{
    const int BL_MAX_LEVEL = 100;

    if(level <= 0) {
        TIMER_Enable(TIMER1, false);
        level = 0;  // in case level is negative
        SLEEP_SleepBlockEnd(sleepEM2);
    } else {
        // Reenable the timer, if it was turned off previously
        if(backlight_level == 0) {
            SLEEP_SleepBlockBegin(sleepEM2);
            TIMER_Enable(TIMER1, true);
        }

        if(level > BL_MAX_LEVEL)
            level = BL_MAX_LEVEL;

        TIMER_CompareBufSet(TIMER1, 1, level);
        TIMER_CompareBufSet(TIMER1, 2, level);
    }

    backlight_level = level;
}

unsigned int backlight_get_level(void)
{
    return backlight_level;
}

