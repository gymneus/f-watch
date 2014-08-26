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
#include <gfx/graphics.h>
#include <drivers/altimeter.h>


#define HIST_SIZE 127

typedef struct hist_buf
{
        //uint16_t *buffer; // data buffer
        uint16_t buffer[HIST_SIZE]; // data buffer
        uint16_t *end;    // end of data buffer
        bool wrap;    // indicates buffer wrap
        uint16_t *head;   // pointer to head
        uint16_t *tail;   // pointer to tail
} hist_buf;

void buf_init(hist_buf *buf, uint16_t size)
{
        uint16_t *scan;

        //buf->buffer = malloc(size);
        //buf->end = buf->buffer + size;
        buf->end = &buf->buffer[HIST_SIZE];
        buf->wrap = false;
        buf->head = buf->buffer;
        buf->tail = buf->buffer;

        scan = buf->buffer;

        while(scan <= buf->end)
        {
                *scan = 0;
                scan++;
        }
}

void buf_write(hist_buf *buf, uint16_t data)
{
        *buf->head++ = data;
        if(buf->head >= buf->end)
        {
                buf->head = buf->buffer;
                buf->wrap = true;
        }
        if(buf->wrap)
        {
                buf->tail++;
                if(buf->tail >= buf->end)
                {
                        buf->tail = buf->buffer;
                }
        }
}

void buf_get_last(hist_buf *buf, uint16_t *last)
{
        if(buf->head == buf->buffer)
        {
                last = buf->end;
        }
        else
        {
                last = buf->head--;
        }
}


void plot_hist(hist_buf *buf, uint8_t y_org, uint8_t y_size)
{
        uint8_t x;
        uint16_t y_min, y_max;
        uint16_t *scan;
        uint16_t y_val;

        buf_get_last(buf, scan);

        y_min = *scan;
        y_max = *scan;

        while(scan != buf->tail)
        {
                scan++;
                if(scan >= buf->end)
                {
                        scan = buf->buffer;
                }
                if(*scan > y_max)
                        y_max = *scan;
                if(*scan < y_min)
                        y_min = *scan;
        }

        buf_get_last(buf, scan);

        // scan x axis (entire screen)
        for(x=0; x<127; x++)
        {
                while(scan != buf->tail)
                {
                        if(*scan != 0)
                        {
                                y_val = (*scan-960) * (10)/y_size;
                                //y_val = (*scan-y_min) * (y_max-y_min)/y_size;
                                lcd_set_pixel(x, y_val + y_org, 1);
                        }
                        scan++;
                }
        }
}


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
        double pressure_calc;
        double pressure_comp;
        hist_buf *pressure_hist;

        /* Setup SysTick Timer for 1 msec interrupts */
        if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        // Backlight LEDs
        GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
        GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

        lcd_init();

        I2CDRV_Init(&i2cInit);

        //ui_init();
	//alti_screen_create();

        err = alti_init();
        sprintf(str, "init: 0x%02x", err);
        text(&font_helv11, 5, 10, str);
        lcd_update();

        err = alti_get_temp_pressure(&temp, &pressure, true);
        err = alti_altitude2mbar(&pressure_calc, 440);
        pressure_comp = pressure - pressure_calc;

        buf_init(pressure_hist, HIST_SIZE);

        while(1)
        {

                //ui_update();

                err = alti_get_temp_pressure(&temp, &pressure, true);
                sprintf(str, "temp: %3.2f C", temp);
                text(&font_helv11, 5, 10, str);
                sprintf(str, "pressure: %5.2f mbar", pressure);
                text(&font_helv11, 5, 20, str);

                buf_write(pressure_hist, (uint16_t)(pressure));

                err = alti_mbar2altitude(pressure, &altitude);
                sprintf(str, "altitude: %4.2f m", altitude);
                text(&font_helv11, 5, 30, str);

                sprintf(str, "p calc: %5.2f mbar", pressure_calc);
                text(&font_helv11, 5, 50, str);

                sprintf(str, "p comp: %5.2f mbar", pressure_comp);
                text(&font_helv11, 5, 60, str);

                err = alti_mbar2altitude(pressure-pressure_comp, &altitude);
                sprintf(str, "alti comp: %4.2f m", altitude);
                text(&font_helv11, 5, 40, str);

                //plot_hist(pressure_hist, 120, 60);

                //sprintf(str, "err: 0x%02x", err);
                //text(&font_helv11, 5, 50, str);
                lcd_update();
                //Delay(1000);
                //box(5, 10, 128, 50, 0);
                lcd_clear();

        }




}

