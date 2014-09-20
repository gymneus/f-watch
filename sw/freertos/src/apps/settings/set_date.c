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

static const int DIST_X = 20;
static const int POS_X = 1;
static const int POS_Y = 30;
static const int SIZE_X = 16;
static const int SIZE_Y = 38;

static void set_date_redraw(struct ui_widget *w)
{
    gfx_text(&w->dc, &font_helv38b, 36, POS_Y, "/", 1);
    gfx_text(&w->dc, &font_helv38b, 80, POS_Y, "/", 1);
}

static struct ui_widget set_date_screen = {
    &set_date_redraw,
    NULL,
    { 0, 0, 127, 127 },
    0,
    WF_ACTIVE | WF_VISIBLE,
    NULL
};

#define SPINBOX_NUMBER 6
enum SPINBOX { D1 = 0, D2, M1, M2, Y1, Y2 };

static const char const *days1 = "0123";
static const char const *months1 = "01";

// Character sets for spinboxes
static const char **char_sets[SPINBOX_NUMBER] = {
    &days1, &char_digits,           // character set for days
    &months1, &char_digits,         // character set for months
    &char_digits, &char_digits      // character set for years
};

// Spinboxes used for setting the hour & minute.
static struct spinbox sb_date[SPINBOX_NUMBER];

// Index that indicates the active spinbox
static int sb_index;

static inline char sb_digit(int idx)
{
    // convert ascii character to a digit by subtracting 0x30 ('0')
    return spinbox_get_value(&sb_date[idx]) - '0';
}

static bool is_leap(int year)
{
    if(year < 1970) {
        if(year < 70)
            year += 2000;
        else
            year += 1900;
    }

    if (year % 400 == 0)
        return true;
    else if (year % 100 == 0)
        return false;
    else if (year % 4 == 0 )
        return true;
    else
        return false;
}

// Checks if spinboxes contain correct values
static bool is_valid(void)
{
    const int days_per_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const int days_per_month_leap[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int day = sb_digit(D1) * 10 + sb_digit(D2);
    int month = sb_digit(M1) * 10 + sb_digit(M2);
    int year = sb_digit(Y1) * 10 + sb_digit(Y2);

    int max_day = is_leap(year) ? days_per_month_leap[month - 1] : days_per_month[month - 1];

    return (day > 0 && day <= max_day && month > 0 && month < 13);
}

// Sets the hour from spinboxes to clock
static void save(void)
{
    struct tm time = clock_get_time();

    time.tm_mday = sb_digit(D1) * 10 + sb_digit(D2);
    time.tm_mon = sb_digit(M1) * 10 + sb_digit(M2) - 1;
    time.tm_year = sb_digit(Y1) * 10 + sb_digit(Y2) + 100;

    clock_set_time(&time);
}

// Sets the spinboxes to the current hour
static void load(void)
{
    struct tm time = clock_get_time();
    int year = time.tm_year - 100;
    int month = time.tm_mon + 1;

    spinbox_set_value(&sb_date[D1], (time.tm_mday / 10) + '0');
    spinbox_set_value(&sb_date[D2], (time.tm_mday % 10) + '0');
    spinbox_set_value(&sb_date[M1], (month / 10) + '0');
    spinbox_set_value(&sb_date[M2], (month % 10) + '0');
    spinbox_set_value(&sb_date[Y1], (year / 10) + '0');
    spinbox_set_value(&sb_date[Y2], (year % 10) + '0');
}

void set_date_main(void* params) {
    (void)(params); // suppress unused parameter warning
    struct event evt;
    int i;

    // initialize user interface
    ui_clear();

    // initialize widgets before adding them
    for(i = 0; i < SPINBOX_NUMBER; ++i) {
        struct rect pos = {POS_X + i * DIST_X + (i / 2 * 4), POS_Y,
                           POS_X + i * DIST_X + (i / 2 * 4) + SIZE_X, POS_Y + SIZE_Y};
        spinbox_init_widget(&sb_date[i], pos, *char_sets[i]);
    }
    sb_index = D1;
    spinbox_set_active(&sb_date[D1], true);

    ui_init_widget(&set_date_screen);

    for(i = 0; i < SPINBOX_NUMBER; ++i) {
        ui_add_widget(&sb_date[i].widget);
    }

    // widget belongs to the main screen
    for(i = 0; i < SPINBOX_NUMBER; ++i) {
        ui_add_child(&set_date_screen, &sb_date[i].widget);
    }
    ui_add_widget(&set_date_screen);

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
                        spinbox_set_active(&sb_date[sb_index], false);
                        spinbox_set_active(&sb_date[++sb_index], true);
                    } else if(is_valid()) {
                        save();
                        return;
                    } else {
                        // the set hour is invalid, start from the beginning
                        spinbox_set_active(&sb_date[sb_index], false);
                        sb_index = D1;
                        spinbox_set_active(&sb_date[D1], true);
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

application set_date = {
    .name = "Set date",      // this will be shown in menu
    .main = set_date_main
};

