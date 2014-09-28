/*
 * Copyright (C) 2014 Julian Lewis
 * @author Grzegorz Daniluk <grzegorz.daniluk@cern.ch>
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
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <em_cmu.h>
#include <em_gpio.h>
#include <em_usart.h>
#include <efm32gg330f1024.h>
#include "lsm303c.h"

static int iron_x, iron_y, iron_z;

extern volatile uint32_t msTicks;
void Delay(uint32_t dlyTicks);

/************************************************/
/* First functions to initialize and access SPI */
/************************************************/
static void spi_init()
{
  USART_InitSync_TypeDef cfg = USART_INITSYNC_DEFAULT;
  USART_TypeDef *spi = SPI_USART;

  CMU_ClockEnable(cmuClock_USART0, true);
  /* basic configuration */
  cfg.baudrate = SPI_BAUDRATE;
  cfg.clockMode = usartClockMode3;  //Clock idle high, sample on rising edge
  cfg.msbf = true;                  //MSB first
  USART_InitSync(SPI_USART, &cfg);

  /* signals routing to location 2 and loopback for half-duplex */
  spi->CTRL |= USART_CTRL_LOOPBK; /* use half-duplex */
  spi->ROUTE = USART_ROUTE_CLKPEN |
               USART_ROUTE_TXPEN  |
               SPI_LOC;

  CMU_ClockEnable(cmuClock_GPIO, true);

  /* GPIO configuration */
  GPIO_PinModeSet(gpioPortC, 11, gpioModePushPull, 0); /* MOSI/MISO */
  GPIO_PinModeSet(gpioPortC, MAG_CS_PIN, gpioModePushPull, 1); //InputPull, 0); /* CS Mag*/
  //GPIO_PinModeSet(gpioPortC, MAG_CS_PIN, gpioModeWiredAndPullUp, 1); // gpioModePushPull, 1); /* CS Mag*/
  //GPIO_PinModeSet(gpioPortC, ACC_CS_PIN, gpioModeWiredAndPullUp, 1); //gpioModePushPull, 1); /* CS Acc*/
  GPIO_PinModeSet(gpioPortC, ACC_CS_PIN, gpioModePushPull, 1); /* CS Acc*/
  GPIO_PinModeSet(gpioPortC,  9, gpioModePushPull, 1); /* Clock */
  GPIO_PinModeSet(gpioPortD,  4, gpioModeInput, 0);    /* Mag drdy */
}

int spi_send(uint8_t dev, uint8_t adr, uint8_t dat)
{
  USART_TypeDef *spi = SPI_USART;

  if (dev == DEV_ACC) {
    /* drive ACC CS down */
    GPIO_PinOutClear(gpioPortC, ACC_CS_PIN);
    GPIO_PinOutSet(gpioPortC, MAG_CS_PIN);
  }
  else if (dev == DEV_MAG) {
    /* drive MAG CS down */
    GPIO_PinOutSet(gpioPortC, ACC_CS_PIN);
    GPIO_PinOutClear(gpioPortC, MAG_CS_PIN);
  }

  USART_TxDouble(spi, adr << _USART_TXDOUBLE_TXDATA0_SHIFT |
                      dat << _USART_TXDOUBLE_TXDATA1_SHIFT);

  /* wait until it's done */
  while (!(USART_StatusGet(spi) & USART_STATUS_TXC)) ;

  /* both CS up */
  GPIO_PinOutSet(gpioPortC, ACC_CS_PIN);
  GPIO_PinOutSet(gpioPortC, MAG_CS_PIN);

  return LSM303_SUCCESS;
}

/* read byte from given a address */
int spi_read(uint8_t dev, uint8_t adr, uint8_t *dat)
{
  USART_TypeDef *spi = SPI_USART;

  if (dev == DEV_ACC) {
    /* drive ACC CS down */
    GPIO_PinOutClear(gpioPortC, ACC_CS_PIN);
    GPIO_PinOutSet(gpioPortC, MAG_CS_PIN);
  }
  else if (dev == DEV_MAG) {
    /* drive MAG CS down */
    GPIO_PinOutSet(gpioPortC, ACC_CS_PIN);
    GPIO_PinOutClear(gpioPortC, MAG_CS_PIN);
  }

  spi->CMD = USART_CMD_RXBLOCKEN;   /* Block loopbacked RX while we send adr to Slave */
  spi->CMD = USART_CMD_CLEARRX;     /* Clear old data, if any */

  spi->TXDOUBLEX = (adr | 1<< 7) << _USART_TXDOUBLEX_TXDATA0_SHIFT | 
                   USART_TXDOUBLEX_TXTRIAT0 | /* tristate Tx */
                   USART_TXDOUBLEX_UBRXAT0  | /* unblock Rx */
                   0x00 << _USART_TXDOUBLEX_TXDATA1_SHIFT | /* dummy Tx data for reading Slave */
                   USART_TXDOUBLEX_TXTRIAT1 | /* once again, errata USART_E101 */
                   USART_TXDOUBLEX_UBRXAT1;

  /* wait for valid rx data */
  while( !(USART_StatusGet(spi) & USART_STATUS_RXDATAV) );

  /* we have new data, turn off tristate Tx */
  spi->CMD = USART_CMD_TXTRIDIS;
  /* both CS up */
  GPIO_PinOutSet(gpioPortC, ACC_CS_PIN);
  GPIO_PinOutSet(gpioPortC, MAG_CS_PIN);

  *dat = (spi->RXDATA & 0xff);
  return LSM303_SUCCESS;
}

/************************************************/
/*                LSM303 stuff                 */
/************************************************/
int lsm303_init()
{
  spi_init();

  /*setup 3-wire SPI for Accelerometer & Magnetometer*/
  //LSM303_ACC_SerialInterfaceMode(LSM303_ACC_SIM_3WIRE_INTERFACE);
	lsm303_serialmode(DEV_ACC, LSM303_ACC_3WIRE);
	lsm303_serialmode(DEV_MAG, LSM303_MAG_3WIRE);

  lsm303_enableaxis(DEV_ACC, 0x7); 
  lsm303_fifo_mode(DEV_ACC, LSM303_FMODE_BYPASS, 1);
  lsm303_fifo_mode(DEV_ACC, LSM303_FMODE_BYPASS, 0);
	lsm303_fullscale(DEV_ACC, LSM303_8G);
  lsm303_odr(DEV_ACC, LSM303_ACC_ODR_50_Hz);
	
	lsm303_odr(DEV_MAG, LSM303_MAG_ODR_20_Hz);
	lsm303_opmode(DEV_MAG, LSM303_MAG_OPM_MED, LSM303_MAG_CONV_CONT);
	lsm303_fullscale(DEV_MAG, LSM303_16Ga);
	lsm303_selftest(DEV_MAG, 0, 0);

	/*load default hard iron calibration values*/
	iron_x = LSM303_IRON_X;
	iron_y = LSM303_IRON_Y;
	iron_z = LSM303_IRON_Z;

  return LSM303_SUCCESS;
}

/* dev: DEV_ACC/DEV_MAG
 * en: 1->enable; 0->disable
 * dir: 1->positive; 0->negative (for ACC) */
int lsm303_selftest(int dev, int en, int dir)
{
  uint8_t val;

  if( (dev==DEV_ACC && !spi_read(dev, LSM303_CTRL5, &val)) ||
      (dev==DEV_MAG && !spi_read(dev, LSM303_CTRL1, &val)) )
    return LSM303_ERROR;

  val &= ~( dev==DEV_ACC ? LSM303_ACC_ST_MASK : LSM303_MAG_ST_MASK );

  if (dev==DEV_ACC && en && dir)
    val |= LSM303_ACC_ST_POS;
  else if (dev==DEV_ACC && en && !dir)
    val |= LSM303_ACC_ST_NEG;
  else if (dev==DEV_MAG && en)
    val |= LSM303_MAG_ST_EN;

  if( (dev==DEV_ACC && !spi_send(dev, LSM303_CTRL5, val)) ||
      (dev==DEV_MAG && !spi_send(dev, LSM303_CTRL1, val)) )
    return LSM303_ERROR;

  return LSM303_SUCCESS;
}

/* Set output data rate
 * dev: DEV_ACC/DEV_MAG
 * odr: appropriate constant from lsm303.h */
int lsm303_odr(int dev, LSM303_ODR_t odr)
{
	uint8_t val;

  if (!spi_read(dev, LSM303_CTRL1, &val))
    return LSM303_ERROR;

  val &= ~( dev==DEV_ACC ? LSM303_ACC_ODR_MASK : LSM303_MAG_ODR_MASK );
  val |= odr;

  if (!spi_send(dev, LSM303_CTRL1, val))
    return LSM303_ERROR;

  return LSM303_SUCCESS;
}

/* set operation mode
 * dev: DEV_ACC/DEV_MAG
 * opm: constant from lsm303.h */
int lsm303_opmode(int dev, LSM303_OPM_t opm, LSM303_CONV_t conv)
{
	uint8_t val;

	if (!spi_read(dev, LSM303_CTRL1, &val)) return LSM303_ERROR;

  val &= ~( dev==DEV_ACC ? LSM303_ACC_OPM_MASK : LSM303_MAG_OPM_MASK );
  val |= opm;

  if( !spi_send(dev, LSM303_CTRL1, val) ) return LSM303_ERROR;

	if (dev==DEV_ACC) return LSM303_SUCCESS;

	/* only for MAG, set conversion mode and opmode for Z axis*/
	if (!spi_read(dev, LSM303_CTRL4, &val)) return LSM303_ERROR;
	val &= ~(LSM303_MAG_OPM_MASK>>3);
	val |= (opm>>3);
	if (!spi_send(dev, LSM303_CTRL4, val)) return LSM303_ERROR;

	if (!spi_read(dev, LSM303_CTRL3, &val)) return LSM303_ERROR;
	val &= ~LSM303_MAG_CONV_MASK;
	val |= conv;
	if (!spi_send(dev, LSM303_CTRL3, val)) return LSM303_ERROR;

	return LSM303_SUCCESS;
}

/* soft reset
 * dev: DEV_ACC/DEV_MAG
 * hard: 1-> reboot, 0-> soft reset */
int lsm303_rst(int dev, int hard)
{
	uint8_t val;

	if( (dev==DEV_ACC && !hard && !spi_read(dev, LSM303_CTRL5, &val)) ||
			(dev==DEV_ACC && hard  && !spi_read(dev, LSM303_CTRL6, &val)) ||
			(dev==DEV_MAG && !spi_read(dev, LSM303_CTRL2, &val)) )
		return LSM303_ERROR;

	if( hard ) 
		val |= (dev==DEV_ACC ? LSM303_ACC_REBOOT : LSM303_MAG_REBOOT);
	else
		val |= (dev==DEV_ACC ? LSM303_ACC_SRST : LSM303_MAG_SRST);

  if( (dev==DEV_ACC && !hard && !spi_send(dev, LSM303_CTRL5, val)) ||
			(dev==DEV_ACC && hard  && !spi_send(dev, LSM303_CTRL6, val)) ||
      (dev==DEV_MAG && !spi_send(dev, LSM303_CTRL2, val)) )
    return LSM303_ERROR;

  return LSM303_SUCCESS;
}

/* temperature readout */
int lsm303_temp(int16_t *temp)
{
	uint8_t val;
	*temp = 0;

	/*enable temp sensor*/
	if( !spi_read(DEV_MAG, LSM303_CTRL1, &val) )
		return LSM303_ERROR;
	val |= LSM303_MAG_TEMP_EN;
	if( !spi_send(DEV_MAG, LSM303_CTRL1, val) )
		return LSM303_ERROR;

	/*read temperature*/
	if( !spi_read(DEV_MAG, LSM303_MAG_TEMP_L, &val) )
		return LSM303_ERROR;
	*temp = (int16_t)val;
	if( !spi_read(DEV_MAG, LSM303_MAG_TEMP_H, &val) )
		return LSM303_ERROR;
	*temp |= ((int16_t)val << 8) & 0xFF00;

	/*disable temp sensor*/
	if( !spi_read(DEV_MAG, LSM303_CTRL1, &val) )
		return LSM303_ERROR;
	val &= ~LSM303_MAG_TEMP_EN;
	if( !spi_send(DEV_MAG, LSM303_CTRL1, val) )
		return LSM303_ERROR;

	return LSM303_SUCCESS;
}

int lsm303_fullscale(int dev, LSM303_FS_t fs)
{
	uint8_t val;

	/*check if _fs_ for proper device*/
	if( (dev==DEV_ACC && !(LSM303_ACC_FS_MASK & fs)) ||
			(dev==DEV_MAG && !(LSM303_MAG_FS_MASK & fs)) )
		return LSM303_ERROR;

	if( (dev==DEV_ACC && !spi_read(dev, LSM303_CTRL4, &val)) ||
			(dev==DEV_MAG && !spi_read(dev, LSM303_CTRL2, &val)) )
		return LSM303_ERROR;

	val &= ~(dev==DEV_ACC ? LSM303_ACC_FS_MASK : LSM303_MAG_FS_MASK);
	val |= fs;

	if( (dev==DEV_ACC && !spi_send(dev, LSM303_CTRL4, val)) ||
			(dev==DEV_MAG && !spi_send(dev, LSM303_CTRL2, val)) )
		return LSM303_ERROR;

	return LSM303_SUCCESS;
}

int lsm303_serialmode(int dev, LSM303_SMODE_t mode)
{
	uint8_t val;

	if( (dev==DEV_ACC && !spi_read(dev, LSM303_CTRL4, &val)) ||
			(dev==DEV_MAG && !spi_read(dev, LSM303_CTRL3, &val)) )
		return LSM303_ERROR;

	/* we'll use SPI, disable I2c */
	val |= (dev==DEV_ACC ? LSM303_ACC_NOI2C : LSM303_MAG_NOI2C);
	/* now set SPI mode */
	val &= ~(dev==DEV_ACC ? LSM303_ACC_3WIRE : LSM303_MAG_3WIRE);
	val |= mode;

	if( (dev==DEV_ACC && !spi_send(dev, LSM303_CTRL4, val)) ||
			(dev==DEV_MAG && !spi_send(dev, LSM303_CTRL3, val)) )
		return LSM303_ERROR;

	return LSM303_SUCCESS;
}

int lsm303_enableaxis(int dev, int mask)
{
	uint8_t val;

	if( dev == DEV_MAG )
		return LSM303_ERROR;

	if(!spi_read(dev, LSM303_CTRL1, &val))
		return LSM303_ERROR;

	val &= ~LSM303_ACC_AXIS_MASK;
	val |= mask;

	if(!spi_send(dev, LSM303_CTRL1, val))
		return LSM303_ERROR;

	return LSM303_SUCCESS;
}

int lsm303_fifo_mode(int dev, LSM303_FMODE_t mode, int en)
{
	uint8_t val;

	if(dev==DEV_MAG)
		return LSM303_ERROR;

	/*first enable/disable fifo*/
	if(!spi_read(dev, LSM303_CTRL3, &val))
		return LSM303_ERROR;
	if(en)
		val |= LSM303_FIFO_EN;
	else
		val &= ~LSM303_FIFO_EN;
	if(!spi_send(dev, LSM303_CTRL3, val))
		return LSM303_ERROR;

	/* don't set mode if FIFO disabled */
	if(!en) return LSM303_SUCCESS;

	/*now set the mode*/
	if(!spi_read(dev, LSM303_ACC_FIFO_CTRL, &val))
		return LSM303_ERROR;
	val &= ~LSM303_FMODE_MASK;
	val |= mode;
	if(!spi_send(dev, LSM303_ACC_FIFO_CTRL, val))
		return LSM303_ERROR;

	return LSM303_SUCCESS;
}

static void lsm303_iron_corr(lsm303_smpl *smpl)
{
	smpl->x -= iron_x;
	smpl->y -= iron_y;
	smpl->z -= iron_z;

	//smpl->x = (int)( (float)smpl->x*siron_x );
	//smpl->y = (int)( (float)smpl->y*siron_y );
	//smpl->z = (int)( (float)smpl->z*siron_z );
}

int lsm303_get_sample(int dev, lsm303_smpl *smpl)
{
	uint8_t val_l, val_h;

	if( !spi_read(dev, LSM303_OUT_X_L, &val_l) ||
			!spi_read(dev, LSM303_OUT_X_H, &val_h) )
		return LSM303_ERROR;
	smpl->x = (int16_t) ((val_h << 8) | val_l);

	if( !spi_read(dev, LSM303_OUT_Y_L, &val_l) ||
			!spi_read(dev, LSM303_OUT_Y_H, &val_h) )
		return LSM303_ERROR;
	smpl->y = (int16_t) ((val_h << 8) | val_l);

	if( !spi_read(dev, LSM303_OUT_Z_L, &val_l) ||
			!spi_read(dev, LSM303_OUT_Z_H, &val_h) )
		return LSM303_ERROR;
	smpl->z = (int16_t) ((val_h << 8) | val_l);

	lsm303_iron_corr(smpl);

	return LSM303_SUCCESS;
}

lsm303_smpl lsm303_calib_iron(lsm303_smpl *max, lsm303_smpl *min)
{
	lsm303_smpl ret;

	iron_x = (min->x + max->x)/2;
	iron_y = (min->y + max->y)/2;
	iron_z = (min->z + max->z)/2;

	ret.x = iron_x;
	ret.y = iron_y;
	ret.z = iron_z;
	return ret;
}

#define LCD_W 100
#define LCD_0 60
int compass_xy(lsm303_smpl *max, lsm303_smpl *min, int *x, int *y)
{
	lsm303_smpl val;

	lsm303_get_sample(DEV_MAG, &val);

	if(val.x >= 0) {
		*y = LCD_0 - (((val.x*1000)/(float)(max->x)) * LCD_W)/2000.0;
	} else if(val.x < 0) {
		*y = LCD_0 + (((val.x*1000)/(float)(min->x)) * LCD_W)/2000.0;
	}

	if(val.y >= 0) {
		*x = LCD_0 + (((val.y*1000)/(float)(max->y)) * LCD_W)/2000.0;
	} else if(val.x < 0) {
		*x = LCD_0 - (((val.y*1000)/(float)(min->y)) * LCD_W)/2000.0;
	}

	if(*x > LCD_0 + LCD_W/2) *x = LCD_0+LCD_W/2;
	if(*x < LCD_0 - LCD_W/2) *x = LCD_0-LCD_W/2;
	if(*y > LCD_0 + LCD_W/2) *y = LCD_0+LCD_W/2;
	if(*y < LCD_0 - LCD_W/2) *y = LCD_0-LCD_W/2;
}
