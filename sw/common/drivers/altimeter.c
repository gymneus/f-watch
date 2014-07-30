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
#include <udelay.h>


static uint16_t calib_data[8];


// Only send a command (no reply expected)
uint8_t ms5806_write_cmd(uint8_t cmd)
{
        I2C_TransferSeq_TypeDef    seq;
        I2C_TransferReturn_TypeDef ret;

        seq.addr  = MS5806_ADDRESS;
        seq.flags = I2C_FLAG_WRITE;   // sequence: write command (1 byte)

        // Add command to buffer
        seq.buf[0].data = cmd;
        seq.buf[0].len  = 1;

        // Initiate transfer
        ret = I2CDRV_Transfer(&seq);
        if(ret != i2cTransferDone)
        {
            return((uint8_t) ret);
        }

        return 0;
}


// Send a command and reads the reply
uint8_t ms5806_read_reg(uint8_t cmd, uint8_t length, uint8_t* buffer)
{
        I2C_TransferSeq_TypeDef    seq;
        I2C_TransferReturn_TypeDef ret;

        seq.addr  = MS5806_ADDRESS;
        seq.flags = I2C_FLAG_WRITE_READ;   // sequence: write command (1 byte), read (n bytes)

        // Add command to sequence
        seq.buf[0].data = cmd;
        seq.buf[0].len  = 1;

        // 
        seq.buf[1].data = buffer;
        seq.buf[1].len  = length;

        // Initiate transfer
        ret = I2CDRV_Transfer(&seq);
        if(ret != i2cTransferDone)
        {
            return((uint8_t) ret);
        }

        return 0;
}


/**
 * @brief Resets and reads the calibration data from the altimeter.
 */
void alti_init(void)
{
        uint8_t i, cmd, ret;

        // calibrate udelay, TODO: move it out of altimeter driver
        UDELAY_Calibrate();

        // Reset
        ret = ms5806_write_cmd(MS5806_CMD_RESET); // TODO: do something with the return value

        // Reads calibration data and store them in static variable
        for(i=0; i<MS5806_PROM_SIZE; i++)
        {
                cmd = MS5806_CMD_READ_PROM + (MS5806_PROM_ADR_MASK & (i << 1));
                ret = ms5806_read_reg(cmd, 2, (uint8_t*) calib_data); // TODO: do something with the return value
        }
}


/**
 * @brief Reads and compensate pressure value.
 * @return Pressure in hundredth of millibars (e.g. 110002 = 1100.02mbar).
 */
int32_t alti_get_pressure(void)
{
        uint8_t ret;
        uint8_t cmd;
        uint32_t adc_val;
        int32_t temp;
        int32_t pressure;
        int64_t tmp;

        // Reads temperature
        temp = alti_get_temperature();

        // Initiate pressure conversion
        cmd = MS5806_CMD_CONV_PRESSURE + (MS5806_OSR_4096 & MS5806_OSR_MASK);
        ret = ms5806_write_cmd(cmd); // TODO: do something with the return value

        // Wait for conversion time (max time 9ms @ OSR=4096)
        UDELAY_Delay(10000); // 10ms

        // ADC read (3 bytes)
        ret = ms5806_read_reg(MS5806_CMD_READ_ADC, 3, (uint8_t*) &adc_val); // TODO: do something with the return value

        // Calulate compensated pressure
        // OFF = OFFT1 + TCO * dT = C2 * 2^17 + (C4 * dT) / 2^6
        // SENS = SENST1 + TCS * dT = C1 * 2^16 + (C3 * dT) / 2^7
        // P = D1 * SENS - OFF = (D1 * SENS / 2^21 - OFF) / 2^15
        // D1 = adc pressure value
        // C1-C4 = calibration data
        pressure = 0;

        return pressure;

}


/**
 * @brief Reads temperature value.
 * @return Temperature in hundredth of celsuis degrees (e.g. 2000 = 20.00 °C).
 */
int32_t alti_get_temperature(void)
{
        uint8_t ret;
        uint8_t cmd;
        uint32_t adc_val;
        int32_t temp;
        int64_t tmp;

        // Initiate temperature conversion
        cmd = MS5806_CMD_CONV_TEMP + (MS5806_OSR_4096 & MS5806_OSR_MASK);
        ret = ms5806_write_cmd(cmd); // TODO: do something with the return value

        // Wait for conversion time (max time 9ms @ OSR=4096)
        UDELAY_Delay(10000); // 10ms

        // ADC read (3 bytes)
        ret = ms5806_read_reg(MS5806_CMD_READ_ADC, 3, (uint8_t*) &adc_val); // TODO: do something with the return value
        // Calculate temperature
        // dT = D2 - TREF = D2 - C5 * 2^8
        // TEMP = 20°C + dT * TEMPSENS = 2000 + dT * C6 / 2^23
        // D2 = adc temperature value
        // C5, C6 = calibration data
        temp = adc_val - calib_data[5] * 256;
        tmp = 2000 + temp * calib_data[6];
        temp = tmp/8388608;

        return temp;
}
