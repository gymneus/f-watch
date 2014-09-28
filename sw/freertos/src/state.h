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

#ifndef STATE_H
#define STATE_H

#include <FreeRTOS.h>

///> Possible states for the watch.
enum watch_state { ACTIVE, IDLE };

/**
 * @brief Initializes the state handler.
 */
void state_init(void);

/**
 * @brief Returns the current state of watch.
 */
enum watch_state get_state(void);

/**
 * @brief Switches to the active state for a period of time.
 */
void reset_active(void);

/**
 * @brief Switches to the active state for a period of time (version
 * that can be called from interrupt service routines).
 * @param task_woken will be set to true if a context switch should
 * be performed (@see xTimerReset()).
 */
void reset_active_irq(portBASE_TYPE *task_woken);

#endif /* STATE_H */
