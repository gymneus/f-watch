/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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
 * @brief LS013B7DH03 LCD driver
 */

#include "lcd.h"
#include <em_cmu.h>
#include <em_usart.h>
#include <em_rtc.h>
#include <udelay.h>

// Framebuffer - pixels are stored as consecutive rows
static uint8_t buffer[LCD_WIDTH / 8 * LCD_HEIGHT];

static void spi_init(void)
{
    USART_InitSync_TypeDef usartInit = USART_INITSYNC_DEFAULT;

    CMU_ClockEnable(LCD_SPI_CLOCK, true);
    usartInit.baudrate = LCD_SPI_BAUDRATE;

    USART_InitSync(LCD_SPI_UNIT, &usartInit);
    LCD_SPI_UNIT->ROUTE = (USART_ROUTE_CLKPEN | USART_ROUTE_TXPEN | LCD_SPI_LOCATION);
}

static void spi_transmit(uint8_t *data, uint8_t length)
{
    while(length > 0)
    {
        // Send only one byte if len==1 or data pointer is not aligned at a 16 bit
        //   word location in memory.
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

static void timer_init(void)
{
    UDELAY_Calibrate();
}

static void timer_delay(uint16_t usecs)
{
    UDELAY_Delay(usecs);
}

static void rtc_setup(unsigned int frequency)
{
    RTC_Init_TypeDef rtc_init = RTC_INIT_DEFAULT;

    // Enable LE domain registers
    if(!(CMU->HFCORECLKEN0 & CMU_HFCORECLKEN0_LE))
        CMU_ClockEnable(cmuClock_CORELE, true);

    if(cmuSelect_LFXO != CMU_ClockSelectGet(cmuClock_LFA))
        CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

    CMU_ClockDivSet(cmuClock_RTC, cmuClkDiv_2);
    CMU_ClockEnable(cmuClock_RTC, true);

    // Initialize RTC
    rtc_init.enable   = false;  // Do not start RTC after initialization is complete
    rtc_init.debugRun = false;  // Halt RTC when debugging
    rtc_init.comp0Top = true;   // Wrap around on COMP0 match
    RTC_Init(&rtc_init);

    RTC_CompareSet(0, (CMU_ClockFreqGet(cmuClock_RTC) / frequency) - 1);

    NVIC_EnableIRQ(RTC_IRQn);
    RTC_IntEnable(RTC_IEN_COMP0);

    RTC_Enable(true);
}

void RTC_IRQHandler(void)
{
    RTC_IntClear(RTC_IF_COMP0);
    GPIO_PinOutToggle(LCD_PORT_EXTCOMIN, LCD_PIN_EXTCOMIN);
}

void lcd_init(void)
{
    timer_init();
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

    // Setup RTC to generate interrupts at given frequency
    rtc_setup(LCD_POL_INV_FREQ);

    lcd_power(1);
    lcd_enable(1);

    lcd_clear();
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
    uint16_t cmd, i;

    // Clear pixel buffer
    for(i = 0; i < sizeof(buffer); ++i)
        buffer[i] = 0x00;

    // Send command to clear the display
    GPIO_PinOutSet(LCD_PORT_SCS, LCD_PIN_SCS);
    timer_delay(6);

    cmd = LCD_CMD_ALL_CLEAR;
    spi_transmit((uint8_t*) &cmd, 2);

    timer_delay(2);
    GPIO_PinOutClear(LCD_PORT_SCS, LCD_PIN_SCS);
}

void lcd_update(void)
{
    // Need to adjust start row by one because LS013B7DH03 starts counting lines
    // from 1, while the DISPLAY interface starts from 0.
    const uint8_t START_ROW = 1;

    // TODO use DMA
    uint16_t cmd;
    uint8_t i;
    uint8_t *p = (uint8_t*) buffer;

    GPIO_PinOutSet(LCD_PORT_SCS, LCD_PIN_SCS);
    timer_delay(6);

    // Send update command and first line address
    cmd = LCD_CMD_UPDATE | (START_ROW << 8);
    spi_transmit((uint8_t*) &cmd, 2);

    for(i = 0; i < LCD_HEIGHT; ++i)
    {
        // Send pixels for this line
        spi_transmit(p, LCD_WIDTH / 8);
        p += (LCD_WIDTH / 8);

        // TODO seems unnecessary
//        if(i == LCD_HEIGHT - 1)
//            cmd = 0xffff;
//        else
        cmd = 0xff | ((START_ROW + i + 1) << 8);

        spi_transmit((uint8_t*) &cmd, 2);
    }

    timer_delay(2);
    GPIO_PinOutClear(LCD_PORT_SCS, LCD_PIN_SCS);
}

void lcd_set_pixel(uint8_t x, uint8_t y, uint8_t value)
{
    uint8_t mask = 1 << (x & 0x07);                     // == 1 << (x % 8)
    uint16_t offset = (y * LCD_WIDTH >> 3) + (x >> 3);  // == (y * LCD_WIDTH / 8) + x / 8

    if(value)
        buffer[offset] |= mask;
    else
        buffer[offset] &= ~mask;
}

void lcd_toggle_pixel(uint8_t x, uint8_t y)
{
    uint8_t mask = 1 << (x & 0x07);                     // == 1 << (x % 8)
    uint16_t offset = (y * LCD_WIDTH >> 3) + (x >> 3);  // == (y * LCD_WIDTH / 8) + x / 8

    buffer[offset] ^= mask;
}

uint8_t lcd_get_pixel(uint8_t x, uint8_t y)
{
    uint8_t mask = 1 << (x & 0x07);                     // == 1 << (x % 8)
    uint16_t offset = (y * LCD_WIDTH >> 3) + (x >> 3);  // == (y * LCD_WIDTH / 8) + x / 8

    return buffer[offset] & mask;
}

