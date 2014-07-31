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

/**
 * @brief headers of micro SD access
 */

#ifndef SD_H
#define SD_H

#include "em_gpio.h"

/* Defines */
#define HFRCO_FREQUENCY            14000000
#define SD_SPI_PERCLK_FREQUENCY    HFRCO_FREQUENCY

/* Peripherals */
#define SD_SPI_CLOCK       cmuClock_USART1
#define SD_SPI_BAUDRATE    1000000
#define SD_SPI_UNIT        USART1
#define SD_SPI_LOCATION    1

// Pinout
#define SD_PORT_MOSI   gpioPortD
#define SD_PIN_MOSI    0
#define SD_PORT_MISO   gpioPortD
#define SD_PIN_MISO    1
#define SD_PORT_CS_N   gpioPortD
#define SD_PIN_CS_N    3
#define SD_PORT_CLOCK  gpioPortD
#define SD_PIN_CLOCK   2
#define SD_PORT_POWER  gpioPortC
#define SD_PIN_POWER   3

//sizes of the SD card
#define SD_SECTOR_SIZE   512       /* Sector size */
#define SD_VOLUME_SIZE  (2 * 1024 * 1024) /* definition for size of sddrive0 : we assume working with a 2GB micro SD card */



/**
 * @brief micro SD initialization routine.
 * @return > 0 means a success
 */
uint32_t sd_init(void);

/**
 * @brief Controls power for the micro SD.
 * @param uint8_t enable turns on the power if greater than 0.
 */
void sd_power(uint8_t enable);
/**
 * @brief Set the number of bytes in a sector of the micro SD
 * @param uint8_t bytes_per_sector number of bytes per sector
 * @return the reply from the card (0 = success)
 */
uint8_t sd_send_block_length(uint32_t bytes_per_sector);

/**
 * @brief enumeration for the possible type of replies coming from the micro SD
 */
typedef enum {
	SD_RESPONSE_R1 = 1,
	SD_RESPONSE_R2 = 2,
	SD_RESPONSE_R3 = 3
} sd_response_type;

#endif /* SD_H */

