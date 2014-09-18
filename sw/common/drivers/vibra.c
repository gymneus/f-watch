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
 * @brief Vibration motor control.
 */

#include "vibra.h"

#include <em_cmu.h>
#include <em_gpio.h>

#define VIBRA_PORT  gpioPortA
#define VIBRA_PIN   3

void vibra_init(void)
{
    GPIO_PinModeSet(VIBRA_PORT, VIBRA_PIN, gpioModePushPull, 0);
}

void vibra_enable(void)
{
    GPIO_PinOutSet(VIBRA_PORT, VIBRA_PIN);
}

void vibra_disable(void)
{
    GPIO_PinOutClear(VIBRA_PORT, VIBRA_PIN);
}

