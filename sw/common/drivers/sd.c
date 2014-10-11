/*
 * Copyright (C) 2014 Julian Lewis
 * @author Xavier Piroux <xavierpiroux@gmail.com>
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
 * @brief implementation of micro SD access
 */

#include "sd.h"
#include "em_cmu.h"
#include "em_usart.h"

#define SD_MASK_IDLE        0x01

/* definitions of MMC/SD commands (in SPI) */

//ACMD<n> means a command sequence of CMD55-CMD<n>

#define SD_CMD0 	(0x40 | 0)	/* GO_IDLE_STATE = reset */
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

#define SD_TOKEN_START_READ  0xFE
#define SD_TOKEN_START_WRITE 0xFE

//functions definition
static void sd_send_dummy_bytes(uint8_t count);
static uint8_t sd_send_command_r1(uint8_t cmd, uint32_t arg);
static uint8_t sd_send_command_r3(uint8_t cmd, uint32_t arg, uint32_t* buffer);

static char is_sd_power_init = 0;
/**
 * @brief has to be called in sd_init() or sd_power()
 * Calling it more than once is NOT a problem
 */
static void sd_power_init(void) {
    if (is_sd_power_init == 0) {
        GPIO_Mode_TypeDef gpioModeSD_EN  = gpioModePushPull;
        GPIO_PinModeSet(SD_PORT_POWER, SD_PIN_POWER, gpioModeSD_EN, 0); /* init pin SD_EN to set power on/off */
        is_sd_power_init = 1;
    }
}
static void sd_power_finish(void) {
    GPIO_PinModeSet(SD_PORT_POWER, SD_PIN_POWER, gpioModeDisabled, 0);
    is_sd_power_init = 0;
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

    usartInit.baudrate = SD_SPI_LO_FREQ; //starting with low frequency is required by specs
    usartInit.msbf = true;
    USART_InitSync(SD_SPI_UNIT, &usartInit);

    SD_SPI_UNIT->ROUTE = (USART_ROUTE_CLKPEN | USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_CSPEN | (SD_SPI_LOCATION << 8));

    GPIO_PinModeSet(SD_PORT_MOSI,  SD_PIN_MOSI,  gpioModeMosi, 0);  /* MOSI */
    GPIO_PinModeSet(SD_PORT_MISO,  SD_PIN_MISO,  gpioModeMiso, 0);  /* MISO */
    GPIO_PinModeSet(SD_PORT_CS_N,  SD_PIN_CS_N,  gpioModeCs,   0);  /* CS */
    GPIO_PinModeSet(SD_PORT_CLOCK, SD_PIN_CLOCK, gpioModeClk,  0);  /* Clock */

    sd_power_init();
}
static void sd_spi_finish(void) {
    USART_Reset(SD_SPI_UNIT);
    GPIO_PinModeSet(SD_PORT_MOSI,  SD_PIN_MOSI,  gpioModeDisabled, 0);  /* MOSI */
    GPIO_PinModeSet(SD_PORT_MISO,  SD_PIN_MISO,  gpioModeDisabled, 0);  /* MISO */
    GPIO_PinModeSet(SD_PORT_CS_N,  SD_PIN_CS_N,  gpioModeDisabled,   0);  /* CS */
    GPIO_PinModeSet(SD_PORT_CLOCK, SD_PIN_CLOCK, gpioModeDisabled,  0);  /* Clock */
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
/**
 * @brief send several times a dummy character (the protocol requires this often)
 * @param count number of dummy byte to transfer
 */
static void sd_send_dummy_bytes(uint8_t count) {
    while (count) {
        sd_spi_transfer_byte(0xFF);
        count--;
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
extern uint32_t guiTracerSD;//TODO:remove
#define SD_INIT_SEND_DUMMY_COUNT 20
#define SD_INIT_TRY_MAX 10
uint32_t sd_init(void) {
    uint32_t res;
    int i = 0;
    guiTracerSD = 0;//TODO:remove
    sd_spi_init();
    guiTracerSD = 10;//TODO:remove
    sd_power(1);
    guiTracerSD = 20;//TODO:remove

    /* initializes the micro SD card */

    //set low speed for clock
    sd_spi_clock_slow();
    guiTracerSD = 30;//TODO:remove

    /* From the specs, the SD might take until 74 clocks sent by the master to init */
    sd_send_dummy_bytes(SD_INIT_SEND_DUMMY_COUNT);
    guiTracerSD = 40;//TODO:remove

    /* go to idle state */
    sd_send_command_r1(SD_CMD0, 0);
    guiTracerSD = 50;//TODO:remove
    i = 0;
    do {
        /* wait the card to be really in idle state */
        res = sd_send_command_r1(SD_CMD55, 0);
        if ((res & SD_MASK_IDLE) != SD_MASK_IDLE) {
            guiTracerSD = 55;//TODO:remove
            //at that point, state is expected to be idle
            return 0;
        }
        res = sd_send_command_r1(SD_ACMD41, 0);
        i++;
    } while ((res & SD_MASK_IDLE) == SD_MASK_IDLE && i < SD_INIT_TRY_MAX);
    guiTracerSD = 60;//TODO:remove
    if (res != 0) {
        return 0;
    }
    res = sd_send_block_length(SD_SECTOR_SIZE);
    guiTracerSD = 70;//TODO:remove
    if (res != 0) {
        return 0;
    }
    //set high speed for clock
    sd_spi_clock_fast();
    guiTracerSD = 80;//TODO:remove
    return 1;
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
/**
 * @brief send a command to the microSD.
 * @param cmd command code to send
 * @param arg 32-bit argument to pass to the SD
 * @return reply (R1) from the SD
 */
static uint8_t sd_send_command_r1(uint8_t cmd, uint32_t arg) {
    uint8_t  n, res;

    cmd &= 0x7F;
    cmd |= 0x40;
   
    /* Send command packet */
    sd_spi_transfer_byte(cmd);                   /* Start + Command index */
    sd_spi_transfer_byte((uint8_t)(arg >> 24));  /* Argument[31..24] */
    sd_spi_transfer_byte((uint8_t)(arg >> 16));  /* Argument[23..16] */
    sd_spi_transfer_byte((uint8_t)(arg >> 8));   /* Argument[15..8] */
    sd_spi_transfer_byte((uint8_t) arg);         /* Argument[7..0] */
    n = 0x01;                                    /* Dummy CRC = 1 , except for a few commands */
    if (cmd == SD_CMD0)
        n = 0x95;                                /* CRC of CMD0 is mandatory and the most important */
    sd_spi_transfer_byte(n);
    
    res = sd_spi_transfer_byte(0xff);            /* Send dummy data and check the reply */
 
    return res;                                  /* Return with the response value */
}
/**
 * @brief send a command to the microSD.
 * @param cmd command code to send
 * @param arg 32-bit argument to pass to the SD
 * @param buffer filled with bytes 1..4 from the SD
 * @return byte 0 (R1) from the SD
 */
static uint8_t sd_send_command_r3(uint8_t cmd, uint32_t arg, uint32_t* buffer) {
    uint8_t  n, res;
    int i;

    cmd &= 0x7F;
    cmd |= 0x40;
   
    /* Send command packet */
    sd_spi_transfer_byte(cmd);                   /* Start + Command index */
    sd_spi_transfer_byte((uint8_t)(arg >> 24));  /* Argument[31..24] */
    sd_spi_transfer_byte((uint8_t)(arg >> 16));  /* Argument[23..16] */
    sd_spi_transfer_byte((uint8_t)(arg >> 8));   /* Argument[15..8] */
    sd_spi_transfer_byte((uint8_t) arg);         /* Argument[7..0] */
    n = 0x01;                                    /* Dummy CRC = 1 , except for a few commands */
    sd_spi_transfer_byte(n);
    
    res = sd_spi_transfer_byte(0xff);            /* Send dummy data and check the reply */
    for (i = 0 ; i < 4 ; i++) {
        buffer[i] = sd_spi_transfer_byte(0xFF);
    }
 
    return res;                                  /* Return with the response value */
}
uint8_t sd_send_block_length(uint32_t bytes_per_sector) {
    if (sd_send_command_r1(SD_CMD16, bytes_per_sector) != 0) {
        return 1;
    }
    return 0;
}
uint8_t sd_read_ocr(uint8_t* buffer) {
    if (sd_send_command_r3(SD_CMD58, 0, buffer) != 0) {
        return 1;
    }
    return 0;
}
int sd_write_block(uint32_t block_addr, const uint8_t *txBuffer) {
    uint8_t res;
    uint32_t byte_count = SD_SECTOR_SIZE;

    res = sd_send_command_r1(SD_CMD24, block_addr);
    if (res != 0) {
        return 0;
    }

    /* send a dummy token to let time to SD before starting sending data */
    sd_send_dummy_bytes(1);
    /* signal telling the SD we start writing */
    sd_spi_transfer_byte(SD_TOKEN_START_WRITE);
    do {
        /* send the 512 byte data block to the SD */
        sd_spi_transfer_byte(*txBuffer++);
        sd_spi_transfer_byte(*txBuffer++);
        sd_spi_transfer_byte(*txBuffer++);
        sd_spi_transfer_byte(*txBuffer++);
        byte_count -= 4;
    } while (byte_count);

    sd_spi_transfer_byte(0xFF); /* CRC (Dummy) */
    sd_spi_transfer_byte(0xFF);
    res = sd_spi_transfer_byte(0xFF); /* Receive a data response */
    if ((res & 0x1F) != 0x05) { /* If not accepted, return with error */
        return 0;
    }
    return 1;
}
#define SD_READ_BLOCK_WAIT_MAX 20
int sd_read_block(uint32_t block_addr, uint8_t *rxBuffer) {
    uint8_t res;
    uint16_t val16;
    uint32_t byte_count = SD_SECTOR_SIZE;
    uint32_t retryCount, framectrl, ctrl;

    res = sd_send_command_r1(SD_CMD17, block_addr);
    if (res != 0) {
        return 0;
    }
    //TODO: continuer
    retryCount = SD_READ_BLOCK_WAIT_MAX;
    do {
        res = sd_spi_transfer_byte(SD_TOKEN_START_READ);
    } while (res == 0xFF && --retryCount);
    if (res != SD_TOKEN_START_READ) {
        /* unexpected data sent by SD, exiting with error status */
        return 0;
    }

    /* Save current configuration. */
    framectrl = SD_SPI_UNIT->FRAME;
    ctrl      = SD_SPI_UNIT->CTRL;

    /* Set frame length to 16 bit. This will increase the effective data rate. */
    SD_SPI_UNIT->FRAME = (SD_SPI_UNIT->FRAME & (~_USART_FRAME_DATABITS_MASK))
                         | USART_FRAME_DATABITS_SIXTEEN;
    SD_SPI_UNIT->CTRL |= USART_CTRL_BYTESWAP;

    /* Clear send and receive buffers. */
    SD_SPI_UNIT->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;

    /* Pipelining - The USART has two buffers of 16 bit in both
     * directions. Make sure that at least one is in the pipe at all
     * times to maximize throughput. */
    SD_SPI_UNIT->TXDOUBLE = 0xffff;
    do {
        SD_SPI_UNIT->TXDOUBLE = 0xffff;

        while (!(SD_SPI_UNIT->STATUS & USART_STATUS_RXDATAV));

        val16 = SD_SPI_UNIT->RXDOUBLE;
        *rxBuffer++ = val16;
        *rxBuffer++ = val16 >> 8;

        byte_count -= 2;
    } while (byte_count);

    /* Next two bytes is the CRC which we discard. */
    while (!(SD_SPI_UNIT->STATUS & USART_STATUS_RXDATAV));
    SD_SPI_UNIT->RXDOUBLE;

    /* Restore old settings. */
    SD_SPI_UNIT->FRAME = framectrl;
    SD_SPI_UNIT->CTRL  = ctrl;

    return 1;     /* Return with success */
}

