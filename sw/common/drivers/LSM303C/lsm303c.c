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

void spi_send(uint8_t dev, uint8_t adr, uint8_t dat)
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
}

/* read byte from given a address */
uint8_t spi_read(uint8_t dev, uint8_t adr)
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

  return spi->RXDATA & 0xff;
}

/************************************************/
/*                LSM303C stuff                 */
/************************************************/
int lsm303_init()
{
  spi_init();

  /*setup 3-wire SPI for Accelerometer & Magnetometer*/
  LSM303C_ACC_SerialInterfaceMode(LSM303C_ACC_SIM_3WIRE_INTERFACE);


  return LSM303_SUCCESS;
}
