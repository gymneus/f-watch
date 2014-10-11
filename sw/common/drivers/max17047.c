/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
 * @author Matthieu Cattin <matthieu.cattin@cern.ch>
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
 * http://www.gnu.org/licenses/gpl-3.0-standalone.html
 * or you may search the http://www.gnu.org website for the version 3 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @brief Fuel gauge driver (MAX17047)
 */

#include "max17047.h"
#include "i2cdrv.h"
#include "delay.h"

uint8_t max17047_read_reg(uint8_t address, uint8_t length, uint8_t* buffer)
{
    I2C_TransferSeq_TypeDef    seq;
    I2C_TransferReturn_TypeDef ret;

    seq.addr  = MAX17047_ADDRESS;
    seq.flags = I2C_FLAG_WRITE_READ;    // write/read sequence

    // Select offset to start reading from
    seq.buf[0].data = &address;
    seq.buf[0].len  = 1;

    // Select location/length of data to be read
    seq.buf[1].data = buffer;
    seq.buf[1].len  = length;

    ret = I2CDRV_Transfer(&seq);
    if(ret != i2cTransferDone)
    {
        return((uint8_t) ret);
    }

    return length;
}

uint8_t max17047_write_reg(uint8_t address, uint8_t length, uint8_t* buffer)
{
    I2C_TransferSeq_TypeDef    seq;
    I2C_TransferReturn_TypeDef ret;

    seq.addr  = MAX17047_ADDRESS;
    seq.flags = I2C_FLAG_WRITE_WRITE;   // write/write sequence

    // Select offset to start reading from
    seq.buf[0].data = &address;
    seq.buf[0].len  = 1;

    // Select location/length of data to be read
    seq.buf[1].data = buffer;
    seq.buf[1].len  = length;

    ret = I2CDRV_Transfer(&seq);
    if(ret != i2cTransferDone)
    {
        return((uint8_t) ret);
    }

    return length;

    // TODO ack polling to determine if the write operation is finished?
}

uint8_t max17047_init(void)
{
        uint16_t status;

        // Check if POR bit is set
        status = max17047_get_status();
        if(status & MAX17047_STS_POR)
        {
                Delay(600); // 600ms

                // restore saved register (learned-values, application registers)
                // TODO: use the eeprom emulator from AN0019

                // clear POR bit
                status &= ~MAX17047_STS_POR;
                max17047_write_reg(MAX17047_REG_STATUS, 2, (uint8_t*) &status);
        }

        return 0;
}

uint8_t max17047_save_regs(void)
{
        // TODO: use the eeprom emulator from AN0019

        return 0;
}

uint16_t max17047_get_status(void)
{
    uint16_t status;

    max17047_read_reg(MAX17047_REG_STATUS, 2, (uint8_t*) &status);

    return status;
}

uint16_t max17047_get_config(void)
{
    uint16_t config;

    max17047_read_reg(MAX17047_REG_CONFIG, 2, (uint8_t*) &config);

    return config;
}

uint8_t max17047_set_config(uint16_t config)
{
    uint8_t err;

    err = max17047_write_reg(MAX17047_REG_CONFIG, 2, (uint8_t*) &config);

    return err;
}

uint16_t max17047_get_voltage(void)
{
    uint16_t tmp;

    max17047_read_reg(MAX17047_REG_VCELL, 2, (uint8_t*) &tmp);

    // convert to mV
    uint32_t volt = tmp * 80 >> 10;

    return (uint16_t) volt;
}

int16_t max17047_get_current(void)
{
    int16_t tmp;

    max17047_read_reg(MAX17047_REG_CURRENT, 2, (uint8_t*) &tmp);

    // convert to mA
    int32_t curr = tmp * 160 >> 10;

    return (int16_t) curr;
}

int8_t max17047_get_temperature(void)
{
    int8_t temperature[2];

    max17047_read_reg(MAX17047_REG_TEMPERATURE, 2, (uint8_t*) temperature);

    return temperature[1];
}

uint8_t max17047_get_charge(void)
{
    uint8_t charge[2];

    max17047_read_reg(MAX17047_REG_SOC_REP, 2, charge);

    return charge[1];
}

uint16_t max17047_get_time_left(void)
{
    uint16_t time;

    max17047_read_reg(MAX17047_REG_TIME_TO_EMPTY, 2, (uint8_t*) &time);

    // convert to minutes
    time >>= 5;
    time *= 3;

    return time;
}

