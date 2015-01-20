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
 * http://www.gnu.org/licenses/gpl-3.0-standalone.html
 * or you may search the http://www.gnu.org website for the version 3 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <em_device.h>

#include <event.h>

#include <drivers/lcd.h>
#include <drivers/gps.h>
#include <gfx/graphics.h>
#include <gfx/ui.h>
#include "widgets/status_bar.h"
#include "settings/settings.h"

#include "application.h"

static void reset_redraw(struct ui_widget *w)
{
    gfx_text(&w->dc, &font_helv22b, 10, 50, "Bottom right", 0);
    gfx_text(&w->dc, &font_helv22b, 20, 70, "to reset", 0);
}


static struct ui_widget reset_screen = {
    reset_redraw,
    NULL,
    {0, 0, 127, 127},
    0,
    WF_ACTIVE | WF_VISIBLE
};

void reset_main(void *params)
{
    (void) params;      // suppress warnings
    struct event evt;

    /* Init UI */
    ui_clear();
    ui_init_widget(&reset_screen);
    ui_add_widget(&reset_screen);
    ui_init_widget(&status_bar);
    ui_add_widget(&status_bar);
    ui_update(NULL);

    while(1) {
        if (xQueueReceive(appQueue, &evt, portMAX_DELAY)) {
            switch (evt.type) {
            case BUTTON_PRESSED:
                if (evt.data.button == BUT_TR ||
                    evt.data.button == BUT_TL)
                        return;
                /* Reset on bottom right and bottom left buttons
                 * pressed */
                if (evt.data.button == BUT_BR)
                    SCB->AIRCR = 0x05FA0004;
                    // Turn GPS off if setting is ON, to prepare it for
                    // the turn-on pulse on reset
                    if (setting_get(&setting_gps_on)) {
                        int i;
                        gps_on_off_pulse();
                        for (i = 0; i < 1000000; i++)
                            ;
                    }
                break;
            }
        }
    }
}

application reset = {
    .name = "Reset",
    .main = reset_main
};
