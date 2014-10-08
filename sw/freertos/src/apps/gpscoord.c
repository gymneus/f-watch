/*
 * Copyright (C) 2014 Julian Lewis
 * @author Theodor Stana <theodor.stana@gmail.com>
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

#include <string.h>
#include <stdio.h>

#include <event.h>

#include <drivers/lcd.h>
#include <gfx/graphics.h>
#include <gfx/ui.h>
#include "widgets/status_bar.h"

#include <bitmaps.h>

#include <drivers/gps.h>

#include "application.h"

#include "settings/settings.h"

static struct gps_coord coord;
static int coord_format;
static int gpsscreen = 0;

static void gps_redraw(struct ui_widget *w)
{
    char buf[16];
    float latdeg;
    float latmin;
    float latsec;
    float londeg;
    float lonmin;
    float lonsec;

    coord_format = setting_coord_style.val;

    if (gps_fixed())
        gps_get_coord(&coord, coord_format);

    /* Decompose degrees, mins, secs for formats 0 & 1 */
    latdeg = coord.lat/100;
    latmin = 100 * (latdeg - (int)latdeg);
    latsec = coord.lat - (int)coord.lat;

    londeg = coord.lon/100;
    lonmin = 100 * (londeg - (int)londeg);
    lonsec = coord.lon - (int)coord.lon;

    gfx_clear(&w->dc, 0);

    /* Display latitude and longitude depending on format */
    if (gpsscreen == 0) {
        if (coord_format == 0) {
            /* raw [deg][min].[sec/60] */
            sprintf(buf, "L: %d deg", (int)latdeg);
            gfx_text(&w->dc, &font_helv22b, 0, 0, buf, 0);
            sprintf(buf, "%2.4f'", latmin);
            gfx_text(&w->dc, &font_helv22b, 22, 20, buf, 0);

            sprintf(buf, "l: %d deg", (int)(londeg));
            gfx_text(&w->dc, &font_helv22b, 0, 50, buf, 0);
            sprintf(buf, "%2.4f'", lonmin);
            gfx_text(&w->dc, &font_helv22b, 15, 70, buf, 0);
        } else if (coord_format == 1) {
            /* [deg] [min] [sec] */
            sprintf(buf, "L: %d deg", (int)latdeg);
            gfx_text(&w->dc, &font_helv22b, 0, 0, buf, 0);
            sprintf(buf, "%d'%2.2f\"", (int)latmin, 100 * latsec);
            gfx_text(&w->dc, &font_helv22b, 5, 20, buf, 0);

            sprintf(buf, "l: %d deg", (int)londeg);
            gfx_text(&w->dc, &font_helv22b, 0, 50, buf, 0);
            sprintf(buf, "%d'%2.2f\"", (int)lonmin, 100 * lonsec);
            gfx_text(&w->dc, &font_helv22b, 5, 70, buf, 0);
        } else if (coord_format == 2) {
            /* [deg].[min/60] */
            sprintf(buf, "L: %2.4f", coord.lat);
            gfx_text(&w->dc, &font_helv22b, 0, 0, buf, 0);

            sprintf(buf, "l: %2.4f", coord.lon);
            gfx_text(&w->dc, &font_helv22b, 5, 50, buf, 0);
        }
    /* Display elevation */
    } else {
        sprintf(buf, "elev:");
        gfx_text(&w->dc, &font_helv22b, 0, 20, buf, 0);
        sprintf(buf, "%5.3f", coord.elev);
        gfx_text(&w->dc, &font_helv22b, 0, 40, buf, 0);
    }
}

static void gps_event(struct ui_widget *w, const struct event *evt)
{
    if (evt->type == GPS_TICK || evt->type == BUTTON_PRESSED)
        w->flags |= WF_DIRTY;
}

static struct ui_widget gps_coord_display = {
    gps_redraw,
    gps_event,
    {16, 16, 111, 111},
    0,
    WF_ACTIVE | WF_VISIBLE
};

static struct ui_widget gps_screen = {
    NULL,
    NULL,
    {0, 0, 127, 127},
    0,
    WF_ACTIVE | WF_VISIBLE
};

void gpscoord_main(void *params)
{
    /* suppress compiler warning */
    (void) params;

    struct event evt;

    /* Init UI */
    ui_clear();

    ui_init_widget(&gps_screen);
    ui_add_widget(&gps_screen);

    ui_init_widget(&gps_coord_display);
    ui_add_widget(&gps_coord_display);
    ui_add_child(&gps_screen, &gps_coord_display);

    ui_init_widget(&status_bar);
    ui_add_widget(&status_bar);

    ui_update(NULL);

    while (1) {
        if (xQueueReceive(appQueue, &evt, portMAX_DELAY)) {
            switch (evt.type) {
            case BUTTON_PRESSED:
                if (evt.data.button == BUT_TR)
                    return;
                else if (evt.data.button == BUT_BR) {
                    /*
                     * Toggle between coordinate and
                     * elevation screens
                     */
                    gpsscreen += 1;
                    gpsscreen %= 2;
                }
                /* fall through */

            case GPS_TICK:
                ui_update(&evt);
            default:
                break;
            }
        }
    }
}

application gpscoord = {
    .name = "uerdefucami",
    .main = gpscoord_main
};
