/**************************************************************************//**
 * @file
 * @brief Simple LED Blink Demo for EFM32GG_DK3750
 * @author Energy Micro AS
 * @version 3.20.3
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2012 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 *****************************************************************************/
#include <stdio.h>
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "sd.h"
#include "lcd.h"
#include "i2cdrv.h"
#include <gfx/graphics.h>

/* Counts 1ms timeTicks */
volatile uint32_t msTicks;

/* Local prototypes */
void Delay(uint32_t dlyTicks);

/**************************************************************************//**
* @brief SysTick_Handler
* Interrupt Service Routine for system tick counter
 *****************************************************************************/
void SysTick_Handler(void)
{
    msTicks++; /* increment counter necessary in Delay()*/
}

/**************************************************************************//**
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 *****************************************************************************/
void Delay(uint32_t dlyTicks)
{
    uint32_t curTicks;

    curTicks = msTicks;
    while ((msTicks - curTicks) < dlyTicks);
}

//todo : supprimer ces declarations de fonctions
void sd_spi_init_not_static(void);
uint8_t sd_spi_transfer_byte_not_static(uint8_t txData);

#define sd_spi_init sd_spi_init_not_static
#define sd_spi_transfer_byte sd_spi_transfer_byte_not_static
uint32_t guiTracerSD = 0;//TODO: remove

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{




    I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

    //int x, y, i;
    char str[20];
    double temp = 0;
    double pressure = 0;
    double lux;
    int i = 0;
    uint8_t res_sd, res_sd_init;
    uint8_t buffer_ocr[4];

    /* Setup SysTick Timer for 1 msec interrupts */
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    // Backlight LEDs
    GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

    lcd_init();

    I2CDRV_Init(&i2cInit);

    //init SPI for microSD
	res_sd_init = sd_init();
	//sd_spi_init();
    //sd_power(1);

    while(1) {
        i++;

        lux = 102.03050 + i;

        sprintf(str, "light: %3.3f lux", lux);
        text(&font_helv11, 5, 10, str);

        temp = 1122.3344 + i;
        pressure = 12.34 + i;
        sprintf(str, "temp: %f C", temp);
        text(&font_helv11, 5, 30, str);
        sprintf(str, "pressure: %f mbar", pressure);
        text(&font_helv11, 5, 40, str);

        sprintf(str, "sd_init() = %d", res_sd_init);
        text(&font_helv11, 5, 60, str);

        sprintf(str, "guiTracerSR = %u", (unsigned int) guiTracerSD);
        text(&font_helv11, 5, 70, str);

        res_sd = sd_read_ocr(buffer_ocr);
        sprintf(str, "sd_read_ocr() = %u = %02X %02X %0X %02X", (unsigned int) res_sd,
            buffer_ocr[0], buffer_ocr[1], buffer_ocr[2], buffer_ocr[3]);
        text(&font_helv11, 5, 80, str);

        GPIO_PinOutToggle(gpioPortE, 11);
        GPIO_PinOutToggle(gpioPortE, 12);

        lcd_update();
        //Delay(1000);
        box(0, 0, 128, 128, 0);
        //lcd_clear();
        Delay(1000);
        if (res_sd_init) {
            uint8_t data[SD_SECTOR_SIZE]= {0};
            static uint32_t addr = 0;

            sprintf(str, "sd_init() = %d", res_sd_init);
            text(&font_helv11, 5, 10, str);

            sprintf(str, "guiTracerSR = %u", (unsigned int) guiTracerSD);
            text(&font_helv11, 5, 20, str);
#if 0
            res_sd = sd_read_block(addr, data);
            sprintf(str, "sd_read_block(%u) = %d", (unsigned int) addr, res_sd);
            text(&font_helv11, 5, 30, str);
            if (res_sd) {
                int baseaddr = 0;
                sprintf(str, "%2x %2x %2x %2x %2x %2x %2x %2x",
                    data[baseaddr + 0], data[baseaddr + 1], data[baseaddr + 2], data[baseaddr + 3],
                    data[baseaddr + 4], data[baseaddr + 5], data[baseaddr + 6], data[baseaddr + 7]);
                text(&font_helv11, 5, 40, str);
                baseaddr = 8;
                sprintf(str, "%2x %2x %2x %2x %2x %2x %2x %2x",
                    data[baseaddr + 0], data[baseaddr + 1], data[baseaddr + 2], data[baseaddr + 3],
                    data[baseaddr + 4], data[baseaddr + 5], data[baseaddr + 6], data[baseaddr + 7]);
                text(&font_helv11, 5, 50, str);
                baseaddr = 16;
                sprintf(str, "%2x %2x %2x %2x %2x %2x %2x %2x",
                    data[baseaddr + 0], data[baseaddr + 1], data[baseaddr + 2], data[baseaddr + 3],
                    data[baseaddr + 4], data[baseaddr + 5], data[baseaddr + 6], data[baseaddr + 7]);
                text(&font_helv11, 5, 60, str);
                baseaddr = 24;
                sprintf(str, "%2x %2x %2x %2x %2x %2x %2x %2x",
                    data[baseaddr + 0], data[baseaddr + 1], data[baseaddr + 2], data[baseaddr + 3],
                    data[baseaddr + 4], data[baseaddr + 5], data[baseaddr + 6], data[baseaddr + 7]);
                text(&font_helv11, 5, 70, str);
            }
#else
            res_sd = sd_write_block(addr, data);
            sprintf(str, "sd_write_block(%u) = %d", (unsigned int) addr, res_sd);
            text(&font_helv11, 5, 30, str);
#endif
            addr += 512;
            lcd_update();
            box(0, 0, 128, 128, 0);
            Delay(2000);
        }
    }
	return 0;
}

