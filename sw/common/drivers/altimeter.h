/*
 * Copyright (C) 2014 Julian Lewis
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
 * @brief MS5806-02BA altimeter sensor driver
 */

#ifndef ALTIMETER_H
#define ALTIMETER_H

#include <stdint.h>

// I2C address
#define MS5806_ADDRESS 0xEE // 0x77 << 1

// Commands
#define MS5806_CMD_READ_ADC      0x00
#define MS5806_CMD_RESET         0x1E
#define MS5806_CMD_CONV_PRESSURE 0x40
#define MS5806_CMD_CONV_TEMP     0x50
#define MS5806_CMD_READ_PROM     0xA0

#define MS5806_PROM_SIZE     0x08
#define MS5806_PROM_ADR_MASK 0x0E

// Oversampling ratio
#define MS5806_OSR_256  0x00
#define MS5806_OSR_512  0x02
#define MS5806_OSR_1024 0x04
#define MS5806_OSR_2048 0x06
#define MS5806_OSR_4096 0x08
#define MS5806_OSR_MASK 0x0E


uint8_t ms5806_write_cmd(uint8_t cmd);
uint8_t ms5806_read_reg(uint8_t cmd, uint8_t length, uint8_t* buffer);

/**
 * @brief Resets and reads the calibration data from the altimeter.
 */
void alti_init(void);

/**
 * @brief Reads and compensate pressure value.
 * @return Pressure in hundredth of millibars (e.g. 110002 = 1100.02mbar).
 */
int32_t alti_get_pressure(void);

/**
 * @brief Reads temperature value.
 * @return Temperature in hundredth of celsuis degrees (e.g. 2000 = 20.00 °C).
 */
int32_t alti_get_temperature(void);


#endif /* ALTIMETER_H */
