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

#ifndef LCD_H
#define LCD_H

#include "em_gpio.h"

// Dimensions
#define LCD_HEIGHT          128
#define LCD_WIDTH           128

// Enable 90* rotation
#define LCD_ROTATE_90

// Pinout
#define LCD_PORT_SCLK       gpioPortC
#define LCD_PIN_SCLK        4
#define LCD_PORT_SI         gpioPortC
#define LCD_PIN_SI          2
#define LCD_PORT_SCS        gpioPortC
#define LCD_PIN_SCS         5
#define LCD_PORT_EXTCOMIN   gpioPortA
#define LCD_PIN_EXTCOMIN    2
#define LCD_PORT_DISP_SEL   gpioPortC
#define LCD_PIN_DISP_SEL    0
#define LCD_PORT_DISP_PWR   gpioPortC
#define LCD_PIN_DISP_PWR    1

// Types of SPI commands
#define LCD_CMD_UPDATE      0x01
#define LCD_CMD_ALL_CLEAR   0x04

// Peripherals
#define LCD_SPI_UNIT        USART2
#define LCD_SPI_CLOCK       cmuClock_USART2
#define LCD_SPI_LOCATION    0
#define LCD_SPI_BAUDRATE    500000
#define LCD_POL_INV_FREQ    60

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

/**
 * @brief LCD initialization routine.
 */
void lcd_init(void);

/**
 * @brief Controls DISP_EN pin - it allows to turn off the display, but preserve the memory.
 * @param uint8_t enable enables the display if greater than 0.
 */
void lcd_enable(uint8_t enable);

/**
 * @brief Controls power for the display.
 * @param uint8_t enable turns on the power if greater than 0.
 */
void lcd_power(uint8_t enable);

/**
 * @brief Clears the LCD and the framebuffer.
 */
void lcd_clear(void);

/**
 * @brief Refreshes the screen with the framebuffer contents.
 */
void lcd_update(void);

/**
 * @brief Switches on/off a single pixel.
 * @param uint8_t x is the x coordinate of the pixel.
 * @param uint8_t y is the y coordinate of the pixel.
 * @param uint8_t value turns off the pixel if 0, turns on otherwise.
 */
static inline void lcd_set_pixel(uint8_t x, uint8_t y, uint8_t value)
{
    extern uint8_t lcd_buffer[];

    // x %= LCD_WIDTH;
    // y %= LCD_HEIGHT;

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
        lcd_buffer[offset] |= mask;
    else
        lcd_buffer[offset] &= ~mask;
}

/**
 * @brief Toggles a single pixel.
 * @param uint8_t x is the x coordinate of the pixel.
 * @param uint8_t y is the y coordinate of the pixel.
 */
static inline void lcd_toggle_pixel(uint8_t x, uint8_t y)
{
    extern uint8_t lcd_buffer[];

    // x %= LCD_WIDTH;
    // y %= LCD_HEIGHT;

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

    lcd_buffer[offset] ^= mask;
}

/**
 * @brief Returns the state of a single pixel.
 * @param uint8_t x is the x coordinate of the pixel.
 * @param uint8_t y is the y coordinate of the pixel.
 */
static inline uint8_t lcd_get_pixel(uint8_t x, uint8_t y)
{
    extern uint8_t lcd_buffer[];

    // x %= LCD_WIDTH;
    // y %= LCD_HEIGHT;

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

    return lcd_buffer[offset] & mask;
}

#endif /* LCD_H */

