/*
 * Copyright (C) 2014 Julian Lewis
 * @author Maciej Suminski <maciej.suminski@cern.ch>
 * @author Matthieu Cattin <matthieu.cattin@cern.ch>
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
 * USB mass storage mode.
 */

#include "application.h"
#include "widgets/status_bar.h"

#include "msdd.h"
#include "msddmedia.h"
#include "em_usb.h"

#include <semphr.h>

static bool init_ok;
static int mutexours = 0;

extern xSemaphoreHandle mutexSdCardAccess;

static void usb_ms_redraw(struct ui_widget *w)
{
    gfx_clear(&w->dc, 0);

    gfx_centered_text(&w->dc, &font_helv17b, 30, "USB mass storage", 1);

    if (mutexours && init_ok) {
        gfx_centered_text(&w->dc, &font_helv17b, 30 + 18, "enabled", 1);

        gfx_centered_text(&w->dc, &font_helv11, 80, "unmount the device", 1);
        gfx_centered_text(&w->dc, &font_helv11, 80 + 12, "before unplugging", 1);
    } else if (!mutexours) {
        gfx_centered_text(&w->dc, &font_helv17b, 80, "media in use", 1);
    } else {
        gfx_centered_text(&w->dc, &font_helv17b, 80, "media error", 1);
    }
}

struct ui_widget usb_ms_screen = {
    usb_ms_redraw,
    NULL,
    { 0, 0, 127, 127 },
    0,
    WF_ACTIVE | WF_VISIBLE,
    NULL
};

void usb_ms_main(void* params) {
    (void)(params); // suppress unused parameter warning
    struct event evt;

    ui_clear();

    ui_init_widget(&usb_ms_screen);
    ui_add_widget(&usb_ms_screen);

    ui_init_widget(&status_bar);
    ui_add_widget(&status_bar);

    if (xSemaphoreTake(mutexSdCardAccess, 0)) {
        mutexours = 1;
        init_ok = MSDDMEDIA_Init();
        if(init_ok) {
            MSDD_Init(-1, -1);
        }
    }

    ui_update(NULL);


    while(1) {
        if(xQueueReceive(appQueue, &evt, 0)) {
            switch(evt.type) {
            case BUTTON_PRESSED:
                if (evt.data.button == BUT_TR ||
                    evt.data.button == BUT_TL) {
                    if (mutexours) {
                        mutexours = 0;
                        USBD_Stop();
                        xSemaphoreGive(mutexSdCardAccess);
                    }
                    return;             // go back to the main menu
                }
                break;

            default:    // suppress warnings
                // ignore events that were not mentioned above
                ui_update(&evt);
                break;
            }
        } else if (mutexours) {
            MSDD_Handler();
        }
    }
}

application usb_ms = {
    .name = "USB drive",
    .main = usb_ms_main
};

