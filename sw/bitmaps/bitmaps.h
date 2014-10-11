/*
 * Copyright (C) 2014 Julian Lewis
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

#ifndef BITMAPS_H
#define BITMAPS_H

#include <stdint.h>

struct rle_bitmap
{
    uint8_t w;
    uint8_t h;
    const uint8_t *data;
};

extern const struct rle_bitmap comp_circle;
extern const struct rle_bitmap gps_disconnected;
extern const struct rle_bitmap comp_circle3;
extern const struct rle_bitmap date_icon;
extern const struct rle_bitmap comp_ico;
extern const struct rle_bitmap gps_receiving;
extern const struct rle_bitmap comp_circle2;
extern const struct rle_bitmap comp_arrow;
extern const struct rle_bitmap usb_ms_icon;
extern const struct rle_bitmap battery;
extern const struct rle_bitmap clock_icon;
extern const struct rle_bitmap example_icon;
extern const struct rle_bitmap battery_charging;
extern const struct rle_bitmap gps_searching;
extern const struct rle_bitmap settings_icon;
extern const struct rle_bitmap game_ico;
extern const struct rle_bitmap comp_arrow2;

#endif /* BITMAPS_H */
