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

#ifndef __MAX17047_H_
#define __MAX17047_H_

#include <stdint.h>
#include <stdbool.h>

// Two-wire protocol address
#define MAX17047_ADDRESS                0x6C

// Register addresses
#define MAX17047_REG_STATUS             0x00
#define MAX17047_REG_VALRT_TH           0x01
#define MAX17047_REG_TALRT_TH           0x02
#define MAX17047_REG_SALRT_TH           0x03
#define MAX17047_REG_AT_RATE            0x04
#define MAX17047_REG_REM_CAP_REP        0x05
#define MAX17047_REG_SOC_REP            0x06
#define MAX17047_REG_AGE                0x07
#define MAX17047_REG_TEMPERATURE        0x08
#define MAX17047_REG_VCELL              0x09
#define MAX17047_REG_CURRENT            0x0A
#define MAX17047_REG_AVR_CURRENT        0x0B
#define MAX17047_REG_SOC_MIX            0x0D
#define MAX17047_REG_SOC_AV             0x0E
#define MAX17047_REG_REM_CAP_MIX        0x0F
#define MAX17047_REG_FULL_CAP           0x10
#define MAX17047_REG_TIME_TO_EMPTY      0x11
#define MAX17047_REG_QRESIDUAL_00       0x12
#define MAX17047_REG_FULL_SOC_THR       0x13
#define MAX17047_REG_AVG_TEMP           0x16
#define MAX17047_REG_CYCLES             0x17
#define MAX17047_REG_DESIGN_CAP         0x18
#define MAX17047_REG_AVG_VCELL          0x19
#define MAX17047_REG_MAX_MIN_TEMP       0x1A
#define MAX17047_REG_MAX_MIN_VCELL      0x1B
#define MAX17047_REG_MAX_MIN_CURRENT    0x1C
#define MAX17047_REG_CONFIG             0x1D
#define MAX17047_REG_ICHG_TERM          0x1E
#define MAX17047_REG_REM_CAP_AV         0x1F
#define MAX17047_REG_VERSION            0x21
#define MAX17047_REG_QRESIDUAL_10       0x22
#define MAX17047_REG_FULL_CAP_NOM       0x23
#define MAX17047_REG_TEMP_NOM           0x24
#define MAX17047_REG_TEMP_LIM           0x25
#define MAX17047_REG_AIN                0x27
#define MAX17047_REG_LEARN_CFG          0x28
#define MAX17047_REG_FILTER_CFG         0x29
#define MAX17047_REG_RELAX_CFG          0x30
#define MAX17047_REG_MISC_CFG           0x2B
#define MAX17047_REG_TGAIN              0x2C
#define MAX17047_REG_TOFF               0x2D
#define MAX17047_REG_CGAIN              0x2E
#define MAX17047_REG_COFF               0x2F
#define MAX17047_REG_QRESIDUAL_20       0x32
#define MAX17047_REG_IAVG_EMPTY         0x36
#define MAX17047_REG_FCTC               0x37
#define MAX17047_REG_RCOMP              0x38
#define MAX17047_REG_TEMP_CO            0x39
#define MAX17047_REG_V_EMPTY            0x3A
#define MAX17047_REG_FSTAT              0x3D
#define MAX17047_REG_TIMER              0x3E
#define MAX17047_REG_SHDN_TIMER         0x3F
#define MAX17047_REG_QRESIDUAL_30       0x42
#define MAX17047_REG_DQACC              0x45
#define MAX17047_REG_DPACC              0x46
#define MAX17047_REG_QH                 0x4D
#define MAX17047_REG_CHAR_TABLE_START   0x80
#define MAX17047_REG_CHAR_TABLE_END     0xAF
#define MAX17047_REG_VFOCV              0xFB
#define MAX17047_REG_SOC_VF             0xFF

// Status register bits
// Power-On Reset (1 == hw/sw POR event occurred; needs reconfig)
#define MAX17047_STS_POR                0x0002
// Battery Status (0 == present, 1 == absent)
#define MAX17047_STS_BST                0x0008
// Minimum Valrt Threshold Exceeded (1 == Vcell < min Valrt)
#define MAX17047_STS_VMN                0x0100
// Minimum Talrt Threshold Exceeded (1 == temperature < min Talrt)
#define MAX17047_STS_TMN                0x0200
// Minimum SOCalrt Threshold Exceeded (1 == SOC < min SOCalrt)
#define MAX17047_STS_SMN                0x0400
// Battery Insertion (1 == battery has been inserted; has to be cleared manually)
#define MAX17047_STS_BI                 0x0800
// Maximum Valrt Threshold Exceeded (1 == Vcell > max Valrt)
#define MAX17047_STS_VMX                0x1000
// Maximum Talrt Threshold Exceeded (1 == temperature > max Talrt)
#define MAX17047_STS_TMX                0x2000
// Maximum SOC Threshold Exceeded (1 == SOC > max SOCalrt)
#define MAX17047_STS_SMX                0x4000
// Battery Removal (1 == battery has been removed; has to be cleared manually)
#define MAX17047_STS_BR                 0x8000

// Configuration register bits
// Enables battery removal alert (default 0)
#define MAX17047_CFG_BER                0x0001
// Enables battery insertion alert (default 0)
#define MAX17047_CFG_BEI                0x0002
// Enables alerts on outputs (default 0)
#define MAX17047_CFG_AEN                0x0004
// Forces thermistor bias switch (default 0)
#define MAX17047_CFG_FTHRM              0x0008
// Enables thermistor (default 1)
#define MAX17047_CFG_ETHRM              0x0010
// ALRT pin as shutdown input (default 0)
#define MAX17047_CFG_ALSH               0x0020
// I2C shutdown, if SDA and SCL are low for more than SHDNTIMER (default 1)
#define MAX17047_CFG_I2CSH              0x0040
// Shutdown (default 0)
#define MAX17047_CFG_SHDN               0x0080
// External temperature measurement written from host (default 1)
#define MAX17047_CFG_TEX                0x0100
// Enables temperature measurement (default 1)
#define MAX17047_CFG_TEN                0x0200
// Enables shutdown when battery is removed (default 0)
#define MAX17047_CFG_AINSH              0x0400
// ALRT pin polarity, 0=active low (default 0)
#define MAX17047_CFG_ALRTP              0x0800
// Voltage ALRT sticky, voltage alerts can only be cleared by sw if 1 (default 0)
#define MAX17047_CFG_VS                 0x1000
// Temperature ALRT sticky, temperature alerts can only be cleared by sw if 1 (default 1)
#define MAX17047_CFG_TS                 0x2000
// SOC ALRT sticky, SOC alerts can only be cleared by sw if 1 (default 0)
#define MAX17047_CFG_SS                 0x4000


/**
 * @brief Reads a register from max17047.
 * @param uint8_t reg Register number (the starting one, if you read more than one).
 * @param uint8_t length Number of bytes to read.
 * @param uint8_t* buffer Buffer to save the read data.
 * @return Length of the read data in case of success, error code otherwise.
 * @see I2C_TransferSeq_TypeDef
 */
uint8_t max17047_read_reg(uint8_t address, uint8_t length, uint8_t* buffer);

/**
 * @brief Writes a register to max17047.
 * @param uint8_t reg Register number (the starting one, if you write more than one).
 * @param uint8_t length Number of bytes to write.
 * @param uint8_t* buffer Buffer with data to be written.
 * @return Length of the written data in case of success, error code otherwise.
 * @see I2C_TransferSeq_TypeDef
 */
uint8_t max17047_write_reg(uint8_t address, uint8_t length, uint8_t* buffer);

/**
 * @brief Initialises the max17047, restores the saved registers in case of a POR.
 * @return 0 if no errors.
 */
uint8_t max17047_init(void);

/**
 * @brief Saves the learned-value and application register.
 *
 * This function should be called periodically (e.g. End-of-charge, End-of-discharge,
 * prior to entering shutdown state).
 *
 + @return 0 if regiters saved successfully.
 */
uint8_t max17047_save_regs(void);

/**
 * @brief Reads the status.
 * @return Status of max17047. Check MAX170474_STS_xxx defines for details.
 */
uint16_t max17047_get_status(void);

/**
 * @brief Reads the configuration.
 * @return Configuration of max17047. Check MAX170474_CFG_xxx defines for details.
 */
uint16_t max17047_get_config(void);

/**
 * @brief Writes the configuration.
 * @param Configuration of max17047. Check MAX170474_CFG_xxx defines for details.
 * @return 0 if no error.
 */
uint8_t max17047_set_config(uint16_t config);

/**
 * @brief Reads the battery voltage.
 * @return Battery voltage (in millivolts).
 */
uint16_t max17047_get_voltage(void);

/**
 * @brief Reads current drawn from the battery.
 * @return Current (in milliampers). Negative values represent discharging,
 * positive values represent charging.
 */
int16_t max17047_get_current(void);

/**
 * @brief Reads temperature sensor.
 * @return Temperature (*C).
 */
int8_t max17047_get_temperature(void);

/**
 * @brief Reads state of charge.
 * @return State of charge (expressed as percentage: 0-100).
 */
uint8_t max17047_get_charge(void);

/**
 * @brief Reads remaining time to empty the battery.
 * @return Time to empty (in minutes).
 */
uint16_t max17047_get_time_left(void);

/**
 * @brief Checks if the battery is charging or discharging.
 * @return true if the battery is charging, false otherwise.
 */
static inline bool max17047_is_charging(void)
{
    return (max17047_get_current() >= 0);
}

#endif /*__MAX17047_H_ */
