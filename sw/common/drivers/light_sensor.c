/*
 * Copyright (C) 2014 Julian Lewis
 * @author Matthieu Cattin <matthieu.cattin@cern.ch>
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
 * http://www.gnu.org/licenses/gpl-3.0-standalone.html
 * or you may search the http://www.gnu.org website for the version 3 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @brief MAX44009 ambient light sensor driver
 */

#include "light_sensor.h"
#include "i2cdrv.h"

// Writes a register
static uint8_t max44009_write_reg(uint8_t reg_add, uint8_t value)
{
        I2C_TransferSeq_TypeDef    seq;
        I2C_TransferReturn_TypeDef ret;

        seq.addr  = MAX44009_ADDRESS;
        seq.flags = I2C_FLAG_WRITE_WRITE;   // sequence: write register address (1 byte), write value (1 byte)

        // Add register address to buffer
        seq.buf[0].data = &reg_add;
        seq.buf[0].len  = 1;

        // Add value to buffer
        seq.buf[1].data = &value;
        seq.buf[1].len  = 1;

        // Initiate transfer
        ret = I2CDRV_Transfer(&seq);
        if(ret != i2cTransferDone)
        {
            return((uint8_t) ret);
        }

        return 0;
}

// Reads a register
static uint8_t max44009_read_reg(uint8_t reg_add, uint8_t *value, uint8_t length)
{
        I2C_TransferSeq_TypeDef    seq;
        I2C_TransferReturn_TypeDef ret;

        seq.addr  = MAX44009_ADDRESS;
        seq.flags = I2C_FLAG_WRITE_READ;   // sequence: write register address (1 byte), read value (1 bytes)

        // Add command to sequence
        seq.buf[0].data = &reg_add;
        seq.buf[0].len  = 1;

        // Read value in buffer
        seq.buf[1].data = value;
        seq.buf[1].len  = length;

        // Initiate transfer
        ret = I2CDRV_Transfer(&seq);
        if(ret != i2cTransferDone)
        {
            return((uint8_t) ret);
        }

        return 0;
}

/*uint8_t light_sensor_set_int(uint8_t enable)
{
        return 0;
}

uint8_t light_sensor_get_isr(uint8_t* isr)
{
        return 0;
}

uint8_t light_sensor_set_cfg(Light_Sensor_Conf_TypeDef *cfg)
{
        return 0;
}*/

///> Converts 16-bit raw register value (registers 3 & 4) to millilux.
static uint32_t raw16_to_lux(uint16_t val)
{
        uint8_t e = (val & 0xf000) >> 12;                   // exponent
        uint8_t m = ((val & 0x0f00) >> 4) | (val & 0x0f);   // mantissa

        return (1 << e) * m * 45;
}

///> Converts 8-bit raw register value (register 3) to millilux.
static uint32_t raw8_to_lux(uint8_t val)
{
        uint8_t e = (val >> 4);                             // exponent
        uint8_t m = (val & 0x0f);                           // mantissa

        return (1 << e) * m * 720;
}


uint8_t light_sensor_get_raw(uint16_t *val)
{
        uint8_t err;

        // Read Lux registers
        err = max44009_read_reg(MAX44009_REG_LUX_H, (uint8_t*)val, 2);

        if(err)
                return err;

        return 0;
}

uint8_t light_sensor_get_lux(uint32_t *lux)
{
        uint16_t raw;
        uint8_t ret = light_sensor_get_raw(&raw);

        if(ret != 0)
            return ret;     // error, no more processing

        *lux = raw16_to_lux(raw);

        return 0;
}

/*uint8_t light_sensor_set_thres(double thres)
{
        return 0;
}

uint8_t light_sensor_set_thres_timer(uint8_t timer)
{
        return 0;
}*/
