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

#include <apps/application.h>
#include <event.h>

#include <drivers/rtc.h>

#include <em_gpio.h>
#include <em_burtc.h>

#include "state.h"

static portBASE_TYPE gpio_irq_dispatcher(uint32_t flags)
{
    // We have not woken a task at the start of the ISR
    portBASE_TYPE task_woken1 = pdFALSE;
    portBASE_TYPE task_woken2 = pdFALSE;

    // Fill the event data
    struct event evt;
    switch(flags)
    {
        // Buttons
        case (1 << 0):      // PA0
            evt.type = BUTTON_PRESSED;
            evt.data.button = BUT_TR;
            break;

        case (1 << 6):      // PC6
            evt.type = BUTTON_PRESSED;
            evt.data.button = BUT_BL;
            break;

        case (1 << 7):      // PC7
            evt.type = BUTTON_PRESSED;
            evt.data.button = BUT_TL;
            break;

        case (1 << 8):      // PA8
            evt.type = BUTTON_PRESSED;
            evt.data.button = BUT_BR;
            break;

        // Sensors
// There is a conflict with the bottom-left button interrupt
//        case (1 << 6):      // PA6
//            evt.type = SENSOR_INT;
//            evt.data.sensor = LIGHT;
//            break;

        case (1 << 10):     // PA10
            evt.type = SENSOR_INT;
            evt.data.sensor = MAGNETOMETER;
            break;

        case (1 << 5):      // PD5
            evt.type = SENSOR_INT;
            evt.data.sensor = ACCELEROMETER;
            break;

        // Unexpected event, do not send it
        default: return pdFALSE;
    }

    // Post the event to the back of the queue
    xQueueSendToBackFromISR(appQueue, &evt, &task_woken1);

    // Switch to active state if a button was pressed
    if(evt.type == BUTTON_PRESSED)
        reset_active_irq(&task_woken2);

    return task_woken1 || task_woken2;
}

void GPIO_EVEN_IRQHandler(void)
{
    uint32_t iflags;

    // Get all even interrupts
    iflags = GPIO_IntGetEnabled() & 0x00005555;

    // Clean only even interrupts
    GPIO_IntClear(iflags);

    portEND_SWITCHING_ISR(gpio_irq_dispatcher(iflags));
}

void GPIO_ODD_IRQHandler(void)
{
    uint32_t iflags;

    // Get all odd interrupts
    iflags = GPIO_IntGetEnabled() & 0x0000AAAA;

    // Clean only odd interrupts
    GPIO_IntClear(iflags);

    portEND_SWITCHING_ISR(gpio_irq_dispatcher(iflags));
}

void BURTC_IRQHandler(void)
{
    // We have not woken a task at the start of the ISR
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    rtc_tick();

    // Fill the event data
    struct event evt;
    evt.type = RTC_TICK;

    // Post the byte to the back of the queue
    xQueueSendToBackFromISR(appQueue, &evt, &xHigherPriorityTaskWoken);

    BURTC_IntClear(BURTC_IFC_COMP0);

    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

