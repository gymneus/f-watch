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

#include <drivers/gps/gps.h>

#include "application.h"

static struct gps_coord coord;
static int coord_format;

static void gps_redraw(struct ui_widget *w)
{
        char buf[16];

        if (gps_fixed())
                gps_get_coord(&coord, coord_format);

        gfx_clear(&w->dc, 0);
        sprintf(buf, "L: -90", (int)(coord.lat / 100));
        gfx_text(&w->dc, &font_helv22b, 0, 0, buf, 0);
        sprintf(buf, "60.7777'");
        gfx_text(&w->dc, &font_helv22b, 15, 20, buf, 0);
        sprintf(buf, "l: -180", (int)(coord.lon / 100));
        gfx_text(&w->dc, &font_helv22b, 0, 40, buf, 0);
        sprintf(buf, "60.7777'");
        gfx_text(&w->dc, &font_helv22b, 15, 60, buf, 0);
        sprintf(buf, "h: 8848m");
        gfx_text(&w->dc, &font_helv22b, 0, 80, buf, 0);

//        sprintf(buf, "%2.4f" )
//        gfx_text(&w->dc, &font_helv22b, 0, 30, buf, 0);
//        sprintf(buf, "%5.0f m", coord.elev);
//        gfx_text(&w->dc, &font_helv22b, 0, 50, buf, 0);
}

static void gps_event(struct ui_widget *w, const struct event *evt)
{
        if (evt->type == RTC_TICK)
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
        struct event evt;
        int i = 0;

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
                if (xQueueReceive(appQueue, &evt, 0)) {
                        switch (evt.type) {
                        case BUTTON_PRESSED:
                                if (evt.data.button == BUT_TR)
                                        return;
                                else if (evt.data.button == BUT_BR) {
                                        coord_format += 1;
                                        coord_format %= 2;
                                }
                                break;
                        case RTC_TICK:
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
