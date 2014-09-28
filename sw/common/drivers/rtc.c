/*
 * Copyright (C) 2014 Julian Lewis
 * @author Matthieu Cattin <matthieu.cattin@cern.ch>
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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

/**
 * @brief Real time clock routines.
 */

#include "rtc.h"

#include <em_burtc.h>
#include <em_rmu.h>
#include <em_cmu.h>

#define LFXO_FREQUENCY                  32768
#define WAKEUP_INTERVAL_MS              1200
#define BURTC_COUNT_BETWEEN_WAKEUP      (((LFXO_FREQUENCY * WAKEUP_INTERVAL_MS) / 1000)-1)

struct rtc_time time = {0, 0};

__attribute__((weak))
void BURTC_IRQHandler(void)
{
    rtc_tick();

    BURTC_IntClear(BURTC_IFC_COMP0);
}

void rtc_init(void)
{
    // Enable LFXO and select it for LFA branch
    CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

    // Enable low energy clocking module clock
    CMU_ClockEnable(cmuClock_CORELE, true);

    // Enable BURTC registers access
    RMU_ResetControl(rmuResetBU, false);

    BURTC_Init_TypeDef burtcInit = BURTC_INIT_DEFAULT;

    burtcInit.enable       = true;                // Enable BURTC after initialization
    burtcInit.mode         = burtcModeEM3;        // BURTC is enabled in EM0-EM3
    burtcInit.debugRun     = false;               // Counter shall keep running during debug halt.
    burtcInit.clkSel       = burtcClkSelLFXO;     // Select LFXO as clock source
    burtcInit.clkDiv       = burtcClkDiv_1;       // Clock prescaler
    burtcInit.lowPowerComp = 0;                   // Number of least significantt clock bits to ignore in low power mode
    burtcInit.timeStamp    = true;                // Enable time stamp on entering backup power domain
    burtcInit.compare0Top  = true;                // Clear counter on compare match
    burtcInit.lowPowerMode = burtcLPDisable;      // Low power operation mode, requires LFXO or LFRCO

    BURTC_CompareSet(0, BURTC_COUNT_BETWEEN_WAKEUP);  // Set top value for comparator

    // Enabling Interrupt from BURTC
    NVIC_EnableIRQ(BURTC_IRQn);
    BURTC_IntEnable(BURTC_IF_COMP0);    // Enable compare interrupt flag

    // Initialize BURTC
    BURTC_Init(&burtcInit);
}

struct rtc_time rtc_get_time(void)
{
    return time;
}

void rtc_set_time(struct rtc_time current)
{
    time = current;
}

void rtc_tick(void)
{
    ++time.epoch;
    time.msecs += 200;

    if(time.msecs == 1000) {
        ++time.epoch;
        time.msecs = 0;
    }
}

