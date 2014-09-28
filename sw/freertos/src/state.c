/*
 * Copyright (C) 2014 Julian Lewis
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
 * @brief Watch states handling.
 */

#include <FreeRTOS.h>
#include <timers.h>

#include <drivers/backlight.h>

#include "state.h"
#include "blight_task.h"

///> Number of ticks when the watch is considered active since active_reset()
#define ACTIVE_STATE_TICKS  10000

enum watch_state current_state = IDLE;
static xTimerHandle timer_handle;
static void state_handler(enum watch_state state);
static void stop_active(void *params);

void state_init(void)
{
    timer_handle = xTimerCreate((signed char*) "active_state",
                                ACTIVE_STATE_TICKS, pdTRUE,
                                (void*) 0, stop_active);

    reset_active();
}

enum watch_state get_state(void)
{
    return current_state;
}

void reset_active(void)
{
    xTimerReset(timer_handle, 0);
    state_handler(ACTIVE);
}

void reset_active_irq(portBASE_TYPE *task_woken)
{
    xTimerResetFromISR(timer_handle, task_woken);
    state_handler(ACTIVE);
}

static void stop_active(void *params)
{
    (void) params;

    xTimerStop(timer_handle, 0);
    state_handler(IDLE);
}

static void state_handler(enum watch_state state)
{
    if(state == current_state)
        return;

    switch(state) {
        case ACTIVE:
            auto_backlight_enable(true);
            break;

        case IDLE:
            auto_backlight_enable(false);
            backlight_set_level(0);
            // TODO switch frequency? goto sleep?
            break;
    }

    current_state = state;
}
