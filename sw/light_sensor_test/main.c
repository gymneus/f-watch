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
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @brief Main file.
 */

#include <stdio.h>
#include <math.h>
#include <delay.h>
#include <i2cdrv.h>
#include <em_device.h>
#include <em_cmu.h>
#include <em_gpio.h>
#include <drivers/lcd.h>
#include <drivers/light_sensor.h>
#include <gfx/graphics.h>
#include <drivers/altimeter.h>



/**
 * @brief  Main function
 */
int main(void)
{
        I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

        //int x, y, i;
        char str[20];
        uint8_t err;
        double temp = 0;
        double pressure = 0;
        double altitude = 0;
        uint32_t lux;

        /* Setup SysTick Timer for 1 msec interrupts */
        if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        // Backlight LEDs
        GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
        GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

        lcd_init();

        I2CDRV_Init(&i2cInit);

        err = alti_init();

        //GPIO_PinOutSet(gpioPortE, 11);
        //GPIO_PinOutSet(gpioPortE, 12);

        while(1)
        {

                err = light_sensor_get_lux(&lux);
                sprintf(str, "light: %u lux", lux);
                text(&font_helv11, 5, 10, str);

                err = alti_get_temp_pressure(&temp, &pressure, true);
                sprintf(str, "temp: %f C", temp);
                text(&font_helv11, 5, 30, str);
                sprintf(str, "pressure: %f mbar", pressure);
                text(&font_helv11, 5, 40, str);

                err = alti_mbar2altitude(pressure, &altitude);
                sprintf(str, "altitude: %f m", altitude);
                text(&font_helv11, 5, 50, str);

                lcd_update();
                //Delay(1000);
                box(0, 0, 128, 128, 0);
                //lcd_clear();
        }




}
