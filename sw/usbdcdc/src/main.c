/**************************************************************************//**
 * @file main.c
 * @brief USB CDC Serial Port adapter example project.
 * @version 3.20.5
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include <string.h>

#include "em_device.h"
#include "em_cmu.h"
#include "em_dma.h"
#include "em_dma.h"
#include "em_gpio.h"
#include "em_int.h"
#include "dmactrl.h"
#include "em_usb.h"
#include "em_leuart.h"
#include "bsp.h"
#include "bsp_trace.h"

#include "usbdbg.h"

/**************************************************************************//**
 *
 * This example shows how to use USB-based debugging.
 *
 * Under Linux, Freewatch should show up as a CDC device. Do dmesg | grep tty
 * to see which one (the latest ACM after download should in principle be it),
 * then connect with a terminal software (picocom works for me) and it will
 * greet you nicely.
 *
 * Note: baudrate and other serial port settings are not important
 *
 *****************************************************************************/

/*** Typedef's and defines. ***/

#define USB_RX_BUF_SIZ   USBDESC_BULK_EP_SIZE /* Packet size when receiving on USB*/
#define USB_TX_BUF_SIZ   127    /* Packet size when transmitting on USB.  */

/*** Function prototypes. ***/


/* Counts 1ms timeTicks */
volatile uint32_t msTicks;

void SysTick_Handler(void)
{
        msTicks++; /* increment counter necessary in Delay()*/
}

void delay(uint32_t dlyTicks)
{
        uint32_t curTicks;

        curTicks = msTicks;
        while ((msTicks - curTicks) < dlyTicks);
}

/**************************************************************************//**
 * @brief main - the entrypoint after reset.
 *****************************************************************************/
int main(void)
{

        CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
        CMU_ClockEnable(cmuClock_HFPER, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        /* Setup SysTick Timer for 1 msec interrupts  */
        if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

        GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);;
        GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 1);;

        usbdbg_init();

        /*
        * When using a debugger it is practical to uncomment the following three
        * lines to force host to re-enumerate the device.
        */
//        USBD_Disconnect();
//        USBTIMER_DelayMs(1000);
//        USBD_Connect();

        char *tmp = "hello, world!\r\n";

        for (;;)
        {
              GPIO_PinOutToggle(gpioPortE, 11);
              GPIO_PinOutToggle(gpioPortE, 12);
              usbdbg_puts(tmp);
              delay(1000);
        }
}

