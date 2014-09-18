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
 * @brief Backlight control.
 */

#ifndef BACKLIGHT_H
#define BACKLIGHT_H

/**
 * @brief Initialize backlight control.
 */
void backlight_init(void);

/**
 * @brief Sets the backlight brightness.
 * @param level is the brightness level to be set. Valid values are in the
 * 0-100 range. 0 stands for completely turned off, 100 is full brightness.
 */
void backlight_set_level(int level);

/**
 * @brief Returns the current level of backlight.
 */
unsigned int backlight_get_level(void);

#endif /* BACKLIGHT_H */

