/*
 * Copyright (C) 2014 Julian Lewis
 * @author Tomasz Wlostowski <tomasz.wlostowski@cern.ch> 
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
 * @brief Basic drawing library.
 */

#ifndef FONT_H
#define FONT_H

#include <stdint.h>

typedef struct
{
    uint8_t min_char, max_char, height;
    const uint8_t *width_table;
    const uint16_t *offset_table;
    const uint8_t *glyph_data;
} font;

// List of available fonts
extern const font font_helv11;
extern const font font_helv17;
extern const font font_helv17b;
extern const font font_xm4x5;
extern const font font_xm4x6;

#endif /* FONT_H */