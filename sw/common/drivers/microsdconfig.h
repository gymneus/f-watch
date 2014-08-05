/*
 * Copyright (C) 2014 Julian Lewis
 * @author Xavier Piroux <xavierpiroux@gmail.com>
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

#ifndef __MICROSD_CONFIG_H
#define __MICROSD_CONFIG_H

/**
 * @brief headers of micro SD access
 */

/* Peripherals */
#define MICROSD_HI_SPI_FREQ   16000000
#define MICROSD_LO_SPI_FREQ   100000
#define MICROSD_CMUCLOCK      cmuClock_USART1
#define MICROSD_USART         USART1
#define MICROSD_LOC           1

#define MICROSD_CMUCLOCK cmuClock_USART1

// Pinout
#define MICROSD_GPIOPORT gpioPortD
#define MICROSD_MOSIPIN  0
#define MICROSD_MISOPIN  1
#define MICROSD_CSPIN    3
#define MICROSD_CLKPIN   2

#define MICROSD_POWER_PORT  gpioPortC
#define MICROSD_POWER_PIN   3

//sizes of the SD card
#define MICROSD_SECTOR_SIZE   512       /* Sector size */
#define MICROSD_VOLUME_SIZE  (1 * 1024 * 1024) /* definition for size of sddrive0 : we assume working with a 1GB micro SD card */

#endif /* __MICROSD_CONFIG_H */
