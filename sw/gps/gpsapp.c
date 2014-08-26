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

#include <drivers/lcd.h>
#include <gfx/graphics.h>
#include <gfx/ui.h>

#include "gps.h"

static struct ui_widget gps_screen = {
        NULL, //gps_redraw,
        NULL, //gps_event,
        {0, 0, 127, 127},
        0,
        WF_ACTIVE | WF_VISIBLE
};

//static struct ui_widget coord_screen = {
//        NULL,
//        NULL,
//};

void main(void *params)
{
        int i = 0,
            j;
        char buf[4];

        /* Init clocks */
        // TODO: move to common init
        CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
        GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

        gps_init();

        // TODO: remove, this is done in startMain()
        lcd_init();
        ui_init();

        /* Init UI */
        ui_clear();
        ui_init_widget(&gps_screen);
        ui_add_widget(&gps_screen);
        ui_update(NULL);

        while (1) {
                i++;
                gfx_clear(&gps_screen.dc, 0);
                sprintf(buf, "%d", i);
                gfx_text(&gps_screen.dc, &font_helv38b, 0, 0, buf, 1);
                ui_update(NULL);
                for (j = 0; j < 100000; j++)
                        ;
        }
}

