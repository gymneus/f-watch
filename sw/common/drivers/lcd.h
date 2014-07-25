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

#ifndef LCD_H
#define LCD_H

#include "em_gpio.h"

// Dimensions
#define LCD_HEIGHT          128
#define LCD_WIDTH           128

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
#define LCD_POL_INV_FREQ    64

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
void lcd_set_pixel(uint8_t x, uint8_t y, uint8_t value);

/**
 * @brief Toggles a single pixel.
 * @param uint8_t x is the x coordinate of the pixel.
 * @param uint8_t y is the y coordinate of the pixel.
 */
void lcd_toggle_pixel(uint8_t x, uint8_t y);

/**
 * @brief Returns the state of a single pixel.
 * @param uint8_t x is the x coordinate of the pixel.
 * @param uint8_t y is the y coordinate of the pixel.
 */
uint8_t lcd_get_pixel(uint8_t x, uint8_t y);

#endif /* LCD_H */

