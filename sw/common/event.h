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
 * http://www.gnu.org/licenses/gpl-3.0-standalone.html
 * or you may search the http://www.gnu.org website for the version 3 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @brief Application event definitions.
 */

#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Possible event types.
 */
enum event_type {
    BUTTON_PRESSED,
    SENSOR_INT,
    RTC_TICK,
    BATTERY_STATUS,
    GPS_TICK,
    GPS_TRACK_ON,
    GPS_TRACK_OFF
};

/**
 * Button markings.
 */
enum button_name {
    BUT_TL,     // top left
    BUT_TR,     // top right
    BUT_BL,     // bottom left
    BUT_BR      // bottom right
};

/**
 * Sensor interrupts.
 */
enum sensor_type {
//    LIGHT,            // disabled for the time being
    MAGNETOMETER,
    ACCELEROMETER
};

struct battery_info {
    uint8_t percentage;
    bool charging;
};

/**
 * Structure describing events received by applications.
 */
struct event {
    ///> Determines the source of event
    enum event_type type;

    ///> Data dependent on the event type
    union {
        enum button_name button;
        enum sensor_type sensor;
        struct battery_info battery;
    } data;
};

#endif /* EVENT_H */
