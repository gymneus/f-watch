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

#ifndef __LIGHT_SENSOR_H_
#define __LIGHT_SENSOR_H_

#include <stdint.h>
#include <stdbool.h>

// I2C address
#define MAX44009_ADDRESS 0x94

// MAX44009 registers
#define MAX44009_REG_INT_STAT   0x00
#define MAX44009_REG_INT_EN     0x01
#define MAX44009_REG_CONFIG     0x02
#define MAX44009_REG_LUX_H      0x03
#define MAX44009_REG_LUX_L      0x04
#define MAX44009_REG_THRES_H    0x05
#define MAX44009_REG_THRES_L    0x06
#define MAX44009_REG_THRES_TIME 0x07

// Configuration register bits
#define MAX44009_CFG_CONT 0x80 // Continuous mode
#define MAX44009_CFG_MAN  0x40 // Manual configuration
#define MAX44009_CFG_CDR  0x08 // Current division ratio
#define MAX44009_CFG_TIM  0x07 // Integration time

/** Ambient light sensor configuration structure. */
/*typedef struct
{
        bool    cont;
        bool    manual;
        bool    cdr;
        uint8_t timer;

} Light_Sensor_Conf_TypeDef;

uint8_t light_sensor_set_int(uint8_t enable);
uint8_t light_sensor_get_isr(uint8_t* isr);
uint8_t light_sensor_set_cfg(Light_Sensor_Conf_TypeDef *cfg);*/

/**
 * Returns raw reading from the light sensor.
 * @param val is the place where the reading will be stored.
 * @return 0 in case of success, error code otherwise.
 */
uint8_t light_sensor_get_raw(uint16_t* val);

/**
 * Returns light level expressed in millilux.
 * @param lux is the variable where the computed value will be stored.
 * @return 0 in case of success, error code otherwise.
 */
uint8_t light_sensor_get_lux(uint32_t *lux);

/*uint8_t light_sensor_set_thres(double thres);
uint8_t light_sensor_set_thres_timer(uint8_t timer);*/

#endif /* LIGHT_SENSOR_H */
