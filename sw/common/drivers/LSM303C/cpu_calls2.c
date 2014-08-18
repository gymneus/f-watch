#include <em_cmu.h>
//#include <em_device.h>
#include <em_gpio.h>
#include <em_usart.h>
//#include <efm32gg_usart.h>
#include <efm32gg330f1024.h>

#include "cpu_calls.h"
#include "LSM303C_ACC_driver.h"
#include "LSM303C_MAG_driver.h"

#define HFRCO_FREQUENCY         14000000
#define SPI_PERCLK_FREQUENCY    HFRCO_FREQUENCY
#define SPI_BAUDRATE            1000000

#define SPI_LOC USART_ROUTE_LOCATION_LOC2 //2
#define BUFSIZE 80
#define SPI_USART USART0
#define ACC_CS_PIN 8
#define MAG_CS_PIN 10

char spi_txbuf[BUFSIZE];
char spi_rxbuf[BUFSIZE];
volatile int spi_txbuf_idx; /* index of spi_txbuf, used by Tx irq handler, must
                             * be initialized to 0 before tx data written to buffer */
volatile int spi_rxbuf_idx; /* index of spi_rxbuf, used by Rx irq handler, must
                             * be initialized to 0 before expected data reception */
int txbuf_sz;

// Based on
// http://community.silabs.com/t5/tkb/articleprintpage/tkb-id/1000/article-id/1985


/*TODO: move away from here*/
int temp_lsm303_init()
{
  uint8_t dat = 0;
  AxesRaw_t meas;

  spi_init();

  //LSM303C_ACC_Reboot();
  /*setup 3-wire SPI for Accelerometer & Magnetometer*/
  LSM303C_ACC_SerialInterfaceMode(LSM303C_ACC_SIM_3WIRE_INTERFACE);
//  LSM303C_MAG_SerialInterfaceMode(LSM303C_MAG_SIM_3_WIRE);

  LSM303C_ACC_ReadReg(LSM303C_ACC_WHO_AM_I_REG, &dat);  /* read WHO_AM_I_A */
  /*ACC self test*/
  //LSM303C_ACC_SelfTest(LSM303C_ACC_ST_POSITIVE);
  
  return dat;
}

int spi_init()
{
  USART_InitSync_TypeDef cfg = USART_INITSYNC_DEFAULT;
  USART_TypeDef *spi = SPI_USART;
  //CHIP_Init();

  /*Enable clock to USART0 and GPIO*/
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
               // USART_ROUTE_CSPEN /* don't know yet about CS, I'll try to drive it 'by hand' */
               SPI_LOC;

  CMU_ClockEnable(cmuClock_GPIO, true);

  /* GPIO configuration */
  GPIO_PinModeSet(gpioPortC, 11, gpioModePushPull, 0); /* MOSI/MISO */
  GPIO_PinModeSet(gpioPortC, MAG_CS_PIN, gpioModePushPull, 1); /* CS Mag*/
  GPIO_PinModeSet(gpioPortC, ACC_CS_PIN, gpioModePushPull, 1); /* CS Acc*/
  GPIO_PinModeSet(gpioPortC,  9, gpioModePushPull, 1); /* Clock */
  GPIO_PinModeSet(gpioPortD,  4, gpioModeInput, 0);    /* Mag drdy */

  /* buffers init */
  spi_txbuf_idx = 0;
  spi_rxbuf_idx = 0;

  /* finally enable USART (Tx & RX)*/
  //USART_Enable(spi, usartEnable);

  /*setup Rx irq*/
  //spi->CMD = USART_CMD_CLEARRX;
  //NVIC_ClearPendingIRQ(USART0_RX_IRQn);
  //NVIC_EnableIRQ(USART0_RX_IRQn);
  //spi->IEN |= USART_IEN_RXDATAV;

  /*setup Tx irq*/
  //spi->CMD = USART_CMD_CLEARTX;
  //NVIC_ClearPendingIRQ(USART0_TX_IRQn);
  //NVIC_EnableIRQ(USART0_TX_IRQn);
  //spi->IEN |= USART_IEN_TXBL;


  return 0;
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

  //spi->TXDOUBLE = adr << _USART_TXDOUBLE_TXDATA0_SHIFT |
  //                dat << _USART_TXDOUBLE_TXDATA1_SHIFT;
  USART_TxDouble(spi, adr << _USART_TXDOUBLE_TXDATA0_SHIFT |
                      dat << _USART_TXDOUBLE_TXDATA1_SHIFT);

  /* wait until it's done */
  while (!(USART_StatusGet(spi) & USART_STATUS_TXC)) ;

  /* both CS up */
  GPIO_PinOutSet(gpioPortC, ACC_CS_PIN);
  GPIO_PinOutSet(gpioPortC, MAG_CS_PIN);
}

/* read byte from given address */
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

//void spi_send(char* buf, int size)
//{
//  USART_TypeDef *spi = SPI_USART;
//  int i; 
//
//  for (i = 0; i<size;  i++) {
//    /* Waiting for the usart to be ready */
//    while (!(uart->STATUS & USART_STATUS_TXBL)) ;
//
//    if (buf) {   
//      /* Writing next byte to USART */
//      uart->TXDATA = *buf;
//      buf++;
//    }   
//    else
//      uart->TXDATA = 0;
//  }
//
//  /*Waiting for transmission of last byte */
//  while (!(uart->STATUS & USART_STATUS_TXC)) ;
//}

/* Rx IRQ handler, receives byte from SPI, and writes to next position in
 * the rxbuf */
//void spi_rx_irq()
//{
//  USART_TypeDef *spi = SPI_USART;
//
//  if (spi->STATUS & USART_STATUS_RXDATAV) {
//    /*we have valid data received from SPI*/
//    if (spi_rxbuf_idx < BUFSIZE)
//      spi_rxbuf[spi_rxbuf_idx++] = spi->RXDATA;
//  }
//}

/* Tx IRQ handler, sends txbuf byte by byte for a given rxbuf_sz, using
 * spi_txbuf_idx to mark subsequent bytes between calls */
//void spi_tx_irq()
//{
//  USART_TypeDef *spi = SPI_USART;
//
//  if (spi->STATUS & USART_STATUS_TXBL) {
//    if (spi_txbuf_idx < txbuf_sz)
//      /* there is next byte in the buffer to be sent */
//      spi->TXDATA = spi_txbuf_idx[spi_txbuf_idx++];
//    else
//      spi->TXDATA = 0;
//  }
//}


//void spi_setup(uint8_t spiNumber, uint8_t location)
//{
//  USART_TypeDef *spi;
//
//  /* Determining USART */
//  switch (spiNumber)
//  {
//  case 0:
//    spi = USART0;
//    break;
//  case 1:
//    spi = USART1;
//    break;
//  case 2:
//    spi = USART2;
//    break;
//  default:
//    return;
//  }
//
//  /* Setting baudrate */
//  //spi->CLKDIV = 128 * (SPI_PERCLK_FREQUENCY / SPI_BAUDRATE - 2);
//
//  /* Configure SPI */
//  spi->CTRL = USART_CTRL_SYNC |  /* synchronous (SPI) mode */
//              USART_CTRL_LOOPBK; /* use half-duplex */
//  /* Clearing old transfers/receptions, and disabling interrupts */
//  spi->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;
//  spi->IEN = 0;
//  /* Enabling pins and setting location */
//  spi->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_CLKPEN | USART_ROUTE_CSPEN | (location << 8);
//  
//  /* Enabling Master, TX and RX */
//  spi->CMD   = USART_CMD_MASTEREN | USART_CMD_TXEN | USART_CMD_RXEN;
//  //spi->CTRL |= USART_CTRL_AUTOCS; /* sorry, no AUTOCS, because ACC and MAG
//                                    /* have separete CS signals and we have
//                                     * to choose witch whom to speak */
//  
//  /* Clear previous interrupts */
//  spi->IFC = _USART_IFC_MASK;
//
//  /* fuck the rest of the USARTs and locations, our case is simple, USART0 & Loc 2 */
//  GPIO_PinModeSet(gpioPortC, 11, gpioModePushPull, 0); /* MOSI/MISO */
//  GPIO_PinModeSet(gpioPortC, 10, gpioModePushPull, 1); /* CS Mag*/
//  GPIO_PinModeSet(gpioPortC,  8, gpioModePushPull, 1); /* CS Acc*/
//  GPIO_PinModeSet(gpioPortC,  9, gpioModePushPull, 1); /* Clock */
//  GPIO_PinModeSet(gpioPortD,  4, gpioModeInput, 0);    /* Mag drdy */
//
//}
