/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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
 * Application template.
 */

#include "application.h"
#include "widgets/status_bar.h"
#include "drivers/backlight.h"
#include "drivers/buzzer.h"
#include "drivers/vibra.h"

#include <string.h>     // for strcpy
#include <stdio.h>      // for sprintf

static char message[32] = "hello world";

static void widget_redraw(struct ui_widget *w)
{
    gfx_clear(&w->dc, 0);
    gfx_text(&w->dc, &font_helv17b, 0, 0, message, 1);
}

static void widget_event(struct ui_widget *w, const struct event *evt)
{
    // screen needs redrawing
    w->flags |= WF_DIRTY;

    switch(evt->type) {
    case BUTTON_PRESSED:
        switch(evt->data.button) {
            case BUT_TR:    // top right button
                backlight_set_level(backlight_get_level() + 10);
                sprintf(message, "top right (bl: %d)", backlight_get_level());
                break;

            case BUT_BR:    // bottom right button
                backlight_set_level(backlight_get_level() - 10);
                sprintf(message, "bot right (bl: %d)", backlight_get_level());
                break;

            case BUT_BL:    // bottom left button
                strcpy(message, "bot left");
                break;

            case BUT_TL:    // this should not happen, it is handled
                break;      // in the main loop
        }

        // short vibration
        vibra_enable();
        vTaskDelay(400 / portTICK_RATE_MS);
        vibra_disable();
        break;

    case RTC_TICK:
        // tick-tock with a buzzer
        buzzer_enable();
        vTaskDelay(400 / portTICK_RATE_MS);
        buzzer_disable();
        // ok, we do not need redrawing
        w->flags &= ~WF_DIRTY;
        break;

    default:    // suppress warnings
        // ok, we do not need redrawing
        w->flags &= ~WF_DIRTY;
        break;
    }
}

struct ui_widget widget = {
    widget_redraw,      // drawing function, called when widget is marked as dirty
    widget_event,       // event handler
    { 0, 20, 127, 59 }, // dimensions
    0,                  // surface pointer; do not change
    WF_ACTIVE | WF_VISIBLE  // flags
};

struct ui_widget example_screen = {     // main screen, contains all widgets
    NULL,
    NULL,
    { 0, 0, 127, 127 },
    0,
    WF_ACTIVE | WF_VISIBLE
};

void example_main(void* params) {
    (void)(params); // suppress unused parameter warning
    struct event evt;

    // this is a good place for hardware initialization
    // (configure interrupts, backlight settings, etc.)

    // initialize user interface
    ui_clear();

    // initialize widgets before adding them
    ui_init_widget(&example_screen);
    ui_init_widget(&widget);

    ui_add_widget(&widget);
    // widget belongs to the main screen
    ui_add_child(&example_screen, &widget);
    ui_add_widget(&example_screen);

    // if you want to have fullscreen just for you
    // commenting the lines below will disable the status bar
    ui_init_widget(&status_bar);
    ui_add_widget(&status_bar);

    // draw the screen for the first time
    ui_update(NULL);

    // event loop
    while(1) {
        // "0" in the line below, if you set it to a positive value then
        // you may have a block of code that is executed when no event arrives
        // for details, see below (else block)
        if(xQueueReceive(appQueue, &evt, portMAX_DELAY)) {
            switch(evt.type) {
            // decide which events are relevant and should be handled
            // you may save some cycles if you list them here instead of
            // lazily handling all of them
            case BUTTON_PRESSED:
                // current convention is to use the top left button to go back
                if(evt.data.button == BUT_TL)
                    return;             // go back to the main menu
                // no break; fall through

            default:    // suppress warnings
                // ignore events that were not mentioned above
                ui_update(&evt);        // forward event to widgets
                break;
            }
        }
        else {
            // that part is executed if timeout occurs
        }
    }
}

application example = {
    .name = "Example",      // this will be shown in menu
    .main = example_main
};

