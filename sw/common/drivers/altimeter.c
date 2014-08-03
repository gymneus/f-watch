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
#include <math.h>
#include <delay.h>


// Calibration data storage
static uint16_t calib_data[8];

// Coefficients for pressure to altitude convserion
static int16_t p2a_coeff[P2A_COEFF_SIZE][4] = {
        {1000,1130,12256,16212},
        {1130,1300,10758,15434},
        {1300,1500,9329,14541},
        {1500,1730,8085,13630},
        {1730,2000,7001,12722},
        {2000,2300,6069,11799},
        {2300,2650,5360,10910},
        {2650,3000,4816,9994},
        {3000,3350,4371,9171},
        {3350,3700,4020,8424},
        {3700,4100,3702,7737},
        {4100,4500,3420,7014},
        {4500,5000,3158,6346},
        {5000,5500,2908,5575},
        {5500,6000,2699,4865},
        {6000,6500,2523,4206},
        {6500,7100,2359,3590},
        {7100,7800,2188,2899},
        {7800,8500,2033,2151},
        {8500,9200,1905,1456},
        {9200,9700,1802,805},
        {9700,10300,1720,365},
        {10300,11000,1638,-139}};


// Only send a command (no reply expected)
uint8_t ms5806_write_cmd(uint8_t cmd)
{
        I2C_TransferSeq_TypeDef    seq;
        I2C_TransferReturn_TypeDef ret;

        seq.addr  = MS5806_ADDRESS;
        seq.flags = I2C_FLAG_WRITE;   // sequence: write command (1 byte)

        // Add command to buffer
        seq.buf[0].data = &cmd;
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
        //seq.flags = I2C_FLAG_READ;   // sequence: read (n bytes)

        // Add command to sequence
        seq.buf[0].data = &cmd;
        seq.buf[0].len  = 1;

        // Read value in buffer
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


// Check PROM CRC
uint8_t ms5806_crc_chk(uint16_t* data)
{
        uint8_t word, bit;
        uint16_t rem, crc;

        rem = 0x00;
        crc = data[7]; // save crc
        data[7] = (0xFF00 & data[7]); // replace crc byte by 0

        for(word=0; word<16; word++)
        {
                // choose LSB or MSB byte
                if(word%2 == 1)
                {
                        rem ^= (uint8_t)(data[word>>1] & 0xFF);
                }
                else
                {
                        rem ^= (uint8_t)(data[word>>1] >> 8);
                }
                for(bit=8; bit>0;bit--)
                {
                        if(rem & 0x8000)
                        {
                                rem = (rem << 1) ^ 0x3000;
                        }
                        else
                        {
                                rem = (rem << 1);
                        }
                }
        }
        rem = 0xF & (rem >> 12);
        data[7] = crc; // restore crc

        return (rem ^ 0x0);
}


/**
 * @brief Resets and reads the calibration data from the altimeter.
 * @return i2c error code or crc error, 0 if no errors.
 */
uint8_t alti_init(void)
{
        uint8_t i, err, cmd, crc;
        uint8_t buf[2*MS5806_PROM_SIZE];

        // Reset the sensor
        err = ms5806_write_cmd(MS5806_CMD_RESET);
        if(err)
        {
                return err;
        }

        // Wait for the sensor to be ready
        Delay(10); // 10ms


        // Reads calibration data and store them in static variable
        for(i=0; i<MS5806_PROM_SIZE; i++)
        {
                cmd = MS5806_CMD_READ_PROM + (MS5806_PROM_ADR_MASK & (i << 1));
                err = ms5806_read_reg(cmd, 2, &buf[2*i]);
                if(err)
                {
                       return err;
                }
                calib_data[i] = (buf[2*i] << 8) + buf[2*i+1];
        }

        // CRC check
        crc = ms5806_crc_chk(calib_data);
        if(crc /= (calib_data[7] & 0xF))
        {
                return -1;
        }

        return 0;
}


/**
 * @brief Reads temperature value.
 * @param temp : Temperature in celsius degrees.
 * @param pressure : Pressure in millibars.
 * @return i2c error code, 0 if no errors.
 */
uint8_t alti_get_temp_pressure(double* temp, double* pressure, uint8_t filter)
{
        uint8_t err, cmd;
        uint8_t buf[3];
        uint32_t d1, d2;
        double dt, off, sens, p;
        static double prev_p = 0;
        float k = 0.125;

        // Initiate temperature conversion
        cmd = MS5806_CMD_CONV_TEMP + (MS5806_OSR_4096 & MS5806_OSR_MASK);
        err = ms5806_write_cmd(cmd);
        if(err)
        {
                return err;
        }

        // Wait for conversion time (max time 9ms @ OSR=4096)
        Delay(10); // 10ms

        // ADC read (3 bytes), raw temperature value
        err = ms5806_read_reg(MS5806_CMD_READ_ADC, 3, buf);
        if(err)
        {
                return err;
        }
        d2 = (buf[0] << 16) + (buf[1] << 8) + buf[2];

        // Initiate pressure conversion
        cmd = MS5806_CMD_CONV_PRESSURE + (MS5806_OSR_4096 & MS5806_OSR_MASK);
        err = ms5806_write_cmd(cmd);
        if(err)
        {
                return err;
        }

        // Wait for conversion time (max time 9ms @ OSR=4096)
        Delay(10); // 10ms

        // ADC read (3 bytes), raw pressure value
        err = ms5806_read_reg(MS5806_CMD_READ_ADC, 3, buf);
        if(err)
        {
                return err;
        }
        d1 = (buf[0] << 16) + (buf[1] << 8) + buf[2];

        // Calculate compensated temperature and pressure values
        dt = d2 - calib_data[5] * pow(2,8);
        off = calib_data[2] * pow(2,17) + dt * calib_data[4]/pow(2,6);
        sens = calib_data[1] * pow(2,16) + dt * calib_data[3]/pow(2,7);
        *temp = (2000 + (dt * calib_data[6])/pow(2,23))/100;
        p = (((d1 * sens)/pow(2,21) - off)/pow(2,15))/100;
        if(filter)
        {
                if(prev_p == 0)
                {
                        prev_p = p;
                }
                *pressure = prev_p * (1 - k) + p * k;
        }
        else
        {
                *pressure = p;
        }

        return 0;
}


/**
 * @brief Helper function to convert from mbar to altitude.
 *
 * Uses piecewise interpolation of pressure to altitude conversion formula (troposhere model)
 * h = 288.15/0.065 * (1 - (p/1013.25)^(0.065*287.052/9.81))
 * source: Intersema (Meas-spec), AN501 - Using MS5534 for Altimeters and Barometers
 *
 * @param pressure : Pressure in millibars.
 * @param altitude : Altitude in merters.
 * @return i2c error code or crc error (-1), 0 if no errors.
 */
uint8_t alti_mbar2altitude(double pressure, double* altitude)
{
        uint8_t i;

        for(i=0; i<P2A_COEFF_SIZE; i++)
        {
                if((pressure*10) < p2a_coeff[i][1])
                {
                        break;
                }
        }

        *altitude = p2a_coeff[i][3] - (pressure*10 - p2a_coeff[i][0]) * p2a_coeff[i][2]/pow(2,11);

        return i;
}
