/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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
 * Clock application.
 */

#include "application.h"
#include "widgets/status_bar.h"
#include <drivers/rtc.h>

#include <stdio.h>

static unsigned int hour, mins, secs, msecs;

static void digital_watch_redraw(struct ui_widget *w)
{
    char buf[20];
    sprintf(buf,"%02d:%02d", hour, mins);

    gfx_clear(&w->dc, 0);
    gfx_text(&w->dc, &font_helv38b, 0, 0, buf, 1);

    sprintf(buf,"%02d.%01d", secs, msecs / 100);
    gfx_text(&w->dc, &font_helv22b, 84, 14, buf, 1);
}

static void digital_watch_event(struct ui_widget *w, const struct event *evt)
{
    // Hour has changed, it is time to redraw the clock
    if(evt->type == RTC_TICK) {
        msecs += 200;
        if(1000 == msecs) {
            ++secs;
            msecs = 0;
        }

        ++secs;
        if(60 == secs) {
            ++mins; secs = 0;
            if(60 == mins) {
                ++hour; mins = 0;
                if(24 == hour)
                    hour = 0; // TODO 12/24 hour format
            }
        }

        w->flags |= WF_DIRTY;
    }
}

struct ui_widget digital_watch = {
    digital_watch_redraw,
    digital_watch_event,
    { 0, 20, 127, 59 },
    0,
    WF_ACTIVE | WF_VISIBLE
};

struct ui_widget clock_screen = {
    NULL,
    NULL,
    { 0, 0, 127, 127 },
    0,
    WF_ACTIVE | WF_VISIBLE
};

void clock_main(void* params) {
    (void)(params); // suppress unused parameter warning
    struct event evt;

    // Restore clock
    struct rtc_time time = rtc_get_time();
    msecs = time.msecs;
    secs = time.epoch;
    mins = secs / 60;
    hour = mins / 60;
    secs %= 60;
    mins %= 60;
    hour %= 24;

    // Initialize user interface
    ui_clear();

    ui_init_widget(&clock_screen);
    ui_init_widget(&digital_watch);

    ui_add_widget(&digital_watch);
    ui_add_child(&clock_screen, &digital_watch);
    ui_add_widget(&clock_screen);

    ui_init_widget(&status_bar);
    ui_add_widget(&status_bar);

    // Draw the screen
    ui_update(NULL);

    // Event loop
    while(1) {
        if(xQueueReceive(appQueue, &evt, 0)) {
            switch(evt.type) {
            case BUTTON_PRESSED:
                if(evt.data.button == BUT_TR)
                    return;             // go back to the main menu
                // no break; fall through

            case RTC_TICK:  // and BUTTON_PRESSED
                ui_update(&evt);        // forward event to widgets

            default:    // suppress warnings
                break;
            }

        }
    }
}

application clock = {
    .name = "Clock",
    .main = clock_main
};

