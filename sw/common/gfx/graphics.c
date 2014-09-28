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
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @brief Basic drawing library.
 */

#include "graphics.h"
#include <drivers/lcd.h>
#include <stdlib.h>

void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t value)
{
    int8_t dx = abs(x1 - x0);
    int8_t sx = x0 < x1 ? 1 : -1;
    int8_t dy = abs(y1 - y0);
    int8_t sy = y0 < y1 ? 1 : -1;
    int8_t err = (dx > dy ? dx : -dy) / 2;
    int8_t e2;

    for(;;)
    {
        lcd_set_pixel(x0, y0, value);

        if(x0 == x1 && y0 == y1)
            break;

        e2 = err;
        if(e2 > -dx) { err -= dy; x0 += sx; }
        if(e2 < dy)  { err += dx; y0 += sy; }
    }
}

void box(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t value)
{
    uint8_t x, y;

    for(x = x0; x <= x1; x++)
    {
        for(y = y0; y <= y1; y++)
        {
            lcd_set_pixel(x, y, value);
        }
    }
}

uint8_t draw_glyph(const struct font *font, uint8_t x0, uint8_t y0, char c)
{
    if(c < font->min_char || c > font->max_char)
        return 0;

    const uint8_t *buf = font->glyph_data + font->offset_table[c - font->min_char];
    uint8_t w = font->width_table[c - font->min_char];
    uint8_t x, y;

    for(y = 0; y < font->height; y++)
    {
        for(x = 0; x < w; x++)
        {
            if(buf[((w + 7) >> 3) * y + (x >> 3)] & (1 << (7 - (x & 7))))
                lcd_set_pixel(x + x0, y + y0, c);
        }
    }

    return w;
}

void text(const struct font *font, uint8_t x, uint8_t y, const char *str)
{
    char c;

    while((c = *str++))
        x += draw_glyph(font, x, y, c);
}

