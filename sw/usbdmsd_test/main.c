/*
 * Copyright (C) 2014 Julian Lewis
 * @author Matthieu Cattin <matthieu.cattin@cern.ch>
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

#include <stdio.h>
#include "em_chip.h"
#include "em_device.h"
#include "em_usb.h"
#include "em_assert.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
//#include "bsp.h"
//#include "bsp_trace.h"
//#include "retargetserial.h"
#include "msdd.h"
#include "msddmedia.h"
#include "usbconfig.h"
#include "drivers/lcd.h"
#include "gfx/graphics.h"
#include "delay.h"

/*****************************************************************************
 * USB Mass Storage Device (MSD) example
 *****************************************************************************/

#if defined( BUSPOWERED )
#if ( ( MSD_MEDIA==MSD_PSRAM_MEDIA ) || ( MSD_MEDIA==MSD_SDCARD_MEDIA ) )
#error "Illegal combination of BUSPOWERED and MSD_MEDIA type."
#endif
#endif

/*****************************************************************************
 * @brief main - the entrypoint after reset.
 *****************************************************************************/
int main(void)
{
        char str[20];

        /* Setup SysTick Timer for 1 msec interrupts */
        if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

        // Just in case something goes wrong
        Delay(200);

        // Chip errata
        CHIP_Init();


        CMU_ClockSelectSet( cmuClock_HF, cmuSelect_HFXO );
        CMU_OscillatorEnable(cmuOsc_LFXO, true, false);

        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        lcd_init();

        lcd_clear();

        sprintf(str, "USB MSD test");
        text(&font_helv17, 5, 10, str);
        lcd_update();

        if ( !MSDDMEDIA_Init() )
        {
                sprintf(str, "Media error!");
                text(&font_helv17, 5, 30, str);
                lcd_update();

                EFM_ASSERT( false );
                for( ;; ){}
        }

        MSDD_Init(gpioPortE, 11); // use backlight as activity indicator led

        for (;;)
        {
                if ( MSDD_Handler() )
                {
                        /* There is no pending activity in the MSDD handler.  */
                        /* Enter sleep mode to conserve energy.               */

                        if ( USBD_SafeToEnterEM2() )
                                EMU_EnterEM2(true);
                        else
                                EMU_EnterEM1();
                }
        }
}
