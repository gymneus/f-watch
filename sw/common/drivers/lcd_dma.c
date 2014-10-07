/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
 * @author Bartosz Bielawski <bartosz.bielawski@cern.ch>
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

#include "em_device.h"
#include "em_dma.h"
#include "em_emu.h"
#include "em_ebi.h"
#include "em_cmu.h"
#include "em_usart.h"
#include "sleep.h"
#include "dmactrl.h"
#include "lcd.h"
#include "lcd_dma.h"

/**
 * @brief Routines for DMA transfer of the frame buffer.
 */

// The DMA channel to use
#define DMA_CHANNEL     0
// Number of DMA transfers
#define DMA_TRANSFERS   3

// Framebuffer to be drawn
extern uint8_t lcd_buffer[LCD_BUF_SIZE];

// Stores pointer to the function called after transfers are finished
DMA_CB_TypeDef dma_callback;

// Configuration structures for two halves of the screen
DMA_DESCRIPTOR_TypeDef dma_cfg_block[DMA_TRANSFERS];

// Flag to indicate DMA state
volatile bool dma_transfer_active = false;

// Called by DMA when transfer is complete
static void lcd_dma_tx_complete(unsigned int channel, bool primary, void *user)
{
    (void) channel; // suppress warnings
    (void) primary;
    (void) user;

    dma_transfer_active = false;

#ifdef FREERTOS
    SLEEP_SleepBlockEnd(sleepEM2);
    xSemaphoreGive(lcd_sem);
#endif /* FREERTOS */
}

void lcd_dma_init(void)
{
    DMA_Init_TypeDef          dma_init;
    DMA_CfgChannel_TypeDef    channel_config;
    DMA_CfgDescrSGAlt_TypeDef descriptor_config;

    CMU_ClockEnable(cmuClock_DMA, true);

    dma_init.hprot = 0;
    dma_init.controlBlock = dmaControlBlock;
    DMA_Init(&dma_init);

    // Setting callback function
    dma_callback.cbFunc = lcd_dma_tx_complete;
    dma_callback.userPtr = NULL;

    // Setting up channel
    channel_config.highPri   = false;                // No high priority
    channel_config.enableInt = true;                 // Enable interrupt
    channel_config.select    = DMAREQ_USART2_TXBL;
    channel_config.cb        = &dma_callback;        // Callback routine
    DMA_CfgChannel(DMA_CHANNEL, &channel_config);

    // Setting up channel descriptor
    descriptor_config.dstInc    = dmaDataIncNone;
    descriptor_config.srcInc    = dmaDataInc1;
    descriptor_config.size      = dmaDataSize1;
    descriptor_config.arbRate   = dmaArbitrate1;
    descriptor_config.hprot     = 0;
    descriptor_config.peripheral = true;
    descriptor_config.dst       = (void *)&(LCD_SPI_UNIT->TXDATA);

    // Lines 0-42
    descriptor_config.src       = &lcd_buffer[0];
    descriptor_config.nMinus1   = LCD_STRIDE * 43 - 1;
    DMA_CfgDescrScatterGather(dma_cfg_block, 0, &descriptor_config);

    // Lines 43-85
    descriptor_config.src       = &lcd_buffer[LCD_STRIDE * 43];
    descriptor_config.nMinus1   = LCD_STRIDE * 43 - 1;
    DMA_CfgDescrScatterGather(dma_cfg_block, 1, &descriptor_config);

    // Lines 86-128
    descriptor_config.src       = &lcd_buffer[LCD_STRIDE * 86];
    descriptor_config.nMinus1   = LCD_STRIDE * 43 - 1;
    DMA_CfgDescrScatterGather(dma_cfg_block, 2, &descriptor_config);

    dma_transfer_active = false;
}

void lcd_dma_send_frame(void)
{
#ifdef FREERTOS
    if(xSemaphoreTake(lcd_sem, portMAX_DELAY) != pdTRUE)
        return;
#else
    while(dma_transfer_active);
#endif /* else FREERTOS */

    dma_transfer_active = true;
    SLEEP_SleepBlockBegin(sleepEM2);
    DMA_ActivateScatterGather(DMA_CHANNEL, true, dma_cfg_block, DMA_TRANSFERS);

    // semaphore is given back in the DMA transfer finished interrupt
}

bool lcd_dma_is_active(void)
{
    return dma_transfer_active;
}

