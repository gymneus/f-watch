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
#include <udelay.h>
#include <i2cdrv.h>
#include <em_device.h>
#include <em_cmu.h>
#include <em_gpio.h>
#include <drivers/lcd.h>
#include <gfx/graphics.h>
#include <drivers/altimeter.h>

/* Counts 1ms timeTicks */
volatile uint32_t msTicks;

/*
 * @brief SysTick_Handler
 * Interrupt Service Routine for system tick counter
 */
void SysTick_Handler(void)
{
    msTicks++; /* increment counter necessary in Delay()*/
}

/*
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 */
void Delay(uint32_t dlyTicks)
{
    uint32_t curTicks;

    curTicks = msTicks;
    while ((msTicks - curTicks) < dlyTicks);
}

/**
 * @brief  Main function
 */
int main(void)
{
        I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

        int x, y, i;
        char str[20];
        uint8_t cmd;
        uint8_t buf[16];
        uint16_t calib_data[8];

        /* Setup SysTick Timer for 1 msec interrupts */
        if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        // Backlight LEDs
        GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
        GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

        lcd_init();

        UDELAY_Calibrate();
        I2CDRV_Init(&i2cInit);

        ms5806_write_cmd(MS5806_CMD_RESET);

        GPIO_PinOutSet(gpioPortE, 11);

        for(i=0; i<MS5806_PROM_SIZE; i++)
        {
                cmd = MS5806_CMD_READ_PROM + (MS5806_PROM_ADR_MASK & (i << 1));
                ms5806_read_reg(cmd, 2, &buf[2*i]);
                //ms5806_read_reg(cmd, 2, (uint8_t*) calib_data);
                //sprintf(str, "C%d:0x%x",i,calib_data[i]);
                sprintf(str, "C%d:0x%x%x",i,buf[2*i],buf[2*i+1]);
                text(&font_helv17, 5, 15*i, str);
                lcd_update();
        }


        GPIO_PinOutSet(gpioPortE, 12);

        /* Infinite blink loop */
        while (1) {
                Delay(200);
                GPIO_PinOutClear(gpioPortE, 11);
                GPIO_PinOutClear(gpioPortE, 12);
        }
}

