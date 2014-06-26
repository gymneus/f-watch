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

#ifndef DISPLAYLS013B7DH03CONFIG_H
#define DISPLAYLS013B7DH03CONFIG_H

#include "em_gpio.h"

#define SHARP_MEMLCD_DEVICE_NAME "lcd"

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

// This is hardwired
//#define LCD_PORT_EXTMODE    gpioPortC
//#define LCD_PIN_EXTMODE     1

// Select how LCD polarity inversion should be handled.
// If POLARITY_INVERSION_EXTCOMIN is defined, the EXTMODE pin is set to HIGH,
// and the polarity inversion is armed for every rising edge of the EXTCOMIN
// pin. The actual polarity inversion is triggered at the next transision of
// SCS. This mode is recommended because it causes less CPU and SPI load
// than the alternative mode, see below.
// If POLARITY_INVERSION_EXTCOMIN is undefined, the EXTMODE pin is set to
// LOW, and the polarity inversion is toggled by sending an SPI command.
// This mode causes more CPU and SPI load than using the EXTCOMIN pin mode.
#define POLARITY_INVERSION_EXTCOMIN

// Define POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE if you want the PAL
// (Platform Abstraction Layer interface) to automatically toggle the
// EXTCOMIN pin.
// If the PAL_TIMER_REPEAT function is defined the EXTCOMIN toggling is
// handled by a timer repeat system, then
// POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE should be undefined.
#define POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE

#endif /* DISPLAYLS013B7DH03CONFIG_H */

