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
 * @brief Automatic backlight regulation.
 */

#ifndef BLIGHT_TASK_H
#define BLIGHT_TASK_H

#include <stdbool.h>

/**
 * Initializes the automatic backlight adjustment task.
 */
void auto_backlight_init(void);

/**
 * Enables/disables automatic backlight adjustment.
 * @param enable decides if the automatic regulation should be enabled.
 * Note that you are still able to change the backlight level manually, but
 * it will be overridden next time the automatic adjustment task is
 * executed.
 */
void auto_backlight_enable(bool enable);

#endif /* BLIGHT_TASK_H */

