/*
 * Copyright (C) 2014 Julian Lewis
 * @author Matthieu Cattin <matthieu.cattin@cern.ch>
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
 * @brief Real time clock routines.
 */

#ifndef RTC_H
#define RTC_H

struct rtc_time {
    ///> Seconds since 01-01-1970 00:00
    unsigned int epoch;

    ///> Milliseconds
    unsigned int msecs;
};

/**
 * @brief Setup backup RTC
 * Using LFRCO clock source and enabling interrupt on COMP0 match
 */
void rtc_init(void);

/**
 * @brief Returns the current time.
 */
struct rtc_time rtc_get_time(void);

/**
 * @brief Sets the time.
 */
void rtc_set_time(struct rtc_time current);

/**
 * @brief Function to be called on every timer tick in the interrupt
 * servicer routine.
 */
void rtc_tick(void);

#endif /* RTC_H */
