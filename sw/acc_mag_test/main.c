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
#include <em_usb.h>
#include <em_leuart.h>
#include <drivers/lcd.h>
#include <drivers/i2cdrv.h>
#include <drivers/max17047.h>
#include <gfx/graphics.h>
#include <stdio.h>
#include <pp-printf.h>
#include <LSM303C/lsm303c.h>
#include <usbdesc.h>

#define ACC_DEMO 0
#define MAG_DEMO 1

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
    int x, y, t, but, calibrated=0;
    char buf[30];
    I2C_Init_TypeDef i2c_init = I2C_INIT_DEFAULT;
    //uint8_t ret = 0;
    //int16_t acc_x[2], acc_y[2], acc_z[2];
		lsm303_smpl smpl, smpl2, mag_max, mag_min;

    /* Setup SysTick Timer for 1 msec interrupts */
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    // Backlight LEDs
    GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

		GPIO_PinModeSet(gpioPortC, 7, gpioModeInput, 1);

    lcd_init();
    I2CDRV_Init(&i2c_init);
    lsm303_init();
		USBD_Init(&initstruct);

    /* Infinite blink loop */
    while (1) {
				lcd_clear();
				but = GPIO_PinInGet(gpioPortC, 7);
//#if MAG_DEMO
	 			/*make the calibration*/
				if(!but) {
					text(&font_helv17, 10, 70, "go!");
					lcd_update();
					lsm303_mag_calibrate(&mag_max, &mag_min);
					calibrated = 1;
				}
//#endif
				//if(calibrated) {
				//	compass_xy(&mag_max, &mag_min, &x, &y);
				//	line(60, 60, x, y, 1);
				//}
#if ACC_DEMO
				spi_read(DEV_ACC, LSM303_WHO_AM_I_REG, (uint8_t*) &t);
        pp_sprintf(buf, "who: 0x%x", (int8_t)t);
        text(&font_helv17b, 5, 10, buf);
        lsm303_get_sample(DEV_ACC, &smpl);
        pp_sprintf(buf, "x:%d", smpl.x);
        text(&font_helv17b, 5, 50, buf);
        pp_sprintf(buf, "y:%d", smpl.y);
        text(&font_helv17b, 5, 70, buf);
        pp_sprintf(buf, "z:%d", smpl.z);
        text(&font_helv17b, 5, 90, buf);
#endif
#if MAG_DEMO
				//sprintf(buf, "min: %d ; %d ; %d", mag_min.x, mag_min.y, mag_min.z);
        //text(&font_helv11, 5, 5, buf);
				//sprintf(buf, "max: %d ; %d ; %d", mag_max.x, mag_max.y, mag_max.z);
        //text(&font_helv11, 5, 15, buf);
        //lsm303_get_sample(DEV_ACC, &smpl2);
				//sprintf(buf, "xa: %d, ya: %d", smpl2.x, smpl2.y);
				//text(&font_helv11, 5, 25, buf);
        //sprintf(buf, "x: %d", smpl.x);
        //text(&font_helv17, 5, 50, buf);
        //sprintf(buf, "y: %d", smpl.y);
        //text(&font_helv17, 5, 70, buf);
        //sprintf(buf, "z: %d", smpl.z);
        //text(&font_helv17, 5, 90, buf);

				/* print compass samples to UART for hard/soft-iron calibration */
        lsm303_get_sample(DEV_MAG, &smpl);
				sprintf(buf, "x:%d y:%d z:%d\n\r", smpl.x, smpl.y, smpl.z);
				USBD_Write(USBDESC_EP_DATA_OUT, (void*)buf, strlen(buf), NULL);
#endif
        //lcd_update();
				Delay(10);
    }
}

