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
 * http://www.gnu.org/licenses/gpl-3.0-standalone.html
 * or you may search the http://www.gnu.org website for the version 3 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @brief LS013B7DH03 LCD driver
 */
// Uncomment if you do not want to use DMA for frame transfer
//#define LCD_NODMA

#include "lcd.h"
#ifndef LCD_NODMA
#include "lcd_dma.h"
#endif
#include <em_cmu.h>
#include <em_usart.h>
#include <em_rtc.h>
#include <em_timer.h>

#ifdef FREERTOS
xSemaphoreHandle lcd_sem;
#endif /* FREERTOS */

// Frame buffer - pixels are stored as consecutive rows
uint8_t lcd_buffer[LCD_BUF_SIZE];
uint8_t * const off_buffer = lcd_buffer + LCD_CONTROL_BYTES;

static void spi_init(void)
{
    CMU_ClockEnable(LCD_SPI_CLOCK, true);

    USART_InitSync_TypeDef usartInit = USART_INITSYNC_DEFAULT;
    usartInit.databits = usartDatabits8;
    usartInit.baudrate = LCD_SPI_BAUDRATE;
    USART_InitSync(LCD_SPI_UNIT, &usartInit);

    /*LCD_SPI_UNIT->CTRL |= USART_CTRL_AUTOCS;*/

    LCD_SPI_UNIT->ROUTE = (USART_ROUTE_CLKPEN | USART_ROUTE_TXPEN | LCD_SPI_LOCATION);
    /*USART_ROUTE_CSPEN*/
}

static void spi_transmit(uint8_t *data, uint16_t length)
{
    while(length > 0)
    {
        // Send only one byte if len==1 or data pointer is not aligned at a 16 bit
        // word location in memory.
        if((length == 1) || ((unsigned int)data & 0x1))
        {
            USART_Tx(LCD_SPI_UNIT, *(uint8_t*)data);
            length--;
            data++;
        }
        else
        {
            USART_TxDouble(LCD_SPI_UNIT, *(uint16_t*)data);
            length -= 2;
            data += 2;
        }
    }

    // Be sure that all transfer have finished
    while(!(LCD_SPI_UNIT->STATUS & USART_STATUS_TXC));
}

static void extcomin_setup(unsigned int frequency)
{
    CMU_ClockEnable(cmuClock_TIMER0, true);

    // Select CC channel parameters
    TIMER_InitCC_TypeDef timerCCInit =
    {
        .cufoa      = timerOutputActionNone,
        .cofoa      = timerOutputActionToggle,
        .cmoa       = timerOutputActionNone,
        .mode       = timerCCModeCompare,
        .filter     = false,
        .prsInput   = false,
        .coist      = false,
        .outInvert  = false,
    };

    // Configure CC channel
    TIMER_InitCC(TIMER0, 2, &timerCCInit);

    // Route CC2 to location 1 (PE12) and enable pin
    TIMER0->ROUTE |= (TIMER_ROUTE_CC2PEN | TIMER_ROUTE_LOCATION_LOC0);

    // Set compare value starting at 0 - it will be incremented in the interrupt handler
    TIMER_CompareBufSet(TIMER0, 2, 0);

    // Set Top Value
    TIMER_TopSet(TIMER0, (CMU_ClockFreqGet(cmuClock_HFPER) / 16)/ frequency);

    // Select timer parameters
    TIMER_Init_TypeDef timerInit =
    {
        .enable     = true,
        .debugRun   = true,
        .prescale   = timerPrescale8,
        .clkSel     = timerClkSelHFPerClk,
        .fallAction = timerInputActionNone,
        .riseAction = timerInputActionNone,
        .mode       = timerModeUp,
        .dmaClrAct  = false,
        .quadModeX4 = false,
        .oneShot    = false,
        .sync       = false,
    };

    // Configure timer
    TIMER_Init(TIMER0, &timerInit);
}

static void lcd_prepend_update_commands()
{
    // Add control codes for all lines
    uint16_t i;
    for(i = 0; i < LCD_HEIGHT; ++i)
    {
        lcd_buffer[i * LCD_STRIDE]     = LCD_CMD_UPDATE;
        lcd_buffer[i * LCD_STRIDE + 1] = i + 1;
    }

    // Ending control command
    lcd_buffer[LCD_HEIGHT * LCD_STRIDE]     = 0xff;
    lcd_buffer[LCD_HEIGHT * LCD_STRIDE + 1] = 0xff;
}

void lcd_init(void)
{
    uint16_t cmd;

#ifdef FREERTOS
    lcd_sem = xSemaphoreCreateMutex();
    if(lcd_sem == NULL) {
        // TODO oops..
    }
#endif /* FREERTOS */

    spi_init();
    // TODO I am pretty sure, it will be already initialized somewhere..
    CMU_ClockEnable(cmuClock_GPIO, true);

    // Setup IOs
    GPIO_PinModeSet(LCD_PORT_SCLK,     LCD_PIN_SCLK,     gpioModePushPull, 0);
    GPIO_PinModeSet(LCD_PORT_SI,       LCD_PIN_SI,       gpioModePushPull, 0);
    GPIO_PinModeSet(LCD_PORT_SCS,      LCD_PIN_SCS,      gpioModePushPull, 0);
    GPIO_PinModeSet(LCD_PORT_DISP_SEL, LCD_PIN_DISP_SEL, gpioModePushPull, 0);
    GPIO_PinModeSet(LCD_PORT_DISP_PWR, LCD_PIN_DISP_PWR, gpioModePushPull, 0);
    GPIO_PinModeSet(LCD_PORT_EXTCOMIN, LCD_PIN_EXTCOMIN, gpioModePushPull, 0);
    // EXTMODE is hardwired
    // GPIO_PinModeSet(LCD_PORT_EXTMODE, LCD_PIN_EXTMODE, gpioModePushPull, 0);

    // Setup timer to generate interrupts at given frequency for EXTCOMIN pin
    extcomin_setup(LCD_POL_INV_FREQ);

    lcd_power(1);

    // Send command to clear the display
    GPIO_PinOutSet(LCD_PORT_SCS, LCD_PIN_SCS);

    cmd = LCD_CMD_ALL_CLEAR;
    spi_transmit((uint8_t*) &cmd, 2);

    GPIO_PinOutClear(LCD_PORT_SCS, LCD_PIN_SCS);

    lcd_clear();
    lcd_prepend_update_commands();

#ifndef LCD_NODMA
    lcd_dma_init();
#endif /* LCD_NODMA */

    lcd_enable(1);
}

void lcd_enable(uint8_t enable)
{
    if(enable)
        GPIO_PinOutSet(LCD_PORT_DISP_SEL, LCD_PIN_DISP_SEL);
    else
        GPIO_PinOutClear(LCD_PORT_DISP_SEL, LCD_PIN_DISP_SEL);
}

void lcd_power(uint8_t enable)
{
    if(enable)
        GPIO_PinOutSet(LCD_PORT_DISP_PWR, LCD_PIN_DISP_PWR);
    else
        GPIO_PinOutClear(LCD_PORT_DISP_PWR, LCD_PIN_DISP_PWR);
}

void lcd_clear(void)
{
    // Using uint32_t instead of uint8_t reduces the number of writes 4 times
    uint32_t *p = (uint32_t*)lcd_buffer;
    uint16_t x, y;

#ifdef FREERTOS
    if(xSemaphoreTake(lcd_sem, portMAX_DELAY) != pdTRUE)
        return;
#endif /* FREERTOS */

    for(y = 0; y < LCD_HEIGHT; ++y) {
        // skip control bytes
        p = (uint32_t*)((uint8_t*)p + 2);

        // clear line
        for(x = 0; x < LCD_STRIDE / sizeof(uint32_t); ++x) {
            *p++ = 0x00;
        }
    }

#ifdef FREERTOS
    xSemaphoreGive(lcd_sem);
#endif /* FREERTOS */
}

void lcd_update(void)
{
    GPIO_PinOutSet(LCD_PORT_SCS, LCD_PIN_SCS);

#ifdef LCD_NODMA
    spi_transmit(lcd_buffer, LCD_BUF_SIZE);
    GPIO_PinOutClear(LCD_PORT_SCS, LCD_PIN_SCS);
#else
    lcd_dma_send_frame();
    // chip select is deasserted in the dma tx complete interrupt
#endif
}

