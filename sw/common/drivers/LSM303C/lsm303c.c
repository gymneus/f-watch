#include <em_cmu.h>
#include <em_gpio.h>
#include <em_usart.h>
#include <efm32gg330f1024.h>
#include "lsm303c.h"

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
  GPIO_PinModeSet(gpioPortC, MAG_CS_PIN, gpioModePushPull, 1); /* CS Mag*/
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
	//lsm303_serialmode(DEV_MAG, LSM303_ACC_3WIRE);

  lsm303_enableaxis(DEV_ACC, 0x7); 
  lsm303_fifo_mode(DEV_ACC, LSM303_FMODE_BYPASS, 1);
  lsm303_fifo_mode(DEV_ACC, LSM303_FMODE_BYPASS, 0);
  lsm303_odr(DEV_ACC, LSM303_ACC_ODR_10_Hz);

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
int lsm303_opmode(int dev, LSM303_OPM_t opm)
{
	uint8_t val;

	if (!spi_read(dev, LSM303_CTRL1, &val))
		return LSM303_ERROR;

  val &= ~( dev==DEV_ACC ? LSM303_ACC_OPM_MASK : LSM303_MAG_OPM_MASK );
  val |= opm;

  if( !spi_send(dev, LSM303_CTRL1, val) )
    return LSM303_ERROR;

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

int lsm303_get_sample(int dev, int16_t *x, int16_t *y, int16_t *z)
{
	uint8_t val_l, val_h;

	if( !spi_read(dev, LSM303_OUT_X_L, &val_l) ||
			!spi_read(dev, LSM303_OUT_X_H, &val_h) )
		return LSM303_ERROR;
	*x = (int16_t) ((val_h << 8) | val_l);

	if( !spi_read(dev, LSM303_OUT_Y_L, &val_l) ||
			!spi_read(dev, LSM303_OUT_Y_H, &val_h) )
		return LSM303_ERROR;
	*y = (int16_t) ((val_h << 8) | val_l);

	if( !spi_read(dev, LSM303_OUT_Z_L, &val_l) ||
			!spi_read(dev, LSM303_OUT_Z_H, &val_h) )
		return LSM303_ERROR;
	*z = (int16_t) ((val_h << 8) | val_l);

	return LSM303_SUCCESS;
}