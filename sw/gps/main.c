/*
 *==============================================================================
 * CERN (BE-CO-HT)
 * Source file for Main
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

#include "em_device.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "em_leuart.h"
#include "em_gpio.h"

#include <drivers/lcd.h>
#include <gfx/graphics.h>

#include "gps.h"

int main()
{
        CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
        GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

        int i = 0,
            j;
        static char tmp[128];
        double lat, lon, elv;
        double spd, dir;
        int yr, mon, day, hr, min, sec;

        lcd_init();
        gps_init();
        text(&font_helv11, 5, 10, "init done!\r\n");
        lcd_update();

        for (;;) {
                GPIO_PinInGet(gpioPortA, 1) ?
                        GPIO_PinOutSet(gpioPortE, 11) :
                        GPIO_PinOutClear(gpioPortE, 11);

                gps_get_utc(&yr, &mon, &day, &hr, &min, &sec);
                sprintf(tmp, "%d-%d-%d %d:%d:%d\r\n", yr, mon, day, hr, min, sec);
                text(&font_helv11, 5, 30+i%60, tmp);
                lcd_update();

                i += 15;
                for (j = 0; j < 100000; j++)
                        ;
        }

        return 0;
}

