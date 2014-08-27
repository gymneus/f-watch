/*
 *==============================================================================
 * CERN (BE-CO-HT)
 * Source file for GPS app
 *==============================================================================
 *
 * author: Theodor Stana (t.stana@cern.ch)
 *
 * date of creation: 2014-08-14
 *
 * version: 1.0
 *
 * description:
 *
 * dependencies:
 *
 * references:
 *
 *==============================================================================
 * GNU LESSER GENERAL PUBLIC LICENSE
 *==============================================================================
 * This source file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version. This source is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details. You should have
 * received a copy of the GNU Lesser General Public License along with this
 * source; if not, download it from http://www.gnu.org/licenses/lgpl-2.1.html
 *==============================================================================
 * last changes:
 *    2014-08-14   Theodor Stana     File created
 *==============================================================================
 * TODO: -
 *==============================================================================
 */

#include <string.h>
#include <stdio.h>

#include <em_device.h>
#include <em_cmu.h>
#include <em_usb.h>
#include <em_leuart.h>
#include <em_gpio.h>

#include <event.h>

#include <drivers/lcd.h>
#include <gfx/graphics.h>
#include <gfx/ui.h>

#include <bitmaps.h>

#include "gps.h"

static struct rle_bitmap gps_ico;
static int gps_ico_blink = 0;

static void gps_redraw(struct ui_widget *w)
{
        char buf[16];
        struct gps_utc utc;

        gps_get_utc(&utc);

        gfx_clear(&w->dc, 0);
        sprintf(buf, "%d:%d:%d", utc.hr,
                                 utc.min,
                                 utc.sec);
        gfx_text(&w->dc, &font_helv17b, 0, 0, buf, 0);
        gfx_draw_bitmap(&w->dc, 0, 112, &gps_ico);
}

static void gps_event(struct ui_widget *w, const struct event *evt)
{
        if (evt->type == GPS_FIX_LOST) {
                gps_ico_blink ^= 1;
                if (gps_ico_blink)
                        memcpy(&gps_ico, &gps_searching,
                                sizeof(struct rle_bitmap));
                else
                        memcpy(&gps_ico, 0, sizeof(struct rle_bitmap));
        } else if (evt->type == GPS_FIX_ACQ) {
                memcpy(&gps_ico, &gps_receiving, sizeof(struct rle_bitmap));
        }
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

static struct ui_widget gps_fix = {
        NULL,
        NULL,
        {0, 112, 15, 112},
        0,
        WF_ACTIVE | WF_VISIBLE
};

void main(void *params)
{
        struct event evt;
        int i = 0;

        /* Init clocks */
        // TODO: move to common init
        CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
        GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

        /* Initialize GPS module */
        gps_init();

        // TODO: remove, this is done in startMain()
        lcd_init();
        ui_init();

        /* Init UI */
        ui_clear();

        ui_init_widget(&gps_screen);
        ui_init_widget(&gps_coord_display);
        ui_add_widget(&gps_screen);

        ui_add_child(&gps_screen, &gps_coord_display);
        ui_add_widget(&gps_coord_display);

//        ui_init_widget(&status_bar);
//        ui_add_widget(&status_bar);

        ui_update(NULL);

        while (1) {
                for (i = 0; i < 100000; i++)
                        ;

                if (gps_fixed()) {
                        evt.type = GPS_FIX_ACQ;
                        ui_update(&evt);
                } else {
                        evt.type = GPS_FIX_LOST;
                        ui_update(&evt);
                }
        }
}

