/*
 * Copyright (C) 2014 Julian Lewis
 * @author Xavier Piroux <xavierpiroux@gmail.com>
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
 * @brief implementation of micro SD access
 */

#include "sd.h"
#include "em_cmu.h"
#include "em_usart.h"

/* definitions of MMC/SD commands (in SPI) */
#define SD_CMD0 	(0x40 | 0)	/* GO_IDLE_STATE */
#define SD_CMD1 	(0x40 | 1)	/* SEND_OP_COND (MMC) */
#define	SD_ACMD41	(0xC0 | 41)	/* SEND_OP_COND (SDC) */
#define SD_CMD8 	(0x40 | 8)	/* SEND_IF_COND */
#define SD_CMD9 	(0x40 | 9)	/* SEND_CSD */
#define SD_CMD10	(0x40 | 10)	/* SEND_CID */
#define SD_CMD12	(0x40 | 12)	/* STOP_TRANSMISSION */
#define SD_ACMD13	(0xC0 | 13)	/* SD_STATUS (SDC) */
#define SD_CMD16	(0x40 | 16)	/* SET_BLOCKLEN */
#define SD_CMD17	(0x40 | 17)	/* READ_SINGLE_BLOCK */
#define SD_CMD18	(0x40 | 18)	/* READ_MULTIPLE_BLOCK */
#define SD_CMD23	(0x40 | 23)	/* SET_BLOCK_COUNT (MMC) */
#define	SD_ACMD23	(0xC0 | 23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define SD_CMD24	(0x40 | 24)	/* WRITE_BLOCK */
#define SD_CMD25	(0x40 | 25)	/* WRITE_MULTIPLE_BLOCK */
#define SD_CMD55	(0x40 | 55)	/* APP_CMD */
#define SD_CMD58	(0x40 | 58)	/* READ_OCR */

/**
 * @brief has to be called when doing basic sd init or when powering on/off.
 * Calling it more than once is not a problem
 */
static void sd_power_init(void) {
    static char first_time = 1;
    if (first_time) {
        GPIO_Mode_TypeDef gpioModeSD_EN  = gpioModePushPull;
        GPIO_PinModeSet(SD_PORT_POWER, SD_PIN_POWER, gpioModeSD_EN, 0); /* init pin SD_EN to set power on/off */
        first_time = 0;
    }
}
static void sd_spi_init(void) {
    USART_InitSync_TypeDef usartInit = USART_INITSYNC_DEFAULT;

    GPIO_Mode_TypeDef gpioModeMosi = gpioModePushPull;
    GPIO_Mode_TypeDef gpioModeMiso = gpioModeInput;
    GPIO_Mode_TypeDef gpioModeCs   = gpioModePushPull;
    GPIO_Mode_TypeDef gpioModeClk  = gpioModePushPull;

    //enable clocks for pins and for associated USART
    CMU_ClockEnable(SD_SPI_CLOCK, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    usartInit.baudrate = SD_SPI_LO_FREQ;
    usartInit.msbf = true;
    USART_InitSync(SD_SPI_UNIT, &usartInit);

    SD_SPI_UNIT->ROUTE = (USART_ROUTE_CLKPEN | USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_CSPEN | (SD_SPI_LOCATION << 8));

    GPIO_PinModeSet(SD_PORT_MOSI,  SD_PIN_MOSI,  gpioModeMosi, 0);  /* MOSI */
    GPIO_PinModeSet(SD_PORT_MISO,  SD_PIN_MISO,  gpioModeMiso, 0);  /* MISO */
    GPIO_PinModeSet(SD_PORT_CS_N,  SD_PIN_CS_N,  gpioModeCs,   0);  /* CS */
    GPIO_PinModeSet(SD_PORT_CLOCK, SD_PIN_CLOCK, gpioModeClk,  0);  /* Clock */

    sd_power_init();
}
//TODO: supprimer cet appel externe (permet d'ignorer le static)
void sd_spi_init_not_static(void) {
    sd_spi_init();
}
/**************************************************************************//**
* @brief Set SPI clock to a low frequency suitable for initial
* card initialization.
*****************************************************************************/
static void sd_spi_clock_slow(void) {
    USART_BaudrateSyncSet(SD_SPI_UNIT, 0, SD_SPI_LO_FREQ);
}

/**************************************************************************//**
* @brief Set SPI clock to maximum frequency.
*****************************************************************************/
static void sd_spi_clock_fast(void) {
    USART_BaudrateSyncSet(SD_SPI_UNIT, 0, SD_SPI_HI_FREQ);
}
//this function writes and reads on SPI
static uint8_t sd_spi_transfer_byte(uint8_t txData) {
    return USART_SpiTransfer(SD_SPI_UNIT, txData);
}
//TODO: supprimer cet appel externe (permet d'ignorer le static)
uint8_t sd_spi_transfer_byte_not_static(uint8_t txData) {
    return sd_spi_transfer_byte(txData);
}
//this function writes and reads (buffers must have the same length)
static void sd_spi_transfer(uint8_t* txData, uint8_t* rxData, int length) {
    int i;
    for (i = 0 ; i < length ; i++) {
        rxData[i] = sd_spi_transfer_byte(txData[i]);
    }
}
//this function writes only, but can be more efficient if bytes are aligned at 16 bits
static void sd_spi_transmit(uint8_t *txBuffer, int length) {
    while (length > 0) {
        // Send only one byte if len==1 or data pointer is not aligned at a 16 bit
        //   word location in memory.
        if((length == 1) || ((unsigned int)txBuffer & 0x1))
        {
            USART_Tx(SD_SPI_UNIT, *(uint8_t*)txBuffer);
            length--;
            txBuffer++;
        } else {
            USART_TxDouble(SD_SPI_UNIT, *(uint16_t*)txBuffer);
            length -= 2;
            txBuffer += 2;
        }
    }

    // Be sure that all transfer have finished
    while(!(SD_SPI_UNIT->STATUS & USART_STATUS_TXC));
}

void sd_power(uint8_t enable) {
    uint8_t pins_mask = 1 << SD_PIN_POWER;//pin = PC3
    uint8_t values_mask = enable ? pins_mask : 0;

    sd_power_init();

    GPIO_PortOutSetVal(SD_PORT_POWER, pins_mask, values_mask);//power on/power off
}
#define SD_INIT_TRY_MAX 10
static uint8_t sd_send_command_r1(uint8_t cmd, uint32_t arg);
uint32_t sd_init(void) {
    uint32_t res;
    int i = 0;
    sd_spi_init();
    sd_power(1);
    //TODO: //set low speed for clock
    //sd_spi_clock_slow();
   /* initializes the micro SD card */
    sd_send_command_r1(SD_CMD0, 0);
    do {
        sd_send_command_r1(SD_CMD55, 0);
        res = sd_send_command_r1(SD_ACMD41, 0);
        i++;
    } while (res == 0 || i > SD_INIT_TRY_MAX);
    if (res != 0) {
        return 0;
    }
    sd_send_block_length(SD_SECTOR_SIZE);
    //TODO: //set high speed for clock
    //sd_spi_clock_fast();
    return 1;
    //TODO: is the init finished?
}
#if 0
/**
 * @brief transform a 32 bit value into a 4 bytes array
 */
static void sd_packarg(uint8_t *dest, unsigned long src) {
    dest[3] = (uint8_t)(src >> 24);
    dest[2] = (uint8_t)(src >> 16);
    dest[1] = (uint8_t)(src >> 8);
    dest[0] = (uint8_t)(src);
}
#endif
static uint8_t sd_send_command_r1(uint8_t cmd, uint32_t arg) {
    uint8_t  n, res;
   
    /* Send command packet */
    sd_spi_transfer_byte(0x40 | cmd);            /* Start + Command index */
    sd_spi_transfer_byte((uint8_t)(arg >> 24));  /* Argument[31..24] */
    sd_spi_transfer_byte((uint8_t)(arg >> 16));  /* Argument[23..16] */
    sd_spi_transfer_byte((uint8_t)(arg >> 8));   /* Argument[15..8] */
    sd_spi_transfer_byte((uint8_t) arg);         /* Argument[7..0] */
    n = 0x01;                                    /* Dummy CRC + Stop */
    sd_spi_transfer_byte(n);
    
    /* Receive command response */
    if (cmd == SD_CMD12) {
        sd_spi_transfer_byte(0xff);              /* Skip a stuff byte when stop reading */
    }
    res = sd_spi_transfer_byte(0xff);            /* Send dummy data and check the reply */
 
    return res;                                  /* Return with the response value */
}
uint8_t sd_send_block_length(uint32_t bytes_per_sector) {
    return sd_send_command_r1(SD_CMD16, bytes_per_sector);
}

