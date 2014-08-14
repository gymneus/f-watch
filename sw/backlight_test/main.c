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
#include <em_rmu.h>


#include "em_emu.h"
#include "em_prs.h"
#include "em_system.h"
#include "em_timer.h"
#include "em_chip.h"


#define BZ_PWM_FREQ 10000
#define BZ_TOP 92

#define BL_PWM_FREQ 10000

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
        burtcInit.clkSel       = burtcClkSelLFXO;     /* Select LFXO as clock source */
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

void timer1Init(void)
{
        CMU_ClockEnable(cmuClock_TIMER1, true);

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
        //TIMER_InitCC(TIMER1, 1, &timerCCInit);
        TIMER_InitCC(TIMER1, 2, &timerCCInit);

        // Route CC1 to location 1 (PE11) and enable pin
        //TIMER1->ROUTE |= (TIMER_ROUTE_CC1PEN | TIMER_ROUTE_LOCATION_LOC1);

        // Route CC2 to location 1 (PE12) and enable pin
        TIMER1->ROUTE |= (TIMER_ROUTE_CC2PEN | TIMER_ROUTE_LOCATION_LOC1);

        // Set Top Value
        TIMER_TopSet(TIMER1, CMU_ClockFreqGet(cmuClock_HFPER)/BL_PWM_FREQ);

        // Set compare value starting at 0 - it will be incremented in the interrupt handler
        //TIMER_CompareBufSet(TIMER1, 1, 0);
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
        //TIMER_IntEnable(TIMER1, TIMER_IF_OF);

        // Enable TIMER1 interrupt vector in NVIC
        //NVIC_EnableIRQ(TIMER1_IRQn);

        // Configure timer
        TIMER_Init(TIMER1, &timerInit);
}

void TIMER1_IRQHandler(void)
{
  uint32_t compareValue;

  /* Clear flag for TIMER1 overflow interrupt */
  TIMER_IntClear(TIMER1, TIMER_IF_OF);

  compareValue = TIMER_CaptureGet(TIMER1, 1);
  /* increment duty-cycle or reset if reached TOP value */
  if( compareValue == TIMER_TopGet(TIMER1))
  {
          //TIMER_CompareBufSet(TIMER1, 1, 0);
          TIMER_CompareBufSet(TIMER1, 2, 0);
  }
  else
  {
          compareValue += 10;
          //TIMER_CompareBufSet(TIMER1, 1, compareValue);
          TIMER_CompareBufSet(TIMER1, 2, compareValue);
  }
}

void timer2Init(void)
{
  /* Enable clock for TIMER2 module */
  CMU_ClockEnable(cmuClock_TIMER2, true);

  /* Select CC channel parameters */
  TIMER_InitCC_TypeDef timerCCInit =
  {
    .cufoa      = timerOutputActionNone,
    .cofoa      = timerOutputActionToggle,
    .cmoa       = timerOutputActionNone,
    .mode       = timerCCModeCompare,
    .filter     = false,
    .prsInput   = false,
    .coist      = false,
    .outInvert  = false,
  };

  /* Configure CC channel 1 */
  TIMER_InitCC(TIMER2, 1, &timerCCInit);

  /* Route CC1 to location 0 (PA9) and enable pin */
  TIMER2->ROUTE |= (TIMER_ROUTE_CC1PEN | TIMER_ROUTE_LOCATION_LOC0);

  /* Set Top Value */
  TIMER_TopSet(TIMER2, BZ_TOP);

  /* Select timer parameter */
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

  /* Configure timer */
  TIMER_Init(TIMER2, &timerInit);
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
        uint32_t compareValue;

        CHIP_Init();

        // Enable LFXO and select it for LFA branch
        CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
        CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

        // Enable HFXO and select it for HF branch
        CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
        CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);

        // Enable low energy clocking module clock
        CMU_ClockEnable(cmuClock_CORELE, true);

        // Enable BURTC registers access
        RMU_ResetControl(rmuResetBU, false);

        // BURTC setup
        setupBurtc();

        // Setup SysTick Timer for 1 msec interrupts
        if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        // Configure backlight LED pins as outputs
        GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
        GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

        // Configure switches pins as inputs without pull-up/down (-> external pull-up)
        GPIO_PinModeSet(gpioPortA, 0, gpioModeInput, 0); // Top right switch
        GPIO_PinModeSet(gpioPortA, 8, gpioModeInput, 0); // Bottom right switch
        GPIO_PinModeSet(gpioPortC, 7, gpioModeInput, 0); // Top left switch
        GPIO_PinModeSet(gpioPortC, 6, gpioModeInput, 0); // Bottom left switch

        // Configure vibrating motor enable pin as push/pull output
        GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 0);

        // Configure buzzer pin as push/pull output
        GPIO_PinModeSet(gpioPortA, 9, gpioModePushPull, 0);

        timer1Init();
        timer2Init();

        lcd_init();

        I2CDRV_Init(&i2cInit);

        err = alti_init();

        err = max17047_set_config(0x2250);


        //GPIO_PinOutSet(gpioPortE, 11);
        //GPIO_PinOutSet(gpioPortE, 12);

        while(1)
        {

                // Top left switch
                if ( !GPIO_PinInGet(gpioPortC, 7) ) {
                        compareValue = TIMER_CaptureGet(TIMER1, 2);
                        /* increment duty-cycle if smaller than TOP value */
                        if( compareValue < TIMER_TopGet(TIMER1))
                        {
                                compareValue += 100;
                                //TIMER_CompareBufSet(TIMER1, 1, compareValue);
                                TIMER_CompareBufSet(TIMER1, 2, compareValue);
                        }
                        Delay(100);
                }

                // Bottom left switch
                if ( !GPIO_PinInGet(gpioPortC, 6) ) {
                        compareValue = TIMER_CaptureGet(TIMER1, 2);
                        /* decrement duty-cycle if bigger than 0 */
                        if( compareValue > 0)
                        {
                                compareValue -= 100;
                                //TIMER_CompareBufSet(TIMER1, 1, compareValue);
                                TIMER_CompareBufSet(TIMER1, 2, compareValue);
                        }
                        Delay(100);
                }

                // Top right switch
                if ( GPIO_PinInGet(gpioPortA, 0) ) {
                        TIMER_Enable(TIMER2, false);
                }
                else {
                        TIMER_Enable(TIMER2, true);
                }

                // Bottom right switch
                if ( GPIO_PinInGet(gpioPortA, 8) ) {
                        GPIO_PinOutClear(gpioPortA, 3);
                }
                else {
                        GPIO_PinOutSet(gpioPortA, 3);
                }


                if(lcd_update_flag)
                {
                        lcd_update_flag = false;

                        err = light_sensor_get_lux(&lux);
                        err = alti_get_temp_pressure(&temp, &pressure, true);
                        err = alti_mbar2altitude(pressure, &altitude);
                        fg_i =  max17047_get_current();
                        fg_var8 =  max17047_get_charge();

                        lcd_clear();

                        sprintf(str, "%02d:%02d", hours, minutes);
                        text(&font_helv38b, 5, 20, str);
                        sprintf(str, "%02d.%1d", milliseconds/1000, (milliseconds%1000)/100);
                        text(&font_helv22b, 89, 34, str);


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

                        sprintf(str, "backlight: %d", compareValue * 100 / TIMER_TopGet(TIMER1));
                        text(&font_helv11, 5, 5, str);

                        lcd_update();
                }

        }
}