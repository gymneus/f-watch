/**************************************************************************//**
 * @file
 * @brief Simple LED Blink Demo for EFM32GG_DK3750
 * @author Energy Micro AS
 * @version 3.20.3
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2012 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 *****************************************************************************/
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

/* Counts 1ms timeTicks */
volatile uint32_t msTicks;

/* Local prototypes */
void Delay(uint32_t dlyTicks);

/**************************************************************************//**
* @brief SysTick_Handler
* Interrupt Service Routine for system tick counter
 *****************************************************************************/
void SysTick_Handler(void)
{
    msTicks++; /* increment counter necessary in Delay()*/
}

/**************************************************************************//**
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 *****************************************************************************/
void Delay(uint32_t dlyTicks)
{
    uint32_t curTicks;

    curTicks = msTicks;
    while ((msTicks - curTicks) < dlyTicks);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// SPI functions and data /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/* Defines */
#define HFRCO_FREQUENCY         14000000
#define SPI_PERCLK_FREQUENCY    HFRCO_FREQUENCY
#define SPI_BAUDRATE            1000000
#if 0

/* Buffer pointers and indexes */
char* slaveTxBuffer;
int slaveTxBufferSize;
volatile int slaveTxBufferIndex;
char* slaveRxBuffer;
int slaveRxBufferSize;
volatile int slaveRxBufferIndex;


/**************************************************************************//**
 * @brief Setup a USART as SPI
 * @param spiNumber is the number of the USART to use (e.g. 1 USART1)
 * @param location is the GPIO location to use for the device
 * @param master set if the SPI is to be master
 *
 * For the freewatch, init of SD =>  SPI_setup(1, 1, 1);
 * For the freewatch, init of LCD => SPI_setup(2, 0, 1);
 *****************************************************************************/
void SPI_setup(uint8_t spiNumber, uint8_t location, bool master)
{
  USART_TypeDef *spi;
  USART_InitSync_TypeDef usartInit = USART_INITSYNC_DEFAULT;

  /* Determining USART */
  switch (spiNumber)
  {
  case 0:
    spi = USART0;
    break;
  case 1:
    spi = USART1;
    break;
  case 2:
    spi = USART2;
    break;
  default:
    return;
  }

  /* Setting baudrate */
  spi->CLKDIV = 128 * (SPI_PERCLK_FREQUENCY / SPI_BAUDRATE - 2);

  /* Configure SPI */
  /* Using synchronous (SPI) mode*/
  spi->CTRL = USART_CTRL_SYNC;
  /* Clearing old transfers/receptions, and disabling interrupts */
  //TODO: remmetre?//spi->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;
  spi->IEN = 0;
  /* Enabling pins and setting location */
  spi->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_CLKPEN | USART_ROUTE_CSPEN | (location << 8);
  
  /* Set GPIO config to slave */
  GPIO_Mode_TypeDef gpioModeMosi = gpioModeInput;
  GPIO_Mode_TypeDef gpioModeMiso = gpioModePushPull;
  GPIO_Mode_TypeDef gpioModeCs   = gpioModeInput;
  GPIO_Mode_TypeDef gpioModeClk  = gpioModeInput;
  
  /* Set to master and to control the CS line */
  if (master)
  {
    /* Enabling Master, TX and RX */
    spi->CMD   = USART_CMD_MASTEREN | USART_CMD_TXEN | USART_CMD_RXEN;
    spi->CTRL |= USART_CTRL_AUTOCS;
    
    /* Set GPIO config to master */
    gpioModeMosi = gpioModePushPull;
    gpioModeMiso = gpioModeInput;
    gpioModeCs   = gpioModePushPull;
    gpioModeClk  = gpioModePushPull;
  }
  else
  {
    /* Enabling TX and RX */
    spi->CMD = USART_CMD_TXEN | USART_CMD_RXEN;
  }

  /* Clear previous interrupts */
  spi->IFC = _USART_IFC_MASK;

  /* IO configuration */
  switch(spiNumber)
  {
    case 0: switch(location)
            {
              case 0: /* IO configuration (USART 0, Location #0) */
                      GPIO_PinModeSet(gpioPortE, 10, gpioModeMosi, 0); /* MOSI */
                      GPIO_PinModeSet(gpioPortE, 11, gpioModeMiso, 0); /* MISO */
                      GPIO_PinModeSet(gpioPortE, 13, gpioModeCs,   0); /* CS */
                      GPIO_PinModeSet(gpioPortE, 12, gpioModeClk,  0); /* Clock */
                      break;
              case 1: /* IO configuration (USART 0, Location #1) */
                      GPIO_PinModeSet(gpioPortE, 7, gpioModeMosi, 0);  /* MOSI */ 
                      GPIO_PinModeSet(gpioPortE, 6, gpioModeMiso, 0);  /* MISO */
                      GPIO_PinModeSet(gpioPortE, 4, gpioModeCs,   0);  /* CS */
                      GPIO_PinModeSet(gpioPortE, 5, gpioModeClk,  0);  /* Clock */
                      break;
              case 2: /* IO configuration (USART 0, Location #2) */
                      GPIO_PinModeSet(gpioPortC, 11, gpioModeMosi, 0); /* MOSI */
                      GPIO_PinModeSet(gpioPortC, 10, gpioModeMiso, 0); /* MISO */
                      GPIO_PinModeSet(gpioPortC,  8, gpioModeCs,   0); /* CS */
                      GPIO_PinModeSet(gpioPortC,  9, gpioModeClk,  0); /* Clock */
                      break;
            default: break;
            }
            break;
    case 1: switch(location)
            {
              case 0: /* IO configuration (USART 1, Location #0) */
                      GPIO_PinModeSet(gpioPortC, 0, gpioModeMosi, 0);  /* MOSI */
                      GPIO_PinModeSet(gpioPortC, 1, gpioModeMiso, 0);  /* MISO */
                      GPIO_PinModeSet(gpioPortB, 8, gpioModeCs,   0);  /* CS */
                      GPIO_PinModeSet(gpioPortB, 7, gpioModeClk,  0);  /* Clock */
                      break;
              case 1: /* IO configuration (USART 1, Location #1) */
                      GPIO_PinModeSet(gpioPortD, 0, gpioModeMosi, 0);  /* MOSI */
                      GPIO_PinModeSet(gpioPortD, 1, gpioModeMiso, 0);  /* MISO */
                      GPIO_PinModeSet(gpioPortD, 3, gpioModeCs,   0);  /* CS */
                      GPIO_PinModeSet(gpioPortD, 2, gpioModeClk,  0);  /* Clock */
                      break;              
            default: break;
            }
            break;
    case 2: switch(location)
            {
              case 0: /* IO configuration (USART 2, Location #0) */
                      GPIO_PinModeSet(gpioPortC, 2, gpioModeMosi, 0);  /* MOSI */
                      //GPIO_PinModeSet(gpioPortC, 3, gpioModeMiso, 0);  /* MISO */ //LCD doesn't have any output
                      GPIO_PinModeSet(gpioPortC, 5, gpioModeCs,   0);  /* CS */
                      GPIO_PinModeSet(gpioPortC, 4, gpioModeClk,  0);  /* Clock */
                      break;
              case 1: /* IO configuration (USART 2, Location #1) */
                      GPIO_PinModeSet(gpioPortB, 3, gpioModeMosi, 0);  /* MOSI */
                      GPIO_PinModeSet(gpioPortB, 4, gpioModeMiso, 0);  /* MISO */
                      GPIO_PinModeSet(gpioPortB, 6, gpioModeCs,   0);  /* CS */
                      GPIO_PinModeSet(gpioPortB, 5, gpioModeClk,  0);  /* Clock */
                      break;              
            default: break;
            }
            break;
    default: break;  
  }

  USART_InitSync(spi, &usartInit);
}



/**************************************************************************//**
 * @brief USART1 RX IRQ Handler Setup
 * @param receiveBuffer points to where to place recieved data
 * @param receiveBufferSize indicates the number of bytes to receive
 *****************************************************************************/
void SPI1_setupRXInt(char* receiveBuffer, int receiveBufferSize)
{
  USART_TypeDef *spi = USART1;

  /* Setting up pointer and indexes */
  slaveRxBuffer      = receiveBuffer;
  slaveRxBufferSize  = receiveBufferSize;
  slaveRxBufferIndex = 0;

  /* Clear RX */
  spi->CMD = USART_CMD_CLEARRX;

  /* Enable interrupts */
  NVIC_ClearPendingIRQ(USART1_RX_IRQn);
  NVIC_EnableIRQ(USART1_RX_IRQn);
  spi->IEN |= USART_IEN_RXDATAV;
}



/**************************************************************************//**
 * @brief USART1 TX IRQ Handler Setup
 * @param transmitBuffer points to the data to send
 * @param transmitBufferSize indicates the number of bytes to send
 *****************************************************************************/
void SPI1_setupTXInt(char* transmitBuffer, int transmitBufferSize)
{
  USART_TypeDef *spi = USART1;

  /* Setting up pointer and indexes */
  slaveTxBuffer      = transmitBuffer;
  slaveTxBufferSize  = transmitBufferSize;
  slaveTxBufferIndex = 0;

  /* Clear TX */
  spi->CMD = USART_CMD_CLEARTX;

  /* Enable interrupts */
  NVIC_ClearPendingIRQ(USART1_TX_IRQn);
  NVIC_EnableIRQ(USART1_TX_IRQn);
  spi->IEN |= USART_IEN_TXBL;
}



/**************************************************************************//**
 * @brief USART1 IRQ Handler Setup
 * @param receiveBuffer points to where received data is to be stored
 * @param receiveBufferSize indicates the number of bytes to receive
 * @param transmitBuffer points to the data to send
 * @param transmitBufferSize indicates the number of bytes to send
 *****************************************************************************/
void SPI1_setupSlaveInt(char* receiveBuffer, int receiveBufferSize, char* transmitBuffer, int transmitBufferSize)
{
  SPI1_setupRXInt(receiveBuffer, receiveBufferSize);
  SPI1_setupTXInt(transmitBuffer, transmitBufferSize);
}



/**************************************************************************//**
 * @brief USART1 RX IRQ Handler
 *****************************************************************************/
void USART1_RX_IRQHandler(void)
{
  USART_TypeDef *spi = USART1;
  uint8_t       rxdata;

  if (spi->STATUS & USART_STATUS_RXDATAV)
  {
    /* Reading out data */
    rxdata = spi->RXDATA;

    if (slaveRxBufferIndex < slaveRxBufferSize)
    {
      /* Store Data */
      slaveRxBuffer[slaveRxBufferIndex] = rxdata;
      slaveRxBufferIndex++;
    }
  }
}



/**************************************************************************//**
 * @brief USART1 TX IRQ Handler
 *****************************************************************************/
void USART1_TX_IRQHandler(void)
{
  USART_TypeDef *spi = USART1;

  if (spi->STATUS & USART_STATUS_TXBL)
  {
    /* Checking that valid data is to be transferred */
    if (slaveTxBuffer != 0)
    {
      /* Writing new data */
      spi->TXDATA = slaveTxBuffer[slaveTxBufferIndex];
      slaveTxBufferIndex++;
      /*Checking if more data is to be transferred */
      if (slaveTxBufferIndex == slaveTxBufferSize)
      {
        slaveTxBuffer = 0;
      }
    }
    else
    {
      /* Sending 0 if no data to send */
      spi->TXDATA = 0;
    }
  }
}

void USART1_Wait_TX_finished(void) {
  USART_TypeDef *spi = USART1;
  while(!(spi->STATUS & USART_STATUS_TXC));
}

void USART1_Wait_RX_finished(void) {
  USART_TypeDef *spi = USART1;
  while(!(spi->STATUS & USART_STATUS_RXDATAV));
}

static void sd_vol0_set_electrical_power(int power) {
  uint8_t pins_mask = 0x8;//pin = PC3
  uint8_t values_mask = power ? 0x8 : 0;

  GPIO_Mode_TypeDef gpioModeSD_EN = gpioModePushPull;
  GPIO_PinModeSet(gpioPortC, 3, gpioModeSD_EN, 0); /* init pin SD_EN to set power on/off */

  GPIO_PortOutSetVal(gpioPortC, pins_mask, values_mask);//power on/power off
}

void sd_send_data_generic(char c) {
  char txBuffer[1];
  char rxBuffer[1];
  txBuffer[0] = c;

  SPI1_setupRXInt(rxBuffer, 1);
  SPI1_setupTXInt(txBuffer, 1);

  USART1_Wait_TX_finished();
  USART1_Wait_RX_finished();
}

void sd_send_data_01010101(void) {
  char* c;
  unsigned char uc = 0b01010101;
  c = (char*) &uc;
  sd_send_data_generic(*c);
}

void sd_send_data_00001111(void) {
  char* c;
  unsigned char uc = 0b00001111;
  c = (char*) &uc;
  sd_send_data_generic(*c);
}
#endif


#define SD_SPI_CLOCK cmuClock_USART1
#define SD_SPI_BAUDRATE SPI_BAUDRATE
#define SD_SPI_UNIT        USART1
#define SD_SPI_LOCATION    1

/******************************************************************************
 * @brief sends data using USART1
 * @param txBuffer points to data to transmit
 * @param bytesToSend bytes will be sent
 *****************************************************************************/
void USART1_sendBuffer(char* txBuffer, int bytesToSend)
{
    while(bytesToSend > 0)
    {
        // Send only one byte if len==1 or data pointer is not aligned at a 16 bit
        //   word location in memory.
        if((bytesToSend == 1) || ((unsigned int)txBuffer & 0x1))
        {
            USART_Tx(SD_SPI_UNIT, *(uint8_t*)txBuffer);
            bytesToSend--;
            txBuffer++;
        }
        else
        {
            USART_TxDouble(SD_SPI_UNIT, *(uint16_t*)txBuffer);
            bytesToSend -= 2;
            txBuffer += 2;
        }
    }

    // Be sure that all transfer have finished
    while(!(SD_SPI_UNIT->STATUS & USART_STATUS_TXC));
}

static void sd_spi_init(void)
{
    USART_InitSync_TypeDef usartInit = USART_INITSYNC_DEFAULT;

  GPIO_Mode_TypeDef gpioModeMosi = gpioModePushPull;
  GPIO_Mode_TypeDef gpioModeMiso = gpioModeInput;
  GPIO_Mode_TypeDef gpioModeCs   = gpioModePushPull;
  GPIO_Mode_TypeDef gpioModeClk  = gpioModePushPull;

    CMU_ClockEnable(SD_SPI_CLOCK, true);
    usartInit.baudrate = SD_SPI_BAUDRATE;

    USART_InitSync(SD_SPI_UNIT, &usartInit);
    SD_SPI_UNIT->ROUTE = (USART_ROUTE_CLKPEN | USART_ROUTE_TXPEN | SD_SPI_LOCATION);

      GPIO_PinModeSet(gpioPortD, 0, gpioModeMosi, 0);  /* MOSI */
      GPIO_PinModeSet(gpioPortD, 1, gpioModeMiso, 0);  /* MISO */
      GPIO_PinModeSet(gpioPortD, 3, gpioModeCs,   0);  /* CS */
      GPIO_PinModeSet(gpioPortD, 2, gpioModeClk,  0);  /* Clock */
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// end of SPI functions and data //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{


    int i;

    /* Setup SysTick Timer for 1 msec interrupts  */
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    //init SPI for microSD
    //SPI_setup(1, 1, 1);
    /* Enabling clock to USART 1*/
    //CMU_ClockEnable(cmuClock_USART1, true);
    //sd_vol0_set_electrical_power(1);
	sd_spi_init();

    for (i =11; i < 13; ++i) {
        GPIO_PinModeSet(gpioPortE, i, gpioModePushPull, 0);
    }

    /* Infinite blink loop */
    while (1) {
        //GPIO_PinOutToggle(gpioPortD, 2);
        //GPIO_PinOutToggle(gpioPortD, 0);
        //GPIO_PinOutToggle(gpioPortD, 0);
        //GPIO_PinOutToggle(gpioPortD, 3);
        //GPIO_PinOutToggle(gpioPortD, 3);
		char c[2] = { 0b00001111 , 0b01010101 };
		USART1_sendBuffer(&c[0], 1);
		//USART1_sendBuffer(&c[1], 1);
		//sd_send_data_00001111();
		//sd_send_data_01010101();
        for (i = 11; i < 13; ++i) {
            GPIO_PinOutToggle(gpioPortE, i);
            //Delay(200);
        }
    }
	return 0;
}

