/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
 * @author Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 * @author Theodor Stana <theodor.stana@gmail.com>
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
 * @brief Status bar widget.
 */

#include "status_bar.h"
#include <bitmaps.h>
#include <event.h>

#include <stdlib.h>
#include <string.h>

#include <drivers/gps.h>

#include "../settings/settings.h"

static const unsigned int GPS_POS = 0;
static const unsigned int TRACK_POS = 16;
static const unsigned int BATTERY_POS = 111;
static const unsigned int BATTERY_BARS = 10;

static unsigned percentage;
static bool charging;

static struct rle_bitmap gps_ico;
static int gps_ico_blink = 0;

static struct rle_bitmap track_ico;

static void status_bar_event(struct ui_widget *w, const struct event *evt)
{
    switch(evt->type) {
    case GPS_TICK:
        if (setting_get(&setting_gps_on)) {
            if (gps_fixed()) {
                memcpy(&gps_ico, &gps_receiving,
                        sizeof(struct rle_bitmap));
            } else {
                gps_ico_blink ^= 1;
                if (gps_ico_blink) {
                    memcpy(&gps_ico, &gps_searching,
                        sizeof(struct rle_bitmap));
                } else {
                    memset(&gps_ico, 0, sizeof(struct rle_bitmap));
                }
            }
        } else {
            memcpy(&gps_ico, 0, sizeof(struct rle_bitmap));
        }

        if (setting_get(&setting_tracking))
            memcpy(&track_ico, &tracking, sizeof(struct rle_bitmap));
        else
            memset(&track_ico, 0, sizeof(struct rle_bitmap));

        w->flags |= WF_DIRTY;

        break;

    case BATTERY_STATUS:
        if(abs(percentage - evt->data.battery.percentage) > 5 ||
                charging != evt->data.battery.charging) {
            percentage = evt->data.battery.percentage;
            charging = evt->data.battery.charging;
            w->flags |= WF_DIRTY;
        }
        break;
    }
}

static void status_bar_redraw(struct ui_widget *w)
{
    unsigned i;

    gfx_clear(&w->dc, 0);

    /*gfx_round_box(&w->dc, 30, -10, 127 - 30, 10, 9, COLOR_BLACK);*/
    /*gfx_centered_text(&w->dc, &font_helv11, 0, "Home", 1);*/

    gfx_draw_bitmap(&w->dc, GPS_POS, 0, &gps_ico);
    gfx_draw_bitmap(&w->dc, TRACK_POS, 0, &track_ico);

    if(charging) {
        gfx_draw_bitmap(&w->dc, BATTERY_POS, 0, &battery_charging);
    } else {
        gfx_draw_bitmap(&w->dc, BATTERY_POS, 0, &battery);

        if(percentage > 100) {
            // Indicate wrong reading (draw the exclamation mark)
            gfx_line(&w->dc, BATTERY_POS + 6, 2, BATTERY_POS + 6, 7, 1);
            gfx_set_pixel(&w->dc, BATTERY_POS + 6, 9, 1);
        } else {
            // Draw bars
            for(i = 0; i < percentage / BATTERY_BARS; ++i)
                gfx_line(&w->dc, BATTERY_POS + 2 + i, 2, BATTERY_POS + 2 + i, 9, 1);
        }
    }
}

struct ui_widget status_bar = {
    status_bar_redraw,
    status_bar_event,
    { 0, 0, 127, 15 },
    0,
    WF_ACTIVE | WF_VISIBLE
};

