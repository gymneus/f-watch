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
#include <em_timer.h>
#include <udelay.h>

// Enable 90* rotation
//#define LCD_ROTATE_90

// Do not use DMA for frame transfer
#define LCD_NODMA

// Additional bytes to control the LCD; required for DMA transfers
#ifdef LCD_NODMA
#define CONTROL_BYTES       0
#else
#define CONTROL_BYTES       2
#endif

// Number of bytes to store one line
#define LCD_STRIDE          (LCD_WIDTH / 8 + CONTROL_BYTES)

// Framebuffer - pixels are stored as consecutive rows
static uint8_t buffer[LCD_STRIDE * LCD_HEIGHT];

static void spi_init(void)
{
    USART_InitSync_TypeDef usartInit = USART_INITSYNC_DEFAULT;

    CMU_ClockEnable(LCD_SPI_CLOCK, true);
    usartInit.baudrate = LCD_SPI_BAUDRATE;

    USART_InitSync(LCD_SPI_UNIT, &usartInit);
    LCD_SPI_UNIT->ROUTE = (USART_ROUTE_CLKPEN | USART_ROUTE_TXPEN | LCD_SPI_LOCATION);
}

static void spi_transmit(uint8_t *data, uint16_t length)
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

void lcd_init(void)
{
    uint16_t cmd;

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

    // Setup timer to generate interrupts at given frequency for EXTCOMIN pin
    extcomin_setup(LCD_POL_INV_FREQ);

    lcd_power(1);

    // Send command to clear the display
    GPIO_PinOutSet(LCD_PORT_SCS, LCD_PIN_SCS);
    timer_delay(6);

    cmd = LCD_CMD_ALL_CLEAR;
    spi_transmit((uint8_t*) &cmd, 2);

    timer_delay(2);
    GPIO_PinOutClear(LCD_PORT_SCS, LCD_PIN_SCS);

    lcd_clear();
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
    uint32_t *p = (uint32_t*)buffer;
    uint16_t i;

    // Clear pixel buffer
    for(i = 0; i < sizeof(buffer) / sizeof(uint32_t); ++i)
        *p++ = 0x00;

#ifndef LCD_NODMA
    // Add control codes
    for(i = 1; i < LCD_HEIGHT; ++i)
    {
        buffer[i * LCD_STRIDE - 2] = 0xff;      // Dummy
        buffer[i * LCD_STRIDE - 1] = (i + 1);   // Address of next line
    }
#endif
}

void lcd_update(void)
{
    // Need to adjust start row by one because LS013B7DH03 starts counting lines
    // from 1, while the DISPLAY interface starts from 0.
    const uint8_t START_ROW = 1;

    // TODO use DMA
    uint16_t cmd;
    uint16_t i;
    uint8_t *p = (uint8_t*) buffer;

    GPIO_PinOutSet(LCD_PORT_SCS, LCD_PIN_SCS);
    timer_delay(6);

    // Send update command and first line address
    cmd = LCD_CMD_UPDATE | (START_ROW << 8);
    spi_transmit((uint8_t*) &cmd, 2);

#ifdef LCD_NODMA
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
#else
    // TODO here the DMA transfer should run in the end
    spi_transmit(p, LCD_STRIDE * LCD_HEIGHT);
#endif

    timer_delay(2);
    GPIO_PinOutClear(LCD_PORT_SCS, LCD_PIN_SCS);
}

void lcd_set_pixel(uint8_t x, uint8_t y, uint8_t value)
{
    x %= LCD_WIDTH;
    y %= LCD_HEIGHT;

#if defined(LCD_ROTATE_90)
    uint8_t mask = 0x80 >> (y & 0x07);
    uint16_t offset = (x * LCD_STRIDE) + ((LCD_HEIGHT - 1 - y) >> 3);
#elif defined(LCD_ROTATE_270)
    uint8_t mask = 1 << (y & 0x07);
    uint16_t offset = ((LCD_WIDTH - x - 1) * LCD_STRIDE) + (y >> 3);
#else
    uint8_t mask = 1 << (x & 0x07);                 // == 1 << (x % 8)
    uint16_t offset = (y * LCD_STRIDE) + (x >> 3);  // == y * LCD_STRIDE + x / 8
#endif

    if(value)
        buffer[offset] |= mask;
    else
        buffer[offset] &= ~mask;
}

void lcd_toggle_pixel(uint8_t x, uint8_t y)
{
    x %= LCD_WIDTH;
    y %= LCD_HEIGHT;

#if defined(LCD_ROTATE_90)
    uint8_t mask = 0x80 >> (y & 0x07);
    uint16_t offset = (x * LCD_STRIDE) + ((LCD_HEIGHT - 1 - y) >> 3);
#elif defined(LCD_ROTATE_270)
    uint8_t mask = 1 << (y & 0x07);
    uint16_t offset = ((LCD_WIDTH - x - 1) * LCD_STRIDE) + (y >> 3);
#else
    uint8_t mask = 1 << (x & 0x07);                 // == 1 << (x % 8)
    uint16_t offset = (y * LCD_STRIDE) + (x >> 3);  // == y * LCD_STRIDE + x / 8
#endif

    buffer[offset] ^= mask;
}

uint8_t lcd_get_pixel(uint8_t x, uint8_t y)
{
    x %= LCD_WIDTH;
    y %= LCD_HEIGHT;

#if defined(LCD_ROTATE_90)
    uint8_t mask = 0x80 >> (y & 0x07);
    uint16_t offset = (x * LCD_STRIDE) + ((LCD_HEIGHT - 1 - y) >> 3);
#elif defined(LCD_ROTATE_270)
    uint8_t mask = 1 << (y & 0x07);
    uint16_t offset = ((LCD_WIDTH - x - 1) * LCD_STRIDE) + (y >> 3);
#else
    uint8_t mask = 1 << (x & 0x07);                 // == 1 << (x % 8)
    uint16_t offset = (y * LCD_STRIDE) + (x >> 3);  // == y * LCD_STRIDE + x / 8
#endif

    return buffer[offset] & mask;
}

