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
 * Interrupt service routines dispatchers. An abstraction layer between
 * hardware interrupts and operating system.
 */

#include "irq_dispatcher.h"

#include <apps/application.h>
#include <event.h>

portBASE_TYPE gpio_irq_dispatcher(uint32_t flags)
{
    // We have not woken a task at the start of the ISR
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    // Fill the event data
    struct event evt;
    evt.type = BUTTON_PRESSED;
    switch(flags)
    {
        case 0x01: evt.data.button = BUT_BL; break;
        case 0x40: evt.data.button = BUT_TR; break;
        case 0x80: evt.data.button = BUT_BR; break;
        case 0x0100: evt.data.button = BUT_TL; break;

        // Unexpected event, do not send it
        default: return xHigherPriorityTaskWoken;
    }

    // Post the byte to the back of the queue
    xQueueSendToBackFromISR( appQueue, &evt, &xHigherPriorityTaskWoken );

    return xHigherPriorityTaskWoken;
}

