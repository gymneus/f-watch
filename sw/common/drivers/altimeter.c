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

#include "altimeter.h"
#include "i2cdrv.h"


static uint16_t calib_data[8];


// Only send a command (no reply expected)
uint8_t ms5806_write_cmd(uint8_t command)
{
        // MS5806_ADDRESS
}


// Send a command and reads the reply
uint8_t ms5806_read_reg(uint8_t command, uint8_t length, uint8_t* buffer)
{
        // MS5806_ADDRESS
}


/**
 * @brief Resets and reads the calibration data from the altimeter.
 */
void alti_init(void)
{
        uint8_t i, cmd, ret;

        // Reset
        ret = ms5806_write_cmd(MS5806_CMD_RESET); // TODO: do something with the return value

        // Reads calibration data and store them in static variable
        for(i=0; i<MS5806_PROM_SIZE; i++)
        {
                cmd = MS5806_CMD_READ_PROM + (MS5906_PROM_ADR_MASK & (i << 1));
                ret = ms5806_read_reg(cmd, 2, calib_data[i]); // TODO: do something with the return value
        }
}


/**
 * @brief Reads and compensate pressure value.
 * @return Pressure in hundredth of millibars (e.g. 110002 = 1100.02mbar).
 */
sint32_t alti_get_pressure(void)
{
        // Initiate temperature conversion

        // ADC read (3 bytes)

        // Calculate temperature

        // Initiate temperature conversion

        // ADC read (3 bytes)

        // Calulate compensated pressure
}


/**
 * @brief Reads temperature value.
 * @return Temperature in hundredth of celsuis degrees (e.g. 2000 = 20.00 °C).
 */
sint32_t alti_get_temperature(void)
{
        // Initiate temperature conversion

        // ADC read (3 bytes)

        // Calculate temperature

}
