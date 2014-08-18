					        
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "freewatch_utils.h"

/* Counts 1ms timeTicks */
volatile uint32_t msTicks;

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

/**************************************************************************//**
 * @brief Reset the MCU
 *****************************************************************************/
void ResetEFM32(void)
{
	SCB->AIRCR = 0x05FA0004;
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
void freewatch_backlight_blinky(int delay, int rep)
{
  int i, c = rep * 2;

    /* Setup SysTick Timer for 1 msec interrupts  */
    if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);

    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);
    GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);

    /* Infinite blink loop */
    while (c--) {
            GPIO_PinOutToggle(gpioPortE, 11);
	    GPIO_PinOutToggle(gpioPortE, 12);
            Delay(delay);
    }
}


