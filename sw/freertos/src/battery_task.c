/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
 * @author Matthieu Cattin <matthieu.cattin@cern.ch>
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
 * @brief Battery status monitor.
 */

#include "battery_task.h"
#include <drivers/max17047.h>

#include <apps/application.h>
#include <event.h>

#include <FreeRTOS.h>
#include <timers.h>
#include <stdbool.h>

///> Number of ticks between battery status polling
#define BATTERY_TASK_PERIOD 4000

///> Timer handle
static xTimerHandle timer_handle;

static bool cur_charging;
static unsigned cur_percentage;

static void battery_monitor_task(void *params)
{
    (void) params;
    struct event evt;

    bool charging = max17047_is_charging();
    unsigned percentage = max17047_get_charge();

    // Send an event if measurements are different
    if(charging != cur_charging || percentage != cur_percentage) {
        evt.type = BATTERY_STATUS;
        evt.data.battery.percentage = percentage;
        evt.data.battery.charging = charging;
        cur_percentage = percentage;
        cur_charging = charging;
        xQueueSendToBack(appQueue, &evt, 0);
    }
}

void battery_monitor_init(void)
{
    // Set invalid values to force an update
    cur_charging = false;
    cur_percentage = 255;

    timer_handle = xTimerCreate((signed char*) "battery_monitor",
                                BATTERY_TASK_PERIOD, pdTRUE,
                                (void*) 0, battery_monitor_task);

    if(xTimerStart(timer_handle, 0) != pdPASS) {
        // TODO kernel panic
    }
}

