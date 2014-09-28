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
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @brief Automatic backlight regulation.
 */

#include "blight_task.h"

#include <FreeRTOS.h>
#include <timers.h>
#include <inttypes.h>

#include <drivers/backlight.h>
#include <drivers/light_sensor.h>

///> Number of ticks between backlight adjustments
#define BLIGHT_TASK_PERIOD 3000

///> Entry that stores an appropriate backlight level value
///> depending on the light sensor readings.
static const struct {
    uint16_t lux;
    uint16_t level;
} blight_lut[] = {
    { 500,   100 },
    { 2000,  90 },
    { 4000,  80 },
    { 10000, 60 },
    { 20000, 30 },
    { 65535, 0 }
};

///> Number of pairs
const int blight_lut_size = sizeof(blight_lut) / sizeof(blight_lut[0]);

///> Current state
static bool auto_enabled = false;

///> Timer handle
static xTimerHandle timer_handle;

///> Currently used index in the lookup table
static int8_t cur_idx = -1;

static void auto_backlight_task(void *params)
{
    (void) params;
    uint32_t lux;

    if(!light_sensor_get_lux(&lux)) {
        // Look for the right value
        uint8_t index;
        for(index = 0; index < blight_lut_size; ++index) {
            if(lux < blight_lut[index].lux)
                break;
        }

        // Change the backlight level if it is different than the previous one
        if(index != cur_idx) {
            backlight_set_level(blight_lut[index].level);
            cur_idx = index;
        }
    }
}

void auto_backlight_init(void)
{
    timer_handle = xTimerCreate((signed char*) "auto_blight",
                                BLIGHT_TASK_PERIOD, pdTRUE,
                                (void*) 0, auto_backlight_task);
}

void auto_backlight_enable(bool enable)
{
    if(enable == auto_enabled)
        return;

    if(enable) {
        if(xTimerStart(timer_handle, 0) != pdPASS) {
            // TODO kernel panic
        }

        // Select appropriate backlight level
        auto_backlight_task(NULL);
    } else {
        xTimerStop(timer_handle, 0);
        cur_idx = -1;
    }

    auto_enabled = enable;
}

