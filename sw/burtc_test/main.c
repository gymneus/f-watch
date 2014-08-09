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
#include <drivers/light_sensor.h>
#include <gfx/graphics.h>
#include <drivers/altimeter.h>
#include <drivers/max17047.h>
#include <em_rtc.h>
#include <em_burtc.h>
#include <em_emu.h>
#include <em_rmu.h>


#define LFXO_FREQUENCY                  32768
#define WAKEUP_INTERVAL_MS              1200
#define BURTC_COUNT_BETWEEN_WAKEUP      (((LFXO_FREQUENCY * WAKEUP_INTERVAL_MS) / 1000)-1)


static uint16_t milliseconds = 0;
static uint8_t minutes = 45;
static uint8_t hours = 10;
static bool lcd_update_flag;


/**
 * @brief BURTC Interrupt Handler clears the flag
 */
void BURTC_IRQHandler(void)
{
        /* Clear interrupt source */
        BURTC_IntClear(BURTC_IFC_COMP0);

        milliseconds += 1200;

        if(milliseconds == 60000)
        {
                milliseconds = 0;
                minutes += 1;
        }

        if(minutes == 60)
        {
                minutes = 0;
                hours += 1;
        }
        if(hours == 24)
        {
                hours = 0;
        }

        lcd_update_flag = true;

}


/**
 * @brief  Setup BURTC
 * Using LFRCO clock source and enabling interrupt on COMP0 match
 */
void setupBurtc(void)
{
        BURTC_Init_TypeDef burtcInit = BURTC_INIT_DEFAULT;

        burtcInit.enable       = true;                /* Enable BURTC after initialization  */
        burtcInit.mode         = burtcModeEM3;        /* BURTC is enabled in EM0-EM3 */
        burtcInit.debugRun     = false;               /* Counter shall keep running during debug halt. */
        burtcInit.clkSel       = burtcClkSelLFRCO;    /* Select LFRCO as clock source */
        burtcInit.clkDiv       = burtcClkDiv_1;       /* Clock prescaler             */
        burtcInit.lowPowerComp = 0;                   /* Number of least significantt clock bits to ignore in low power mode */
        burtcInit.timeStamp    = true;                /* Enable time stamp on entering backup power domain */
        burtcInit.compare0Top  = true;                /* Clear counter on compare match */
        burtcInit.lowPowerMode = burtcLPDisable;      /* Low power operation mode, requires LFXO or LFRCO */

        BURTC_CompareSet(0, BURTC_COUNT_BETWEEN_WAKEUP);  /* Set top value for comparator */

        /* Enabling Interrupt from BURTC */
        NVIC_EnableIRQ(BURTC_IRQn);
        BURTC_IntEnable( BURTC_IF_COMP0 );    /* Enable compare interrupt flag */

        /* Initialize BURTC */
        BURTC_Init(&burtcInit);
}


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


        // Enable LFXO and select it for LFA branch
        CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
        CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

        // Enable low energy clocking module clock
        CMU_ClockEnable(cmuClock_CORELE, true);

        // Enable BURTC registers access
        RMU_ResetControl(rmuResetBU, false);

        // BURTC setup
        setupBurtc();


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
                err = alti_get_temp_pressure(&temp, &pressure, true);
                err = alti_mbar2altitude(pressure, &altitude);
                fg_i =  max17047_get_current();
                fg_var8 =  max17047_get_charge();


                if(lcd_update_flag)
                {
                        lcd_update_flag = false;

                        lcd_clear();

                        sprintf(str, "%02d:%02d:%02d.%1d", hours, minutes, milliseconds/1000, (milliseconds%1000)/100);
                        text(&font_helv17b, 5, 20, str);


                        sprintf(str, "light: %3.3f lux", lux);
                        text(&font_helv11, 5, 60, str);

                        sprintf(str, "temp: %f C", temp);
                        text(&font_helv11, 5, 70, str);
                        sprintf(str, "pressure: %f mbar", pressure);
                        text(&font_helv11, 5, 80, str);

                        sprintf(str, "altitude: %f m", altitude);
                        text(&font_helv11, 5, 90, str);

                        sprintf(str, "fg current: %d mA", fg_i);
                        text(&font_helv11, 5, 100, str);

                        sprintf(str, "fg charge: %d", fg_var8);
                        text(&font_helv11, 5, 110, str);


                        lcd_update();
                }
        }
}
