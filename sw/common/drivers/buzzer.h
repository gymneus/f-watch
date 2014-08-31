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
 * @brief Buzzer control.
 */

#ifndef BUZZER_H
#define BUZZER_H

/**
 * @brief Initialize buzzer.
 */
void buzzer_init(void);

/**
 * @brief Turns on the buzzer.
 */
void buzzer_enable(void);

/**
 * @brief Turns off the buzzer.
 */
void buzzer_disable(void);

/**
 * @brief Sets the frequency of buzzer sound.
 * @param frequency is the frequency to be set. Valid values are between 6
 * and 5500. If the requested frequency is out of the valid range, it will
 * be set to either the closest valid value (either the lowest one or the
 * highest one).
 */
void buzzer_set_freq(int frequency);

#endif /* BUZZER_H */

