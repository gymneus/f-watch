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
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "sd.h"

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

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{


    int i;
    uint8_t res;

    /* Setup SysTick Timer for 1 msec interrupts  */
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    //init SPI for microSD
	sd_spi_init_not_static();
    sd_power(1);

    for (i =11; i < 13; ++i) {
        GPIO_PinModeSet(gpioPortE, i, gpioModePushPull, 0);
    }

    /* Infinite blink loop */
    while (1) {
        //this code is ok but not what we want (it only test spi protocol, we want now to test the SD)
        //res = sd_spi_transfer_byte(0b00001111);
        //res = sd_spi_transfer_byte(0b01010101);
        //res = sd_spi_transfer_byte(0b00110011);
        //send dummy stuff
        res = sd_spi_transfer_byte(0xFF);
        res++;
        Delay(1);
        for (i = 11; i < 13; ++i) {
            GPIO_PinOutToggle(gpioPortE, i);
            res = sd_spi_transfer_byte(0b00110011);
            Delay(1);
        }
    }
	return 0;
}

