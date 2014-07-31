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

#include <em_device.h>
#include <em_cmu.h>
#include <em_gpio.h>
#include <drivers/lcd.h>
#include <drivers/i2cdrv.h>
#include <drivers/max17047.h>
#include <gfx/graphics.h>
#include <stdio.h>

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
    int x, y;
    char buf[16];
    I2C_Init_TypeDef i2c_init = I2C_INIT_DEFAULT;
    uint16_t version = 0;
    uint8_t ret = 0;

    /* Setup SysTick Timer for 1 msec interrupts */
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    // Backlight LEDs
    GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

    lcd_init();
    I2CDRV_Init(&i2c_init);

    for(x = 0; x < LCD_WIDTH / 4; ++x)
    {
        for(y = 0; y < LCD_HEIGHT / 4; ++y)
        {
            lcd_set_pixel(x, y, (x >> 3 ^ y >> 3) % 2);
        }
    }

    text(&font_helv11, 5, 65, "helv11");
    text(&font_helv17, 5, 75, "helv17");
    text(&font_helv17b, 5, 90, "helv17b");
    text(&font_xm4x5, 5, 110, "xm4x5");
    text(&font_xm4x6, 5, 120, "xm4x6");

    box(100, 100, 120, 120, 1);
    line(100, 40, 120, 70, 1);

    /* Infinite blink loop */
    while (1) {
        GPIO_PinOutToggle(gpioPortE, 11);
        Delay(200);
        GPIO_PinOutToggle(gpioPortE, 12);
        Delay(200);

        // Print the version of the fuel gauge chip
        ret = max17047_read_reg(MAX17047_REG_VERSION, 2, (uint8_t*)&version);
        sprintf(buf, "ver %d", version);
        text(&font_helv17, 64, 0, buf);
        version = 0;    // be sure that it is read every time

        lcd_update();
    }
}

