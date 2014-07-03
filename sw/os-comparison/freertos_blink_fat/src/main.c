/**************************************************************************//**
 * @file
 * @brief FreeRTOS Blink Demo for Energy Micro EFM32GG_STK3700 Starter Kit
 * @author Energy Micro AS
 * @version 3.20.3
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2013 Energy Micro AS, http://www.energymicro.com</b>
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
#include <stdlib.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "croutine.h"

#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"

#include "sleep.h"

#define STACK_SIZE_FOR_TASK    (configMINIMAL_STACK_SIZE + 10)
#define TASK_PRIORITY          (tskIDLE_PRIORITY + 1)

/* Structure with parameters for LedBlink */
typedef struct
{
  /* Delay between blink of led */
  portTickType delay;
  /* Number of led */
  int          ledNo;
} TaskParams_t;


/**************************************************************************//**
 * @brief Simple task which is blinking led
 * @param *pParameters pointer to parameters passed to the function
 *****************************************************************************/
static void LedBlink(void *pParameters)
{
  TaskParams_t     * pData = (TaskParams_t*) pParameters;
  const portTickType delay = pData->delay;
  
  for (;;)
  {
    GPIO_PinOutToggle(gpioPortD, pData->ledNo);
    vTaskDelay(delay);
  }
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  int i;

  /* Chip errata */
  CHIP_Init();

  /* Enable clocks */
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  for (i = 1; i < 5; ++i) {
      GPIO_PinModeSet(gpioPortD, i, gpioModePushPull, 0);
  }

  /* Initialize SLEEP driver, no callbacks are used */
  SLEEP_Init(NULL, NULL);
#if (configSLEEP_MODE < 3)
  /* do not let to sleep deeper than define */
  SLEEP_SleepBlockBegin((SLEEP_EnergyMode_t)(configSLEEP_MODE+1));
#endif

  /* Parameters value for taks*/
  static TaskParams_t parametersToTask1 = { 500 / portTICK_RATE_MS, 1 };
  static TaskParams_t parametersToTask2 = { 250 / portTICK_RATE_MS, 2 };
  static TaskParams_t parametersToTask3 = { 125 / portTICK_RATE_MS, 3 };
  static TaskParams_t parametersToTask4 = {  62 / portTICK_RATE_MS, 4 };

  /*Create two task for blinking leds*/
  xTaskCreate( LedBlink, (const signed char *) "LedBlink1", STACK_SIZE_FOR_TASK, &parametersToTask1, TASK_PRIORITY, NULL);
  xTaskCreate( LedBlink, (const signed char *) "LedBlink2", STACK_SIZE_FOR_TASK, &parametersToTask2, TASK_PRIORITY, NULL);
  xTaskCreate( LedBlink, (const signed char *) "LedBlink3", STACK_SIZE_FOR_TASK, &parametersToTask3, TASK_PRIORITY, NULL);
  xTaskCreate( LedBlink, (const signed char *) "LedBlink4", STACK_SIZE_FOR_TASK, &parametersToTask4, TASK_PRIORITY, NULL);

  f_dotest( 0 ); /* test of using FAT32 */

  /*Start FreeRTOS Scheduler*/
  vTaskStartScheduler();

  return 0;
}
