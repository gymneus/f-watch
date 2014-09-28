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
 * @brief Application event definitions.
 */

#ifndef EVENT_H
#define EVENT_H

/**
 * Possible event types.
 */
enum event_type {
    BUTTON_PRESSED,
    SENSOR,
    RTC_TICK,
    GPS_FRAME_RDY,
    GPS_PARSE_RDY
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
 * Structure describing events received by applications.
 */
struct event {
    ///> Determines the source of event
    enum event_type type;

    ///> Data dependent on the event type
    union {
        enum button_name button;
    } data;
};

#endif /* EVENT_H */
