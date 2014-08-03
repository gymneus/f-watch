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
 * @brief MAX44009 ambient light sensor driver
 */

#include "light_sensor.h"
#include <math.h>
#include <stdio.h>
#include "i2cdrv.h"


// Writes a register
uint8_t max44009_write_reg(uint8_t reg_add, uint8_t value)
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
uint8_t max44009_read_reg(uint8_t reg_add, uint8_t* value)
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
        seq.buf[1].len  = 1;

        // Initiate transfer
        ret = I2CDRV_Transfer(&seq);
        if(ret != i2cTransferDone)
        {
            return((uint8_t) ret);
        }

        return 0;
}


uint8_t light_sensor_set_int(uint8_t enable)
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
}


uint8_t light_sensor_get_lux(double* lux)
{
        uint8_t err, high_b, low_b, m, e;

        /* TODO
         * For accurate reading, high and low registers have to
         * be read during the same i2c transfer.
         * => Apparently not possible with the current i2cdrvr!
         *
         * For now, only read high byte
        */

        // Read Lux registers high byte
        err = max44009_read_reg(MAX44009_REG_LUX_H, &high_b);
        if(err)
        {
                return err;
        }

        /*
        // Read Lux register low byte
        err = max44009_read_reg(MAX44009_REG_LUX_L, &low_b);
        if(err)
        {
                return err;
        }
        */

        // Extract mantissa and exponent
        m = high_b & 0xff;
        //m = ((high_b & 0xff) << 4) + (low_b & 0xff);
        e = ((high_b & 0xff00) >> 4);

        // Calculate Lux value
        *lux = pow(2,(double) e) * m * 0.72;
        //*lux = pow(2,(double) e) * m * 0.045;

        return 0;
}


uint8_t light_sensor_set_thres(double thres)
{
        return 0;
}


uint8_t light_sensor_set_thres_timer(uint8_t timer)
{
        return 0;
}
