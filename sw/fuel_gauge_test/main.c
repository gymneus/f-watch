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
#include <drivers/max17047.h>



/**
 * @brief  Main function
 */
int main(void)
{
        I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

        int x, y, i;
        char str[20];
        uint8_t err;
        double temp = 0;
        double pressure = 0;
        double altitude = 0;
        double lux;
        uint16_t fg_var16;
        uint8_t fg_var8;
        int8_t fg_temp;
        int16_t fg_i;

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

        err = max17047_set_config(0x2250);

        //GPIO_PinOutSet(gpioPortE, 11);
        //GPIO_PinOutSet(gpioPortE, 12);

        while(1)
        {

                err = light_sensor_get_lux(&lux);
                sprintf(str, "light: %3.3f lux", lux);
                text(&font_helv11, 5, 10, str);

                err = alti_get_temp_pressure(&temp, &pressure, true);
                sprintf(str, "temp: %f C", temp);
                text(&font_helv11, 5, 20, str);
                sprintf(str, "pressure: %f mbar", pressure);
                text(&font_helv11, 5, 30, str);

                err = alti_mbar2altitude(pressure, &altitude);
                sprintf(str, "altitude: %f m", altitude);
                text(&font_helv11, 5, 40, str);

                fg_var16 =  max17047_get_status();
                sprintf(str, "fg status: 0x%x", fg_var16);
                text(&font_helv11, 5, 50, str);

                fg_var16 =  max17047_get_config();
                sprintf(str, "fg config: 0x%x", fg_var16);
                text(&font_helv11, 5, 60, str);

                fg_var16 =  max17047_get_voltage();
                sprintf(str, "fg voltage: %d mV", fg_var16);
                text(&font_helv11, 5, 70, str);

                fg_i =  max17047_get_current();
                sprintf(str, "fg current: %d mA", fg_i);
                text(&font_helv11, 5, 80, str);

                fg_temp =  max17047_get_temperature();
                sprintf(str, "fg temp: %d C", fg_temp);
                text(&font_helv11, 5, 90, str);

                fg_var8 =  max17047_get_charge();
                sprintf(str, "fg charge: %d", fg_var8);
                text(&font_helv11, 5, 100, str);

                fg_var16 =  max17047_get_time_left();
                sprintf(str, "fg left: %d min", fg_var16);
                text(&font_helv11, 5, 110, str);


                for(x=0; x<128; x++)
                {
                        for(y=0; y<128; y++)
                        {
                                lcd_toggle_pixel(x, y);
                        }
                }
                lcd_update();
                //Delay(1000);
                box(0, 0, 128, 128, 0);

                //Delay(1000);
                //box(0, 0, 128, 128, 0);
                //lcd_clear();
        }




}
