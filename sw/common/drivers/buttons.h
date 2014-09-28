/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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

/**
 * @brief Button routines
 */

#ifndef BUTTONS_H
#define BUTTONS_H

// Top left button
#define BUT_TL_PORT     gpioPortA
#define BUT_TL_PIN      8
// Top right button
#define BUT_TR_PORT     gpioPortC
#define BUT_TR_PIN      6
// Bottom left button
#define BUT_BL_PORT     gpioPortA
#define BUT_BL_PIN      0
// Bottom right button
#define BUT_BR_PORT     gpioPortC
#define BUT_BR_PIN      7

/**
 * @brief Iniailize button IOs & interrupts.
 */
void buttons_init(void);

#endif /* BUTTONS_H */

