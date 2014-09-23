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

#include <drivers/i2cdrv.h>
#include <drivers/backlight.h>
#include <drivers/buttons.h>
#include <drivers/buzzer.h>
#include <drivers/lcd.h>
#include <drivers/rtc.h>
#include <drivers/vibra.h>

#include <gfx/ui.h>
#include <apps/app_list.h>
#include "battery_task.h"
#include "blight_task.h"
#include "state.h"

int main(void)
{
    // Chip errata
    CHIP_Init();

    // Enable clocks
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
    I2CDRV_Init(&i2cInit);

    backlight_init();
    buttons_init();
    buzzer_init();
    vibra_init();
    rtc_init();
    lcd_init();

    ui_init();
    auto_backlight_init();
    battery_monitor_init();
    state_init();

    // Initialize SLEEP driver, no callbacks are used
    SLEEP_Init(NULL, NULL);
#if (configSLEEP_MODE < 3)
    // do not let to sleep deeper than define
    SLEEP_SleepBlockBegin((SLEEP_EnergyMode_t)(configSLEEP_MODE+1));
#endif

    startMain(&menu);

    // Start FreeRTOS Scheduler
    vTaskStartScheduler();

    return 0;
}

