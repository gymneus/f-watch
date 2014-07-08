/***************************************************************************//**
 * @file
 * @brief EEPROM driver for 24AA024 (2Kbit) EEPROM device on the DK.
 * @version 3.20.5
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/




#ifndef __EEPROM_H
#define __EEPROM_H

#include "em_device.h"

/***************************************************************************//**
 * @addtogroup Drivers
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Eeprom
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

/** I2C device address for EEPROM on DK */
#define EEPROM_DVK_ADDR    0xA0


/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

int EEPROM_Read(I2C_TypeDef *i2c,
                uint8_t addr,
                unsigned int offset,
                uint8_t *data,
                unsigned int len);
int EEPROM_Write(I2C_TypeDef *i2c,
                 uint8_t addr,
                 unsigned int offset,
                 uint8_t *data,
                 unsigned int len);

#ifdef __cplusplus
}
#endif

/** @} (end group Eeprom) */
/** @} (end group Drivers) */

#endif /* __EEPROM_H */
