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

#include "clock.h"

#include "application.h"
#include "widgets/status_bar.h"
#include <drivers/rtc.h>

#include <stdio.h>
#include <time.h>

static struct rtc_time rtc;
static struct tm cur_time;

static void digital_watch_redraw(struct ui_widget *w) {
    char buf[32];

    gfx_clear(&w->dc, 0);

    strftime(buf, sizeof(buf), "%H:%M", &cur_time);
    gfx_text(&w->dc, &font_helv38b, 4, 0, buf, 1);

    // sprintf must be used, so we can display msecs too
    sprintf(buf, "%02d.%01d", cur_time.tm_sec, rtc.msecs / 100);
    gfx_text(&w->dc, &font_helv22b, 88, 14, buf, 1);

    strftime(buf, sizeof(buf), "%a %d %b %Y", &cur_time);
    gfx_centered_text(&w->dc, &font_helv17, 40, buf, 1);
}

static void digital_watch_event(struct ui_widget *w, const struct event *evt) {
    // Hour has changed, it is time to redraw the clock
    if(evt->type == RTC_TICK) {
        rtc = rtc_get_time();
        localtime_r((time_t*) &rtc.epoch, &cur_time);

        w->flags |= WF_DIRTY;
    }
}

struct ui_widget digital_watch = {
    digital_watch_redraw,
    digital_watch_event,
    { 0, 35, 127, 35 + 53 },
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
    rtc = rtc_get_time();
    localtime_r((time_t*) &rtc.epoch, &cur_time);

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

application clock_app = {
    .name = "Clock",
    .main = clock_main
};

// Settings
struct tm clock_get_time(void) {
    rtc = rtc_get_time();
    localtime_r((time_t*) &rtc.epoch, &cur_time);

    return cur_time;
}

void clock_set_time(struct tm *time) {
    struct rtc_time r;
    r.msecs = 0;
    r.epoch = mktime(time);

    rtc_set_time(r);
}

