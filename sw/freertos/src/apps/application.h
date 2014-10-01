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
 * User application data structures and routines.
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

#include <em_gpio.h>
#include <udelay.h>

void debug();

///> Capacity of event queue
#define APP_QUEUE_LEN   16

///> Shared application stack size
#define APP_STACK_SIZE  (configMINIMAL_STACK_SIZE)

///> Prioriuty of application task
#define APP_PRIORITY            (tskIDLE_PRIORITY + 1)
#define BKGND_APP_PRIORITY     (tskIDLE_PRIORITY + 2)


///> Shared application task handle
extern xTaskHandle appTask;

///> Shared application event queue
extern xQueueHandle appQueue;

/**
 * @brief An application entry, used by the menu application.
 */
typedef struct {
    ///> Application name
    const char* name;

    /**
     * @brief Main application routine.
     * @params Optional parameters, dependendent on application.
     */
    void (*main)(void* params);
} application;

/**
 * @brief Initializes the application task and event queue.
 * After that runs one as the main application.
 * @param app is the application to be run as the main one.
 */
void startMain(application* app);

#endif /* APPLICATION_H */

