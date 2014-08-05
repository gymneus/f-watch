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

#include "em_emu.h"
#include "em_prs.h"
#include "em_system.h"
#include "em_timer.h"
#include "em_chip.h"


#define PWM_FREQ 10000


void TIMER1_IRQHandler(void)
{
  uint32_t compareValue;

  /* Clear flag for TIMER1 overflow interrupt */
  TIMER_IntClear(TIMER1, TIMER_IF_OF);

  compareValue = TIMER_CaptureGet(TIMER1, 1);
  /* increment duty-cycle or reset if reached TOP value */
  if( compareValue == TIMER_TopGet(TIMER1))
  {
          TIMER_CompareBufSet(TIMER1, 1, 0);
          TIMER_CompareBufSet(TIMER1, 2, 0);
  }
  else
  {
          compareValue += 10;
          TIMER_CompareBufSet(TIMER1, 1, compareValue);
          TIMER_CompareBufSet(TIMER1, 2, compareValue);
  }
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

        CHIP_Init();

        // Enable HFXO and select it for HF branch
        CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
        CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);

        /* Setup SysTick Timer for 1 msec interrupts */
        if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        CMU_ClockEnable(cmuClock_TIMER1, true);

        // Backlight LEDs
        GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
        GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

        lcd_init();

        I2CDRV_Init(&i2cInit);

        err = alti_init();

        // Select CC channel parameters
        TIMER_InitCC_TypeDef timerCCInit =
                {
                        .eventCtrl  = timerEventEveryEdge,
                        .edge       = timerEdgeBoth,
                        .prsSel     = timerPRSSELCh1,
                        .cufoa      = timerOutputActionNone,
                        .cofoa      = timerOutputActionNone,
                        .cmoa       = timerOutputActionToggle,
                        .mode       = timerCCModePWM,
                        .filter     = false,
                        .prsInput   = false,
                        .coist      = false,
                        .outInvert  = false,
                };

        // Configure CC channel 1
        TIMER_InitCC(TIMER1, 1, &timerCCInit);
        TIMER_InitCC(TIMER1, 2, &timerCCInit);

        // Route CC1 to location 1 (PE11) and enable pin
        TIMER1->ROUTE |= (TIMER_ROUTE_CC1PEN | TIMER_ROUTE_LOCATION_LOC1);

        // Route CC2 to location 1 (PE12) and enable pin
        TIMER1->ROUTE |= (TIMER_ROUTE_CC2PEN | TIMER_ROUTE_LOCATION_LOC1);

        // Set Top Value
        TIMER_TopSet(TIMER1, CMU_ClockFreqGet(cmuClock_HFPER)/PWM_FREQ);

        // Set compare value starting at 0 - it will be incremented in the interrupt handler
        TIMER_CompareBufSet(TIMER1, 1, 0);
        TIMER_CompareBufSet(TIMER1, 2, 0);

        // Select timer parameters
        TIMER_Init_TypeDef timerInit =
                {
                        .enable     = true,
                        .debugRun   = true,
                        .prescale   = timerPrescale64,
                        .clkSel     = timerClkSelHFPerClk,
                        .fallAction = timerInputActionNone,
                        .riseAction = timerInputActionNone,
                        .mode       = timerModeUp,
                        .dmaClrAct  = false,
                        .quadModeX4 = false,
                        .oneShot    = false,
                        .sync       = false,
                };

        // Enable overflow interrupt
        TIMER_IntEnable(TIMER1, TIMER_IF_OF);

        // Enable TIMER1 interrupt vector in NVIC
        NVIC_EnableIRQ(TIMER1_IRQn);

        // Configure timer
        TIMER_Init(TIMER1, &timerInit);


        //GPIO_PinOutSet(gpioPortE, 11);
        //GPIO_PinOutSet(gpioPortE, 12);

        //box(0, 0, 128, 128, 1);

        while(1)
        {

                err = light_sensor_get_lux(&lux);
                sprintf(str, "light: %3.3f lux", lux);
                text(&font_helv11, 5, 10, str);

                err = alti_get_temp_pressure(&temp, &pressure, true);
                sprintf(str, "temp: %f C", temp);
                text(&font_helv11, 5, 30, str);
                sprintf(str, "pressure: %f mbar", pressure);
                text(&font_helv11, 5, 40, str);

                err = alti_mbar2altitude(pressure, &altitude);
                sprintf(str, "altitude: %f m", altitude);
                text(&font_helv11, 5, 50, str);

                sprintf(str, "clock: %d",CMU_ClockFreqGet(cmuClock_HFPER));
                text(&font_helv11, 5, 60, str);

                //lcd_update();
                for(x=0; x<128; x++)
                {
                        for(y=0; y<128; y++)
                        {
                                lcd_toggle_pixel(x, y);
                        }
                }
                lcd_update();
                Delay(1000);
                box(0, 0, 128, 128, 0);
                //lcd_clear();
        }




}
