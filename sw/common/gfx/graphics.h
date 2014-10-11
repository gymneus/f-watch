/*
 * Copyright (C) 2014 Julian Lewis
 * @author Tomasz Wlostowski <tomasz.wlostowski@cern.ch> 
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
 * @brief Basic drawing library.
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "font.h"

/*
 * @brief TODO
 */
void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t value);

/**
 * @brief TODO
 */
void box(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t value);

/**
 * @brief TODO
 */
uint8_t draw_glyph(const struct font *font, uint8_t x0, uint8_t y0, char c);

/**
 * @brief TODO
 */
void text(const struct font *font, uint8_t x, uint8_t y, const char *str);

#endif /* GRAPHICS_H */
