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
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @brief Button routines
 */

#include "buttons.h"
#include <em_gpio.h>

__attribute__((weak))
void GPIO_EVEN_IRQHandler(void)
{
    uint32_t iflags;

    // Get all even interrupts
    iflags = GPIO_IntGetEnabled() & 0x00005555;

    // Clean only even interrupts
    GPIO_IntClear(iflags);
}

__attribute__((weak))
void GPIO_ODD_IRQHandler(void)
{
    uint32_t iflags;

    // Get all odd interrupts
    iflags = GPIO_IntGetEnabled() & 0x0000AAAA;

    // Clean only odd interrupts
    GPIO_IntClear(iflags);
}

void buttons_init(void)
{
    // Configure interrupt pin as input with pull-up
    // TODO there are external pull-ups - we should disable either int or ext
    GPIO_PinModeSet(BUT_TL_PORT, BUT_TL_PIN, gpioModeInputPull, 1);
    GPIO_PinModeSet(BUT_TR_PORT, BUT_TR_PIN, gpioModeInputPull, 1);
    GPIO_PinModeSet(BUT_BL_PORT, BUT_BL_PIN, gpioModeInputPull, 1);
    GPIO_PinModeSet(BUT_BR_PORT, BUT_BR_PIN, gpioModeInputPull, 1);

    // Set falling edge interrupt and clear/enable it
    GPIO_IntConfig(BUT_TL_PORT, BUT_TL_PIN, false, true, true);
    GPIO_IntConfig(BUT_TR_PORT, BUT_TR_PIN, false, true, true);
    GPIO_IntConfig(BUT_BL_PORT, BUT_BL_PIN, false, true, true);
    GPIO_IntConfig(BUT_BR_PORT, BUT_BR_PIN, false, true, true);

    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}

