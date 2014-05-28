/**************************************************************************//**
 * @file
 * @brief FreeRTOS Blink Demo for Energy Micro EFM32GG_STK3700 Starter Kit
 * @author Energy Micro AS
 * @version 3.20.3
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs Software License Agreement. See 
 * "http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt"  
 * for details. Before using this software for any purpose, you must agree to the 
 * terms of that agreement.
 *
 ******************************************************************************/



#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "croutine.h"

#include "em_chip.h"
#include "bsp.h"
#include "bsp_trace.h"

#include "sleep.h"

#include "segmentlcd.h"

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


typedef char lcdText_t[8];
lcdText_t lcdtext;

xQueueHandle msgBox;


/**************************************************************************//**
 * @brief Simple task which is blinking led
 * @param *pParameters pointer to parameters passed to the function
 *****************************************************************************/
static void PrintLcdThread(void const *argument) {
  lcdText_t *rptr = &lcdtext;
  (void)argument;                 /* Unused parameter. */

  while (1)
  {
    /* Wait for message */
    if (xQueueReceive(msgBox, rptr, portMAX_DELAY))
    {
      SegmentLCD_Write(*rptr);
    }
  }
}


static void PutOnLcd(void const *arg)
{
	static int count = 0;

	/* Infinite loop */
	while (1)
	{
	    count = (count+1)&0xF;
	    BSP_LedsSet(count);

	    /* Send message to PrintLcdThread */
	    /* Allocate memory for the message */
	    lcdText_t *mptr = &lcdtext;

	    /* Set the message content */
	    (*mptr)[0] = count>=10 ? '1' : '0';
	    (*mptr)[1] = count%10 + '0';
	    (*mptr)[2] = '\0';
	    /* Send message */
	    xQueueSend(msgBox, (void *)mptr, portMAX_DELAY);

	    /* Wait now for half a second */
	    vTaskDelay(500 / portTICK_RATE_MS);
	}
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  /* Chip errata */
  CHIP_Init();
  /* If first word of user data page is non-zero, enable eA Profiler trace */
  BSP_TraceProfilerSetup();

  /* Initialize LED driver */
  BSP_LedsInit();
  /* Setting state of leds*/
  BSP_LedSet(0);
  BSP_LedSet(1);

#if (configSLEEP_MODE < 3)
  /* do not let to sleep deeper than define */
  SLEEP_SleepBlockBegin((SLEEP_EnergyMode_t)(configSLEEP_MODE+1));
#endif

  msgBox = xQueueCreate(16, sizeof(lcdText_t));

  /*Create two task for blinking leds*/
  xTaskCreate( PutOnLcd, (const signed char *) "PutOnLcd", STACK_SIZE_FOR_TASK, NULL, TASK_PRIORITY, NULL);
  xTaskCreate( PrintLcdThread, (const signed char *) "PrintLcdThread", STACK_SIZE_FOR_TASK, NULL, TASK_PRIORITY, NULL);

  /*Start FreeRTOS Scheduler*/
  vTaskStartScheduler();

  return 0;
}
