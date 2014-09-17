/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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
 * Settings application.
 */

#include "application.h"
#include "widgets/status_bar.h"
#include "widgets/spinbox.h"
#include "clock.h"
#include <time.h>

static const int DIST_X = 24;
static const int POS_X = 20;
static const int POS_Y = 30;
static const int SIZE_X = 16;
static const int SIZE_Y = 38;

static void set_time_redraw(struct ui_widget *w)
{
    gfx_centered_text(&w->dc, &font_helv38b, POS_Y, ":", 1);
}

static struct ui_widget set_time_screen = {
    &set_time_redraw,
    NULL,
    { 0, 0, 127, 127 },
    0,
    WF_ACTIVE | WF_VISIBLE,
    NULL
};

#define SPINBOX_NUMBER 4
enum SPINBOX { H1 = 0, H2, M1, M2 };

// Spinboxes used for setting the hour & minute.
static struct spinbox sb_time[SPINBOX_NUMBER];

// Index that indicates the active spinbox
static int sb_index;

static inline char sb_digit(int idx)
{
    // convert ascii character to a digit by subtracting 0x30 ('0')
    return spinbox_get_value(&sb_time[idx]) - '0';
}

// Checks if spinboxes contain correct values
static bool is_valid(void)
{
    return (sb_digit(H1) < 3 && sb_digit(H2) < 5 && sb_digit(M1) < 6);
}

// Sets the hour from spinboxes to clock
static void save(void)
{
    struct tm time = clock_get_time();

    time.tm_hour = sb_digit(H1) * 10 + sb_digit(H2);
    time.tm_min = sb_digit(M1) * 10 + sb_digit(M2);
    time.tm_sec = 0;

    clock_set_time(&time);
}

// Sets the spinboxes to the current hour
static void load(void)
{
    struct tm time = clock_get_time();

    spinbox_set_value(&sb_time[H1], (time.tm_hour / 10) + '0');
    spinbox_set_value(&sb_time[H2], (time.tm_hour % 10) + '0');
    spinbox_set_value(&sb_time[M1], (time.tm_min / 10) + '0');
    spinbox_set_value(&sb_time[M2], (time.tm_min % 10) + '0');
}

void set_time_main(void* params) {
    (void)(params); // suppress unused parameter warning
    struct event evt;
    int i;

    // initialize user interface
    ui_clear();

    // initialize widgets before adding them
    for(i = 0; i < 4; ++i) {
        struct rect pos = {POS_X + i * DIST_X, POS_Y,
                           POS_X + i * DIST_X + SIZE_X, POS_Y + SIZE_Y};
        spinbox_init_widget(&sb_time[i], pos, char_digits);
    }
    sb_index = H1;
    spinbox_set_active(&sb_time[H1], true);

    ui_init_widget(&set_time_screen);

    for(i = 0; i < 4; ++i) {
        ui_add_widget(&sb_time[i].widget);
    }

    // widget belongs to the main screen
    for(i = 0; i < 4; ++i) {
        ui_add_child(&set_time_screen, &sb_time[i].widget);
    }
    ui_add_widget(&set_time_screen);

    ui_init_widget(&status_bar);
    ui_add_widget(&status_bar);

    load();

    ui_update(NULL);

    // event loop
    while(1) {
        if(xQueueReceive(appQueue, &evt, 0)) {
            switch(evt.type) {
            case BUTTON_PRESSED:
                if(evt.data.button == BUT_TL) {
                    return;             // go back to the main menu
                } else if(evt.data.button == BUT_TR) {
                    if(sb_index < SPINBOX_NUMBER - 1) {
                        spinbox_set_active(&sb_time[sb_index], false);
                        spinbox_set_active(&sb_time[++sb_index], true);
                    } else if(is_valid()) {
                        save();
                        return;
                    } else {
                        // the set hour is invalid, start from the beginning
                        spinbox_set_active(&sb_time[sb_index], false);
                        sb_index = H1;
                        spinbox_set_active(&sb_time[H1], true);
                    }
                }

                ui_update(&evt);
                break;

            default:    // suppress warnings
                break;
            }
        }
    }
}

application set_time = {
    .name = "Set time",      // this will be shown in menu
    .main = set_time_main
};

