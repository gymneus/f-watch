/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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
 * @brief Main file.
 */

#include <em_chip.h>
#include <em_gpio.h>
#include <em_cmu.h>
#include <sleep.h>

#include <apps/app_list.h>
#include <drivers/buttons.h>
#include <drivers/lcd.h>
#include <drivers/rtc.h>
#include <gfx/ui.h>
#include <drivers/gps/gps.h>

xSemaphoreHandle semGps;

int main(void)
{
    // Chip errata
    CHIP_Init();

    // Enable clocks
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    buttons_init();
    rtc_init();
    lcd_init();
    ui_init();

    vSemaphoreCreateBinary(semGps);
    gps_init();

    GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

    // Initialize SLEEP driver, no callbacks are used
    SLEEP_Init(NULL, NULL);
#if (configSLEEP_MODE < 3)
    // do not let to sleep deeper than define
    SLEEP_SleepBlockBegin((SLEEP_EnergyMode_t)(configSLEEP_MODE+1));
#endif

    startMain(&menu);

    /* Create background task for GPS */
    if (xTaskCreate(gpsbkgrnd.main, (const signed char *)gpsbkgrnd.name,
        APP_STACK_SIZE, NULL, BKGND_APP_PRIORITY, NULL) != pdPASS) {
        // TODO oops..
    }

    // Start FreeRTOS Scheduler
    vTaskStartScheduler();

    return 0;
}

