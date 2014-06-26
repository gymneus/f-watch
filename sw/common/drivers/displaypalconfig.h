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
 * @brief Display Platform Abstraction Layer configuration file.
 */

#ifndef DISPLAYPALCONFIG_H
#define DISPLAYPALCONFIG_H

#include "em_usart.h"
#include "em_cmu.h"

// The user should select one of the following as RTC clock source.
// Selects the LFXO oscillator as source for the RTC clock.
#define PAL_RTC_CLOCK_LFXO

// Selects the LFRCO oscillator as source for the RTC clock.
//#define PAL_RTC_CLOCK_LFRCO

// Selects the ULFRCO oscillator as source for the RTC clock.
//#define PAL_RTC_CLOCK_ULFRCO


// Select which USART device to use as SPI interface.
#define PAL_SPI_USART_UNIT USART2

// Select which USART clock is used to clock the SPI interface.
#define PAL_SPI_USART_CLOCK cmuClock_HFPER

// Route location of the USART/SPI pins.
#define PAL_SPI_USART_LOCATION 0

// Specifies the SPI baud rate.
#define PAL_SPI_BAUDRATE 500000

#endif /* DISPLAYPALCONFIG_H */

